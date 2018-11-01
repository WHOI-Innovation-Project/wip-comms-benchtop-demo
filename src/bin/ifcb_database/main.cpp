// Copyright (C) 2018 Woods Hole Oceanographic Institution
//
// This file is part of the WHOI Innovation Project (WIP) Comms Project
// ("wip-comms").
//
// wip-comms is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// wip-comms is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with wip-comms in the COPYING.md file at the project root.
// If not, see <http://www.gnu.org/licenses/>.

#include "goby/middleware/single-thread-application.h"

#include <pqxx/pqxx>

#include "wip-comms/messages/groups.h"
#include "wip-comms/config/ifcb_database_config.pb.h"

#include "progressive_imagery/goby/groups.h"
#include "progressive_imagery/progressive_imagery.pb.h"


using goby::glog;
using AppBase = goby::SingleThreadApplication<wip::protobuf::IFCBDatabaseConfig>;
namespace dpi = dsl::progressive_imagery;

class IFCBDatabaseInterface : public AppBase
{
public:
    IFCBDatabaseInterface() :
        AppBase(),
        connection_(make_connection_string())
        {
            {
                std::string query = "create table if not exists " + cfg().progress_table() +
                    " ( image_id int unique, fraction_acked real, retrieved boolean default false ) ";

                glog.is_debug1() && glog << query << std::endl;
                pqxx::work t(connection_);
                t.exec(query);
                t.commit();
            }

            // reset all image retrieval upon reboot
            {
                std::string query = "update " + cfg().progress_table() + " set retrieved = false";
                glog.is_debug1() && glog << query << std::endl;
                pqxx::work t(connection_);
                t.exec(query);
                t.commit();
            }

            interprocess().subscribe<dpi::groups::transfer_progress, dsl::protobuf::TransferProgress>(
                [this](const dsl::protobuf::TransferProgress& p)
                {
                    // update received stats
                    pqxx::work t(connection_);
                    auto pt = cfg().progress_table();

                    for(auto& rx_status : p.rx_status_last_ack())
                    {
                        auto id = std::to_string(rx_status.image_id());
                        auto fraction_acked = std::to_string(rx_status.fraction_received());

                        glog.is_debug2() && glog << "id: " << id << ", frac: " + fraction_acked << std::endl;

                        
                        std::string update_query = "update " + pt + " set fraction_acked = " + fraction_acked + " where image_id = " + id;
                        t.exec(update_query);
                    }
                    t.commit();                    

                    if(p.entirely_unsent_images() < cfg().max_images_per_query())
                        retrieve_more_images();
                });
            
            retrieve_more_images();
            
        }

private:
    std::string make_connection_string()
        {
            std::string c;
            c += "dbname=" + cfg().ifcb_database();
            c += " user=" + cfg().user();
            c += " password=" + cfg().password();
            c += " host=" + cfg().host();
            c += " port=" + std::to_string(cfg().port());
            return c;
        }

    void retrieve_more_images()
        {
            auto dt = cfg().ifcb_data_table();
            auto pt = cfg().progress_table();

            auto xmin = std::to_string(cfg().min_x_pixels());
            auto ymin = std::to_string(cfg().min_y_pixels());
            
            // return the next N images that have not been transmitted to the threshold
            std::string select_query = "select " + dt + ".image_id," + dt + ".image from " + dt + " ";
            std::string select_body = "left join " + pt + " " +
                "on " + dt + ".image_id = " + pt + ".image_id " +
                "where (" +
                "(" + pt + ".fraction_acked < " + std::to_string(cfg().pass1_fraction()) + " or " + pt + ".fraction_acked is null) and " +
                "(" + pt + ".retrieved = false or " + pt + ".retrieved is null) and " +
                "(" + dt + ".x < " + xmin + " or " + dt + ".y < " + ymin + ") " + 
                ") " + 
                "order by ifcb.image_id asc limit " + std::to_string(cfg().max_images_per_query());
            select_query += select_body;
            
            glog.is_debug1() && glog << select_query << std::endl;
            pqxx::work t(connection_);
            auto result = t.exec(select_query);
            
            for(auto row : result)
            {
                auto id = row["image_id"].as<int>();
                pqxx::binarystring image(row["image"]);
                std::cout << id << " " << image.size() << std::endl;

                std::stringstream image_path;
                image_path << cfg().img_out_dir() << "/img_" << std::setw(10) << std::setfill('0') << id << ".ppm";
                std::ofstream oimg(image_path.str().c_str());

                if(oimg.is_open())
                    oimg.write(image.get(), image.size());
                else
                    glog.is_warn() && glog << "Failed to write image: " << image_path.str() << std::endl;

                // set retrieved flag
                std::string insert_query = "insert into " + pt + " (image_id, fraction_acked, retrieved) values (" + std::to_string(id) + ", 0, true) on conflict (image_id) do update set retrieved = true";
                t.exec(insert_query);
            }
            
            t.commit();
            
        }
    
    
private:    
    pqxx::connection connection_;
};

int main(int argc, char* argv[])
{ return goby::run<IFCBDatabaseInterface>(argc, argv); }

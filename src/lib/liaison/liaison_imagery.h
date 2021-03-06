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

#ifndef LIAISONIMAGERY20180801H
#define LIAISONIMAGERY20180801H

#include <Wt/WText>
#include <Wt/WCssDecorationStyle>
#include <Wt/WBorder>
#include <Wt/WColor>
#include <Wt/WVBoxLayout>
#include <Wt/WImage>
#include <Wt/WPushButton>
#include <Wt/WGroupBox>
#include <Wt/WDoubleSpinBox>

#include "wip-comms/config/liaison_config.pb.h"
#include "wip-comms/messages/groups.h"
#include "wip-comms/messages/gps.pb.h"
#include "wip-comms/messages/environmental.pb.h"
#include "wip-comms/messages/image_attributes.pb.h"

#include "goby/middleware/liaison/liaison_container.h"
#include "goby/middleware/multi-thread-application.h"


#include "progressive_imagery/goby/groups.h"
#include "progressive_imagery/progressive_imagery.pb.h"

namespace wip
{
    class ImageryCommsThread;
    
    class LiaisonImagery : public goby::common::LiaisonContainerWithComms<LiaisonImagery,
        ImageryCommsThread>
    {
    public:
        LiaisonImagery(const goby::common::protobuf::LiaisonConfig& cfg);

        void handle_updated_image(const dsl::protobuf::UpdatedImageEvent& event);
        void handle_received_status(const dsl::protobuf::ReceivedStatus& status);
        void handle_received_veh_status(const wip::protobuf::GPSPosition& status);
        void handle_received_env_data(const wip::protobuf::EnvironmentalData& env_data);
        void handle_received_image_attributes(const wip::protobuf::ImagesAttributes& attrs);
        
    private:
        void toggle_selection(const Wt::WMouseEvent& e, int id)
        {            
            auto& data = images_.at(id);

            // progressive imagery can only handle up to 10 updates at a time
            if(!data.selected && selected_ids.size() == 10)
            {
                info_text_->setText("You can only select up to 10 images at a time.");
                return;
            }

            data.selected = data.selected ? false : true;
            if(data.selected)
                selected_ids.insert(id);
            else
                selected_ids.erase(id);

                                      
            update_border(id);

        }
        
        
        void update_info(const Wt::WMouseEvent& e, int id)
        {
            const auto& last_status = images_.at(id).last_status;
            
            std::stringstream ss;
            ss << "image_id: " << id << "\n"
               << "received: " << last_status.fraction_received()*100 << "%";

            auto dccl_encoded_bytes = images_.at(id).dccl_encoded_bytes;
            if(dccl_encoded_bytes > 0)
                ss << " (" << static_cast<int>(last_status.fraction_received()*dccl_encoded_bytes) << "/" << dccl_encoded_bytes << "B)";
            
            
            ss << "\n";
            
            info_text_->setText("<pre>" + ss.str() + "</pre>");
            active_id = id;
        }

        void update_border(int id)
        {
            
            auto& data = images_.at(id);
            auto frac_rx = data.last_status.fraction_received();
            auto container = data.image_container;
            
            
            if(data.selected)
            {
                Wt::WBorder border(Wt::WBorder::Solid, Wt::WBorder::Medium, Wt::WColor(0, 0, 255));
                container->decorationStyle().setBorder(border);

            }
            else
            {
                Wt::WBorder border(Wt::WBorder::Solid, Wt::WBorder::Medium, Wt::WColor(255*(1-frac_rx), 255*frac_rx, 0));
                container->decorationStyle().setBorder(border);
            }

            // for some reason needs to be done to display border change
            container->disable();
            container->enable();

        }

        void create_image_container(int image_id, int xsiz = -1, int ysiz = -1, int dccl_encoded_bytes= -1);
        
    private:
        
        const protobuf::LiaisonImageryConfig& imagery_cfg_;
        

        Wt::WGroupBox* control_container_;
        Wt::WText* request_text_;
        Wt::WDoubleSpinBox* request_frac_;
        Wt::WPushButton* request_button_;
        
        Wt::WGroupBox* status_container_;
        Wt::WText* status_text_;
        Wt::WText* env_text_;

        Wt::WGroupBox* image_container_;

        struct ImageData
        {
            Wt::WContainerWidget* image_container;
            Wt::WImage* image { nullptr };
            bool image_loaded { false };
            int dccl_encoded_bytes { -1 };
            dsl::protobuf::ImageRxStatus last_status;
            bool selected { false };
        };
        
        std::map<int, ImageData> images_;

        Wt::WPanel* info_panel_;
        Wt::WText* info_text_;

        int active_id { -1 };

        std::set<int> selected_ids;

    };
    
     
    class ImageryCommsThread : public goby::common::LiaisonCommsThread<LiaisonImagery>
    {
    public:
    ImageryCommsThread(LiaisonImagery* wt_app, const goby::common::protobuf::LiaisonConfig& config, int index) :
        goby::common::LiaisonCommsThread<LiaisonImagery>(wt_app, config, index),
            wt_app_(wt_app)
            {
                interprocess().subscribe<dsl::progressive_imagery::groups::updated_image,
                    dsl::protobuf::UpdatedImageEvent>(
                        [this](const dsl::protobuf::UpdatedImageEvent& updated_image)
                        {
                            wt_app_->post_to_wt(
                                [=]() { wt_app_->handle_updated_image(updated_image); });   
                        });

                interprocess().subscribe<dsl::progressive_imagery::groups::received_status,
                    dsl::protobuf::ReceivedStatus>(
                        [this](const dsl::protobuf::ReceivedStatus& status)
                        {
                            wt_app_->post_to_wt(
                                [=]() { wt_app_->handle_received_status(status); });
                        });

                intervehicle().subscribe_dynamic<wip::protobuf::GPSPosition>(
                        [this](const wip::protobuf::GPSPosition& status)
                        {
                            wt_app_->post_to_wt(
                                [=]() { wt_app_->handle_received_veh_status(status); });
                        });

                intervehicle().subscribe_dynamic<wip::protobuf::EnvironmentalData>(
                        [this](const wip::protobuf::EnvironmentalData& env_data)
                        {
                            wt_app_->post_to_wt(
                                [=]() { wt_app_->handle_received_env_data(env_data); });
                        });
                
                intervehicle().subscribe_dynamic<wip::protobuf::ImagesAttributes>(
                        [this](const wip::protobuf::ImagesAttributes& attrs)
                        {
                            wt_app_->post_to_wt(
                                [=]() { wt_app_->handle_received_image_attributes(attrs); });
                        });

                
                
                
            }
            ~ImageryCommsThread()
            {
            }
            
        private:
            friend class LiaisonImagery;
            LiaisonImagery* wt_app_;
            
        };
    
    
}

#endif

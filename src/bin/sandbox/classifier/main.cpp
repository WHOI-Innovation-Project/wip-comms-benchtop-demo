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

#include "progressive_imagery/goby/groups.h"
#include "progressive_imagery/progressive_imagery.pb.h"

#include "wip-comms/messages/groups.h"
#include "wip-comms/config/classifier_config.pb.h"
#include "wip-comms/messages/image_attributes.pb.h"

using AppBase = goby::SingleThreadApplication<wip::protobuf::ClassifierConfig>;
using goby::glog;

class Classifier : public AppBase
{
public:
    Classifier() :
        AppBase()
        {
            interprocess().subscribe<dsl::progressive_imagery::groups::encode_report,
                                     dsl::protobuf::EncodeReport>(
                [this](const dsl::protobuf::EncodeReport& enc_report)
                {
                    
                    auto& attribute = *current_attributes_.add_attribute();
                    attribute.set_image_id(enc_report.encoded_header().image_id());
                    attribute.set_xsiz(enc_report.encoded_header().siz().xsiz());
                    attribute.set_ysiz(enc_report.encoded_header().siz().ysiz());
                    attribute.set_dccl_encoded_bytes(enc_report.dccl_encoded_bytes());

                    if(current_attributes_.attribute_size() == num_images_per_attribute_msg)
                    {
                        glog.is_debug1() && glog << current_attributes_.DebugString() << std::endl;

                        //interprocess().publish<>(current_attributes_);
                        current_attributes_.Clear();
                    }
                    
                }
                );
        }
private:
    wip::protobuf::ImagesAttributes current_attributes_;
    
    const int num_images_per_attribute_msg { static_cast<int>(wip::protobuf::ImagesAttributes::descriptor()->FindFieldByName("attribute")->options().GetExtension(dccl::field).max_repeat()) };
    
    
    
            
        
};


int main(int argc, char* argv[])
{ return goby::run<Classifier>(argc, argv); }

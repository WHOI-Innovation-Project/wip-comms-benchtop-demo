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

#include <Wt/WFileResource>

#include <Wt/WPanel>

#include <boost/filesystem.hpp>

#include "liaison_imagery.h"

using goby::glog;
using namespace Wt;

wip::LiaisonImagery::LiaisonImagery(const goby::common::protobuf::LiaisonConfig& cfg)
    : goby::common::LiaisonContainerWithComms<LiaisonImagery, ImageryCommsThread>(cfg),
    imagery_cfg_(cfg.GetExtension(wip::protobuf::imagery_config)),
    control_container_(new Wt::WGroupBox("Control", this)),
    request_text_(new Wt::WText("Fraction requested: ", control_container_)),
    request_frac_(new Wt::WDoubleSpinBox(control_container_)),
    request_button_(new Wt::WPushButton("Request", control_container_)),
    image_container_(new Wt::WGroupBox("Images", this)),
    info_panel_(new WPanel(image_container_)),
    info_text_(new WText)
{

    info_panel_->addStyleClass("fixed-left");
    info_panel_->setTitle("Image Information");
    info_panel_->setCentralWidget(info_text_);
    info_panel_->setCollapsible(true);
    
    request_button_->clicked().connect(
        [this](const Wt::WMouseEvent& e)
        {
            dsl::protobuf::QueueAction action;

            // copy so toggle_selection can remove them
            auto ids = selected_ids;
            for(auto id: ids)
            {
                auto& tx_action = *action.add_queue_action();
                tx_action.set_image_id(id);
                tx_action.set_fraction_goal(request_frac_->value());
                toggle_selection(Wt::WMouseEvent(), id);
            }

            this->post_to_comms([=]() { this->goby_thread()->interprocess().publish<dsl::progressive_imagery::groups::queue_action>(action); });
        }
        );

    request_frac_->setRange(0, 1);
    request_frac_->setSingleStep(0.1);
    request_frac_->setDecimals(1);
    request_frac_->setValue(1);
    
    set_name("WIPImagery");
}


void wip::LiaisonImagery::handle_updated_image(const dsl::protobuf::UpdatedImageEvent& event)
{
    //    glog.is_debug1() && glog << "updating image: " << event.ShortDebugString() << std::endl;
    boost::filesystem::path image(event.image_path());
    
    auto it = images_.find(event.image_id());
    if(it == images_.end())
    {
        auto c = new WContainerWidget(image_container_);
        c->setInline(true);
        c->setPadding(10);
        auto wimage = new WImage(new WFileResource(image.native()), c);

        images_.insert(std::make_pair(event.image_id(), ImageData({ c, wimage })));
        auto id = event.image_id();
        
        wimage->mouseWentOver().connect(boost::bind(&LiaisonImagery::update_info, this, _1, id));
        wimage->clicked().connect(boost::bind(&LiaisonImagery::toggle_selection, this, _1, id));
    }
    else
    {
        it->second.image->imageLink().resource()->setChanged();
    }
    
    
}

void wip::LiaisonImagery::handle_received_status(const dsl::protobuf::ReceivedStatus& status)
{
    for(const auto& image_rx : status.image_status())
    {
        auto it = images_.find(image_rx.image_id());
        if(it != images_.end())
        {
            // only update if changed
            if(!it->second.last_status.IsInitialized() || it->second.last_status.SerializeAsString() != image_rx.SerializeAsString())
            {
                it->second.last_status = image_rx;
                update_border(image_rx.image_id());
                
                if(image_rx.image_id() == active_id)
                    update_info(Wt::WMouseEvent(), image_rx.image_id());
            }
        }
    }
}

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

#include <Wt/WImage>
#include <Wt/WFileResource>

#include <boost/filesystem.hpp>

#include "liaison_imagery.h"

using goby::glog;
using namespace Wt;

wip::LiaisonImagery::LiaisonImagery(const goby::common::protobuf::LiaisonConfig& cfg)
    : goby::common::LiaisonContainerWithComms<LiaisonImagery, ImageryCommsThread>(cfg),
      imagery_cfg_(cfg.GetExtension(wip::protobuf::imagery_config)),
      main_layout_(new Wt::WVBoxLayout(this)),
      image_container_(new Wt::WContainerWidget(this))
{
    main_layout_->addWidget(image_container_);
    

    set_name("WIPImagery");
}


void wip::LiaisonImagery::handle_updated_image(const dsl::protobuf::UpdatedImageEvent& event)
{
    glog.is_debug1() && glog << "updating image: " << event.ShortDebugString() << std::endl;
    boost::filesystem::path image(event.image_path());

    auto png_file = image.parent_path() / image.stem();
    png_file += ".png";

    std::string convert_cmd = "convert " + image.native() + " " + png_file.native();
    
    system(convert_cmd.c_str());
    
    auto it = images_.find(event.image_id());
    if(it == images_.end())
    {
        auto c = new WContainerWidget(image_container_);
        c->setInline(true);
        c->setPadding(10);
        auto wimage = new WImage(new WFileResource(png_file.native()), c);
        images_.insert(std::make_pair(event.image_id(), wimage));
    }
    else
    {
        it->second->imageLink().resource()->setChanged();
    }
    
    
}


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

using namespace Wt;

wip::LiaisonImagery::LiaisonImagery(const goby::common::protobuf::LiaisonConfig& cfg)
    : imagery_cfg_(cfg.GetExtension(wip::protobuf::imagery_config)),
      main_layout_(new Wt::WVBoxLayout(this)),
      image_container_(new Wt::WContainerWidget(this))
{
    main_layout_->addWidget(image_container_);
    
    boost::filesystem::path image_dir(imagery_cfg_.image_dir());
    if(!boost::filesystem::exists(image_dir))
    {
        goby::glog.is_warn() && goby::glog << "No such directory: " << image_dir << std::endl;
    }
    else
    {
        
        std::vector<boost::filesystem::path> images;
        std::copy(boost::filesystem::directory_iterator(image_dir),
                  boost::filesystem::directory_iterator(),
                  std::back_inserter(images));

        std::sort(images.begin(), images.end());  

        for(const auto& image : images)
        {
            auto c = new WContainerWidget(image_container_);
            c->setInline(true);
            c->setPadding(10);
            new WImage(new WFileResource(image.native()), c);
        }
        
    }
    
    

    set_name("WIPImagery");
}


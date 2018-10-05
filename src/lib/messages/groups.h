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

#ifndef WIP_COMMS_GROUPS20180712H
#define WIP_COMMS_GROUPS20180712H

#include "goby/middleware/group.h"

namespace wip
{
    namespace groups
    {    
        constexpr goby::Group ready{"wip::ready"};        
        constexpr goby::Group status{"wip::status"};

        namespace gps
        {
            constexpr goby::Group raw_in{"wip::gps::raw_in"};
            constexpr goby::Group raw_out{"wip::gps::raw_out"};
            constexpr goby::Group data{"wip::gps"};
        }

        namespace env
        {
	     constexpr goby::Group data{"wip::env"};
        }

        namespace image
        {
            constexpr goby::Group attributes{"wip::image::attributes"};
        }
    }
}

#endif

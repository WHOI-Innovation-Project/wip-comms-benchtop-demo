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

#include "goby/middleware/liaison/liaison_container.h"
#include "goby/middleware/multi-thread-application.h"

namespace wip
{
    class LiaisonImagery : public goby::common::LiaisonContainer
    {
    public:
        LiaisonImagery(goby::SimpleThread<goby::common::protobuf::LiaisonConfig>* goby_thread, const goby::common::protobuf::LiaisonConfig& cfg, Wt::WContainerWidget* parent = 0);
            
    private:
        Wt::WVBoxLayout* main_layout_;
            
    };
}

#endif

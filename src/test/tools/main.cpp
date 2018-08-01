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
#include "goby/middleware/liaison/groups.h"

#include "config.pb.h"
#include "gps.pb.h"


using AppBase = goby::SingleThreadApplication<LiaisonTestConfig>;

class LiaisonTest : public AppBase
{
public:
    LiaisonTest()
        {
            interprocess().subscribe<goby::liaison::groups::commander_out,
                                     wip::protobuf::GPSPosition>(
                                         [](const wip::protobuf::GPSPosition& pos)
                                         {
                                             std::cout << "Received: " << pos.DebugString() << std::endl;
                                         }
                                         );         
                                     
        }
};




int main(int argc, char* argv[])
{ return goby::run<LiaisonTest>(argc, argv); }

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

#include "groups.h"
#include "config.pb.h"
#include "gps.pb.h"
#include "status.pb.h"

using AppBase = goby::SingleThreadApplication<LiaisonTestConfig>;

class LiaisonTest : public AppBase
{
public:
    LiaisonTest() : AppBase(1*boost::units::si::hertz)
        {
            // intervehicle().subscribe<goby::liaison::groups::commander_out,
            //                          wip::protobuf::GPSPosition>(
            //                              [](const wip::protobuf::GPSPosition& pos)
            //                              {
            //                                  std::cout << "Received: " << pos.DebugString() << std::endl;
            //                              }
            //                              );         

            intervehicle().subscribe_dynamic<wip::protobuf::Status>([](const wip::protobuf::Status& status)
                                                            {
                                                                std::cout << "IV: Received: " << status.DebugString() << std::endl;
                                                            }
                                         );

            interprocess().subscribe<wip::groups::status,
                                     wip::protobuf::Status>([](const wip::protobuf::Status& status)
                                                            {
                                                                std::cout << "IP: Received: " << status.DebugString() << std::endl;
                                                            }
                                         );
            
        }

    void loop() override
        {
            wip::protobuf::Status status;
            status.set_time_with_units(goby::time::now());
            status.set_src(cfg().modem_id());
            intervehicle().publish<wip::groups::status>(status);
            std::cout << "Sending: " << status.ShortDebugString() << std::endl;
        }
    
};




int main(int argc, char* argv[])
{ return goby::run<LiaisonTest>(argc, argv); }

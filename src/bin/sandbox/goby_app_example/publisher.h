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

#ifndef MULTITHREAD_PUBLISHER_H
#define MULTITHREAD_PUBLISHER_H

#include "goby/middleware/multi-thread-application.h"

#include "wip-comms/messages/gps.pb.h"
#include "wip-comms/messages/groups.h"

#include "wip-comms/config/gps_config.pb.h"

class BasicPublisher : public goby::SimpleThread<wip::protobuf::GPSDriverConfig>
{
public:
BasicPublisher(const wip::protobuf::GPSDriverConfig& config)
    : goby::SimpleThread<wip::protobuf::GPSDriverConfig>(config, 10 /*hertz*/)
        {
            using goby::glog;
            using namespace goby::common::logger;
        }

    void loop() override
        {
            using goby::glog;
            using namespace goby::common::logger;

            wip::protobuf::GPSPosition gps;
            gps.set_latitude(10 + std::rand() % 5);
            gps.set_longitude(12 + std::rand() % 10);
            
            glog.is(VERBOSE) && glog << "Tx: " << gps.DebugString() << std::flush;
            interthread().publish<wip::groups::gps>(gps);
        }    
};

#endif

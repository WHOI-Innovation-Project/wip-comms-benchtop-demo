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

#ifndef MULTITHREAD_SUBSCRIBER_H
#define MULTITHREAD_SUBSCRIBER_H

#include "goby/middleware/single-thread-application.h"

#include "wip-comms/messages/gps.pb.h"
#include "wip-comms/messages/groups.h"

#include "wip-comms/config/gps_config.pb.h"

using ThreadBase = goby::SimpleThread<wip::protobuf::GPSDriverConfig>;

class BasicSubscriber : public ThreadBase
{
public:
BasicSubscriber(const wip::protobuf::GPSDriverConfig& config, int index)
    : ThreadBase(config, 0, index)
        {
            auto gps_callback = [this] (const wip::protobuf::GPSPosition& gps)
                { this->incoming_gps(gps); };

            // subscribe only on the interthread layer
            interthread().subscribe<wip::groups::gps, wip::protobuf::GPSPosition>(gps_callback);
        }

    void incoming_gps(const wip::protobuf::GPSPosition gps)
        {
            using goby::glog;
            using namespace goby::common::logger;

            glog.is(VERBOSE) && glog <<  "Rx: " << ThreadBase::index() <<  ": " << gps.DebugString() << std::flush;
        }
};

#endif

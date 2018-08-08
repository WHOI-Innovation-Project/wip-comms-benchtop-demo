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

#ifndef REGEX_MULTITHREAD_SUBSCRIBER_H
#define REGEX_MULTITHREAD_SUBSCRIBER_H

#include "goby/middleware/multi-thread-application.h"

#include "wip-comms/messages/groups.h"
#include "wip-comms/messages/status.pb.h"
#include "config.pb.h"

using ThreadBase = goby::SimpleThread<wip::protobuf::RegexConfig>;

class RegexSubscriber : public ThreadBase
{
public:
RegexSubscriber(const wip::protobuf::RegexConfig& config, int index)
    : ThreadBase(config, this->loop_max_frequency(), index)
    {
        using goby::glog;
        
        interprocess().subscribe_regex(
            [this](const std::vector<unsigned char>& data,
                   int scheme,
                   const std::string& type,
                   const goby::Group& group)
            {
                auto pb_msg = dccl::DynamicProtobufManager::new_protobuf_message<std::unique_ptr<google::protobuf::Message>>(type);
                pb_msg->ParseFromArray(&data[0], data.size());
                
                glog.is_debug1() && glog << "Received " << type << "/" << group << " msg: " << pb_msg->ShortDebugString() << std::endl;

            },
            { goby::MarshallingScheme::PROTOBUF },
            "wip\\.protobuf\\.Status", "wip::status");
    }

    void loop() override
    {
        goby::glog.is_debug1() && goby::glog << "loop" << std::endl;
        usleep(100000);
        
    }
    
};

#endif

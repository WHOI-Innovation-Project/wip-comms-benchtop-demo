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

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

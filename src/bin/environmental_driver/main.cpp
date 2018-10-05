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

#include <linux/i2c-dev.h>
#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>
#include <unistd.h>

#include <boost/units/io.hpp>

#include "goby/middleware/single-thread-application.h"

#include "wip-comms/messages/groups.h"
#include "wip-comms/messages/environmental.pb.h"
#include "wip-comms/config/environmental_driver_config.pb.h"

using AppBase = goby::SingleThreadApplication<wip::protobuf::EnvironmentalDriverConfig>;
using goby::glog;

class EnvironmentalDriver : public AppBase
{
public:
    EnvironmentalDriver() :
        AppBase(1*boost::units::si::hertz)
        {
            i2c_file_ = open(cfg().i2c_dev().c_str(), O_RDWR); 
            const int addr = cfg().humidistat_addr();
            if (ioctl(i2c_file_, I2C_SLAVE, addr) < 0) 
            {
                glog.is_die() && glog << "Failure setting addr: " << addr << std::endl;
            }
        }
private:
    void loop() override
        {
            wip::protobuf::EnvironmentalData env_data;
            env_data.set_time_with_units(goby::time::now());
            { 
                std::uint8_t reg = 0xE5; // Measure relative humidity (hold master mode)
                std::int16_t res = 0;

                char buf[10];
                buf[0] = reg;
                write(i2c_file_, buf, 1);


                int read_res = read(i2c_file_, buf, 2);
                while(read_res != 2) {
                    usleep(1000);
                    read_res = read(i2c_file_, buf, 2);
                } 

                res |= buf[1];
                res |= (buf[0] << 8);
   
                glog.is_debug1() && glog << std::hex << res << std::endl;
                auto rel_humidity = ((125.0*res)/65536)-6;
                glog.is_debug1() && glog << "Relativity Humidity (%): " << rel_humidity << std::endl;
                env_data.set_humidity(rel_humidity);
            }

            {
                std::uint8_t reg = 0xE3; // Measure temp (hold master mode)
                std::int16_t res = 0;

                char buf[10];
                buf[0] = reg;
                write(i2c_file_, buf, 1);

                int read_res = read(i2c_file_, buf, 2);
                while(read_res != 2) {
                    usleep(1000);
                    read_res = read(i2c_file_, buf, 2);
                } 

                res |= buf[1];
                res |= (buf[0] << 8);
   
                glog.is_debug1() && glog << std::hex << res << std::endl;
                auto temp((((175.72*res)/65536)-46.85)*boost::units::absolute<boost::units::celsius::temperature>());
                glog.is_debug1() && glog << "Temp (deg C): " << temp << std::endl;
                env_data.set_temp_with_units(temp);
            }
            double secs = env_data.time_with_units<goby::time::SITime>()/boost::units::si::seconds;
            if(static_cast<int>(secs) % cfg().intervehicle_period() == 0)
            {
                glog.is_debug1() && glog << "Publishing intervehicle" << std::endl;
                intervehicle().publish<wip::groups::env::data>(env_data);
            }
            else
            {
                interprocess().publish<wip::groups::env::data>(env_data);
            }
        }
private:
    int i2c_file_;
};


int main(int argc, char* argv[])
{ return goby::run<EnvironmentalDriver>(argc, argv); }

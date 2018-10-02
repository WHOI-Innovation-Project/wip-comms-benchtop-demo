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

#include "cgsn-mooring/serial/ascii_line_based_serial.h"
#include "wip-comms/messages/groups.h"
#include "wip-comms/messages/gps.pb.h"
#include "wip-comms/config/gps_config.pb.h"
#include "goby/util/linebasedcomms/nmea_sentence.h"
#include "goby/util/geodesy.h"

using AppBase = goby::MultiThreadApplication<wip::protobuf::GPSDriverConfig>;
using goby::glog;

class GPSDriver : public AppBase
{
public:
    GPSDriver() :
        AppBase(),
        geodesy_({ cfg().origin().lat_with_units(), cfg().origin().lon_with_units() })
        {
            interprocess().subscribe<wip::groups::gps::raw_in, cgsn::protobuf::SensorRaw>(
                [this](const cgsn::protobuf::SensorRaw& raw)
                {
                    parse(raw);
                }
                );
            
            
            launch_thread<cgsn::io::AsciiLineSerialThread<wip::groups::gps::raw_in, wip::groups::gps::raw_out>>(cfg().serial());
        }

private:
    void parse(const cgsn::protobuf::SensorRaw& raw);
    using Quantity_of_Microseconds = boost::units::quantity<decltype(boost::units::si::micro*boost::units::si::seconds), std::int64_t>;
    Quantity_of_Microseconds nmea_time_to_microseconds(int nmea_time, int nmea_date);
            
    using Quantity_of_Degrees = boost::units::quantity<boost::units::degree::plane_angle>;
    Quantity_of_Degrees nmea_geo_to_degrees(double nmea_geo, char hemi);

private:
    goby::util::UTMGeodesy geodesy_;
};


int main(int argc, char* argv[])
{ return goby::run<GPSDriver>(argc, argv); }

void GPSDriver::parse(const cgsn::protobuf::SensorRaw& raw)
{
    try
    {
        goby::util::NMEASentence nmea(raw.raw_data());

        if(nmea.sentence_id() == "RMC")
        {
            glog.is_verbose() && glog << "[Parser]: Parsing RMC: " << raw.raw_data() << std::flush;

            
            wip::protobuf::GPSPosition position;
            
            enum RMCFields {
                TALKER = 0,
                UTC = 1,
                VALIDITY = 2,
                LAT = 3,
                LAT_HEMI = 4,
                LON = 5,
                LON_HEMI = 6,
                SOG_KNOTS = 7,
                TRACK_DEGREES = 8,
                UT_DATE = 9,
                MAG_VAR_DEG = 10,
                MAG_VAR_HEMI = 11,
                RMC_SIZE = 12};

            if(nmea.size() < RMC_SIZE)
                throw(goby::util::bad_nmea_sentence("Message too short"));
            
            position.set_time_with_units(nmea_time_to_microseconds(nmea.as<float>(UTC), nmea.as<int>(UT_DATE)));

            bool fix_valid = nmea.as<char>(VALIDITY) == 'A';
            position.set_fix_valid(fix_valid);
            
            if(fix_valid)
            {
                position.set_latitude_with_units(nmea_geo_to_degrees(nmea.as<double>(LAT), nmea.as<char>(LAT_HEMI)));
                position.set_longitude_with_units(nmea_geo_to_degrees(nmea.as<double>(LON), nmea.as<char>(LON_HEMI)));
            }
            
            
            glog.is_debug1() && glog << "[Parser]: Parsed message is: " << position.ShortDebugString() << std::endl;

            if(static_cast<int>(position.time()) % cfg().intervehicle_period() == 0)
            {
                glog.is_debug1() && glog << "Publishing intervehicle" << std::endl;
                intervehicle().publish<wip::groups::gps::data>(position);
            }
            else
            {
                interprocess().publish<wip::groups::gps::data>(position);
            }
        }
        else
        {
            //            glog.is_verbose() && glog << "[Parser]: Ignoring sentence: " << raw.raw_data() << std::flush;
        }
    }
    catch(goby::util::bad_nmea_sentence& e)
    {
        glog.is_warn() && glog << "[Parser]: Invalid NMEA sentence: " << e.what() << std::endl;
    }               

}


GPSDriver::Quantity_of_Microseconds GPSDriver::nmea_time_to_microseconds(int nmea_time, int nmea_date)
{
    namespace si = boost::units::si;
    static const auto microseconds = si::micro*si::seconds;
    
    using namespace boost::posix_time;
    using namespace boost::gregorian;
    ptime unix_epoch(date(1970,1,1),time_duration(0,0,0));
    
    int hours = nmea_time / 10000;
    nmea_time -= hours*10000;
    int minutes = nmea_time / 100;
    nmea_time -= minutes*100;
    int seconds = nmea_time;

    int day = nmea_date / 10000;
    nmea_date -= day*10000;
    int month = nmea_date / 100;
    nmea_date -= month*100;
    int year = nmea_date;

    try
    {
        ptime given_time(date((year < 70) ? (year + 2000) : (year + 1900), month, day),
                         time_duration(hours, minutes, seconds));
        
        if (given_time == not_a_date_time)
        {
            return -1*microseconds;
        }
        else
        {
            const int microsec_in_sec = 1000000;

            date_duration date_diff = given_time.date() - date(1970,1,1);
            time_duration time_diff = given_time.time_of_day();
        
            return
                (static_cast<std::int64_t>(date_diff.days())*24*3600*microsec_in_sec + 
                static_cast<std::int64_t>(time_diff.total_seconds())*microsec_in_sec +
                static_cast<std::int64_t>(time_diff.fractional_seconds()) /
                 (time_duration::ticks_per_second() / microsec_in_sec)) *microseconds;
        }
    }
    catch(std::exception& e)
    {
        return -1*microseconds;
    }
}


GPSDriver::Quantity_of_Degrees GPSDriver::nmea_geo_to_degrees(double nmea_geo, char hemi)
{
    // DDMM.MMMM
    double deg_int = std::floor(nmea_geo / 1e2);
    double deg_frac = (nmea_geo - (deg_int * 1e2)) / 60;

    double deg = std::numeric_limits<double>::quiet_NaN();
    
    if(hemi == 'N' || hemi == 'E')
        deg = (deg_int + deg_frac);
    else if(hemi == 'S' || hemi == 'W')
        deg = -(deg_int + deg_frac);

    return deg*boost::units::degree::degrees;
}

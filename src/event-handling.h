/*
 *   Copyright (C) 2017 Event-driven Perception for Robotics
 *   Author: arren.glover@iit.it
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <yarp/os/all.h>
#include <iCub/eventdriven/all.h>
#include <mutex>
#include <string>

using namespace ev;
using namespace yarp::os;

/******************************************************************************/
//rateCalcThread
/******************************************************************************/
class rateCalcThread : public Thread
{
private:

    std::mutex m;
    yarp::sig::Vector rates; //[left-rate right-rate]

    std::string port_name;
    vReadPort<AE> input_port;

    double left_period;
    double right_period;
    unsigned int left_count;
    unsigned int right_count;

public:

    void setInputPortName(std::string name);
    yarp::sig::Vector getRate();

    virtual bool threadInit();
    virtual void run();
    virtual void onStop();

};

/******************************************************************************/
//eventRateConfiguration
/******************************************************************************/
class eventRateConfiguration : public RFModule
{

private:

    double period;

    rateCalcThread rate_calculator;
    BufferedPort<yarp::sig::Vector> rateOutPort;

public:

    eventRateConfiguration()  {}
    bool configure(yarp::os::ResourceFinder &rf);

    bool interruptModule();
    double getPeriod();
    bool updateModule();

};

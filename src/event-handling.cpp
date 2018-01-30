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

#include "event-handling.h"
#include <math.h>

/******************************************************************************/
//main
/******************************************************************************/
int main(int argc, char * argv[])
{
    /* initialize yarp network */
    Network yarp;
    if(!yarp.checkNetwork()) {
        yError() << "Could not find YARP network";
        return -1;
    }

    /* prepare and configure the resource finder */
    ResourceFinder rf;
    rf.setDefaultConfigFile("event-rate-calc.ini");
    rf.setDefaultContext("eventdriven");
    rf.configure(argc, argv);

    /* instantiate the module */
    eventRateConfiguration mymodule;
    return mymodule.runModule(rf);
}

/******************************************************************************/
//rateCalcThread
/******************************************************************************/

void rateCalcThread::setInputPortName(std::string name)
{
    port_name = name;
}

yarp::sig::Vector rateCalcThread::getRate()
{

    // FILL IN CODE HERE
    // calculate the event rate from count and period.
    // remember to scale the time value

    return rates;

}

bool rateCalcThread::threadInit()
{
    rates.resize(2, 0.0);
    return q_alloc.open(port_name + "/vBottle:i");
}

void rateCalcThread::run()
{

    yarp::os::Stamp stamp;
    while(!isStopping()) {

        // FILL IN CODE HERE
        // ask for a q, count the left and right events, count the time period
        // deallocate the q.
        // what else can we count?

    }
}

void rateCalcThread::onStop()
{
    q_alloc.close();
    q_alloc.releaseDataLock();
}

/******************************************************************************/
//eventRateConfiguration
/******************************************************************************/
bool eventRateConfiguration::configure(yarp::os::ResourceFinder &rf)
{
    //get the module update rate
    period = rf.check("period", yarp::os::Value(0.01)).asDouble();

    //get the module name
    std::string name = rf.check("name", yarp::os::Value("/vRate")).asString();

    //open the output port displaying the rate
    if(!rateOutPort.open(name + "/rate:o")) {
        return false;
    }

    //set the input port name and start the thread
    rate_calculator.setInputPortName(
                rf.check("name", yarp::os::Value("/vRate")).asString());
    return rate_calculator.start();
}

double eventRateConfiguration::getPeriod()
{
    return period;
}

bool eventRateConfiguration::updateModule()
{
    // FILL IN CODE HERE
    // get the rates and output it to the out port

    return !isStopping();
}

bool eventRateConfiguration::interruptModule() {
    rate_calculator.stop();
    rateOutPort.close();
    return yarp::os::RFModule::interruptModule();
}






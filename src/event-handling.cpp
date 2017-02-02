#include "event-handling.h"
#include <math.h>

/******************************************************************************/
//main
/******************************************************************************/

int main(int argc, char * argv[])
{
    /* initialize yarp network */
    yarp::os::Network yarp;

    /* prepare and configure the resource finder */
    yarp::os::ResourceFinder rf;
    rf.setDefaultConfigFile("spikingModel.ini");
    rf.setDefaultContext("eventdriven");
    rf.configure(argc, argv);

    /* instantiate the module */
    eventHandlerConfiguration mymodule;
    return mymodule.runModule(rf);
}

/******************************************************************************/
//spikingConfiguration
/******************************************************************************/
bool eventHandlerConfiguration::configure(yarp::os::ResourceFinder &rf)
{
    return eh.initialise(rf.check("name", yarp::os::Value("/vTutorialEventHandling")).asString(),
                                   rf.check("strict", yarp::os::Value(true)).asBool(),
                                   rf.check("height", yarp::os::Value(240)).asInt(),
                                   rf.check("width", yarp::os::Value(304)).asInt());
}


/******************************************************************************/
//spikingModel
/******************************************************************************/
bool eventHandler::initialise(std::string name, bool strict, unsigned int height, unsigned int width)
{

    this->strict = strict;
    if(strict) {
        std::cout << "Setting " << name << " to strict" << std::endl;
        setStrict();
    }

    this->useCallback();
    if(!open(name + "/vBottle:i"))
        return false;
    if(!outputPort.open(name + "/vBottle:o"))
        return false;

    this->height = height;
    this->width = width;

    return true;

}

void eventHandler::onRead(vBottle &input)
{
    //remove the left camera and flip the image on the y-axis
    //port envelopes, vQueue, getas, ...

    //FILL IN THE CODE HERE

}

void eventHandler::interrupt()
{
    outputPort.interrupt();
    yarp::os::BufferedPort<vBottle>::interrupt();
}

void eventHandler::close()
{
    outputPort.close();
    yarp::os::BufferedPort<vBottle>::close();
}

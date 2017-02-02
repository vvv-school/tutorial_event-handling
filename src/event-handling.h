#include <yarp/os/all.h>
#include <yarp/sig/Matrix.h>
#include <yarp/sig/Image.h>
#include <iCub/eventdriven/all.h>

using ev::event;
using ev::vBottle;
using ev::AddressEvent;
using ev::vQueue;
using ev::getas;

class eventHandler : public yarp::os::BufferedPort<vBottle>
{
private:

    //parameters
    bool strict;

    int height;
    int width;

    //ouput ports
    yarp::os::BufferedPort<vBottle> outputPort;


public:

    eventHandler() : strict(false) {}
    bool initialise(std::string name, bool strict = false, unsigned int height = 240, unsigned int width = 304);
    void close();
    void interrupt();

    void onRead(vBottle &input);

};

class eventHandlerConfiguration : public yarp::os::RFModule
{

private:

    eventHandler eh;

public:

    eventHandlerConfiguration()  {}
    bool configure(yarp::os::ResourceFinder &rf);

    bool interruptModule() {
        eh.interrupt();
        return yarp::os::RFModule::interruptModule();
    }
    bool close() {
        eh.close();
        return yarp::os::RFModule::close();
    }
    double getPeriod() { return 1.0; }
    bool updateModule() { return !isStopping(); }

};

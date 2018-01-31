# Tutorial on Handling the Event Driven Data Stream
This tutorial introduces the basic components for handling event-driven data stream. You will use a simple application to learn the dedicated data structures and functions in the event-driven library and the basic components required to create an event-driven module.

Main topics:
- Event definition in c++
- How events are stored in packets
- How to read/write events to YARP ports

### Requirements:
You should be familiar with the framework used by event-driven, and the required software and datasets, introduced in the [first tutorial](https://github.com/vvv-school/tutorial_event-driven-framework).

# The event-driven library

The event-driven library allows you to use datastructures and helper modules to read and write events on YARP ports, perform basic filtering, and more complex organisation. Generic access to timing information depending on the hardware is also available. To include the libraries and use the namespace:

```c++
#include "iCub/eventdriven/all.h"
using namespace ev;
``` 

### Code: events:

There are several types of events in the event-driven library (we will use just one) and therefore we define a base class of event, the vEvent:

```c++
class vEvent
{
public:
    static const std::string tag;
    unsigned int stamp:31;

    vEvent();
    virtual ~vEvent();

    virtual event<> clone();
    virtual void encode(yarp::os::Bottle &b) const;
    virtual void encode(std::vector<YARP_INT32> &b, unsigned int &pos) const;
    virtual bool decode(const yarp::os::Bottle &packet, int &pos);
    virtual yarp::os::Property getContent() const;
    virtual std::string getType() const;
    virtual int getChannel() const;
    virtual void setChannel();
};

``` 
The vEvent only contains a tag and a timestamp, which that __something__ happened at a specific time. Encoding and decoding functions specify the bit order of data such that software and hardware are compatible. We can therefore read and write to neuromorphic hardware without **too much** trouble. Events are encoded and loaded into ``yarp::os::Bottles`` for transmission within the YARP framework.

The event cameras produce events with a sensor array address space, and the vEvent is not enough to represent this extra information. The AddressEvent is inherited from the vEvent and contains more datafields and the tag will be different: 

```c++
class AddressEvent : public vEvent
{
public:
    static const std::string tag;
    unsigned int x:10; // u position of the camera pixel
    unsigned int y:10; // v position of the camera pixel
    unsigned int channel:1; // left/right camera
    unsigned int polarity:1; //increase / decrease in light
    
 ...
```
Other events can define such things as velocity, a spatial distribution, or a point in 3D space by extending the data fields further.

When allocating events, we wrap them using shared pointers to avoid excessive memory allocation as events are passed around a module. In many applications we don't know exactly for how long we need to keep an event and how often we might need to use it for processing; we don't have a frame for which we know we can compute all pixels together. Shared pointers simplifies the handling of memory. If you are familiar with shared\_pts:
```c++
event<vEvent>
```
is the same as
```c++
std::shared_ptr<vEvent>
```
The above definition will be a (shared) pointer to any type of event, as all events derive from the vEvent class. In almost all cases events should be instantiated using the ``event<>`` form and **not directly**.

## Code: vQueue:

When you read events from the port (e.g. from ``/zynqGrabber/vBottle:o``) you will get multiple events in a packet. This packet is stored in a ``ev::vQueue``. The vQueue is a wrapper for a `std::deque< event<> >` so we can:
```c++
vQueue q;
q.push_front(v); q.push_back(v);
q.pop_front(v); q.pop_back(v);
```
Events are stored in temporal order - which can be exploited for processing purposes. Typically we don't need to make new events from scratch as the ``zynqGrabber`` module does it for us. The vQueue can hold *any* type of event, and instead we typically need to access events as a *specific* types:
```c++
auto v2 = is_event<AddressEvent>(v1);
```
and also upgrade events by adding new information to the previous event.
```c++
auto v2 = make_event<FlowEvent>(v1);
v2->vx = ...
v2->vy = ...
```
So to process the event stream, we typically iterate through the ``vQueue`` and perform some processing:

```c++
vQueue q, q_corner;
for(size_t i = 0; i < q.size(); i++) {
  //access events
  auto v = is_event<AddressEvent>(q.at(i));
  //do some processing
  bool is_corner = detect_corner(v);
  //and upgrade the event if needed
  if(is_corner) {
    auto v_corner = make_event<InterestEvent>(v);
    q_corner.push_back(v_corner);
  }
}
```

## Code: qAllocator:

The ``ev::qAllocator`` is a class that uses a ``yarp::os::BufferedPort`` to read events asynchronously (it uses a callback) and allocates the ``ev::vQueue`` for each packet. 


To put all this together we will complete a tutorial that reads events and calculates the event rate.

![ev_hand](./misc/tutorial1.png)

Your module will 

1. read events using the callback function (OnRead), 
1. compute the event rate and display the result in **yarpscope**
1. modify the address events to correct the dataset.
1. use **vFramer** to create images and display them with **yarpview**


###### AE
A simple view

![ae](./misc/AE.png)

###### ISO
A more beautiful view

![iso](./misc/iso.png)

###### FLOW
Visualise optical flow (we'll see this in the next tutorial)

![flow](./misc/AEFLOW.png)


























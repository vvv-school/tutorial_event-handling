# Tutorial on Handling Event Driven Data Stream -- Handling Events
This tutorial introduces the basic components for handling event-driven data stream. You will use a simple application to learn the dedicated data structures and functions in the event-driven library and the basic components required to create an event-driven module.

# Requirements:
Your current set-up comprises YARP, iCub (some of the event-driven modules require iCub) and iCubContrib (for easy install of the executables). In addition you need the **robotology-playground/event-driven**(https://github.com/robotology-playground/event-driven.git) library.

### Note: 
The event-driven code is already installed in your virtual machine (alternatively, you should have followed the installing instructions in the vvv [wiki](https://github.com/vvv-school/vvv-school.github.io/blob/master/instructions/how-to-prepare-your-system.md#install-event-driven)), however, as there have been few updates, you need to `git pull`. 

# The event-driven library

The event-driven library allows you to use datastructures and helper modules to handle events. 

```javascript
#include "iCub/eventdriven/all.h"
using namespace ev;
``` 

In this tutorial you will learn:

- Datastructures
  - events
  - vBottle
  - vQueue

- Modules
  - vFramer

Some other useful (but not covered today) modules are:
- event storage structures (e.g. surface), 
- camera un-distortion, 
- salt and pepper filter, 
- robot interfaces, 
- circle detection, 
- cluster tracking 
- ...

### Events in event-driven libraries:

```javascript
EXAMPLE OF VEVENT - snapshot of code - store time
//FILL HERE THE CODE

``` 

- ADDRESSEVENT (AE)  - adds coordinates (x, y), channel (left, right) and polarity (on, off)

### YARP vBottle
How do we send events between processes?

**Using YARP obviously!**

Event Packets (vBottle):

The vBottle is a class that inherits from a standard YARP bottle so we can:

1. can be sent over YARP ports/bufferedports without modification
1. instant compatibility with yarpdataplayer and yarpdatadumper
1. we use the flexibility and search-ability of bottles (so we can send different event types easily - we'll see them later)

However you can only put and get vEvents with a vBottle. It wraps some encoding and decoding functions.

Code to use a vBottle:

```javascript
vBottle vbot;
event<> v;
//adding events is simple
vbot.add(v);

//you can get all events, get a specific event type, ensure sorted (by timestamp)
vQueue = vbot.get(v);
vQueue = vbot.getSorted<AddressEvent>(v);
```
The vQueue is a wrapper for a `std::deque< event<> >` so we can:

```javascript
vQueue q;
event<> v;

q.push_front(v);
q.push_back(v);
q.pop_front(v);
q.pop_back(v);
```

and we can iterate through the q to access events:

```javascript
vQueue::iterator qi;
for(qi = q.front(); qi != q.end(); qi++) {

    //access events
    event<AddressEvent> = getas<AddressEvent>(*qi);
    //do some processing

}
```

Our test application will be:

![ev_hand](./misc/tutorial1.png)

Your module will 

1. read events in the callback function (OnRead), 
1. compute the event rate and display the result in **yarpscope**
1. modify the address events in the bottle (as you wish) 
1. use **vFramer** to create images and display them with **yarpview**

### Modules: vFramer

Unlike standard cameras, there is no "images" or "frames" when using events so we need to create an image frame from recent events that have occurred if we want to visualise the output on a synchronous display. This means that we are going to grab all of the events within a given time window (e.g. 30ms) and create a frame.

The vFramer has different modes of operation:

###### AE

![ae](./misc/ae.png)

###### ISO

![iso](./misc/iso.png)

###### FLOW

![flow](./misc/flow.png)


























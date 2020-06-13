#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include <chrono>
#include <iostream>
#include <mutex>
#include <list>
#include <numeric>
#include <Queue.h>

using namespace std;
using namespace std::chrono_literals;

namespace driver {

class Processor {
public:
    Processor(Queue* msg_queue, double meters_per_pulse);
    virtual ~Processor();
    void run();
    Queue* msg_queue;
    int msgs_lost = 0;
    double mpp;
    double dist_total = 0;
    uint64_t pulses_total = 0;
private:
    list<Message> msgs_buff;
    chrono::time_point<chrono::high_resolution_clock> prev_t;
    unsigned int calc_msgs_lost(const Message& msg_prev, const Message& msg_curr);
};

} /* namespace driver */

#endif /* PROCESSOR_H_ */

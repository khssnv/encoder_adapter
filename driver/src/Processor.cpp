#include <Processor.h>

namespace driver {

Processor::Processor(Queue* msg_queue, double meters_per_count):
        msg_queue(msg_queue), mpp(meters_per_count)
{
    prev_t = chrono::high_resolution_clock::now();
}

Processor::~Processor()
{
}

/*
unsigned int Processor::calc_msgs_lost(const Message& msg_prev, const Message& msg_curr)
{
    return 0;
}
*/

void Processor::run()
{
    Message msg_prev;
    Message msg_curr;

    cout << "Starting proc loop" << endl;

    for(;;)
    {
        unique_lock<mutex> lck(msg_queue->m);
        msg_queue->cv.wait(lck);
        const auto now = chrono::high_resolution_clock::now();
        while (!msg_queue->q_ptr->empty())
        {
            msg_curr = msg_queue->q_ptr->front();
            msgs_buff.push_back(msg_curr);
            // msg_curr.print();
            msg_queue->q_ptr->pop();
        }
        auto dt_ms = chrono::duration_cast<chrono::milliseconds>(now - prev_t);
        if(dt_ms > 1000ms)
        {
            int new_pulses = accumulate(msgs_buff.begin(), msgs_buff.end(),
                                        0,
                                        [](int acc, const Message& msg) { return acc + (int)msg.count; });
            pulses_total += (uint64_t)new_pulses;
            dist_total += mpp * new_pulses;
            cout << "Msgs count: " << msgs_buff.size() << ", new pulses: " << new_pulses << ", new dist: " << new_pulses * mpp
                 << ", pulses total: " << pulses_total << ", dist total: " << dist_total << endl;
            msgs_buff.erase(msgs_buff.begin(), msgs_buff.end());
            prev_t = now;
        }
    }
}

} /* namespace driver */

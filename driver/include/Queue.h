#ifndef QUEUE_H_
#define QUEUE_H_

#include <queue>
#include <mutex>
#include <condition_variable>
#include <Message.h>

using namespace std;

namespace driver
{

class Queue
{
public:
    Queue ();
    virtual ~Queue ();
    mutex m;
    condition_variable cv;
    queue<Message>* q_ptr;
};

} /* namespace driver */

#endif /* QUEUE_H_ */

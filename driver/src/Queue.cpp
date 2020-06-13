#include "Queue.h"

namespace driver
{

Queue::Queue (): q_ptr(new queue<Message>)
{
}

Queue::~Queue ()
{
}

} /* namespace driver */

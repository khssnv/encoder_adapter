#ifndef RECIEVER_H_
#define RECIEVER_H_

#include <chrono>
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <Queue.h>

using namespace std;
using namespace boost::asio;

namespace driver {

class Reciever {
public:
	Reciever(Queue* msg_queue, string port);
	virtual ~Reciever();
	void run();
	Queue* msg_queue;
	string port;
};

} /* namespace driver */

#endif /* RECIEVER_H_ */

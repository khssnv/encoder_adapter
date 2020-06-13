#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>
#include <iostream>

using namespace std;

namespace driver {

class Message {
public:
    Message();
    Message(uint8_t seq, uint16_t count);
    Message(const Message& m);
    virtual ~Message();
    static int from_string(string candidate, Message& msg_buf);
    void print(void);
    uint8_t seq;
    uint16_t count;
};

} /* namespace driver */

#endif /* MESSAGE_H_ */

#include <Message.h>

namespace driver
{

Message::Message(): seq(0), count(0)
{
}

Message::Message(uint8_t seq, uint16_t count): seq(seq), count(count)
{
}

Message::Message(const Message& m): seq(m.seq), count(m.count)
{
}

Message::~Message()
{
}

int Message::from_string(string candidate, Message& msg_buf)
{
    // candidate example: <#d1:0006:b9\r\n>
    string seq_str = candidate.substr(1, 2);
    string count_str = candidate.substr(4, 4);
    string crc8_str = candidate.substr(9, 2);

    msg_buf.seq = stoul(seq_str, nullptr, 16);
    msg_buf.count = stoul(count_str, nullptr, 16);

    return 0;
}

void Message::print(void)
{
    cout << "msg seq: " << static_cast<int>(seq) << ", count: " << count << endl;
}

} /* namespace driver */

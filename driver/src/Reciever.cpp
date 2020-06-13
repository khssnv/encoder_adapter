#include <mutex>
#include <Reciever.h>
#include <Message.h>

namespace driver {

Reciever::Reciever(Queue* msg_queue, string port):
        msg_queue(msg_queue), port(port)
{
}

Reciever::~Reciever()
{
}

void Reciever::run()
{
    serial_port_base::baud_rate baudrate(115200);
    serial_port_base::flow_control flow_ctrl(serial_port_base::flow_control::none);
    serial_port_base::parity parity(serial_port_base::parity::none);
    serial_port_base::stop_bits stop_bits(serial_port_base::stop_bits::one);

    io_service io;
    serial_port ser(io, port);

    ser.set_option(baudrate);
    ser.set_option(flow_ctrl);
    ser.set_option(parity);
    ser.set_option(stop_bits);

    boost::asio::streambuf buf;

    std::cout << "Starting recv loop" << endl;

    for(;;)
    {
        // const auto t1 = chrono::high_resolution_clock::now();
        read_until(ser, buf, "\r\n");
        std::istream is(&buf);
        std::string msg_raw;
        std::getline(is, msg_raw);

        // parse message
        Message msg;
        Message::from_string(msg_raw, msg);
        lock_guard<mutex> lock(msg_queue->m);
        msg_queue->q_ptr->push(msg);
        msg_queue->cv.notify_one();
        // msg.print();
        /*
        const auto t2 = chrono::high_resolution_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
        cout << "Recv loop elapsed: " << elapsed.count() << " ms" << endl;
        */
    }
}

} /* namespace driver */

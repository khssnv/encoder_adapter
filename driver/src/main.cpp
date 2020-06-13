#include <iostream>
#include <thread>
#include <boost/program_options.hpp>

#include <Message.h>
#include <Queue.h>
#include <Reciever.h>
#include <Processor.h>

using namespace std;
namespace po = boost::program_options;
using namespace driver;

int main(int argc, char** argv)
{
    cout << "Starting" << endl;

    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("port", po::value<string>(), "set device port")
            ("mpp", po::value<double>(), "set how much meters in one encoder pulse")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help"))
    {
        cout << desc << endl;
        return 0;
    }

    if(vm.count("port"))
    {
        cout << "Connecting to device at " << vm["port"].as<string>() << endl;
    }
    else
    {
        cout << "Port not set, please specify port." << endl;
        return 1;
    }

    if(vm.count("mpp"))
    {
        cout << "Assuming " << vm["mpp"].as<double>() << "meters per one encoder pulse" << endl;
    }
    else
    {
        cout << "Meters per encoder pulse not set, please specify it." << endl;
        return 1;
    }

    Queue* shared_queue_ptr = new Queue();

    Reciever recv(shared_queue_ptr, vm["port"].as<string>());
    thread recv_th(&Reciever::run, &recv);

    Processor proc(shared_queue_ptr, vm["mpp"].as<double>());
    thread proc_th(&Processor::run, &proc);

    recv_th.join();
    proc_th.join();

    return 0;
}

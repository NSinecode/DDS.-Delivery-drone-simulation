#include "pystream.h"

std::string to_string(Vector3 other)
{
    return std::to_string(other.x) + ":" + std::to_string(other.y) + ":" + std::to_string(other.z) + ";";
}

std::string to_string(Vector2 other)
{
    return std::to_string(other.x) + ":" + std::to_string(other.y) + ";";
}

Vector2 Vec3toVec2(Vector3 other)
{
    return {other.x, other.z};
}

std::string SendRecieveData(std::string data)
{
    using namespace std::chrono_literals;

    // initialize the zmq context with a single IO thread
    zmq::context_t context{ 1 };

    // construct a REP (reply) socket and bind to interface
    zmq::socket_t socket{ context, zmq::socket_type::rep};
    socket.bind("tcp://*:5555");


    zmq::message_t request;

    // receive a request from client
    socket.recv(request, zmq::recv_flags::none);
    //std::cout << "Received " << request.to_string() << std::endl;

    // simulate work
    //std::this_thread::sleep_for(1s);

    // send the reply to the client
    socket.send(zmq::buffer(data), zmq::send_flags::none);


    return request.to_string();
}

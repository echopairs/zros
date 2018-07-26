//
// Created by pairs on 7/26/18.
//

#pragma once

#include <zmq.hpp>
namespace zeromq
{
    class ZmqHelp
    {
    public:
        /// from zhelpers.cpp
        static bool s_send(zmq::socket_t &socket, const std::string &msg)
        {
            zmq::message_t message(msg.size());
            memcpy(message.data(), msg.data(), msg.size());
            bool rc = socket.send(message);
            return rc;
        }
        static bool s_sendmore(zmq::socket_t &socket, const std::string &msg)
        {
            zmq::message_t message(msg.size());
            memcpy(message.data(), msg.data(), msg.size());
            bool rc = socket.send(message, ZMQ_SNDMORE);
            return rc;
        }
        static std::string s_recv(zmq::socket_t &socket)
        {
            zmq::message_t message;
            socket.recv(&message);
            return std::string(static_cast<char*>(message.data()), message.size());
        }
    };
}

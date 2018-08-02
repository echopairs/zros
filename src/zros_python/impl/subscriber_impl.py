# -*- coding: utf-8 -*-
""" impl subscriber management

data: 2018-08-02
author: pairs
"""
import abc
import logging
import zmq
import threading
import zros_python.pb.zros_pb2 as zpb2

logger = logging.getLogger(__name__)


class ISubStub(object):
    __metaclass__ = abc.ABCMeta

    def __init__(self, topic):
        self._topic = topic
        self._pub_mutex = threading.Lock()
        self._publishers_address = {}

    @abc.abstractmethod
    def start(self):
        pass

    @abc.abstractmethod
    def stop(self):
        pass

    @abc.abstractmethod
    def add_publisher(self, address):
        pass

    @abc.abstractmethod
    def remove_publisher(self, address):
        pass


class ZmqStubStub(ISubStub):
    """

    """
    def __init__(self, topic, callback):
        super(ZmqStubStub, self).__init__(topic)
        self._callback = callback
        self._context = zmq.Context()
        self._subscriber = self._context.socket(zmq.SUB)
        self._stop_mutex = threading.Lock()
        self._stop = False

        self._running_mutex = threading.Lock()
        self._running = False
        self._receive_thread = threading.Thread(target=ZmqStubStub._receive_message, args=(self,))

    def start(self):
        with self._running_mutex:
            if self._running:
                logger.warning(u'the topic %s was already receive message')
                return True
            self._subscriber.setsockopt(zmq.SUBSCRIBE, '')
            self._receive_thread.daemon = True
            self._receive_thread.start()
            self._running = True

    def stop(self):
        with self._running_mutex:
            with self._stop_mutex:
                self._stop = True
                self._running = False
        return True

    def add_publisher(self, address):
        with self._pub_mutex:
            if address not in self._publishers_address:
                self._subscriber.connect(address)
                self._publishers_address[address] = 1
        self.start()
        return True

    def remove_publisher(self, address):
        with self._pub_mutex:
            if address in self._publishers_address:
                del self._publishers_address[address]
        pass

    def _receive_message(self):
        while True:
            with self._stop_mutex:
                if self._stop is True:
                    break
                else:
                    data = self._subscriber.recv()
                    self._callback(data)


class SubscribersImpl(object):
    """

    """
    def __init__(self):
        self._subscriber_mutex = threading.Lock()
        self._subscribers = {}  # key: topic, value:(subscriber, stub)

    def register_publisher(self, publisher_info):
        topic = publisher_info.topic
        status = zpb2.Status()
        with self._subscriber_mutex:
            if topic not in self._subscribers:
                logger.warning(u'please check register publisher %s logical error ', topic)
                status.code = zpb2.Status.NOT_FOUND
                status.details = topic + u' publisher not register on'
                return status

            publish_address = publisher_info.physical_node_info.real_address
            pair = self._subscribers[topic]
            stub = pair[1]
            stub.add_publisher(publish_address)
            status.code = zpb2.Status.OK
            logger.info(u'register publisher to : %s topic on address %s success', topic, publish_address)
            return status

    def unregister_publisher(self, publisher_info):
        pass

    def register_subscriber(self, subscriber):
        topic = subscriber.get_topic()
        with self._subscriber_mutex:
            if topic in self._subscribers:
                logger.warning(u'please check this subscriber already register %s ', topic)
                return False
            stub = ZmqStubStub(topic, subscriber.handle_raw_message)
            pair = (subscriber, stub)
            self._subscribers[topic] = pair
            return True

    def unregister_subscriber(self, topic):
        with self._subscriber_mutex:
            if topic in self._subscribers:
                del self._subscribers[topic]

# -*- coding: utf-8 -*-
""" impl publisher management

data: 2018-08-02
author: pairs
"""
import abc
import logging
import threading
import zmq

logger = logging.getLogger(__name__)


class PubStub(object):
    __metaclass__ = abc.ABCMeta

    def __init__(self, topic):
        self._topic = topic
        self._address = None

    @abc.abstractmethod
    def publish(self, message):
        pass

    def set_address(self, address):
        self._address = address

    def get_address(self):
        return self._address


class ZmqPubStub(PubStub):
    """

    """
    def __init__(self, topic):
        super(ZmqPubStub, self).__init__(topic)
        self._mtx = threading.Lock()
        self._context = zmq.Context()
        self._pub_stub = self._context.socket(zmq.PUB)
        port_selected = self._pub_stub.bind_to_random_port('tcp://*', min_port=5000, max_port=65535, max_tries=100)
        self.set_address(u'tcp://localhost:' + str(port_selected))
        logger.info(u'topic %s bind on address %s', topic, self.get_address())

    def publish(self, message):
        with self._mtx:
            self._pub_stub.send_string(message)


class PublishersImpl(object):
    """

    """
    def __init__(self):
        self._pub_mutex = threading.Lock()
        self._publishers = {}

    def register_publisher(self, publisher):
        topic = publisher.get_topic()
        with self._pub_mutex:
            if topic in self._publishers:
                logger.warning(u'the publisher %s already register', topic)
                return
            address = self._create_publisher(topic)
            publisher.set_address(address)

    def publish(self, topic, message):
        with self._pub_mutex:
            if topic in self._publishers:
                self._publishers[topic].publish(message)
            else:
                logger.warning(u'please check publisher %s if exist', topic)

    def _create_publisher(self, topic):
        self._publishers[topic] = ZmqPubStub(topic)
        address = self._publishers[topic].get_address()
        logger.info(u'register publisher: %s on address success', address)
        return address

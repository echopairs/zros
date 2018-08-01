# -*- coding: utf-8 -*-

""" The basic publisher definition.
Users can use 'Publisher' to publish some topic related messages.

data: 2018-08-02
author: pairs
"""

import abc
import logging
import google.protobuf.message as gpm


logger = logging.getLogger(__name__)


class IPublisher(object):
    __metaclass__ = abc.ABCMeta

    def __init__(self, topic, node_handle):
        self._topic = topic
        self._node_handle = node_handle
        self._address = None

    def get_node_handle(self):
        return self._node_handle

    def get_topic(self):
        return self._topic

    def set_address(self, address):
        self._address = address

    def get_address(self):
        return self._address


class Publisher(IPublisher):
    """

    """
    def __init__(self, topic, message_cls, node_handle):
        super(Publisher, self).__init__(topic, node_handle)
        self._message_cls = message_cls
        assert (message_cls is None or
                isinstance(message_cls(), unicode) or
                isinstance(message_cls(), gpm.Message))

    def publish(self, message, timeout=None):
        """

        :param message:
        :param timeout:
        :return:
        """
        assert (message is self._message_cls or
                isinstance(message, self._message_cls))
        if hasattr(message, u'SerializeToString'):
            content = message.SerializeToString()
        else:
            content = message.encode('utf-8')

        self._node_handle.publish(self._topic, content, timeout)

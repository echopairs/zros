# -*- coding: utf-8 -*-

""" The basic subscriber definition.
Users can use 'Subscriber' to subscribe some topic related messages.

data: 2018-08-02
author: pairs
"""

import abc
import logging
import google.protobuf.message as gpm
import traceback

logger = logging.getLogger(__name__)


class ISubscriber(object):
    __metaclass__ = abc.ABCMeta

    def __init__(self, topic, node_handle):
        self._topic = topic
        self._node_handle = node_handle

    def get_topic(self):
        return self._topic

    def get_node_handle(self):
        return self._node_handle


class Subscriber(ISubscriber):
    """

    """
    def __init__(self, topic, callback, message_cls, node_handle):
        super(Subscriber, self).__init__(topic, node_handle)
        self._message_cls = message_cls
        self._callback = callback
        assert (message_cls is None or
                isinstance(message_cls(), unicode) or
                isinstance(message_cls(), gpm.Message))

    def handle_raw_message(self, message):
        """

        :param message:
        :return:
        """
        try:
            if hasattr(self._message_cls, u'ParseFromString'):
                msg = self._message_cls()
                msg.ParseFromString(bytes(message))
            else:
                msg = message.decode('utf-8')
            self._callback(msg)
        except Exception:
            logger.error(u' handle publish message error: %s ', traceback.format_exc())

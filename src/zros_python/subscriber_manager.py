# -*- coding: utf-8 -*-
""" The subscriber management

data: 2018-08-02
author: pairs
"""

import zros_python.impl.subscriber_impl as impl
import zros_python.zros as zros
import logging

logger = logging.getLogger(__name__)


class SubscriberManager(object):
    """

    """
    def __init__(self):
        self._impl = impl.SubscribersImpl()
        self._gsd = zros.get_service_discovery()
        self._gsd.set_register_publisher_cb(self._impl.register_publisher)
        self._gsd.set_unregister_publisher_cb(self._impl.unregister_publisher)

    def register_subscriber(self, subscriber):
        # 1. register to master first
        ok = self._gsd.add_subscriber(subscriber)
        if ok:
            logger.info(u'register subscriber %s to master success', subscriber.get_topic())
            return self._impl.register_subscriber(subscriber)
        logger.warning(u'register subscriber %s to master failed', subscriber.get_topic())
        return False

    def unregister_subscriber(self, topic):
        return self._impl.unregister_subscriber(topic)

# -*- coding: utf-8 -*-
""" The publisher management

data: 2018-08-01
author: pairs
"""
import zros_python.impl.publisher_impl as impl
import zros_python.zros as zros
import logging

logger = logging.getLogger(__name__)


class PublisherManager(object):
    """

    """
    def __init__(self):
        self._impl = impl.PublishersImpl()
        self._gsd = zros.get_service_discovery()

    def register_publisher(self, publisher):
        self._impl.register_publisher(publisher)
        ok = self._gsd.add_publisher(publisher)
        if ok is False:
            logger.warning(u'register publisher %s to master failed', publisher.get_topic())
            return False
        return True

    def unregister_publisher(self, topic):
        pass

    def publish(self, topic, content):
        self._impl.publish(topic, content)

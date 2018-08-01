# -*- coding: utf-8 -*-

""" The basic manager all ServiceServer

data: 2018-07-31
author: pairs
"""

import zros_python.impl.service_servers_impl as impl
import zros_python.zros as zros
import logging

logger = logging.getLogger(__name__)


class ServiceServerManager(object):
    """

    """
    def __init__(self, service_address, update_address_cb):
        """

        :param service_address:
        :param update_address_cb:
        """
        self._impl = impl.GrpcServersImpl(service_address, update_address_cb)
        self._gsd = zros.get_service_discovery()

    def start(self):
        self._impl.start()

    def stop(self):
        self._impl.stop()

    def register_server(self, server):
        # 1. register to master first
        ok = self._gsd.add_service_server(server)
        if ok:
            logger.info(u'register server %s to master success', server.get_service_name())
            return self._impl.register_server(server)
        else:
            logger.error(u'register server %s to master failed', server.get_service_name())
            return False

    def unregister_server(self, service_name):
        self._gsd.unregister_service_server(service_name)


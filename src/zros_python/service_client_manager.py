# -*- coding: utf-8 -*-
""" The service client management

data: 2018-08-01
author: pairs
"""

import zros_python.impl.service_clients_impl as impl
import zros_python.zros as zros
import logging

logger = logging.getLogger(__name__)


class ServiceClientManager(object):
    """

    """
    def __init__(self):
        self._impl = impl.ServiceClientsImpl()
        self._gsd = zros.get_service_discovery()
        self._gsd.set_register_service_server_cb(self._impl.register_service_client)
        self._gsd.set_unregister_service_server_cb(self._impl.unregister_service_client)

    def register_client(self, client):
        self._impl.register_client(client)
        ok = self._gsd.add_service_client(client)
        if ok:
            logger.info(u'register client %s to master success', client.get_service_name())
            return True
        else:
            self._impl.unregister_service_client(client.get_service_name())
            logger.warning(u'register client %s to master failed', client.get_service_name())
            return False

    def unregister_client(self, service_name):
        pass

    def call(self, service_name, content, cli_info, timeout):
        return self._impl.call(service_name, content, cli_info, timeout)

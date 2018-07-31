# -*- coding: utf-8 -*-

import logging
import zros_python.impl.service_discovery_impl as impl


class ServiceDiscovery(object):

    def __init__(self, master_address, agent_address):
        self._impl = impl.ServiceDiscoveryImpl(master_address, agent_address)
        if self._impl.is_connected_master() is False:
            raise Exception(u'connected master failed')

    def spin(self):
        self._impl.spin()

    def add_service_server(self, server):
        return self._impl.add_service_server(server)

    def add_service_client(self, client):
        return self._impl.add_service_client(client)

    def add_publisher(self, publisher):
        return self._impl.add_publisher(publisher)

    def add_subscriber(self, subscriber):
        return self._impl.add_subscriber(subscriber)


if __name__ == '__main__':
    logging.basicConfig(format=u'[%(name)s - %(levelname)s]: %(message)s',
                        level=logging.NOTSET)
    sd = ServiceDiscovery(u'localhost:23333', u'[::]:')
    sd.spin()

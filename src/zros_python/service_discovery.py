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

    def set_register_service_server_cb(self, callback):
        return self._impl.set_register_service_server_cb(callback)

    def set_unregister_service_server_cb(self, callback):
        return self.set_unregister_service_server_cb(callback)

    def set_register_publisher_cb(self, callback):
        return self.set_register_publisher_cb(callback)

    def set_unregister_publisher_cb(self, callback):
        return self.set_unregister_publisher_cb(callback)


if __name__ == '__main__':
    logging.basicConfig(format=u'[%(name)s - %(levelname)s]: %(message)s',
                        level=logging.NOTSET)
    sd = ServiceDiscovery(u'localhost:23333', u'[::]:')
    sd.spin()

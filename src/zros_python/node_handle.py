# -*- coding: utf-8 -*-
""" Node handle management

data: 2018-07-31
author: pairs
"""

import zros_python.service_server_manager as ssm
import zros_python.service_client_manager as scm
import zros_python.publisher_manager as pm
import zros_python.subscriber_manager as sm
import zros_python.service_client as service_client
import zros_python.service_server as service_server
import zros_python.publisher as publisher
import logging

logger = logging.getLogger(__name__)


class NodeHandle(object):
    """

    """
    def __init__(self, node_name, node_address=u'[::]:'):
        self._node_name = node_name
        self._node_address = node_address
        self._service_server_mgr = ssm.ServiceServerManager(self._node_address, self.set_node_address)
        self._service_client_mgr = scm.ServiceClientManager()
        self._publisher_manager = pm.PublisherManager()
        self._subscriber_manager = sm.SubscriberManager()
        self.spin()

    def advertise_service(self, service_name, service_func, req_cls, res_cls):
        """

        :param service_name:
        :param service_func:
        :param req_cls:
        :param res_cls:
        :return:
        """
        server = service_server.ServiceServer(service_name, service_func, req_cls, res_cls, self)
        ok = self._service_server_mgr.register_server(server)
        if ok is False:
            logging.error(u'advertise service %s failed ', service_name)
            return None
        else:
            return server

    def service_client(self, service_name, req_cls, res_cls, client_info=""):
        """

        :param service_name:
        :param req_cls:
        :param res_cls:
        :param client_info:
        :return:
        """
        client = service_client.ServiceClient(service_name, client_info, req_cls, res_cls, self)
        ok = self._service_client_mgr.register_client(client)
        if ok is False:
            logger.error(u'service client %s failed', service_name)
            return None
        else:
            return client

    def advertise(self, topic, message_cls):
        assert (isinstance(topic, unicode) and topic != u"")
        pub = publisher.Publisher(topic, message_cls, self)
        ok = self._publisher_manager.register_publisher(pub)
        if ok is False:
            logger.error(u'advertise publisher %s failed', topic)
            return None
        else:
            return pub

    def subscribe(self, topic, callback, message_cls):
        """

        :param topic:
        :param callback:
        :param message_cls:
        :return:
        """
        pass

    def call(self, service_name, content, cli_info, timeout):
        """

        :param service_name:
        :param content:
        :param cli_info:
        :param timeout:
        :return:
        """
        return self._service_client_mgr.call(service_name, content, cli_info, timeout)

    def publish(self, topic, content, timeout):
        self._publisher_manager.publish(topic, content)

    def spin(self):
        self._service_server_mgr.start()

    def set_node_address(self, address):
        self._node_address = address

    def get_node_address(self):
        return self._node_address

    def get_node_name(self):
        return self._node_name

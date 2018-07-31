# -*- coding: utf-8 -*-
""" Node handle management

data: 2018-07-31
author: pairs
"""


class NodeHandle(object):
    """

    """
    def __init__(self, node_name):
        self._node_name = node_name
        self._service_server_mgr = None
        self._service_client_mgr = None
        self._publisher_manager = None
        self._subscriber_manager = None
        self._node_address = None

    def advertise_service(self, service_name, service_func, req_cls, res_cls):
        """

        :param service_name:
        :param service_func:
        :param req_cls:
        :param res_cls:
        :return:
        """
        pass

    def service_client(self, service_name, req_cls, res_cls, client_info=""):
        """

        :param service_name:
        :param req_cls:
        :param res_cls:
        :param client_info:
        :return:
        """
        pass

    def advertise(self, topic, message_cls):
        pass

    def subscribe(self, topic, callback, message_cls):
        """

        :param topic:
        :param callback:
        :param message_cls:
        :return:
        """
        pass

    def call(self):
        pass

    def publish(self):
        pass

    def spin(self):
        pass

    def set_node_address(self, address):
        self._node_address = address

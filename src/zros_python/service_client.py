# -*- coding: utf-8 -*-

""" The basic service client definition.

data: 2018-08-01
author: pairs
"""

import abc
import logging
import google.protobuf.message as gpm
import zros_python.pb.zros_pb2 as zpb2

logger = logging.getLogger(__name__)


class IServiceClient(object):
    __metaclass__ = abc.ABCMeta
    """
    """
    def __init__(self, service_name, node_handle):
        self._service_name = service_name
        self._node_handle = node_handle
        self._ready = False

    def get_service_name(self):
        return self._service_name

    def get_node_handle(self):
        return self._node_handle

    def set_ready(self, ready):
        self._ready = ready

    def get_ready(self):
        return self._ready


class ServiceClient(IServiceClient):
    """

    """
    def __init__(self, service_name, client_info, req_cls, res_cls, node_handle):
        """

        :param service_name:
        :param client_info:
        :param req_cls:
        :param res_cls:
        :param node_handle:
        """
        super(ServiceClient, self).__init__(service_name, node_handle)
        self._client_info = client_info
        self._req_cls = req_cls
        self._res_cls = res_cls
        assert (req_cls is None or
                isinstance(req_cls(), unicode) or
                isinstance(req_cls(), gpm.Message))

        assert (res_cls is None or
                isinstance(res_cls(), unicode) or
                isinstance(res_cls(), gpm.Message))

    def call(self, request, timeout=None):
        """

        :param request:
        :param timeout:
        :return:
        """
        assert (request is self._req_cls or isinstance(request, self._req_cls))
        if hasattr(request, u'SerializeToString'):
            content = request.SerializeToString()
        else:
            content = request.encode('utf-8')

        response = self._node_handle.call(self._service_name, content, self._client_info, timeout)
        if response.status.code != zpb2.Status.OK:
            logger.error(u'call server %s failed, the reason is %', self.get_service_name(), response.status.details)
            return None
        else:
            res = self._res_cls()
            if hasattr(res, u'ParseFromString'):
                res.ParseFromString(response.response_data)
            else:
                res = response.response_data.decode('utf-8')
        return res

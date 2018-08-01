# -*- coding: utf-8 -*-

""" The basic service server definition.

data: 2018-07-31
author: pairs
"""
import traceback
import abc
import google.protobuf.message as gpm
import zros_python.pb.zros_pb2 as zpb2
import logging

logger = logging.getLogger(__name__)


class IServiceServer(object):
    __metaclass__ = abc.ABCMeta

    def __init__(self, service_name, node_handle):
        super(IServiceServer, self).__init__()
        self._service_name = service_name
        self._node_handle = node_handle

    def get_service_name(self):
        return self._service_name

    def get_node_handle(self):
        return self._node_handle

    @abc.abstractmethod
    def invoke(self, service_req):
        """

        :param service_req: a service request object
        :return: a service response object
        """
        pass


class ServiceServer(IServiceServer):
    """

    """
    def __init__(self, service_name, service_cb, req_cls, res_cls, node_handle):
        """

        :param service_name:
        :param service_cb:
        :param req_cls:
        :param res_cls:
        :param node_handle:
        """
        super(ServiceServer, self).__init__(service_name, node_handle)
        self._service_cb = service_cb
        self._req_cls = req_cls
        self._res_cls = res_cls
        assert (req_cls is None or
                isinstance(req_cls(), unicode) or
                isinstance(req_cls(), gpm.Message))

        assert (res_cls is None or
                isinstance(res_cls(), unicode) or
                isinstance(res_cls(), gpm.Message))

    def invoke(self, service_req):
        """
        :param self:
        :param service_req: a service request object
        :return: a service response object
        """

        if hasattr(self._req_cls, u'ParseFromString'):
            req = self._req_cls()
            req.ParseFromString(service_req.request_data)
        else:
            req = service_req.request_data.decode('utf-8')
        response = zpb2.ServiceResponse()
        response.service_name = service_req.service_name
        try:
            res = self._service_cb(req)
            response.status.code = zpb2.Status.OK
        except Exception:
            res = self._res_cls()
            response.status.code = zpb2.Status.UNKNOWN
            logger.error(u'Server service function error: %s', traceback.format_exc())
        assert (res is self._res_cls or isinstance(res, self._res_cls))

        if hasattr(res, u"SerializeToString"):
            response.response_data = res.SerializeToString()
        else:
            response.response_data = res.encode('utf-8')
        return response

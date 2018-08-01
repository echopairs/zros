# -*- coding: utf-8 -*-
""" impl service server management

data: 2018-07-31
author: pairs
"""

import abc
import logging
import threading
import zros_python.pb.zros_pb2_grpc as zpbg2
import zros_python.pb.zros_pb2 as zpb2
import grpc
from concurrent import futures

logger = logging.getLogger(__name__)


class IServersImpl(object):
    __metaclass__ = abc.ABCMeta

    def __init__(self, address):
        super(IServersImpl, self).__init__()
        self._address = address
        self._servers_mutex = threading.Lock()
        self._servers = {}

    def register_server(self, server):
        """

        :param server: ServiceServer
        :return:
        """
        service_name = server.get_service_name()
        with self._servers_mutex:
            if service_name in self._servers:
                logger.warning(u'register server: %s  have already register', service_name)
            else:
                self._servers[service_name] = server
                logger.info(u'register server: %s succeed', service_name)

    def unregister_server(self, service_name):
        """
        :param service_name:
        :return:
        """
        with self._servers_mutex:
            if service_name in self._servers:
                del self._servers[service_name]
                logger.info(u'unregister server: %s succeed', service_name)

    @abc.abstractmethod
    def start(self):
        pass

    @abc.abstractmethod
    def stop(self):
        pass


class GrpcServersImpl(IServersImpl, zpbg2.ServiceRPCServicer):
    """

    """
    def __init__(self, service_address, update_address_cb):
        """

        :param service_address:
        """
        super(GrpcServersImpl, self).__init__(service_address)
        self._grpc_server = None
        self._update_address_cb = update_address_cb

    def start(self):
        self._grpc_server = grpc.server(futures.ThreadPoolExecutor(max_workers=4))
        zpbg2.add_ServiceRPCServicer_to_server(self, self._grpc_server)
        if self._address == u'[::]:':
            port = self._grpc_server.add_insecure_port(u'[::]:')
            self._address = u'localhost:' + str(port)
        else:
            port = self._grpc_server.add_insecure_port(self._address)
        if port == 0:
            raise Exception(u'start service server failed')
        self._update_address_cb(self._address)
        self._grpc_server.start()
        logger.info(u'service servers run on %s', self._address)

    def stop(self):
        if self._grpc_server:
            self._grpc_server.stop(0)

    def __del__(self):
        self.stop()

    def InvokeService(self, request, context):
        """

        :param request:
        :param context:
        :return:
        """
        with self._servers_mutex:
            server = self._servers.get(request.service_name, None)
        if not server:
            response = zpb2.ServiceResponse()
            response.service_name = request.service_name
            response.status.code = zpb2.Status.UNKNOWN
            response.status.details = u"There is no server for the service"
            return response
        return server.invoke(request)

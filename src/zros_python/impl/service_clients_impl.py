# -*- coding: utf-8 -*-
""" impl service client management

data: 2018-08-01
author: pairs
"""

import abc
import logging
import threading
import grpc
import zros_python.pb.zros_pb2 as zpb2
import zros_python.pb.zros_pb2_grpc as zpbg2
import traceback

logger = logging.getLogger(__name__)


class RpcStub(object):
    __metaclass__ = abc.ABCMeta
    """
    
    """
    def __init__(self):
        pass

    @abc.abstractmethod
    def call(self, service_name, content, cli_info, timeout):
        pass


class GrpcStub(RpcStub):
    def __init__(self, address):
        super(GrpcStub, self).__init__()
        channel = grpc.insecure_channel(address)
        self._stub = zpbg2.ServiceRPCStub(channel)

    def call(self, service_name, content, cli_info, timeout):
        request = zpb2.ServiceRequest()
        response = zpb2.ServiceResponse()
        request.service_name = service_name
        request.request_data = content
        try:
            return self._stub.InvokeService(request)
        except Exception:
            logger.error(u'GRPC INNER ERROR' + traceback.format_exc())
            response.status.code = zpb2.Status.UNKNOWN
            response.status.details = u'GRPC INNER ERROR' + traceback.format_exc()
            return response


class ServiceClientsImpl(object):
    """

    """
    def __init__(self):
        self._clients_mutex = threading.Lock()
        self._services_mutex = threading.Lock()
        self._stubs_mutex = threading.Lock()
        self._clients = {}
        self._services_address = {}
        self._stubs = {}

    def register_client(self, client):
        service_name = client.get_service_name()
        with self._clients_mutex:
            if service_name in self._clients:
                logger.warning(u'service client %s already register', service_name)
            else:
                self._clients[service_name] = client

    def unregister_client(self, service_name):
        # todo
        pass

    def call(self, service_name, content, cli_info, timeout):
        response = zpb2.ServiceResponse()
        with self._clients_mutex:
            if service_name not in self._clients:
                logger.error(u'please check if register this client %s', service_name)
                response.status.code = zpb2.Status.UNKNOWN
                response.status.details = u'please check if register this client ' + service_name
                return response

        stub = self._get_rpc_stub(service_name)
        if stub is None:
            logger.warning(u'stub is None')
            response.status.code = zpb2.Status.NOT_FOUND
            response.status.details = u'service call stub not found, probably waiting for establishing yet.'
            return response
        return stub.call(service_name, content, cli_info, timeout)

    def register_service_client(self, server_info):
        service_name = server_info.service_name
        status = zpb2.Status()
        with self._clients_mutex:
            if service_name not in self._clients:
                logger.warning(u'please check register service client logical error %s', service_name)
                status.code = zpb2.Status.NOT_FOUND
                status.details = service_name + u' client not register on'
                return status
            self._clients[service_name].set_ready(True)

        with self._services_mutex:
            self._services_address[service_name] = server_info.physical_node_info.real_address
            status.code = zpb2.Status.OK
            logger.info(u'register client: %s on address %s success', service_name,
                        server_info.physical_node_info.real_address)
            return status

    def unregister_service_client(self, server_info):
        service_name = server_info.service_name
        status = zpb2.Status()
        logger.info(u'unregister service client %s ', service_name)
        # todo
        return status

    def _get_rpc_stub(self, service_name):
        with self._services_mutex:
            with self._stubs_mutex:
                if service_name not in self._services_address:
                    logger.error(u'get rpc stub failed: there is no this %s service client', service_name)
                    return None
                address = self._services_address[service_name]
                if address not in self._stubs:
                    self._stubs[address] = GrpcStub(address)
                return self._stubs[address]

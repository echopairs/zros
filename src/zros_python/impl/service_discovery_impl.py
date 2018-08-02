# -*- coding: utf-8 -*-

import zros_python.pb.zros_pb2_grpc as zpbg2
import zros_python.pb.zros_pb2 as zpb2
import grpc
import logging
import traceback
from concurrent import futures

logger = logging.getLogger(__name__)


class ServiceDiscoveryImpl(zpbg2.ServiceDiscoveryRPCServicer):
    _STUB_CALL_SHORT_TIME_OUT = 5
    _STUB_CALL_LONG_TIME_OUT = 3000     # seconds

    def __init__(self, master_address, agent_address):
        super(ServiceDiscoveryImpl, self).__init__()
        self._master_address = master_address
        self._agent_address = agent_address

        channel = grpc.insecure_channel(master_address)
        self._master_rpc_stub = zpbg2.MasterRPCStub(channel)
        self._grpc_server = None
        self._deal_register_service_server = None
        self._deal_unregister_service_server_cb = None
        self._deal_register_publisher_cb = None
        self._deal_unregister_publisher_cb = None

    def is_connected_master(self, connect_time_out=None):
        time_out = self._STUB_CALL_SHORT_TIME_OUT if connect_time_out is None \
            else connect_time_out
        request = zpb2.PingRequest()
        try:
            response = self._master_rpc_stub.Ping(request, time_out)
            if response.code == zpb2.Status.OK:
                return True
            else:
                logger.warning(u"connect master failed %s ", response.details)
                return False
        except Exception:
            logger.error(traceback.format_exc())
            return False

    def add_publisher(self, publisher):
        timeout = self._STUB_CALL_SHORT_TIME_OUT
        request = zpb2.PublisherInfo()
        request.topic = publisher.get_topic()
        request.physical_node_info.agent_address = self._agent_address
        request.physical_node_info.real_address = publisher.get_address()
        try:
            response = self._master_rpc_stub.RegisterPublisher(request, timeout)
            if response.code == zpb2.Status.OK:
                return True
            else:
                logger.error(u'register publisher %s failed', request.publisher_name)
                return False
        except Exception:
            logger.error(u'register publisher %s failed', request.publisher_name)
            return False

    def add_subscriber(self, subscriber):
        pass

    def add_service_server(self, server):
        timeout = self._STUB_CALL_SHORT_TIME_OUT
        request = zpb2.ServiceServerInfo()
        request.service_name = server.get_service_name()
        request.physical_node_info.agent_address = self._agent_address

        node_handle = server.get_node_handle()
        node_name = node_handle.get_node_name()
        node_address = node_handle.get_node_address()

        request.physical_node_info.name = node_name
        request.physical_node_info.real_address = node_address
        try:
            response = self._master_rpc_stub.RegisterServiceServer(request, timeout)
            if response.code == zpb2.Status.OK:
                return True
            else:
                logger.error(u'register service server %s failed', request.service_name)
                return False
        except Exception:
            logger.error(traceback.format_exc())
            return False

    def unregister_service_server(self, service_name):
        pass

    def add_service_client(self, client):
        timeout = self._STUB_CALL_SHORT_TIME_OUT
        request = zpb2.ServiceClientInfo()
        request.service_name = client.get_service_name()
        request.physical_node_info.agent_address = self._agent_address
        node_handle = client.get_node_handle()
        node_name = node_handle.get_node_name()
        node_address = node_handle.get_node_address()
        request.physical_node_info.name = node_name
        request.physical_node_info.real_address = node_address
        try:
            response = self._master_rpc_stub.RegisterServiceClient(request, timeout)
            if response.code == zpb2.Status.OK:
                return True
            else:
                logger.error(u'register service client %s failed', request.service_name)
                return False
        except Exception:
            logger.error((traceback.format_exc()))
            return False

    def spin(self):
        self._grpc_server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
        zpbg2.add_ServiceDiscoveryRPCServicer_to_server(self, self._grpc_server)
        if self._agent_address == '[::]:':
            port = self._grpc_server.add_insecure_port(u'[::]:')
            self._agent_address = 'localhost:' + str(port)
        else:
            port = self._grpc_server.add_insecure_port(self._agent_address)
        if port == 0:
            raise Exception(u'service discovery bind port failed')
        self._grpc_server.start()
        logger.info(u'service discovery server run on %s', self._agent_address)

    def set_register_service_server_cb(self, callback):
        self._deal_register_service_server = callback

    def set_unregister_service_server_cb(self, callback):
        self._deal_unregister_service_server_cb = callback

    def set_register_publisher_cb(self, callback):
        self._deal_register_publisher_cb = callback

    def set_unregister_publisher_cb(self, callback):
        self._deal_unregister_publisher_cb = callback

    def RegisterPublisher(self, request, context):
        if self._deal_register_publisher_cb:
            response = self._deal_register_publisher_cb(request)
        else:
            logger.error(u'please set deal register publisher callback first')
            response = zpb2.Status()
            response.code = zpb2.Status.INVALID_ARGUMENT
            response.details = u'not set deal register publisher callback'
        return response

    def UnregisterPublisher(self, request, context):
        if self._deal_unregister_publisher_cb:
            response = self._deal_unregister_publisher_cb(request)
        else:
            logger.error(u'please set deal unregister publisher callback first')
            response = zpb2.Status()
            response.code = zpb2.Status.INVALID_ARGUMENT
            response.details = u'not set deal unregister publisher callback'
        return response

    def RegisterServiceServer(self, request, context):
        if self._deal_register_service_server:
            response = self._deal_register_service_server(request)
        else:
            logger.error(u'please set deal register service server callback first')
            response = zpb2.Status()
            response.code = zpb2.Status.INVALID_ARGUMENT
            response.details = u'not set deal register service server callback'
        return response

    def UnregisterServiceServer(self, request, context):
        if self._deal_unregister_service_server_cb:
            response = self._deal_unregister_service_server_cb(request)
        else:
            logger.error(u'please set deal unregister service server callback first')
            response = zpb2.Status()
            response.code = zpb2.Status.INVALID_ARGUMENT
            response.details = u'not set deal unregister service server callback'
        return response

    def Ping(self, request, context):
        response = zpb2.Status()
        response.code = zpb2.Status.OK
        return response

    def __del__(self):
        if self._grpc_server:
            self._grpc_server.stop(0)


if __name__ == '__main__':
    impl = ServiceDiscoveryImpl(u"localhost:23333", u'[::]:')
    logging.basicConfig(format=u'[%(name)s - %(levelname)s]: %(message)s',
                        level=logging.NOTSET)
    ok = impl.is_connected_master()
    if ok:
        logger.info("connect master ok")
    else:
        logger.error("connect master failed")
    impl.spin()

# -*- coding: utf-8 -*-
""" An example for service server

data: 2018-08-01
author: pairs
"""

import zros_python.zros as zros
import zros_python.node_handle as node
import logging
import zros_python.pb.test_service_pb2 as tspb2
import time

logger = logging.getLogger(__name__)

_ONE_DAY_IN_SECONDS = 60 * 60 * 24

def service_func(request):
    logger.info(u'receive request details is %s ', request.detail)
    response = tspb2.TestServiceResponse()
    return response

def serve():
    # 1. init & run service discovery
    master_address = u'localhost:23333'
    zros.init(master_address)

    # 2. create node
    node_name = u'example_service_server'
    node_handle = node.NodeHandle(node_name)

    # # 3. create service server
    service_name = u'test_service'
    node_handle.advertise_service(service_name, service_func,
                                  tspb2.TestServiceRequest,
                                  tspb2.TestServiceResponse)
    try:
        while True:
            time.sleep(_ONE_DAY_IN_SECONDS)
    except KeyboardInterrupt:
        pass

if __name__ == '__main__':
    logging.basicConfig(format=u'[%(name)s - %(levelname)s]: %(message)s',
                        level=logging.NOTSET)
    serve()
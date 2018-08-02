# -*- coding: utf-8 -*-
""" An example for service client

data: 2018-08-01
author: pairs
"""

import zros_python.zros as zros
import zros_python.node_handle as node
import zros_python.pb.test_service_pb2 as tspb2
import time
import traceback
import logging

logger = logging.getLogger(__name__)

_TICK_TASK_TIME = 1

def call():
    # 1. init & run service discovery
    master_address = u'localhost:23333'
    zros.init(master_address)

    # 2. create node
    node_name = u'example_service_client'
    node_handle = node.NodeHandle(node_name)

    # 3. create service client
    service_name = u'test_service'
    client = node_handle.service_client(service_name,
                               tspb2.TestServiceRequest,
                               tspb2.TestServiceResponse)
    if client is None:
        logger.error(u'service client %s failed', service_name)
        return
    # 4. call
    request = tspb2.TestServiceRequest()
    request.detail = u'hello world'
    while True:
        try:
            if client.get_ready() is True:
                res = client.call(request)
                if res is None:
                    logger.warning(u'call service %s failed', service_name)
                else:
                    logger.info(u'call service %s success', service_name)
                    logger.info(u'response detail is %s ', res.detail)
        except Exception:
            logger.error(traceback.format_exc())
        time.sleep(_TICK_TASK_TIME)

if __name__ == '__main__':
    logging.basicConfig(format=u'[%(name)s-%(levelname)s]: %(message)s',
                        level=logging.NOTSET)
    call()
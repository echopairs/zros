# -*- coding: utf-8 -*-
""" An example for publisher

data: 2018-08-02
author: pairs
"""

import zros_python.zros as zros
import zros_python.node_handle as node
import zros_python.pb.test_message_pb2 as tmpb2
import time
import traceback
import logging

logger = logging.getLogger(__name__)

_PUB_TICK_TIME = 1

def publish():
    # 1. init & run service discovery
    master_address = u'localhost:23333'
    zros.init(master_address)

    # 2. create node
    node_name = u'example_publisher'
    node_handle = node.NodeHandle(node_name)

    # 3. create pub
    topic = u'test_topic'
    pub = node_handle.advertise(topic, tmpb2.TestMessage)
    if pub is None:
        logger.error(u'publisher %s advertise failed', topic)
        return

    # 4. publish message
    message = tmpb2.TestMessage()
    message.detail = u'hello world'
    message.count = 7
    while True:
        try:
            pub.publish(message)
        except Exception:
            logger.error(traceback.format_exc())
        time.sleep(_PUB_TICK_TIME)


if __name__ == '__main__':
    logging.basicConfig(format=u'[%(name)s-%(levelname)s]: %(message)s',
                        level=logging.NOTSET)
    publish()

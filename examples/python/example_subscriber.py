# -*- coding: utf-8 -*-
""" An example for subscriber

data: 2018-08-02
author: pairs
"""

import zros_python.zros as zros
import zros_python.node_handle as node
import zros_python.pb.test_message_pb2 as tmpb2
import time
import traceback
import logging
import signal
import os

logger = logging.getLogger(__name__)

_SPIN_TICK_TIME = 1

def callback(message):
    logger.info(u'receive publish message %s, %d', message.detail, message.count)

def subscribe():
    # 1. init & run service discovery
    master_address = u'localhost:23333'
    zros.init(master_address)

    # 2. create node
    node_name = u'example_subscriber'
    node_handle = node.NodeHandle(node_name)

    # 3. create subscriber
    topic = u'test_topic'
    sub = node_handle.subscribe(topic, callback, tmpb2.TestMessage)
    if sub is None:
        logger.error(u'subscriber %s advertise failed', topic)
        return

    while True:
        time.sleep(_SPIN_TICK_TIME)

def quit(signal, frame):
    os._exit(0)

if __name__ == '__main__':
    logging.basicConfig(format=u'[%(name)s-%(levelname)s]: %(message)s',
                        level=logging.NOTSET)

    signal.signal(signal.SIGINT, quit)
    signal.signal(signal.SIGTERM, quit)
    subscribe()

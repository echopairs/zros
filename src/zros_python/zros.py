# -*- coding: utf-8 -*-
""" zros management

data: 2018-07-31
author: pairs
"""
import logging
import zros_python.service_discovery as service_discovery
logger = logging.getLogger(__name__)

g_service_discovery = None


def init(master_address, agent_address='[::]:'):
    """
    Args:
    :param master_address:  the master's info, like "localhost:23333"
    :param agent_address:   the service discovery address, recommended default
    :return:
    """
    global g_service_discovery
    g_service_discovery = service_discovery.ServiceDiscovery(master_address, agent_address)
    spin()


def is_initialized():
    global g_service_discovery
    return g_service_discovery is not None


def spin():
    global g_service_discovery
    g_service_discovery.spin()

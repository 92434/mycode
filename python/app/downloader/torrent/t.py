# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月17日 肖飞 All rights reserved
#   
#   文件名称：t.py
#   创 建 者：肖飞
#   创建日期：2017年12月17日 星期日 12时00分14秒
#   修改日期：2017年12月31日 星期日 19时37分42秒
#   描    述：
#
#================================================================
import libtorrent as lt
import optparse
import sys
import time

import log
logging = log.dict_configure()
logger = logging.getLogger('default')

def dump(obj):
    for attr in dir(obj):
        if hasattr(obj, attr):
            obj_attr = getattr(obj, attr)
            logger.debug("obj.%s = %s" % (attr, obj_attr))

def dump_dict(obj, indent = 0):
    if not type(obj) == dict:
        return

    for key, value in obj.items():
        logger.debug("%s%s:%s" % ('  ' * indent, key, repr(value)))
        if type(value) == dict:
            dump_dict(value, indent + 1)

def open_session():
    s = lt.session()
    s.listen_on(6881, 6891)
    return s

def get_torrent_info(torrent_file):
    f = open("test.torrent", 'rb')
    content = f.read()
    decoded_content = lt.bdecode(content)
    f.close()
    dump_dict(decoded_content)
    info = lt.torrent_info(decoded_content)

    return info

def add_torrent_task(s, torrent_file, output_path = '.'):
    info = get_torrent_info(torrent_file)
    #dump(info)

    dict_torrent_params = {
            'save_path': output_path,
            'storage_mode': lt.storage_mode_t.storage_mode_sparse,
            'ti': info,
            }

    logger.debug('dict_torrent_params:%s' %(dict_torrent_params))

    torrent_handle = s.add_torrent(dict_torrent_params)

    return torrent_handle

def print_task_status(torrent_handle):
    while True:
        status = torrent_handle.status()
        if status.is_seeding:
            logger.debug('status.is_seeding:%s' %(status.is_seeding))
            break

        state_str = [
                'queued',
                'checking',
                'downloading metadata',
                'downloading',
                'finished',
                'seeding',
                'allocating'
                ]

        if status.state >= len(state_str):
            state = '%s' %(status.state)
        else:
            state = state_str[status.state]

        logger.debug('%.2f%% complete (down: %.1f kb/s up: %.1f kB/s peers: %d) %s' %(
                    status.progress * 100,
                    status.download_rate / 1000,
                    status.upload_rate / 1000,
                    status.num_peers,
                    state,
                    )
                )
        time.sleep(1)

def download_torrent(torrent_file):
    s = open_session()
    torrent_handle = add_torrent_task(s, torrent_file)
    print_task_status(torrent_handle)

def main(argv):
    options = optparse.OptionParser()
    options.add_option('-t', '--torrent_file', dest='torrent_file', help='torrent_file', default = None)
    opts, args = options.parse_args(argv)
    logger.debug('opts:%s' %(opts))
    logger.debug('args:%s' %(args))
    if len(args):
        options.print_help()
        return

    if not opts.torrent_file:
        options.print_help()
        return
    download_torrent(opts.torrent_file)

if '__main__' == __name__:
    main(sys.argv[1:])

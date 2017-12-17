# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月17日 肖飞 All rights reserved
#   
#   文件名称：test.py
#   创 建 者：肖飞
#   创建日期：2017年12月17日 星期日 12时00分14秒
#   修改日期：2017年12月17日 星期日 12时07分27秒
#   描    述：
#
#================================================================
import libtorrent as lt
import time

ses = lt.session()
ses.listen_on(6881, 6891)

e = lt.bdecode(open("test.torrent", 'rb').read())
info = lt.torrent_info(e)

params = { 'save_path': '.', \
    'storage_mode': lt.storage_mode_t.storage_mode_sparse, \
    'ti': info }
h = ses.add_torrent(params)

s = h.status()
while (not s.is_seeding):
    s = h.status()

    state_str = ['queued', 'checking', 'downloading metadata', \
        'downloading', 'finished', 'seeding', 'allocating']
    if s.state >= len(state_str):
        state = ''
    else:
        state = state_str[s.state]
    print '%.2f%% complete (down: %.1f kb/s up: %.1f kB/s peers: %d) %s' % \
        (s.progress * 100, s.download_rate / 1000, s.upload_rate / 1000, \
        s.num_peers, state)

    time.sleep(1)
def main():
    pass

if '__main__' == __name__:
    main()

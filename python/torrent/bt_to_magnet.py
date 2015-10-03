#import bencode, hashlib, base64, urllib
#torrent = open('ubuntu-12.04.2-server-amd64.iso.torrent', 'rb').read()
#metadata = bencode.bdecode(torrent)
#hashcontents = bencode.bencode(metadata['info'])
#digest = hashlib.sha1(hashcontents).digest()
#b32hash = base64.b32encode(digest)
#params = {'xt': 'urn:btih:%s' % b32hash,
#		   'dn': metadata['info']['name'],
#		   'tr': metadata['announce'],
#		   'xl': metadata['info']['length']}
#paramstr = urllib.urlencode(params)
#magneturi = 'magnet:?%s' % paramstr
#print magneturi
#
#import libtorrent as bt
#info = bt.torrent_info('test.torrent')
#print "magnet:?xt=urn:btih:%s&dn=%s" % (info.info_hash(), info.name())

import sys
import os

import bencode, hashlib, base64, urllib
import traceback
def bt_to_magnet1(bt_file):
	try:
		with open(bt_file) as f:
			torrent = f.read()
			metadata = bencode.bdecode(torrent)

			info = metadata.get('info')

			params = {}

			if not info:
				print 'no info field'
				sys.exit()

			hashcontents = bencode.bencode(info)
			digest = hashlib.sha1(hashcontents).digest()
			b32hash = base64.b32encode(digest)
			params.update(
				{
					'xt': 'urn:btih:%s' % b32hash,
				}
			)

			dn = info.get('name')

			if dn:
				params.update(
					{
						'dn': dn,
					}
				)

			tr = metadata.get('announce')
			if tr:
				params.update(
					{
						'tr': tr,
					}
				)

			xl = info.get('length')
			if xl:
				params.update(
					{
						'xl': xl,
					}
				)


			paramstr = urllib.urlencode(params)
			#paramstr = '&'.join(['='.join([key, value]) for key,value in params.items()])
			magneturi = 'magnet:?%s' % paramstr
			print magneturi
	except Exception, e:
		print '%s(%s)' %(e.message, bt_file)
		exstr = traceback.format_exc()
		print exstr


import libtorrent as bt
def bt_to_magnet2(bt_file):
	info = bt.torrent_info(bt_file)
	print "magnet:?xt=urn:btih:%s&dn=%s" % (info.info_hash(), info.name())

if __name__ == "__main__":
	if len(sys.argv) != 2:
		print 'arg error: \n\t%s %s' %(os.path.abspath(sys.argv[0]), '[bt file]')
		sys.exit(-1)
	#bt_to_magnet1(sys.argv[1])
	bt_to_magnet2(sys.argv[1])

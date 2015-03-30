#!/usr/bin/env python

import sys

def output(s):
  sys.stderr.write(s + "\n") # output progress to stderr(Notice that it is NOT for outputing actual error message, 
                             # I use stderr just to catch stdout easily(the same as curl) so i can merge these flvs later)

argc = len(sys.argv)
if argc == 2:
  format = 'super'
elif argc == 3:
  format = sys.argv[2]
else:
  output("Usage: %s videourl [videoquality=normal|high|super|...]" % sys.argv[0])
  output(" e.g.")
  output("   %s http://v.youku.com/v_show/id_XMzMzMjE0MjE2.html super" % sys.argv[0])
  exit(1)

videourl = sys.argv[1]

import urllib2
import urllib
url = 'http://www.flvcd.com/parse.php?kw=' + urllib.quote(videourl)  + '&format=' + format
user_agent = 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.57 Safari/536.11'

req = urllib2.Request(url)
# add some headers to fake Chrome Browser(if we don't do so, there will be a problem when try to get tudou video)
req.add_header('host', 'www.flvcd.com')
req.add_header('Referer', 'http://www.flvcd.com')
req.add_header('User-Agent', user_agent)
req.add_header('Accept-Language', 'en-us,en;q=0.5')
req.add_header('Accept-Encoding', 'gzip, deflate')
req.add_header('Accept-Charset', 'ISO-8859-1,utf-8;q=0.7,*;q=0.7')
# req.add_header('Keep-Alive', '115')
res = urllib2.urlopen(req)
html = res.read()

import re
pattern = re.compile('<input\s+type="hidden"\s+name="inf"\s+value="([^"]+)')
firstmatch = pattern.search(html)
urls = firstmatch.group(1)
urls = unicode(urls, 'gbk'); # urls turns out to be utf8 encoding

urlpattern = re.compile('<[NU]>(.+)')
result = urlpattern.findall(urls)

data = [result[i:i+2] for i in range(0, len(result), 2)]
count = len(data)
files = []

urllib.URLopener.version = user_agent
opener = urllib.URLopener();
output('\n--- Start to download from url "%s" (%d block(s) in total):' % (videourl, count))
for k, v in enumerate(data):
  output('  >downloading Block %.2d of %.2d ...' % (k+1, count))
  filename = (v[0].replace('"', '\\"').replace('/', '') + '.flv').encode('utf-8')
  if (v[1].find('tudou.com') != -1):
    opener.retrieve(v[1], filename)
  else:
    urllib.urlretrieve(v[1], filename)
  # urlretrieve with custom header(s)
  #req = urllib2.Request(v[1])
  #req.add_header('User-Agent', 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.57 Safari/536.11')
#  f = open(v[0] + '.flv', 'wb')
#  f.write(urllib2.urlopen(req).read())
#  f.close()
  # urllib.urlretrieve(v[1], v[0] + '.flv')
  
  files.append(filename)
  output('  downloaded Block.%.2d completely<' % (k+1,))
output('--- finished ---\n')
print('"' + '" "'.join(files) + '"')

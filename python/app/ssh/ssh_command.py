# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2018年01月08日 肖飞 All rights reserved
#   
#   文件名称：ssh_command.py
#   创 建 者：肖飞
#   创建日期：2018年01月08日 星期一 18时28分09秒
#   修改日期：2018年01月09日 星期二 12时16分48秒
#   描    述：
#
#================================================================
import paramiko
import sys
import optparse

import log
logging = log.dict_configure()
logger = logging.getLogger('default')

def get_ssh_client(server, username = None, password = None):
   ssh_client = paramiko.SSHClient() 
   ssh_client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
   ssh_client.connect(server, username=username, password=password)
   return ssh_client

def execute_command(ssh_client, command):
    stdin, stdout, stderr = ssh_client.exec_command(command)
    return stdin, stdout, stderr

def main(argv):
    options = optparse.OptionParser()
    options.add_option('-s', '--server', dest='server', help='server', default = None)
    options.add_option('-u', '--username', dest='username', help='username', default = None)
    options.add_option('-p', '--password', dest='password', help='password', default = None)
    options.add_option('-c', '--command', dest='command', help='command', default = None)
    opts, args = options.parse_args(argv)
    logger.debug('opts:%s' %(opts))
    logger.debug('args:%s' %(args))
    if len(args):
        options.print_help()
        return

    if not opts.server:
        options.print_help()
        return
    if not opts.username:
        options.print_help()
        return
    if not opts.password:
        options.print_help()
        return
    if not opts.command:
        options.print_help()
        return

    ssh_client = get_ssh_client(opts.server, opts.username, opts.password)
    stdin, stdout, stderr = execute_command(ssh_client, opts.command)
    print stdout.read()
    ssh_client.close()

if '__main__' == __name__:
    main(sys.argv[1:])

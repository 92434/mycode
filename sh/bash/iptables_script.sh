xiaofei_script
#!/bin/bash
iptables -F
iptables -F -t nat
iptables -X

iptables -P INPUT DROP
iptables -P OUTPUT ACCEPT
iptables -P FORWARD ACCEPT

#load connection-tracking modules
modprobe iptable_nat
modprobe ip_conntrack_ftp
modprobe ip_nat_ftp

iptables -A INPUT -f -m limit --limit 100/sec --limit-burst 100 -j ACCEPT
iptables -A FORWARD -p icmp --icmp-type echo-request -m limit --limit 1/s --limit-burst 10 -j ACCEPT
iptables -A INPUT -p tcp -m tcp --tcp-flags SYN,RST,ACK SYN -m limit --limit 20/sec --limit-burst 200 -j ACCEPT

iptables -A INPUT -s 122.70.x.x -j ACCEPT
iptables -A INPUT -i lo -j ACCEPT
iptables -A OUTPUT -o lo -j ACCEPT
iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
iptables -A INPUT -p tcp -m multiport --dport 80,22 -j ACCEPT







xiaofei_script
#echo "Starting kerryhu-iptables rules..."
#!/bin/bash
# BY kerryhu
# QQ:263205768
# MAIL:king_819@163.com
# BLOG:http://kerry.blog.51cto.com
#this is a common firewall created by 2010-3-27
 
IPT="/sbin/iptables"
CONNECTION_TRACKING="1"
CLASS_A="10.0.0.0/8"
CLASS_B="172.16.0.0/12"
CLASS_C="192.168.0.0/16"
CLASS_D_MULTICAST="224.0.0.0/4"
CLASS_E_RESERVED_NET="240.0.0.0/5"
BROADCAST_SRC="0.0.0.0" 
BROADCAST_DEST="255.255.255.255"
LOOPBACK_INTERFACE="lo"
#Remove any existing rules
$IPT -F 
$IPT -X
#setting default firewall policy
$IPT -P FORWARD DROP
$IPT -P INPUT DROP
$IPT -P OUTPUT DROP
  
#setting for loopback interface
$IPT -A INPUT -i lo -j ACCEPT
$IPT -A OUTPUT -o lo -j ACCEPT
 
# Stealth Scans and TCP State Flags
# All of the bits are cleared
$IPT -A INPUT -p tcp --tcp-flags ALL NONE -j DROP
# SYN and FIN are both set
$IPT -A INPUT -p tcp --tcp-flags SYN,FIN SYN,FIN -j DROP
# SYN and RST are both set
$IPT -A INPUT -p tcp --tcp-flags SYN,RST SYN,RST -j DROP
# FIN and RST are both set
$IPT -A INPUT -p tcp --tcp-flags FIN,RST FIN,RST -j DROP
# FIN is the only bit set, without the expected accompanying ACK
$IPT -A INPUT -p tcp --tcp-flags ACK,FIN FIN -j DROP
# PSH is the only bit set, without the expected accompanying ACK
$IPT -A INPUT -p tcp --tcp-flags ACK,PSH PSH -j DROP
# URG is the only bit set, without the expected accompanying ACK
$IPT -A INPUT -p tcp --tcp-flags ACK,URG URG -j DROP
 
# Using Connection State to By-pass Rule Checking
if [ "$CONNECTION_TRACKING" = "1" ]; then
    $IPT -A INPUT  -m state --state ESTABLISHED,RELATED -j ACCEPT
    $IPT -A OUTPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
    $IPT -A INPUT -m state --state INVALID -j DROP
    $IPT -A OUTPUT -m state --state INVALID -j DROP
fi
##################################################################
# Source Address Spoofing and Other Bad Addresses
# Refuse spoofed packets pretending to be from 
# the external interface.s IP address 
# Refuse packets claiming to be from a Class A private network
$IPT -A INPUT -s $CLASS_A -j DROP 
# Refuse packets claiming to be from a Class B private network
$IPT -A INPUT -s $CLASS_B -j DROP 
# Refuse packets claiming to be from a Class C private network
$IPT -A INPUT -s $CLASS_C -j DROP 
$IPT -A INPUT -s 0.0.0.0/8 -j DROP
$IPT -A INPUT -s 169.254.0.0/16 -j DROP
$IPT -A INPUT -s 192.0.2.0/24 -j DROP
###################################################################
#setting access rules
#允许出站域名解析
$IPT -A OUTPUT -p udp  --dport 53 -j ACCEPT 
#$IPT -A OUTPUT -p tcp  -d 61.177.7.1 --dport 53 -j ACCEPT
#$IPT -A OUTPUT -p udp  -d 61.177.7.1 --dport 53 -j ACCEPT
#时钟同步
$IPT -A OUTPUT -d 192.43.244.18 -j ACCEPT
#$IPT -A OUTPUT -p udp -d 192.43.244.18 --dport 123 -j ACCEPT
#允许ping出
$IPT -A OUTPUT -p icmp -j ACCEPT
#允许ftp备份
#$IPT -A OUTPUT -p tcp -d 222.102.153.191 --dport 21 -j ACCEPT
#$IPT -A OUTPUT -p tcp -d 222.102.153.191 --dport 20 -j ACCEPT
$IPT -A OUTPUT -d 222.102.153.191 -j ACCEPT
#允许出站http
$IPT -A OUTPUT -p tcp  --dport 80 -j ACCEPT
#允许yum更新
$IPT -A OUTPUT -p tcp -d mirrors.163.com -j ACCEPT
#允许入站ssh
$IPT -A INPUT -p tcp -s 58.102.13.91 --dport 22 -j ACCEPT
#允许cacti监控
#$IPT -A INPUT -p tcp -s 222.102.153.192 --dport 161 -j ACCEPT
$IPT -A INPUT -s 222.102.153.192 -j ACCEPT
#$IPT -A INPUT -p tcp  --dport 443 -j ACCEPT
#$IPT -A INPUT -p tcp  --dport 80 -j ACCEPT
#$IPT -A INPUT -p tcp -s 127.0.0.1 --dport 3306 -j ACCEPT




xiaofei_script
#!/bin/sh
#
modprobe ipt_MASQUERADE
modprobe ip_conntrack_ftp
modprobe ip_nat_ftp
iptables -F
iptables -t nat -F
iptables -X
iptables -t nat -X
###########################INPUT键###################################

iptables -P INPUT DROP
iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
iptables -A INPUT -p tcp -m multiport --dports 110,80,25 -j ACCEPT
iptables -A INPUT -p tcp -s 192.168.0.0/24 --dport 139 -j ACCEPT
#允许内网samba,smtp,pop3,连接
iptables -A INPUT -i eth1 -p udp -m multiport --dports 53 -j ACCEPT
#允许dns连接
iptables -A INPUT -p tcp --dport 1723 -j ACCEPT
iptables -A INPUT -p gre -j ACCEPT
#允许外网vpn连接
iptables -A INPUT -s 192.186.0.0/24 -p tcp -m state --state ESTABLISHED,RELATED -j ACCEPT
iptables -A INPUT -i ppp0 -p tcp --syn -m connlimit --connlimit-above 15 -j DROP
#为了防止DOS太多连接进来,那么可以允许最多15个初始连接,超过的丢弃
iptables -A INPUT -s 192.186.0.0/24 -p tcp --syn -m connlimit --connlimit-above 15 -j DROP
#为了防止DOS太多连接进来,那么可以允许最多15个初始连接,超过的丢弃
iptables -A INPUT -p icmp -m limit --limit 3/s -j LOG --log-level INFO --log-prefix "ICMP packet IN: "
iptables -A INPUT -p icmp -j DROP
#禁止icmp通信-ping 不通
iptables -t nat -A POSTROUTING -o ppp0 -s 192.168.0.0/24 -j MASQUERADE
#内网转发
iptables -N syn-flood
iptables -A INPUT -p tcp --syn -j syn-flood
iptables -I syn-flood -p tcp -m limit --limit 3/s --limit-burst 6 -j RETURN
iptables -A syn-flood -j REJECT
#防止SYN攻击 轻量
#######################FORWARD链###########################
iptables -P FORWARD DROP
iptables -A FORWARD -p tcp -s 192.168.0.0/24 -m multiport --dports 80,110,21,25,1723 -j ACCEPT
iptables -A FORWARD -p udp -s 192.168.0.0/24 --dport 53 -j ACCEPT
iptables -A FORWARD -p gre -s 192.168.0.0/24 -j ACCEPT
iptables -A FORWARD -p icmp -s 192.168.0.0/24 -j ACCEPT
#允许 vpn客户走vpn网络连接外网
iptables -A FORWARD -m state --state ESTABLISHED,RELATED -j ACCEPT
iptables -I FORWARD -p udp --dport 53 -m string --string "tencent" -m time --timestart 8:15 --timestop 12:30 --days Mon,Tue,Wed,Thu,Fri,Sat  -j DROP
#星期一到星期六的8:00-12:30禁止qq通信
iptables -I FORWARD -p udp --dport 53 -m string --string "TENCENT" -m time --timestart 8:15 --timestop 12:30 --days Mon,Tue,Wed,Thu,Fri,Sat  -j DROP
#星期一到星期六的8:00-12:30禁止qq通信
iptables -I FORWARD -p udp --dport 53 -m string --string "tencent" -m time --timestart 13:30 --timestop 20:30 --days Mon,Tue,Wed,Thu,Fri,Sat  -j DROP
iptables -I FORWARD -p udp --dport 53 -m string --string "TENCENT" -m time --timestart 13:30 --timestop 20:30 --days Mon,Tue,Wed,Thu,Fri,Sat  -j DROP
#星期一到星期六的13:30-20:30禁止QQ通信
iptables -I FORWARD -s 192.168.0.0/24 -m string --string "qq.com" -m time --timestart 8:15 --timestop 12:30 --days Mon,Tue,Wed,Thu,Fri,Sat  -j DROP
#星期一到星期六的8:00-12:30禁止qq网页
iptables -I FORWARD -s 192.168.0.0/24 -m string --string "qq.com" -m time --timestart 13:00 --timestop 20:30 --days Mon,Tue,Wed,Thu,Fri,Sat  -j DROP
#星期一到星期六的13:30-20:30禁止QQ网页
iptables -I FORWARD -s 192.168.0.0/24 -m string --string "ay2000.net" -j DROP
iptables -I FORWARD -d 192.168.0.0/24 -m string --string "宽频影院" -j DROP
iptables -I FORWARD -s 192.168.0.0/24 -m string --string "色情" -j DROP
iptables -I FORWARD -p tcp --sport 80 -m string --string "广告" -j DROP
#禁止ay2000.net，宽频影院，色情，广告网页连接 ！但中文 不是很理想
iptables -A FORWARD -m ipp2p --edk --kazaa --bit -j DROP
iptables -A FORWARD -p tcp -m ipp2p --ares -j DROP
iptables -A FORWARD -p udp -m ipp2p --kazaa -j DROP
#禁止BT连接
iptables -A FORWARD -p tcp --syn --dport 80 -m connlimit --connlimit-above 15 --connlimit-mask 24
#######################################################################
sysctl -w net.ipv4.ip_forward=1 &>/dev/null
#打开转发
#######################################################################
sysctl -w net.ipv4.tcp_syncookies=1 &>/dev/null
#打开 syncookie （轻量级预防 DOS 攻击）
sysctl -w net.ipv4.netfilter.ip_conntrack_tcp_timeout_established=3800 &>/dev/null
#设置默认 TCP 连接痴呆时长为 3800 秒（此选项可以大大降低连接数）
sysctl -w net.ipv4.ip_conntrack_max=300000 &>/dev/null
#设置支持最大连接树为 30W（这个根据你的内存和 iptables 版本来，每个 connection 需要 300 多个字节）
#######################################################################
iptables -I INPUT -s 192.168.0.50 -j ACCEPT
iptables -I FORWARD -s 192.168.0.50 -j ACCEPT
#192.168.0.50是我的机子，全部放行！
############################完#########################################

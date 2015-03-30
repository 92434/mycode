#!/bin/bash
echo "$# parameters"

channel=1
bandwidth=0
txpower=55
mp_rate=108

test=0

echo "start test wifi"


#if [ $# -ge 1 ] 
#then
	channel=$1
#fi
#if [ $# -ge 2 ] 
#then
	bandwidth=$2
#fi
#if [ $# -ge 3 ] 
#then 
	txpower=$3
#fi
#if [ $# -ge 4 ] 
#then
	mp_rate=$4
#fi

echo "channel = $channel, bandwidth = $bandwidth, 
      txpower = $txpower, mp_rate = $mp_rate"
#init wlan0 and mp mode

ifconfig wlan0 up
sleep 1
iwpriv wlan0 mp_start

echo "iwpriv wlan0 mp_channel $channel"
iwpriv wlan0 mp_channel $channel
echo "iwpriv wlan0 mp_bandwidth 40M=$bandwidth,shortGI=0"
iwpriv wlan0 mp_bandwidth 40M=$bandwidth,shortGI=0
iwpriv wlan0 mp_ant_tx a
echo "iwpriv wlan0 mp_txpower patha=$txpower,pathb=$txpower"
iwpriv wlan0 mp_txpower patha=$txpower,pathb=$txpower
echo "iwpriv wlan0 mp_rate $mp_rate" 
iwpriv wlan0 mp_rate $mp_rate
iwpriv wlan0 mp_ctx background

read test

iwpriv wlan0 mp_ctx stop
iwpriv wlan0 mp_stop
ifconfig wlan0 down

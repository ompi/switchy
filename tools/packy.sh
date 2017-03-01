#!/bin/sh

if [[ -z $1 ]]; then
  echo "Please specify ethernet interface."
  exit 1
fi

interface=$1

wait_for_link() {
  echo -n "waiting for ${interface} link $1"
  while [[ `cat /sys/class/net/${interface}/operstate` != "$1" ]]; do
    echo -n '.'
    sleep .5
  done
  echo
}

setup_port() {
  dhcpcd -k ${interface}

  iptables -A INPUT -i ${interface} -j DROP
  iptables -A OUTPUT -o ${interface} -j DROP
  ip6tables -A OUTPUT -o ${interface} -j DROP
  iptables -A OUTPUT -o ${interface} -p icmp --icmp-type echo-request -j ACCEPT

  ip link set ${interface} up
  ip addr add 10.0.0.1/24 dev ${interface}
}

poke_port() {
  packet_num=$(( $i * 10 ))
  ping -b 10.0.0.255 -c ${packet_num} -f -s 954 -W 1   # 1000 octets on wire
}

setup_port

for i in `seq 5`; do
  echo "poking port $i."
  wait_for_link up
  sleep 1
  poke_port $i
  wait_for_link down
done

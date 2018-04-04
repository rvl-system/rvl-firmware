# Setting up the wifi AP:

```Bash
sudo apt-get install hostapd dnsmasq
sudo systemctl stop hostapd
sudo systemctl stop dnsmasq
```

/etc/dhcpcd.conf

```
nohook wpa_supplicant
interface wlan0
static ip_address=192.168.42.1/24
static routers=192.168.42.1
```

/etc/dnsmasq.conf

```
interface=wlan0
  dhcp-range=192.168.42.2,192.168.42.100,255.255.255.0,24h
```

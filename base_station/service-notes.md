Create a service in `/etc/systemd/system/raver-lights.service` with the following contents:

```
[Unit]
Requires=systemd-networkd.socket
After=systemd-networkd.socket

[Service]
ExecStartPre=/lib/systemd/systemd-networkd-wait-online --interface=wlan0
ExecStart=/usr/bin/node /home/pi/raver-lights/base_station/bin/cli.js
Restart=always
StandardOutput=syslog
StandardError=syslog
SyslogIdentifier=raver-lights
User=srv-raver-lights
Group=srv-raver=lights
Environment=NODE_ENV=production
User=root

[Install]
WantedBy=multi-user.target
```

Run the following commands:

```
sudo systemctl enable systemd-networkd-wait-online.service
sudo systemctl enable raver-lights
```

Then reboot

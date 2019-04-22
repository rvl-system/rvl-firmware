# Setting up the wifi AP:

Follow instructions [here](http://www.raspberryconnect.com/network/item/333-raspberry-pi-hotspot-access-point-dhcpcd-method).

Note: there was an update to Raspbian that changed thinks a bit. You'll need to run these commands once you're done with everything from the above instructions:

```bash
sudo systemctl unmask hostapd
sudo systemctl enable hostapd
sudo systemctl start hostapd
```

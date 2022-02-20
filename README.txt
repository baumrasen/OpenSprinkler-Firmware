============================================
==== OpenSprinkler AVR/RPI/BBB Firmware ====
============================================

This is a unified OpenSprinkler firmware for Arduino, and Linux-based OpenSprinklers such as OpenSprinkler Pi.

For OS (Arduino-based OpenSprinkler) 2.x:
https://openthings.freshdesk.com/support/solutions/articles/5000165132-how-to-compile-opensprinkler-firmware

For OSPi/OSBO or other Linux-based OpenSprinkler:
https://openthings.freshdesk.com/support/solutions/articles/5000631599-installing-and-updating-the-unified-firmware

with OLED-display
https://github.com/hallard/ArduiPi_OLED.git
checkout beside opensprinkler folder
install it:
http://hallard.me/adafruit-oled-display-driver-for-pi/
On my Raspberry Pi 4 with Debian Bullseye I had to do this, to geht the example working:
sudo apt-get install gcc-7 g++-7
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 60 --slave /usr/bin/g++ g++ /usr/bin/g++-7
sudo update-alternatives --set gcc "/usr/bin/gcc-7"

============================================
Questions and comments:
http://www.opensprinkler.com
============================================

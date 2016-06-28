# estank
Code for a wirelessly controlled Arduino tank

# Summary
I wanted to build a wireless tank using a Tamiya tank chassis and an ethernet Arduino.  Especially because the Ethernet Arduino and my motor shield cannot be combined, I ended up needing a few more parts, but at this point it's working!

# Features

* Ethernet library for serving a web page from an Ethernet Arduino and handling direction change requests
* The server part is very simple, requires only one page by using redirects
* Requested direction or mode is passed over Wire Library to Arduino Uno connected to the motor driver

# Hardware Components

This code assumes the following hardware:

* Ethernet Arduino (should mostly work with an Ethernet Shield) [1]
* TP-LINK TL-MR3040 3G/4G Wireless N150 (For Wi-Fi) [2]
* Seeed Motor Shield V1.2 [3]
* Arduino Uno [4]
* Tamiya 70108 Tracked Vehicle Chassis [5]
* Tamiya 70168 Powered Double Gearbox L/R Independent 4-Speed Kit [6]
* FTDI Cable for programming Ethernet Arduino
* Electric motors, batteries, etc.

[1]: https://www.arduino.cc/en/Main/ArduinoBoardEthernet
[2]: https://www.amazon.com/TP-LINK-TL-MR3040-Wireless-Portable-Compatible/dp/B0088PPFP4
[3]: http://www.seeedstudio.com/wiki/Motor_Shield_V1.0
[4]: https://www.arduino.cc/en/Main/ArduinoBoardUno
[5]: https://www.amazon.com/Tamiya-70108-Tracked-Vehicle-Chassis/dp/B00061HHTK
[6]: https://www.amazon.com/Tamiya-Powered-Gearbox-Independent-4-Speed/dp/B001Q13BIU

# Future Directions

Possible improvements:

* An articulated turret that could fire Nerf darts
* Lighter weight Wi-Fi solution than TP-Link


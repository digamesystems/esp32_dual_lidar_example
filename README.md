# esp32_dual_lidar_example

A little example program showing how to use two LIDAR sensors to establish direction of travel. 

The program makes us of Bud Ryerson's `TFMPlus` class: https://github.com/budryerson/TFMini-Plus to communicate with Benewake [TFMINI-Plus](http://en.benewake.com/product/detail/5c345cd0e5b3a844c472329b.html) LIDAR Sensors.

Raw LIDAR signals are run through a smoothing algorithm and then a distance threshold is used to establish "presence" in front of each sensor. Presence on one sensor, followed by presence on both establishes direction of travel. In this example, the thresholds are hard-coded. Results are output to the `Serial` port in an offset format suitable for the serial plotter utility in the Arduino IDE.

## Initial Prototype

As a quick prototype, I used two TFMini-S LIDARs, a breadboard with the ESP32, and a [cheap plastic enclosure](https://www.amazon.com/gp/product/B073Y7RHQ4?th=1) from Amazon. 

![Prototype](/doc/proto1.jpg)


## Example 'IN' Event (Serial Plotter Output)

![Serial Plotter Output](/doc/in_event.jpg)


## Example 'OUT' Event 

![Serial Plotter Output](/doc/out_event.jpg)



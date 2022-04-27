# esp32_dual_lidar_example

A little example program showing how to use two LIDAR sensors to establish direction of travel. 

The program makes us of Bud Ryerson's `TFMPlus` class: https://github.com/budryerson/TFMini-Plus to communicate with Benewake [TFMINI-Plus](http://en.benewake.com/product/detail/5c345cd0e5b3a844c472329b.html) LIDAR Sensors.

Raw LIDAR signals are run through a smoothing algorithm and then a distance threshold is used to establish "presence" in front of each sensor. Presence on one sensor, followed by presence on both establishes direction of travel. In this example, the thresholds are hard-coded. Results are output to the `Serial` port in an offset format suitable for the serial plotter utility in the Arduino IDE.

## Initial Prototype

As a quick prototype, I used two TFMini-S LIDARs, a breadboard with the ESP32, and a [cheap plastic enclosure](https://www.amazon.com/gp/product/B073Y7RHQ4?th=1) from Amazon. 

<img src="/doc/proto1.jpg" width="50%" />

The assembled prototype with it's base station unit for an application to track passengers entering / leaving a shuttle bus. The counter and base station communicate via Bluetooth. The base station provides an eInk display for the driver of the shuttle bus showing in / out counts as the bus navigates its route.

<img src="/doc/proto2.jpg" width="50%" />

## Example 'IN' Event (Serial Plotter Output)

<img src="/doc/in_event.jpg" width="50%" />

## Example 'OUT' Event 

<img src="/doc/out_event.jpg" width="50%" />


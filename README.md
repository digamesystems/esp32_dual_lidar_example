# esp32_dual_lidar_example
A little example program showing how to use two lidar sensors to establish direction of travel. 

Raw LIDAR signals are run through a smoothing algorithm and then a distance threshold is used to establish "presence" in front of each sensor. Presence on one sensor, followed by presence on both establishes direction of travel. 

##Example 'IN' Event

![Serial Plotter Output](/doc/in_event.jpg)


##Example 'OUT' Event

![Serial Plotter Output](/doc/out_event.jpg)



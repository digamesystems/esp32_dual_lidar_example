/*
    This program uses a pair of TFMini-plus LIDARs to count people moving
    through a portal. 
    
    The two sensors are used in combination to determine direction of travel and 
    thus if a person is entering or exiting.

    Hint: When running, use the Serial Plotter utility to see what's going on.
    
    LIDAR Sensor:
    http://en.benewake.com/product/detail/5c345cd0e5b3a844c472329b.html

    Written for the ESP32 WROOM Dev board V4 (Incl. multiple UARTs, WiFi, Bluetooth, and stacks of I/O.):
    https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/hw-reference/esp32c3/user-guide-devkitc-02.html

    Written by J. Price
    Copyright 2021, Digame Systems. All rights reserved.

    Testing the Sensor: 
    https://youtu.be/LPxUawpHUEk
    https://youtu.be/7mM1T-jgChU
    https://youtu.be/dQw4w9WgXcQ

 */

#include <TFMPlus.h>      // Include Bud Ryerson's nifty TFMini Plus LIDAR Library v1.5.0
                          // https://github.com/budryerson/TFMini-Plus

// The ESP32 module has 2 serial ports in addition the programming port for 
// use by applications. We assign one to each TFMINI-S /Plus

#define tfMiniUART_1 Serial1
#define tfMiniUART_2 Serial2

TFMPlus tfmP_1;   // Create a TFMini Plus object
TFMPlus tfmP_2;   // Create another TFMini Plus object

// Smoothing the LIDAR signals to get rid of noise and introduce a bit 
// of a decay time. 
float smoothed_LIDAR_1 = 0.0;
float smoothed_LIDAR_2 = 0.0;

// We're tracking the visibility of the target on both sensors. Valid 
// events go from only visible on one sensor to being visible on both. 
// Direction is determined by which sensor sees the target first. 
int previousState = 0; 
int state = 0;  


// Declares
void init_TFMPlus (TFMPlus &tfmP, int port=1); 
int  process_LIDAR(TFMPlus &tfmP, float &smoothed, int offset);


//****************************************************************************************
// Device initialization                                   
//****************************************************************************************
void setup()
{
    Serial.begin(115200);   // Intialize terminal serial port
    delay(1000);            // Give port time to initalize
    
    Serial.println("*****************************************************");
    Serial.println("ParkData LIDAR Sensor Example");
    Serial.println("Version 1.0");
    Serial.println("Copyright 2021, Digame Systems. All rights reserved.");
    Serial.println("*****************************************************");

    // You might get away without the pin designations for TX and RX. I had to map them
    // to the custom PCB we made for the project.

                     // BAUD   PARITY    TX RX 
    tfMiniUART_1.begin(115200,SERIAL_8N1,25,33);  // Initialize TFMPLus device serial port.
    tfMiniUART_2.begin(115200,SERIAL_8N1,27,26);  // Initialize TFMPLus device serial port.

    init_TFMPlus(tfmP_1, 1);
    init_TFMPlus(tfmP_2, 2);
    
}


//****************************************************************************************
// Main Loop                                   
//****************************************************************************************
void loop(){
  int value = 0;
  state = 0;
  value = process_LIDAR(tfmP_1, smoothed_LIDAR_1, 100); 
  if (value>=0){
    
      state = state + value;
      value = process_LIDAR(tfmP_2, smoothed_LIDAR_2, 110);
      state = state + value * 2;
      
      Serial.print(" ");
      Serial.println();

      if ((previousState == 2)&&(state ==3)){
        Serial.println("IN_Event!");  
      }

      if ((previousState == 1) && (state == 3)){
        Serial.println("OUT_Event!");  
      }
      
      previousState = state;
  }
}


//****************************************************************************************
// Initialize a sensor. Issue a soft reset and set the data collection rate to 100 Hz.
//****************************************************************************************

void init_TFMPlus(TFMPlus &tfmP, int port){
   
  // Initialize device library object and pass device serial port to the object.
  if (port == 1){
    tfmP.begin(&tfMiniUART_1);   
  }else if (port == 2){
    tfmP.begin(&tfMiniUART_2);   
  }else{
    Serial.println("Unknown Port.");
    return;
  }
     
  // Send some commands to configure the TFMini-Plus
  
  // Perform a system reset
  Serial.print("Activating LIDAR Sensor ");
  Serial.println(port);
  
  if( tfmP.sendCommand(SOFT_RESET, 0)){
    Serial.println("Sensor Active.");
  }
  else{
    Serial.println("TROUBLE ACTIVATING LIDAR!");                    
    tfmP.printReply();
  }

  delay(1000);

  // Set the acquisition rate to 100 Hz.
  Serial.printf( "Adjusting Frame Rate... ");
  if( tfmP.sendCommand(SET_FRAME_RATE, FRAME_100)){
    Serial.println("Frame Rate Adjusted.");
  }
  else tfmP.printReply();
 
}

//****************************************************************************************
// This routine collects raw LIDAR samples, applies a smoothing routine to the data
// and sends data to the serial port. An offset makes it easier to see what is going
// on in the serial plotter.  
//****************************************************************************************

int process_LIDAR(TFMPlus &tfmP, float &smoothed, int offset){
  
  int16_t tfDist = 0;    // Distance to object in centimeters
  int16_t tfFlux = 0;    // Strength or quality of return signal
  int16_t tfTemp = 0;    // Internal temperature of Lidar sensor chip

  float smoothingFactor = 0.95; // The closer to 1.0, the smoother the data
  
  int targetVisible = false;
  
  tfmP.sendCommand(TRIGGER_DETECTION, 0);
  
  // Read the LIDAR Sensor
  if( tfmP.getData( tfDist, tfFlux, tfTemp)) { 

    //Filter the measured distance
    smoothed = smoothed * smoothingFactor + (float)tfDist * (1.0 - smoothingFactor); 
        
    Serial.print(smoothed);
    Serial.print(" ");

    targetVisible = (smoothed < 160);

    if (targetVisible){
      Serial.print(300 + offset);
    } else {
      Serial.print(200 + offset);
    }  
    Serial.print(" ");

    if (targetVisible) {
      return 1;
    } else {
      return 0;
    }
   
  }

  return -1; 
    
}

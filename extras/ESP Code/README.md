This is a modified example from the Bluepad32 library.  
  
To reduce latency most prints to the serial interface pointed to by "Serial" were converted to only print out in debug mode (reduces cycle time)  
  
In this example, "Serial1" of the ESP32-C3 WROOM chip (IO6 & IO7) is connected to "Serial1" of an ATMEGA 2560 (TXD2 & RXD2, respectively) through a 3.3V/5V Logic Level Shifter.  
  
After loading this code onto the ESP32-C3, any supported controllers placed in pairing mode connect fairly quickly.  
  
So far only support for XBox bluetooth controllers has been added to this library, though other controllers may be compatible.  
  
"Serial1" print statements were added allowing the connected chip to receive controller connection and disconnection messages as well as input status messages.  
  
The format of the messages in the "dumpGamepad" function is important and required for this library to successfully parse the input state of a connected controller.  
  
This code can handle up to four connected controllers at one time, though the connected chip will ignore any controllers outside of the specified amount.
"# Arduino-XBox-Controller-Handler" 

This library is intended to be used in the arduino IDE and processes strings sent from an XBox controller receiver.  
  
This is useful when available esp chips do not have the required I/O for the purpose of the project.  
  
A key feature of the library is the ability to provide pointers to custom functions to be executed as input handlers upon the change of a controller state (these can vary by controller index as well)  
  
Utilizing an ESP chip with another arduino compatible chip featuring extensive I/O capabilities can be beneficial for a handful of purposes.   
  
This library accepts strings formatted in a specific manner, such that the code running on the receiver will have to package data in this specific way.  
  
Upon instantiation of a ControllerManager object, the provided "receiverSerial" serial pointer will indicate which uart channel the receiver is communicating over.  
  
A ControllerManager object can handle up to 4 controllers and is determined by the provided "controllerCount" parameter upon instantiation.  
  
There is a provided ControllerManagerConfig struct that packages all necessary instantiation parameters into a single variable, including uart communication configuration parameters.  
  
This library has been tested with an ESP32-C3 WROOM communicating with an ATMEGA 2560 over Serial1.  The code for the ESP is provided and was compiled/uploaded through the arduino IDE. (note: this code is a modified form of a BluePad example)  
  
For more information on setting an ESP chip up to run code utilizing the bluepad library visit: https://github.com/ricardoquesada/bluepad32  
  
While the bluepad library supports many types of controllers, this library is specific to an xbox controller, though it may work for other types of controllers as well.  
  
If using a vastly different controller, this library can be used as a template for parsing the input states.  
  
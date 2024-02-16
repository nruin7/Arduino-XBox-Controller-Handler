#include "XBoxControllerHandler.h"

XBoxControllerHandler::XBoxControllerHandler(XBoxControllerHandlerConfig config) {
  m_uartConfig = config.uartConfig;
  m_processorSerial = config.processorSerial;
  m_receiverSerial = config.receiverSerial;
  m_resetPin = config.resetPin;
  m_controllerCount = config.controllerCount;
  //for(int i = 0; i < m_controllerCount);
  m_controllers = new Controller[m_controllerCount];
  for(int i = 0; i < m_controllerCount; i++)
  {
    m_controllers[i] = Controller(i);
  }
}

XBoxControllerHandler::~XBoxControllerHandler() {
  delete[] m_controllers;
}

bool XBoxControllerHandler::Setup() {
  m_receiverSerial->begin(m_uartConfig.baudRate, GetSerialConfig(m_uartConfig));
  pinMode(m_resetPin, OUTPUT);
  digitalWrite(m_resetPin, HIGH);
  delay(2000);
  m_processorSerial->println(F("Controller Manager Initialized"));
  return true;
}

void XBoxControllerHandler::Loop() {
  ProcessSerial();
}

void XBoxControllerHandler::Reset() {
  m_processorSerial->println("Hardware Reset");
  digitalWrite(m_resetPin, LOW);
  delay(1000);
  digitalWrite(m_resetPin, HIGH);
  delay(1000);
  m_processorSerial->println("Done");
}

void XBoxControllerHandler::SetBtnOnPress(void (*fxPtr)(int controllerIndex, ControllerButtons controllerButton)) {
  for(int i = 0; i < m_controllerCount; i++)
  {
    m_controllers[i].SetBtnOnPress(fxPtr);
  }
}

void XBoxControllerHandler::SetBtnOnRelease(void (*fxPtr)(int controllerIndex, ControllerButtons controllerButton)) {
  for(int i = 0; i < m_controllerCount; i++)
  {
    m_controllers[i].SetBtnOnRelease(fxPtr);
  }
}

void XBoxControllerHandler::SetTrigOnChange(void (*fxPtr)(int controllerIndex, ControllerTriggers controllerTrigger, int newVal)) {
  for(int i = 0; i < m_controllerCount; i++)
  {
    m_controllers[i].SetTrigOnChange(fxPtr);
  }
}

void XBoxControllerHandler::SetStickOnChange(void (*fxPtr)(int controllerIndex, ControllerSticks controllerStick, int xVal, int yVal)) {
  for(int i = 0; i < m_controllerCount; i++)
  {
    m_controllers[i].SetStickOnChange(fxPtr);
  }
}

void XBoxControllerHandler::SetTrigTolerance(int controllerIndex, ControllerTriggers trigger, int tolerance) {
  switch(trigger)
  {
    case LEFT_TRIG:
      m_controllers[controllerIndex].leftTrig.SetTolerance(tolerance);
      break;
    case RIGHT_TRIG:
      m_controllers[controllerIndex].rightTrig.SetTolerance(tolerance);
      break;
    default:
      m_processorSerial->println("Invalid Trigger");
      break;
  }
}

void XBoxControllerHandler::SetStickToleranceX(int controllerIndex, ControllerSticks stick, int tolerance) {
  switch(stick)
  {
    case LEFT_STICK:
      m_controllers[controllerIndex].leftStick.SetToleranceX(tolerance);
      break;
    case RIGHT_STICK:
      m_controllers[controllerIndex].rightStick.SetToleranceX(tolerance);
      break;
    default:
      m_processorSerial->println("Invalid Stick");
      break;
  }
}

void XBoxControllerHandler::SetStickToleranceY(int controllerIndex, ControllerSticks stick, int tolerance) {
  switch(stick)
  {
    case LEFT_STICK:
      m_controllers[controllerIndex].leftStick.SetToleranceY(tolerance);
      break;
    case RIGHT_STICK:
      m_controllers[controllerIndex].rightStick.SetToleranceY(tolerance);
      break;
    default:
      m_processorSerial->println("Invalid Stick");
      break;
  }
}

void XBoxControllerHandler::ProcessSerial() {
  if(m_receiverSerial->available()) {
    String data = m_receiverSerial->readStringUntil('\n');
    if(data.length() > 2) {
      while(data.endsWith("\r") || data.endsWith("\n")) {
        data.remove(data.length() - 1);
      }
    }
    if(data.length() > 0 && data != "\r" && data != "\n") {
      //m_processorSerial->print("Recv: '");
      //m_processorSerial->print(data);
      //m_processorSerial->print("'\n");
      if(data.substring(0, 4) == "DATA")
      {
        //m_processorSerial->println(data.substring(5));
        ParseData(data);
      }else if(data == "OUT OF SLOTS")
      {
        m_processorSerial->println(data);
      }else if(data == "DISCONNECT: NOT FOUND")
      {
        m_processorSerial->println(data);
      }else if(data.substring(0, 11) == "DISCONNECT:")
      {
        m_processorSerial->println(data);
      }else if(data.substring(0, 10) == "CONNECTED:")
      {
        m_processorSerial->println(data);
      }
    }
  }
}

void XBoxControllerHandler::ParseData(String data) {
  int startPos = 0;
  int messageIndex = 0;
  int controllerIndex = 0;
  String substring = "";
  for (int i = 0; i < data.length(); i++) {
    if (data.charAt(i) == '|') {
      if(messageIndex != 0)
      {
        substring = data.substring(startPos, i);
        if(messageIndex == 1)
        {
          controllerIndex = substring.toInt();
        }else if(controllerIndex >= 0 && controllerIndex < m_controllerCount)
        {
          ProcessData(substring, controllerIndex, messageIndex);
        }else
        {
          return;
        }
      }
      messageIndex++;
      startPos = i + 1;
    }
  }
  if (controllerIndex >= 0 && controllerIndex < m_controllerCount)
  {
      substring = data.substring(startPos, data.length());
      ProcessData(substring, controllerIndex, messageIndex);
  }
  //m_processorSerial->println();
  //m_processorSerial->println();
}

void XBoxControllerHandler::ProcessData(String data, int controllerIndex, int messageIndex) {\
  int messageVal = data.toInt();
  if(data != "0" && messageVal == 0)
  {
    m_processorSerial->print(F("Error Processing Data: "));
    m_processorSerial->println(data);
    return;
  }
  switch(messageIndex)
  {
    case 2:
      //Encoded D-Pad
      //m_processorSerial->print("D PAD: ");
      //m_processorSerial->println(messageVal);
      m_controllers[controllerIndex].SetButton(UP_DPAD, (messageVal >> 0) & 1);
      m_controllers[controllerIndex].SetButton(DOWN_DPAD, (messageVal >> 1) & 1);
      m_controllers[controllerIndex].SetButton(RIGHT_DPAD, (messageVal >> 2) & 1);
      m_controllers[controllerIndex].SetButton(LEFT_DPAD, (messageVal >> 3) & 1);
      break;
    case 3:
      //Encoded Buttons
      //m_processorSerial->print("BUTTONS: ");
      //m_processorSerial->println(messageVal);
      m_controllers[controllerIndex].SetButton(A_BTN, (messageVal >> 0) & 1);
      m_controllers[controllerIndex].SetButton(B_BTN, (messageVal >> 1) & 1);
      m_controllers[controllerIndex].SetButton(X_BTN, (messageVal >> 2) & 1);
      m_controllers[controllerIndex].SetButton(Y_BTN, (messageVal >> 3) & 1);
      m_controllers[controllerIndex].SetButton(LEFT_BUMP, (messageVal >> 4) & 1);
      m_controllers[controllerIndex].SetButton(RIGHT_BUMP, (messageVal >> 5) & 1);
      m_controllers[controllerIndex].SetButton(LEFT_TRIG_BTN, (messageVal >> 6) & 1);
      m_controllers[controllerIndex].SetButton(RIGHT_TRIG_BTN, (messageVal >> 7) & 1);
      m_controllers[controllerIndex].SetButton(LEFT_STICK_BTN, (messageVal >> 8) & 1);
      m_controllers[controllerIndex].SetButton(RIGHT_STICK_BTN, (messageVal >> 9) & 1);
      break;
    case 4:
      //Left Stick X
      //m_processorSerial->print("L STICK X: ");
      //m_processorSerial->println(messageVal);
      m_controllers[controllerIndex].SetStickX(LEFT_STICK, messageVal);
      break;
    case 5:
      //Left Stick Y
      //m_processorSerial->print("L STICK Y: ");
      //m_processorSerial->println(messageVal);
      m_controllers[controllerIndex].SetStickY(LEFT_STICK, messageVal);
      break;
    case 6:
      //Right Stick X
      //m_processorSerial->print("R STICK X: ");
      //m_processorSerial->println(messageVal);
      m_controllers[controllerIndex].SetStickX(RIGHT_STICK, messageVal);
      break;
    case 7:
      //Right Stick Y
      //m_processorSerial->print("R STICK Y: ");
      //m_processorSerial->println(messageVal);
      m_controllers[controllerIndex].SetStickY(RIGHT_STICK, messageVal);
      break;
    case 8:
      //Left Trigger
      //m_processorSerial->print("L TRIG: ");
      //m_processorSerial->println(messageVal);
      m_controllers[controllerIndex].SetTrigger(LEFT_TRIG, messageVal);
      break;
    case 9:
      //Right Trigger
      //m_processorSerial->print("R TRIG: ");
      //m_processorSerial->println(messageVal);
      m_controllers[controllerIndex].SetTrigger(RIGHT_TRIG, messageVal);
      break;
    case 10:
      //Misc Buttons
      //m_processorSerial->print("MISC: ");
      //m_processorSerial->println(messageVal);
      m_controllers[controllerIndex].SetButton(XBOX_BTN, (messageVal >> 0) & 1);
      m_controllers[controllerIndex].SetButton(VIEW_BTN, (messageVal >> 1) & 1);
      m_controllers[controllerIndex].SetButton(MENU_BTN, (messageVal >> 2) & 1);
      m_controllers[controllerIndex].SetButton(SHARE_BTN, (messageVal >> 3) & 1);
      break;
    default:
      //Unaccounted For
      m_processorSerial->print(F("Unrecognized Index "));
      m_processorSerial->print(messageIndex);
      m_processorSerial->print(F(" Message: "));
      m_processorSerial->println(data);
      break;
  }
}

int XBoxControllerHandler::GetSerialConfig(UartConfig uartConfig) {
  int serialConfig = -1;
  switch(uartConfig.dataBits) {
    case DATA_5:
      switch(uartConfig.parityBits) {
        case PARITY_NONE:
          switch(uartConfig.stopBits) {
            case STOP_1:
              serialConfig = SERIAL_5N1;
              break;
            case STOP_2:
              serialConfig = SERIAL_5N2;
              break;
            default:
              serialConfig = -1;
          }
          break;
        case PARITY_ODD:
          switch(uartConfig.stopBits) {
            case STOP_1:
              serialConfig = SERIAL_5O1;
              break;
            case STOP_2:
              serialConfig = SERIAL_5O2;
              break;
            default:
              serialConfig = -1;
          }
          break;
        case PARITY_EVEN:
          switch(uartConfig.stopBits) {
            case STOP_1:
              serialConfig = SERIAL_5E1;
              break;
            case STOP_2:
              serialConfig = SERIAL_5E2;
              break;
            default:
              serialConfig = -1;
          }
          break;
        default:
          serialConfig = -1;
      }
      break;
    case DATA_6:
      switch(uartConfig.parityBits) {
        case PARITY_NONE:
          switch(uartConfig.stopBits) {
            case STOP_1:
              serialConfig = SERIAL_6N1;
              break;
            case STOP_2:
              serialConfig = SERIAL_6N2;
              break;
            default:
              serialConfig = -1;
          }
          break;
        case PARITY_ODD:
          switch(uartConfig.stopBits) {
            case STOP_1:
              serialConfig = SERIAL_6O1;
              break;
            case STOP_2:
              serialConfig = SERIAL_6O2;
              break;
            default:
              serialConfig = -1;
          }
          break;
        case PARITY_EVEN:
          switch(uartConfig.stopBits) {
            case STOP_1:
              serialConfig = SERIAL_6E1;
              break;
            case STOP_2:
              serialConfig = SERIAL_6E2;
              break;
            default:
              serialConfig = -1;
          }
          break;
        default:
          serialConfig = -1;
      }
      break;
    case DATA_7:
      switch(uartConfig.parityBits) {
        case PARITY_NONE:
          switch(uartConfig.stopBits) {
            case STOP_1:
              serialConfig = SERIAL_7N1;
              break;
            case STOP_2:
              serialConfig = SERIAL_7N2;
              break;
            default:
              serialConfig = -1;
          }
          break;
        case PARITY_ODD:
          switch(uartConfig.stopBits) {
            case STOP_1:
              serialConfig = SERIAL_7O1;
              break;
            case STOP_2:
              serialConfig = SERIAL_7O2;
              break;
            default:
              serialConfig = -1;
          }
          break;
        case PARITY_EVEN:
          switch(uartConfig.stopBits) {
            case STOP_1:
              serialConfig = SERIAL_7E1;
              break;
            case STOP_2:
              serialConfig = SERIAL_7E2;
              break;
            default:
              serialConfig = -1;
          }
          break;
        default:
          serialConfig = -1;
      }
      break;
    case DATA_8:
    switch(uartConfig.parityBits) {
        case PARITY_NONE:
          switch(uartConfig.stopBits) {
            case STOP_1:
              serialConfig = SERIAL_8N1;
              break;
            case STOP_2:
              serialConfig = SERIAL_8N2;
              break;
            default:
              serialConfig = -1;
          }
          break;
        case PARITY_ODD:
          switch(uartConfig.stopBits) {
            case STOP_1:
              serialConfig = SERIAL_8O1;
              break;
            case STOP_2:
              serialConfig = SERIAL_8O2;
              break;
            default:
              serialConfig = -1;
          }
          break;
        case PARITY_EVEN:
          switch(uartConfig.stopBits) {
            case STOP_1:
              serialConfig = SERIAL_8E1;
              break;
            case STOP_2:
              serialConfig = SERIAL_8E2;
              break;
            default:
              serialConfig = -1;
          }
          break;
        default:
          serialConfig = -1;
      }
      break;
    default:
      serialConfig = -1;
  }
  return serialConfig;
}
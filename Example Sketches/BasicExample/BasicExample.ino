#include <XBoxControllerHandler.h>
#include <Arduino.h>

HardwareSerial* processorSerial = &Serial;
HardwareSerial* receiverSerial = &Serial2;
int numControllers = 4;

XBoxControllerHandlerConfig config = {
  .uartConfig = {
    .baudRate = BAUD_115200,
    .dataBits = DATA_8,
    .stopBits = STOP_1,
    .parityBits = PARITY_NONE,
  },
  .processorSerial = processorSerial,
  .receiverSerial = receiverSerial,
  .resetPin = 35,
  .controllerCount = numControllers
};

XBoxControllerHandler controllerHandler(config);

void setup() {
  processorSerial->begin(config.uartConfig.baudRate);
  if(controllerHandler.Setup())
  {
    AttachHandlers();
  }else
  {
    processorSerial->println(F("Controller Manager Failed Initialization"));
  }
}

void loop() {
  ProcessSerialInput();
  controllerHandler.Loop();
}

void ProcessSerialInput() {
  if(processorSerial->available()) {
    String input = processorSerial->readStringUntil('\n');
    if(input == "RESET\r") {
      controllerHandler.Reset();
    }
  }
}

void AttachHandlers() {
  controllerHandler.SetBtnOnPress(&ButtonPressed);
  controllerHandler.SetBtnOnRelease(&ButtonReleased);
  controllerHandler.SetTrigOnChange(&TriggerChanged);
  controllerHandler.SetStickOnChange(&StickChanged);
}

void ButtonPressed(int controllerIndex, ControllerButtons controllerButton) {
  processorSerial->print("Controller ");
  processorSerial->print(controllerIndex);
  processorSerial->print(" Button Pressed: ");
  switch(controllerButton)
  {
    case A_BTN:
      processorSerial->println("A");
      break;
    case B_BTN:
      processorSerial->println("B");
      break;
    case X_BTN:
      processorSerial->println("X");
      break;
    case Y_BTN:
      processorSerial->println("Y");
      break;
    case RIGHT_DPAD:
      processorSerial->println("->");
      break;
    case LEFT_DPAD:
      processorSerial->println("<-");
      break;
    case UP_DPAD:
      processorSerial->println("^");
      break;
    case DOWN_DPAD:
      processorSerial->println("v");
      break;
    case RIGHT_BUMP:
      processorSerial->println("R Bump");
      break;
    case LEFT_BUMP:
      processorSerial->println("L Bump");
      break;
    case XBOX_BTN:
      processorSerial->println("XBOX");
      break;
    case MENU_BTN:
      processorSerial->println("MENU");
      break;
    case VIEW_BTN:
      processorSerial->println("VIEW");
      break;
    case SHARE_BTN:
      processorSerial->println("SHARE");
      break;
    case LEFT_TRIG_BTN:
      processorSerial->println("L Trig");
      break;
    case RIGHT_TRIG_BTN:
      processorSerial->println("R Trig");
      break;
    case LEFT_STICK_BTN:
      processorSerial->println("L Stick");
      break;
    case RIGHT_STICK_BTN:
      processorSerial->println("R Stick");
      break;
    default:
      processorSerial->println("Unhandled Input...");
      break;
  }
  processorSerial->println("");
}

void ButtonReleased(int controllerIndex, ControllerButtons controllerButton) {
  processorSerial->print("Controller ");
  processorSerial->print(controllerIndex);
  processorSerial->print(" Button Released: ");
  switch(controllerButton)
  {
    case A_BTN:
      processorSerial->println("A");
      break;
    case B_BTN:
      processorSerial->println("B");
      break;
    case X_BTN:
      processorSerial->println("X");
      break;
    case Y_BTN:
      processorSerial->println("Y");
      break;
    case RIGHT_DPAD:
      processorSerial->println("->");
      break;
    case LEFT_DPAD:
      processorSerial->println("<-");
      break;
    case UP_DPAD:
      processorSerial->println("^");
      break;
    case DOWN_DPAD:
      processorSerial->println("v");
      break;
    case RIGHT_BUMP:
      processorSerial->println("R Bump");
      break;
    case LEFT_BUMP:
      processorSerial->println("L Bump");
      break;
    case XBOX_BTN:
      processorSerial->println("XBOX");
      break;
    case MENU_BTN:
      processorSerial->println("MENU");
      break;
    case VIEW_BTN:
      processorSerial->println("VIEW");
      break;
    case SHARE_BTN:
      processorSerial->println("SHARE");
      break;
    case LEFT_TRIG_BTN:
      processorSerial->println("L Trig");
      break;
    case RIGHT_TRIG_BTN:
      processorSerial->println("R Trig");
      break;
    case LEFT_STICK_BTN:
      processorSerial->println("L Stick");
      break;
    case RIGHT_STICK_BTN:
      processorSerial->println("R Stick");
      break;
    default:
      processorSerial->println("Unhandled Input...");
      break;
  }
  processorSerial->println("");
}

void TriggerChanged(int controllerIndex, ControllerTriggers controllerTrigger, int newVal) {
  processorSerial->print("Controller ");
  processorSerial->print(controllerIndex);
  processorSerial->print(" Trigger Changed: ");
  switch(controllerTrigger)
  {
    case LEFT_TRIG:
      processorSerial->print("Left - ");
      processorSerial->println(newVal);
      break;
    case RIGHT_TRIG:
      processorSerial->print("Right - ");
      processorSerial->println(newVal);
      break;
    default:
      processorSerial->println("Unhandled Input...");
      break;
  }
  processorSerial->println("");
}

void StickChanged(int controllerIndex, ControllerSticks controllerStick, int xVal, int yVal)
{
  processorSerial->print("Controller ");
  processorSerial->print(controllerIndex);
  processorSerial->print(" Stick Changed: ");
  switch(controllerStick)
  {
    case LEFT_STICK:
      processorSerial->print("Left - (");
      processorSerial->print(xVal);
      processorSerial->print(", ");
      processorSerial->print(yVal);
      processorSerial->println(")");
      break;
    case RIGHT_STICK:
      processorSerial->print("Right - (");
      processorSerial->print(xVal);
      processorSerial->print(", ");
      processorSerial->print(yVal);
      processorSerial->println(")");
      break;
    default:
      processorSerial->println("Unhandled Input...");
      break;
  }
  processorSerial->println("");
}
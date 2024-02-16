#ifndef XBOX_CONTROLLER_HANDLER_H
#define XBOX_CONTROLLER_HANDLER_H

#define MAXCONTROLLERS 4

#include <Arduino.h>
#include "Controller.h"
#include "UartConfig.h"

struct XBoxControllerHandlerConfig {
  UartConfig uartConfig;
  HardwareSerial* processorSerial;
  HardwareSerial* receiverSerial;
  int resetPin;
  int controllerCount;
};

class XBoxControllerHandler {
  public:
    XBoxControllerHandler(XBoxControllerHandlerConfig config);
    ~XBoxControllerHandler();
    bool Setup();
    void Loop();
    void Reset();
    void SetBtnOnPress(void (*fxPtr)(int controllerIndex, ControllerButtons controllerButton));
    void SetBtnOnRelease(void (*fxPtr)(int controllerIndex, ControllerButtons controllerButton));
    void SetTrigOnChange(void (*fxPtr)(int controllerIndex, ControllerTriggers controllerTrigger, int newVal));
    void SetStickOnChange(void (*fxPtr)(int controllerIndex, ControllerSticks controllerStick, int xVal, int yVal));
    void SetTrigTolerance(int controllerIndex, ControllerTriggers trigger, int tolerance);
    void SetStickToleranceX(int controllerIndex, ControllerSticks stick, int tolerance);
    void SetStickToleranceY(int controllerIndex, ControllerSticks stick, int tolerance);

  private:
    void ProcessSerial();
    void ParseData(String data);
    void ProcessData(String data, int controllerIndex, int messageIndex);
    int GetSerialConfig(UartConfig uartConfig);
    int m_controllerCount;
    Controller* m_controllers;
    UartConfig m_uartConfig;
    HardwareSerial* m_processorSerial;
    HardwareSerial* m_receiverSerial;
    int m_resetPin;
};

#endif
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include "ControllerInputs.h"

enum ControllerButtons {
  A_BTN,
  B_BTN,
  X_BTN,
  Y_BTN,
  RIGHT_DPAD,
  LEFT_DPAD,
  UP_DPAD,
  DOWN_DPAD,
  RIGHT_BUMP,
  LEFT_BUMP,
  XBOX_BTN,
  MENU_BTN,
  VIEW_BTN,
  SHARE_BTN,
  LEFT_TRIG_BTN,
  RIGHT_TRIG_BTN,
  LEFT_STICK_BTN,
  RIGHT_STICK_BTN
};

enum ControllerTriggers
{
  LEFT_TRIG,
  RIGHT_TRIG
};

enum ControllerSticks
{
  LEFT_STICK,
  RIGHT_STICK
};

class Controller {
  public:
    Controller();
    Controller(int index);
    void Setup();
    void SetBtnOnPress(void (*fxPtr)(int controllerIndex, ControllerButtons controllerButton));
    void SetBtnOnRelease(void (*fxPtr)(int controllerIndex, ControllerButtons controllerButton));
    void SetTrigOnChange(void (*fxPtr)(int controllerIndex, ControllerTriggers controllerTrigger, int newVal));
    void SetStickOnChange(void (*fxPtr)(int controllerIndex, ControllerSticks controllerStick, int xVal, int yVal));
    void SetButton(ControllerButtons controllerButton, bool value);
    void SetTrigger(ControllerTriggers controllerTrigger, int value);
    void SetStickX(ControllerSticks controllerStick, int value);
    void SetStickY(ControllerSticks controllerStick, int value);
    ControllerBtn aBtn;
    ControllerBtn bBtn;
    ControllerBtn xBtn;
    ControllerBtn yBtn;
    ControllerBtn rightDPad;
    ControllerBtn leftDPad;
    ControllerBtn upDPad;
    ControllerBtn downDPad;
    ControllerBtn rightBump;
    ControllerBtn leftBump;
    ControllerBtn xboxBtn;
    ControllerBtn menuBtn;
    ControllerBtn viewBtn;
    ControllerBtn shareBtn;
    ControllerTrig rightTrig;
    ControllerTrig leftTrig;
    ControllerStick rightStick;
    ControllerStick leftStick;

  private:
    void (*m_btnOnPress)(int controllerIndex, ControllerButtons controllerButton);
    void (*m_btnOnRelease)(int controllerIndex, ControllerButtons controllerButton);
    void (*m_trigOnChange)(int controllerIndex, ControllerTriggers controllerTrigger, int newVal);
    void (*m_stickOnChange)(int controllerIndex, ControllerSticks controllerStick, int xVal, int yVal);
    int m_index;
};

#endif
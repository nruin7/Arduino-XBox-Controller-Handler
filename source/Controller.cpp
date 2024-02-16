#include "Controller.h"

Controller::Controller() {
  m_index = -1;
}

Controller::Controller(int index) {
  m_index = index;
}

void Controller::Setup() {

}

void Controller::SetBtnOnPress(void (*fxPtr)(int controllerIndex, ControllerButtons controllerButton)) {
  m_btnOnPress = fxPtr;
}

void Controller::SetBtnOnRelease(void (*fxPtr)(int controllerIndex, ControllerButtons controllerButton)) {
  m_btnOnRelease = fxPtr;
}

void Controller::SetTrigOnChange(void (*fxPtr)(int controllerIndex, ControllerTriggers controllerTrigger, int newVal)) {
  m_trigOnChange = fxPtr;
}

void Controller::SetStickOnChange(void (*fxPtr)(int controllerIndex, ControllerSticks controllerStick, int xVal, int yVal)) {
  m_stickOnChange = fxPtr;
}

void Controller::SetButton(ControllerButtons controllerButton, bool value) {
  bool changed = false;
  bool pressed = false;
  switch(controllerButton)
  {
    case A_BTN:
      if(aBtn.SetStatus(value))
      {
        changed = true;
        pressed = aBtn.GetStatus();
      }
      break;
    case B_BTN:
      if(bBtn.SetStatus(value))
      {
        changed = true;
        pressed = bBtn.GetStatus();
      }
      break;
    case X_BTN:
      if(xBtn.SetStatus(value))
      {
        changed = true;
        pressed = xBtn.GetStatus();
      }
      break;
    case Y_BTN:
      if(yBtn.SetStatus(value))
      {
        changed = true;
        pressed = yBtn.GetStatus();
      }
      break;
    case RIGHT_DPAD:
      if(rightDPad.SetStatus(value))
      {
        changed = true;
        pressed = rightDPad.GetStatus();
      }
      break;
    case LEFT_DPAD:
      if(leftDPad.SetStatus(value))
      {
        changed = true;
        pressed = leftDPad.GetStatus();
      }
      break;
    case UP_DPAD:
      if(upDPad.SetStatus(value))
      {
        changed = true;
        pressed = upDPad.GetStatus();
      }
      break;
    case DOWN_DPAD:
      if(downDPad.SetStatus(value))
      {
        changed = true;
        pressed = downDPad.GetStatus();
      }
      break;
    case RIGHT_BUMP:
      if(rightBump.SetStatus(value))
      {
        changed = true;
        pressed = rightBump.GetStatus();
      }
      break;
    case LEFT_BUMP:
      if(leftBump.SetStatus(value))
      {
        changed = true;
        pressed = leftBump.GetStatus();
      }
      break;
    case XBOX_BTN:
      if(xboxBtn.SetStatus(value))
      {
        changed = true;
        pressed = xboxBtn.GetStatus();
      }
      break;
    case MENU_BTN:
      if(menuBtn.SetStatus(value))
      {
        changed = true;
        pressed = menuBtn.GetStatus();
      }
      break;
    case VIEW_BTN:
      if(viewBtn.SetStatus(value))
      {
        changed = true;
        pressed = viewBtn.GetStatus();
      }
      break;
    case SHARE_BTN:
      if(shareBtn.SetStatus(value))
      {
        changed = true;
        pressed = shareBtn.GetStatus();
      }
      break;
    case LEFT_TRIG_BTN:
      if(leftTrig.trigBtn.SetStatus(value))
      {
        changed = true;
        pressed = leftTrig.trigBtn.GetStatus();
      }
      break;
    case RIGHT_TRIG_BTN:
      if(rightTrig.trigBtn.SetStatus(value))
      {
        changed = true;
        pressed = rightTrig.trigBtn.GetStatus();
      }
      break;
    case LEFT_STICK_BTN:
      if(leftStick.stickBtn.SetStatus(value))
      {
        changed = true;
        pressed = leftStick.stickBtn.GetStatus();
      }
      break;
    case RIGHT_STICK_BTN:
      if(rightStick.stickBtn.SetStatus(value))
      {
        changed = true;
        pressed = rightStick.stickBtn.GetStatus();
      }
      break;
    default:
      break;
  }
  if(changed)
  {
    if(pressed == true)
    {
      m_btnOnPress(m_index, controllerButton);
    }else
    {
      m_btnOnRelease(m_index, controllerButton);
    }
  }
}

void Controller::SetTrigger(ControllerTriggers controllerTrigger, int value) {
  bool changed = false;
  int newVal = 0;
  switch(controllerTrigger)
  {
    case LEFT_TRIG:
      if(leftTrig.SetTrigVal(value))
      {
        changed = true;
        newVal = leftTrig.GetTrigVal();
      }
      break;
    case RIGHT_TRIG:
      if(rightTrig.SetTrigVal(value))
      {
        changed = true;
        newVal = rightTrig.GetTrigVal();
      }
      break;
    default:
      break;
  }
  if(changed)
  {
    m_trigOnChange(m_index, controllerTrigger, newVal);
  }
}

void Controller::SetStickX(ControllerSticks controllerStick, int value) {
  bool changed = false;
  int xVal = 0;
  int yVal = 0;
  switch(controllerStick)
  {
    case LEFT_STICK:
      if(leftStick.SetStickValX(value))
      {
        changed = true;
        xVal = leftStick.GetStickValX();
        yVal = leftStick.GetStickValY();
      }
      break;
    case RIGHT_STICK:
      if(rightStick.SetStickValX(value))
      {
        changed = true;
        xVal = rightStick.GetStickValX();
        yVal = rightStick.GetStickValY();
      }
      break;
    default:
      break;
  }
  if(changed)
  {
    m_stickOnChange(m_index, controllerStick, xVal, yVal);
  }
}

void Controller::SetStickY(ControllerSticks controllerStick, int value) {
  bool changed = false;
  int xVal = 0;
  int yVal = 0;
  switch(controllerStick)
  {
    case LEFT_STICK:
      if(leftStick.SetStickValY(value))
      {
        changed = true;
        xVal = leftStick.GetStickValX();
        yVal = leftStick.GetStickValY();
      }
      break;
    case RIGHT_STICK:
      if(rightStick.SetStickValY(value))
      {
        changed = true;
        xVal = rightStick.GetStickValX();
        yVal = rightStick.GetStickValY();
      }
      break;
    default:
      break;
  }
  if(changed)
  {
    m_stickOnChange(m_index, controllerStick, xVal, yVal);
  }
}
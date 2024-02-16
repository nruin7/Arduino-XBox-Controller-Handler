#ifndef CONTROLLERINPUTS_H
#define CONTROLLERINPUTS_H

#include <Arduino.h>

class ControllerBtn {
  public:
    ControllerBtn();
    bool SetStatus(bool pressed);
    bool GetStatus();

  private:
    bool m_pressed;
};

class ControllerTrig {
  public:
    ControllerTrig();
    void SetTolerance(int tolerance);
    bool SetTrigVal(int trigVal);
    int GetTrigVal();
    ControllerBtn trigBtn;

  private:
    int m_trigVal;
    int m_tolerance;
};

class ControllerStick {
  public:
    ControllerStick();
    void SetToleranceX(int tolerance);
    void SetToleranceY(int tolerance);;
    bool SetStickValX(int stickValX);
    bool SetStickValY(int stickValY);
    int GetStickValX();
    int GetStickValY();
    ControllerBtn stickBtn;

  private:
    int m_stickValX;
    int m_stickValY;
    int m_toleranceX;
    int m_toleranceY;
};

#endif
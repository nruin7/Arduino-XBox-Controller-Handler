#include "ControllerInputs.h"

//Controller Button Implementation
ControllerBtn::ControllerBtn() {
  m_pressed = false;
}

bool ControllerBtn::SetStatus(bool pressed)
{
  if(m_pressed != pressed)
  {
    m_pressed = pressed;
    return true;
  }
  return false;
}

bool ControllerBtn::GetStatus()
{
  return m_pressed;
}


//Controller Trigger Implementation
ControllerTrig::ControllerTrig()
{
  m_tolerance = 1;
  m_trigVal = 0;
}

void ControllerTrig::SetTolerance(int tolerance)
{
  m_tolerance = tolerance;
}

bool ControllerTrig::SetTrigVal(int trigVal)
{
  if(m_trigVal != trigVal)
  {
    if(abs(m_trigVal - trigVal) >= m_tolerance)
    {
      m_trigVal = trigVal;
      return true;
    }
  }
  return false;
}

int ControllerTrig::GetTrigVal()
{
  return m_trigVal;
}


//Controller Stick Implementation
ControllerStick::ControllerStick()
{
  m_toleranceX = 1;
  m_toleranceY = 1;
  m_stickValX = 0;
  m_stickValY = 0;
}

void ControllerStick::SetToleranceX(int tolerance)
{
  m_toleranceX = tolerance;
}

void ControllerStick::SetToleranceY(int tolerance)
{
  m_toleranceY = tolerance;
}

bool ControllerStick::SetStickValX(int stickValX)
{
  if(m_stickValX != stickValX)
  {
    if(abs(m_stickValX - stickValX) >= m_toleranceX)
    {
      m_stickValX = stickValX;
      return true;
    }
  }
  return false;
}

bool ControllerStick::SetStickValY(int stickValY)
{
  if(m_stickValY != stickValY)
  {
    if(abs(m_stickValY - stickValY) >= m_toleranceY)
    {
      m_stickValY = stickValY;
      return true;
    }
  }
  return false;
}

int ControllerStick::GetStickValX()
{
  return m_stickValX;
}

int ControllerStick::GetStickValY()
{
  return m_stickValY;
}
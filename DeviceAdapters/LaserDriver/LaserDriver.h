///////////////////////////////////////////////////////////////////////////////
// FILE:          LaserDriver.h
// PROJECT:       Micro-Manager
// SUBSYSTEM:     DeviceAdapters
//-----------------------------------------------------------------------------
// DESCRIPTION:   Device adapter for the laser driver used in a project by
//                Daniel Schröder at the FSU Jena. Uses a Velleman K8061
//                interface board via the comedilib driver.
//                
// AUTHOR:        John Wigg
//                
// COPYRIGHT:     
//
// LICENSE:       This file is distributed under the BSD license.
//                License text is included with the source distribution.
//
//                This file is distributed in the hope that it will be useful,
//                but WITHOUT ANY WARRANTY; without even the implied warranty
//                of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//                IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//                CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//                INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES.
//

#ifndef _LASERDRIVER_H_
#define _LASERDRIVER_H_

#include "InterfaceBoard.h"

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ModuleInterface.h"

//////////////////////////////////////////////////////////////////////////////
// Error codes
//
#define ERR_UNKNOWN_MODE         102

class LaserDriver : public CGenericBase<LaserDriver>  
{
public:
   LaserDriver();
   ~LaserDriver();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* name) const;      
   
   // LaserDriver API
   // ---------------
   int SetLEDVoltage(int idx, double voltage) const;
   int SetLEDEnabled(int idx, bool enabled) const;

   int OnLedOnOff(MM::PropertyBase* pProp, MM::ActionType eAct);
   int OnLedVoltage(MM::PropertyBase* pProp, MM::ActionType eAct);

   bool Busy() { return false; }

private:
   bool initialized_;
   InterfaceBoard interface_;
};

#endif //_LASERDRIVER_H_

///////////////////////////////////////////////////////////////////////////////
// FILE:          LaserDriver.cpp
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

#include "LaserDriver.h"

#include "InterfaceBoard.h"

#include <iostream>

const char* g_LaserDriverName = "LaserDriver";

const char* ON = "On";
const char* OFF = "Off";

///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////

/**
 * List all supported hardware devices here
 */
MODULE_API void InitializeModuleData()
{
   RegisterDevice(g_LaserDriverName, MM::GenericDevice, "Laser driver device adapter.");
}

MODULE_API MM::Device* CreateDevice(const char* deviceName)
{
   if (deviceName == 0)
      return 0;

   // decide which device class to create based on the deviceName parameter
   if (strcmp(deviceName, g_LaserDriverName) == 0)
   {
      // create camera
      return new LaserDriver();
   }

   // ...supplied name not recognized
   return 0;
}

MODULE_API void DeleteDevice(MM::Device* pDevice)
{
   delete pDevice;
}

///////////////////////////////////////////////////////////////////////////////
// MMCamera implementation
// ~~~~~~~~~~~~~~~~~~~~~~~

/**
* MMCamera constructor.
* Setup default all variables and create device properties required to exist
* before intialization. In this case, no such properties were required. All
* properties will be created in the Initialize() method.
*
* As a general guideline Micro-Manager devices do not access hardware in the
* the constructor. We should do as little as possible in the constructor and
* perform most of the initialization in the Initialize() method.
*/
LaserDriver::LaserDriver() {
   // call the base class method to set-up default error codes/messages
   InitializeDefaultErrorMessages();

   CPropertyAction* pAct = new CPropertyAction (this, &LaserDriver::OnLedVoltage);

   int ret;
   ret = CreateFloatProperty("Voltage Analog 1", 0.0, false, pAct);
   ret = CreateFloatProperty("Voltage Analog 2", 0.0, false, pAct);
   ret = CreateFloatProperty("Voltage Analog 3", 0.0, false, pAct);
   ret = CreateFloatProperty("Voltage Analog 4", 0.0, false, pAct);
   ret = CreateFloatProperty("Voltage Analog 5", 0.0, false, pAct);

   assert(ret == DEVICE_OK);

   ret = SetPropertyLimits("Voltage Analog 1", 0.0, 5.0);
   ret = SetPropertyLimits("Voltage Analog 2", 0.0, 5.0);
   ret = SetPropertyLimits("Voltage Analog 3", 0.0, 5.0);
   ret = SetPropertyLimits("Voltage Analog 4", 0.0, 5.0);
   ret = SetPropertyLimits("Voltage Analog 5", 0.0, 5.0);

   assert(ret == DEVICE_OK);

   pAct = new CPropertyAction (this, &LaserDriver::OnLedOnOff);

   ret = CreateStringProperty("Enable Digital 1", OFF, false, pAct);
   ret = CreateStringProperty("Enable Digital 2", OFF, false, pAct);
   ret = CreateStringProperty("Enable Digital 3", OFF, false, pAct);
   ret = CreateStringProperty("Enable Digital 4", OFF, false, pAct);
   ret = CreateStringProperty("Enable Digital 5", OFF, false, pAct);

   assert(ret == DEVICE_OK);

   std::vector<std::string> digitalValues;
   digitalValues.push_back(OFF);
   digitalValues.push_back(ON);

   ret = SetAllowedValues("Enable Digital 1", digitalValues);
   ret = SetAllowedValues("Enable Digital 2", digitalValues);
   ret = SetAllowedValues("Enable Digital 3", digitalValues);
   ret = SetAllowedValues("Enable Digital 4", digitalValues);
   ret = SetAllowedValues("Enable Digital 5", digitalValues);

   assert(ret == DEVICE_OK);

   /*
    // Description property
   int ret = CreateProperty(MM::g_Keyword_Description, "MMCamera example adapter", MM::String, true);
   assert(ret == DEVICE_OK);

   // camera type pre-initialization property
   ret = CreateProperty(g_CameraModelProperty, g_CameraModel_A, MM::String, false, 0, true);
   assert(ret == DEVICE_OK);

   vector<string> modelValues;
   modelValues.push_back(g_CameraModel_A);
   modelValues.push_back(g_CameraModel_A); 

   ret = SetAllowedValues(g_CameraModelProperty, modelValues);
   assert(ret == DEVICE_OK);

   // create live video thread
   thd_ = new SequenceThread(this);
   */
}

/**
* MMCamera destructor.
* If this device used as intended within the Micro-Manager system,
* Shutdown() will be always called before the destructor. But in any case
* we need to make sure that all resources are properly released even if
* Shutdown() was not called.
*/
LaserDriver::~LaserDriver()
{
   if (initialized_)
      Shutdown();
}

/**
* Obtains device name.
* Required by the MM::Device API.
*/
void LaserDriver::GetName(char* name) const
{
   // We just return the name we use for referring to this
   // device adapter.
   CDeviceUtils::CopyLimitedString(name, g_LaserDriverName);
}

/**
* Intializes the hardware.
* Typically we access and initialize hardware at this point.
* Device properties are typically created here as well.
* Required by the MM::Device API.
*/
int LaserDriver::Initialize()
{
   if (initialized_)
      return DEVICE_OK;

   interface_.Open();
   if (!interface_.DeviceIsOpen()) {
      LogMessage("Comedi driver could not open the device!");
      return DEVICE_ERR;
   }

   initialized_ = true;
   return DEVICE_OK;
}

/**
* Shuts down (unloads) the device.
* Ideally this method will completely unload the device and release all resources.
* Shutdown() may be called multiple times in a row.
* Required by the MM::Device API.
*/
int LaserDriver::Shutdown()
{

   initialized_ = false;
   return DEVICE_OK;
}

int LaserDriver::OnLedOnOff(MM::PropertyBase* pProp, MM::ActionType eAct) {   
   // TODO: Check MM::BeforeSet to check whether values are as expected.
   if (eAct == MM::AfterSet) {
      std::string value;
      std::string pName = pProp->GetName();
      pProp->Get(value);

      int ret;
      int idx = -1;

      if (pName == "Enable Digital 1") {
         idx = 0;
      } else if (pName == "Enable Digital 2") {
         idx = 1;
      } else if (pName == "Enable Digital 3") {
         idx = 2;
      } else if (pName == "Enable Digital 4") {
         idx = 3;
      } else if (pName == "Enable Digital 5") {
         idx = 4;
      }

      if (value == ON) {
         ret = SetLEDEnabled(idx, true);
      } else {
         ret = SetLEDEnabled(idx, false);
      }

      if (ret != DEVICE_OK) {
         // error occured; revert values and return
         if (value == ON) {
            pProp->Set(OFF);
         } else {
            pProp->Set(ON);
         }
         return ret;
      }
   }
   return DEVICE_OK;
}

int LaserDriver::OnLedVoltage(MM::PropertyBase* pProp, MM::ActionType eAct) {
   if (eAct == MM::AfterSet) {
      double value;
      std::string pName = pProp->GetName();
      pProp->Get(value);

      int ret;
      int idx = -1;

      if (pName == "Voltage Analog 1") {
         idx = 0;
      } else if (pName == "Voltage Analog 2") {
         idx = 1;
      } else if (pName == "Voltage Analog 3") {
         idx = 2;
      } else if (pName == "Voltage Analog 4") {
         idx = 3;
      } else if (pName == "Voltage Analog 5") {
         idx = 4;
      }

      ret = SetLEDVoltage(idx, value);

      if (ret != DEVICE_OK) {
         // error occure
         // TODO: Maybe this should be handled in some way?
         return ret;
      }
   }

   return DEVICE_OK;
}

int LaserDriver::SetLEDEnabled(int idx, bool enabled) const {
   if (!interface_.DeviceIsOpen()) {
      LogMessage("No open device!");
      return DEVICE_ERR;
   } else {
      LogMessage("Device is open; attempting to write.");
   }
   int ret = interface_.WriteDigital(idx, enabled);
   if (ret == 1) { // error
      return DEVICE_ERR;
   }
   return DEVICE_OK;
}

int LaserDriver::SetLEDVoltage(int idx, double voltage) const {
   int ret = interface_.WriteAnalog(idx, voltage);
   if (ret == 1) { // error
      // Debug
      LogMessage("Could not set analog value!", false);
      return DEVICE_ERR;
   }
   return DEVICE_OK;
}
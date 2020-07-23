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
#include "ModuleInterface.h"

using namespace std;

const char* g_LaserDriverName = "LaserDriver";

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

int LaserDriver::SetLEDEnabled(int idx, bool enabled) const {
    return DEVICE_OK;
}

int LaserDriver::SetLEDVoltage(int idx, double voltage) const {
    return DEVICE_OK;
}
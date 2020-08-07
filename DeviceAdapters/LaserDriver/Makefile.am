AM_CXXFLAGS = $(MMDEVAPI_CXXFLAGS)
deviceadapter_LTLIBRARIES = libmmgr_dal_LaserDriver.la
libmmgr_dal_LaserDriver_la_SOURCES = LaserDriver.cpp LaserDriver.h \
   InterfaceBoard.cpp InterfaceBoard.h \
   ../../MMDevice/MMDevice.h ../../MMDevice/DeviceBase.h
libmmgr_dal_LaserDriver_la_LIBADD = $(MMDEVAPI_LIBADD) -lcomedi
libmmgr_dal_LaserDriver_la_LDFLAGS = $(MMDEVAPI_LDFLAGS)
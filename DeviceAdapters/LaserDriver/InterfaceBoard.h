#ifndef _INTERFACEBOARD_H_
#define _INTERFACEBOARD_H_

#include <comedilib.h>

/* NOTE: These subdevice indices are for K8061 ONLY */
#define SUBDEV_AO 1 /* analog output */
#define SUBDEV_DO 3 /* digital output */

const char filename[] = "/dev/comedi0";

class InterfaceBoard
{
    public:
        InterfaceBoard();
        ~InterfaceBoard();
        int Open();
        int WriteAnalog(unsigned int channel, double physical_value) const;
        int WriteDigital(unsigned int channel, bool value) const;
        bool DeviceIsOpen() const;
    private:
        comedi_t *device_;
        int analog_n_channels_; /* Number of analog channels. */
        int digital_n_channels_; /* Number of digital channels. */
};

#endif // _INTERFACEBOARD_H_
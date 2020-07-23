#include <InterfaceBoard.h>

#include <comedilib.h>

InterfaceBoard::InterfaceBoard() {
	device_ = comedi_open(filename);
	if (device_ == NULL) {
		comedi_perror(filename);
		return; // TODO: error handling
	}
    analog_n_channels_ = comedi_get_n_channels(device_, SUBDEV_AO);
    digital_n_channels_ = comedi_get_n_channels(device_, SUBDEV_DO);
}

InterfaceBoard::~InterfaceBoard() {

}

int InterfaceBoard::WriteAnalog(unsigned int channel, double physical_value) const
{
    if (channel > analog_n_channels_ - 1) { // invalid channel index
        return 1;
    }
    comedi_range *range_info = comedi_get_range(device_, SUBDEV_AO, channel, 0);
    lsampl_t data = comedi_from_phys(physical_value, range_info, range_info->max);

    int res = comedi_data_write(device_, SUBDEV_AO, channel, 0, AREF_GROUND, data);
    if (res != 1) { // error occured
        return 1;
    }
        
    return 0;
}

int InterfaceBoard::WriteDigital(unsigned int channel, bool value) const {
    if (channel > digital_n_channels_ - 1) { // invalid channel index
        return 1;
    }
    int res = comedi_dio_write(device_, SUBDEV_DO, channel, (int)value);
    if (res != 1) { // error occured
        return 1;
    }
    return 0;
}
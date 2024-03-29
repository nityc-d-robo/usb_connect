#include <usb_connect.hpp>

#include <libusb-1.0/libusb.h>
#include <iostream>
#include <string>

UsbConnect::UsbConnect(void){
	;
}

void UsbConnect::setUsb(UsbConfig* usb_config_){
	usb_config = usb_config_;
}

int UsbConnect::openUsb(void){
	int return_status = 0;
	std::string str_tmp;
	
	libusb_init(&context);
	handle = libusb_open_device_with_vid_pid(context, usb_config->vendor_id, usb_config->product_id);

	if(handle == NULL){
		errorUsb("Couldn't open.");
		return UsbStatus::USB_DEVICE_NOT_FOUND;
	}

	infoUsb("Open.");

	return_status = libusb_set_auto_detach_kernel_driver(handle, 1);

	if(return_status != libusb_error::LIBUSB_SUCCESS){
		errorUsb(str_tmp.append(libusb_error_name(return_status)) + " couldn't detach driver.");
		return UsbStatus::USB_OTHER_ERROR;
	}

	return_status = libusb_claim_interface(handle, usb_config->b_interface_number);

	if(return_status != libusb_error::LIBUSB_SUCCESS){
		errorUsb("In clame_interface " + str_tmp.append(libusb_error_name(return_status)));

		switch(return_status){
			case libusb_error::LIBUSB_ERROR_NOT_FOUND:
				return UsbStatus::USB_DEVICE_NOT_FOUND;
			case libusb_error::LIBUSB_ERROR_BUSY:
				return UsbStatus::USB_BUSY;
			case libusb_error::LIBUSB_ERROR_NO_DEVICE:
				return UsbStatus::USB_DISCONNECTED;
			default:
				return UsbStatus::USB_OTHER_ERROR;
		}
	}

	return_status = libusb_set_interface_alt_setting(handle, usb_config->b_interface_number, 0);

	if(return_status == libusb_error::LIBUSB_ERROR_NO_DEVICE){
		errorUsb("In altanate setting " + str_tmp.append(libusb_error_name(return_status)));
		switch(return_status){
			case libusb_error::LIBUSB_ERROR_NOT_FOUND:
				return UsbStatus::USB_DEVICE_NOT_FOUND;
			case libusb_error::LIBUSB_ERROR_NO_DEVICE:
				return UsbStatus::USB_DISCONNECTED;
			default:
				return UsbStatus::USB_OTHER_ERROR;
		}
	}

	infoUsb("Setup all collect.");

	return UsbStatus::USB_SUCCESS;
}

int UsbConnect::writeUsb(const std::string& write_data_, EndPoint end_point_, uint32_t timeout_){
	int return_status = 0;
	int write_size = 0;
	std::string str_tmp;

	return_status = libusb_bulk_transfer(handle, libusb_endpoint_direction::LIBUSB_ENDPOINT_OUT | end_point_, (unsigned char*)write_data_.c_str(), write_data_.size(), &write_size, timeout_);

	if(return_status != libusb_error::LIBUSB_SUCCESS){
		errorUsb("In writing data " + str_tmp.append(libusb_error_name(return_status)));

		switch(return_status){
			case libusb_error::LIBUSB_ERROR_TIMEOUT:
				return UsbStatus::USB_TIMEOUT;
			case libusb_error::LIBUSB_ERROR_PIPE:
				return UsbStatus::USB_ENDPOINT_HALT;
			case libusb_error::LIBUSB_ERROR_OVERFLOW:
				return UsbStatus::USB_OVERFLOW;
			case libusb_error::LIBUSB_ERROR_NO_DEVICE:
				if(openUsb() != UsbStatus::USB_SUCCESS){
					return UsbStatus::USB_DISCONNECTED;
				}else{
					return_status = libusb_bulk_transfer(handle, libusb_endpoint_direction::LIBUSB_ENDPOINT_OUT | end_point_, (unsigned char*)write_data_.c_str(), write_data_.size(), &write_size, timeout_);

					if(return_status == libusb_error::LIBUSB_SUCCESS){
						return write_size;
					}else{
						errorUsb("In reading data " + str_tmp.append(libusb_error_name(return_status)));

						switch(return_status){
							case libusb_error::LIBUSB_ERROR_TIMEOUT:
								return UsbStatus::USB_TIMEOUT;
							case libusb_error::LIBUSB_ERROR_PIPE:
								return UsbStatus::USB_ENDPOINT_HALT;
							case libusb_error::LIBUSB_ERROR_OVERFLOW:
								return UsbStatus::USB_OVERFLOW;
							case libusb_error::LIBUSB_ERROR_NO_DEVICE:
								return UsbStatus::USB_DISCONNECTED;
							case libusb_error::LIBUSB_ERROR_BUSY:
								return UsbStatus::USB_BUSY;
							case libusb_error::LIBUSB_ERROR_INVALID_PARAM:
								return UsbStatus::USB_BUSY;
							default:
								return UsbStatus::USB_OTHER_ERROR;
						}
					}
				}
			case libusb_error::LIBUSB_ERROR_BUSY:
				return UsbStatus::USB_BUSY;
			case libusb_error::LIBUSB_ERROR_INVALID_PARAM:
				return UsbStatus::USB_INVALID_PARAM;
			default:
				return UsbStatus::USB_OTHER_ERROR;
		}
	}

	return write_size;
}

int UsbConnect::writeUsb(const uint8_t* write_data_, uint32_t data_size_, EndPoint end_point_, uint32_t timeout_){
	int return_status = 0;
	int write_size = 0;
	std::string str_tmp;

	return_status = libusb_bulk_transfer(handle, libusb_endpoint_direction::LIBUSB_ENDPOINT_OUT | end_point_, (unsigned char*)write_data_, data_size_, &write_size, timeout_);

	if(return_status != libusb_error::LIBUSB_SUCCESS){
		errorUsb("In writing data " + str_tmp.append(libusb_error_name(return_status)));

		switch(return_status){
			case libusb_error::LIBUSB_ERROR_TIMEOUT:
				return UsbStatus::USB_TIMEOUT;
			case libusb_error::LIBUSB_ERROR_PIPE:
				return UsbStatus::USB_ENDPOINT_HALT;
			case libusb_error::LIBUSB_ERROR_OVERFLOW:
				return UsbStatus::USB_OVERFLOW;
			case libusb_error::LIBUSB_ERROR_NO_DEVICE:
				if(openUsb() != UsbStatus::USB_SUCCESS){
					return UsbStatus::USB_DISCONNECTED;
				}else{
					return_status = libusb_bulk_transfer(handle, libusb_endpoint_direction::LIBUSB_ENDPOINT_OUT | end_point_, (unsigned char*)write_data_, data_size_, &write_size, timeout_);

					if(return_status == libusb_error::LIBUSB_SUCCESS){
						return write_size;
					}else{
						errorUsb("In reading data " + str_tmp.append(libusb_error_name(return_status)));

						switch(return_status){
							case libusb_error::LIBUSB_ERROR_TIMEOUT:
								return UsbStatus::USB_TIMEOUT;
							case libusb_error::LIBUSB_ERROR_PIPE:
								return UsbStatus::USB_ENDPOINT_HALT;
							case libusb_error::LIBUSB_ERROR_OVERFLOW:
								return UsbStatus::USB_OVERFLOW;
							case libusb_error::LIBUSB_ERROR_NO_DEVICE:
								return UsbStatus::USB_DISCONNECTED;
							case libusb_error::LIBUSB_ERROR_BUSY:
								return UsbStatus::USB_BUSY;
							case libusb_error::LIBUSB_ERROR_INVALID_PARAM:
								return UsbStatus::USB_INVALID_PARAM;
							default:
								return UsbStatus::USB_OTHER_ERROR;
						}
					}
				}
			case libusb_error::LIBUSB_ERROR_BUSY:
				return UsbStatus::USB_BUSY;
			case libusb_error::LIBUSB_ERROR_INVALID_PARAM:
				return UsbStatus::USB_INVALID_PARAM;
			default:
				return UsbStatus::USB_OTHER_ERROR;
		}
	}

	return write_size;
}

int UsbConnect::readUsb(uint8_t* read_data_, uint32_t data_size_, EndPoint end_point_, uint32_t timeout_){
	int return_status = 0;
	int read_size = 0;
	std::string str_tmp;

	return_status = libusb_bulk_transfer(handle, libusb_endpoint_direction::LIBUSB_ENDPOINT_IN | end_point_, read_data_, data_size_, &read_size, timeout_);

	if(return_status != libusb_error::LIBUSB_SUCCESS){
		errorUsb("In reading data " + str_tmp.append(libusb_error_name(return_status)));

		switch(return_status){
			case libusb_error::LIBUSB_ERROR_TIMEOUT:
				return UsbStatus::USB_TIMEOUT;
			case libusb_error::LIBUSB_ERROR_PIPE:
				return UsbStatus::USB_ENDPOINT_HALT;
			case libusb_error::LIBUSB_ERROR_OVERFLOW:
				return UsbStatus::USB_OVERFLOW;
			case libusb_error::LIBUSB_ERROR_NO_DEVICE:
				if(openUsb() != UsbStatus::USB_SUCCESS){
					return UsbStatus::USB_DISCONNECTED;
				}else{
					return_status = libusb_bulk_transfer(handle, libusb_endpoint_direction::LIBUSB_ENDPOINT_IN | end_point_, read_data_, data_size_, &read_size, timeout_);

					if(return_status == libusb_error::LIBUSB_SUCCESS){
						return read_size;
					}else{
						errorUsb("In reading data " + str_tmp.append(libusb_error_name(return_status)));

						switch(return_status){
							case libusb_error::LIBUSB_ERROR_TIMEOUT:
								return UsbStatus::USB_TIMEOUT;
							case libusb_error::LIBUSB_ERROR_PIPE:
								return UsbStatus::USB_ENDPOINT_HALT;
							case libusb_error::LIBUSB_ERROR_OVERFLOW:
								return UsbStatus::USB_OVERFLOW;
							case libusb_error::LIBUSB_ERROR_NO_DEVICE:
								return UsbStatus::USB_DISCONNECTED;
							case libusb_error::LIBUSB_ERROR_BUSY:
								return UsbStatus::USB_BUSY;
							case libusb_error::LIBUSB_ERROR_INVALID_PARAM:
								return UsbStatus::USB_INVALID_PARAM;
							default:
								return UsbStatus::USB_OTHER_ERROR;
						}
					}
				}
			case libusb_error::LIBUSB_ERROR_BUSY:
				return UsbStatus::USB_BUSY;
			case libusb_error::LIBUSB_ERROR_INVALID_PARAM:
				return UsbStatus::USB_INVALID_PARAM;
			default:
				return UsbStatus::USB_OTHER_ERROR;
		}
	}
	
	return read_size;
}

void UsbConnect::closeUsb(void){
	libusb_release_interface(handle, usb_config->b_interface_number);
	libusb_close(handle);
	libusb_exit(context);

	infoUsb("Close.");
}

void __attribute__((weak)) UsbConnect::errorUsb(std::string error_str_){
	std::cerr << std::to_string(usb_config->vendor_id) << ':' << std::to_string(usb_config->product_id) << " / " << error_str_ << std::endl;
}

void __attribute__((weak)) UsbConnect::infoUsb(std::string info_str_){
	std::cout << std::to_string(usb_config->vendor_id) << ':' << std::to_string(usb_config->product_id) << " / " << info_str_ << std::endl;
}

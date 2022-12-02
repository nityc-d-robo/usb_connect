#include <usb_connect.hpp>

#include <libusb-1.0/libusb.h>
#include <iostream>
#include <string>

UsbConnect::UsbConnect(const UsbConfig* usb_config_){
	usb_config = usb_config_;
}

int UsbConnect::openUsb(void){
	int return_status = 0;

	libusb_init(&context);
	handle = libusb_open_device_with_vid_pid(context, usb_config->vendor_id, usb_config->product_id);

	if(handle == NULL){
		errorUsb("couldn't open.");
		return 2;
	}

	return_status = libusb_set_auto_detach_kernel_driver(handle, 1);

	if(return_status != libusb_error::LIBUSB_SUCCESS){
		errorUsb(std::to_string(libusb_error_name(return_status)) + " couldn't detach driver.");
		return 2;
	}

	return_status = libusb_claim_interface(handle, usb_config->b_interface_number);

	if(return_status != libusb_error::LIBUSB_SUCCESS){
		errorUsb("In clame_interface " + std::to_string(libusb_error_name(return_status)));
		return 2;
	}

	return_status = libusb_set_interface_alt_setting(handle, usb_config->b_interface_number, 0);

	if(return_status == libusb_error::LIBUSB_ERROR_NO_DEVICE){
		errorUsb("In altanate setting "+ std::to_string(libusb_error_name(return_status)));
		return 2;
	}

	return 0;
}

int UsbConnect::writeUsb(const std::string& write_data_, EndPoint end_point_, uint32_t timeout_){
	int return_status = 0;

	return_status = libusb_bulk_transfer(handle, libusb_endpoint_direction::LIBUSB_END_POINT_OUT | end_point_, write_data_.c_str(), write_data_.size(), timeout_);

	if(return_status != libusb_error::LIBUSB_SUCCESS){
		errorUsb("In writing data " + std::to_string(libusb_error_name(return_status)));

		return 2;
	}

	return 0;
}

int UsbConnect::writeUsb(const uint8_t* write_data_, uint32_t data_size_, EndPoint end_point_, uint32_t timeout_){
	int return_status = 0;

	return_status = libusb_bulk_transfer(handle, libusb_endpoint_direction::LIB_USB_END_POINT_OUT | end_point_, write_data_, data_size_, timeout_);

	if(return_status != libusb_error::LIBUSB_SUCCESS){
		errorUsb("In writing data " + std::to_string(libusb_error_name(return_status)));

		return 2;
	}

	return 0;
}

int UsbConnect::readUsb(uint8_t* read_data_, uint32_t data_size_, EndPoint end_point_, uint32_t timeout_){
	int return_status = 0;

	return_status = libusb_bulk_transfer(handle, libusb_endpoint_direction::LIB_USB_END_POINT_IN | end_point_, read_data_, data_size_, timeout_);

	if(return_status != libusb_error::LIBUSB_SUCCESS){
		errorUsb("In reading data " + std::to_string(libusb_error_name(return_status)));

		return 2;
	}
	
	return 0;
}

void UsbConnect::closeUsb(void){
	libusb_release_interface(handle, usb_config->b_interface_number);
	libusb_close(handle);
	libusb_exit(&context);
}

void __attribute__((weak)) UsbConnect::errorUsb(std::string error_str_){
	std::cerr << std::to_string(usb_config->vendor_id) << ':' << std::to_string(usb_config->product_id) << " / " << error_str_ << std::endl;
}

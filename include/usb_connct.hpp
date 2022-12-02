#pragma once

#include <libusb-1.0/libusb.h>
#include <string>

class UsbConncet{
	public:
		UsbConnect(int vendor_id_, int product_id_);
		int openUsb(void);
		int writeUsb(std::string write_data_);
		int writeUsb(const uint8_t* wite_data_, uint32_t data_size_);
		int readUsb(uint8_t* read_data_, uint32_t data_size_);
		int raedUsb(std::string& write_data_);
		int closeUsb(void);
		void __attribute__((weak)) errorUsb(std::string error_str_);
	private:
		libusb_context *context;
		libusb_device_handle *handle;
		int vendor_id, product_id;
};

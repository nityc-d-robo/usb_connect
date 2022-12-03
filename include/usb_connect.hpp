#pragma once

#include <libusb-1.0/libusb.h>
#include <string>

typedef enum EndPoint{
	EP1 = 1, EP2 = 2
}EndPoint;

typedef struct UsbConfig{
	int vendor_id;
	int product_id;
	int b_interface_number;
}UsbConfig;

class UsbConnect{
	public:
		UsbConnect(UsbConfig* usb_config_);
		int openUsb(void);
		int writeUsb(const std::string& write_data_, EndPoint end_point_, uint32_t timeout_);
		int writeUsb(const uint8_t* wite_data_, uint32_t data_size_, EndPoint end_point_, uint32_t timeout_);
		int readUsb(uint8_t* read_data_, uint32_t data_size_, EndPoint end_point_, uint32_t timeout_);
		void closeUsb(void);
		void __attribute__((weak)) errorUsb(std::string error_str_);
	private:
		libusb_context* context;
		libusb_device_handle* handle;
		UsbConfig* usb_config;
};

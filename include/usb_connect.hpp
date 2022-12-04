#pragma once

#include <libusb-1.0/libusb.h>
#include <string>

typedef enum UsbStatus{
	USB_SUCCESS          = 0,
	USB_DEVICE_NOT_FOUND = -1,
	USB_DISCONNECTED     = -2,
	USB_BUSY             = -3,
	USB_TIMEOUT          = -4,
	USB_ENDPOINT_HALT    = -5,
	USB_OVERFLOW         = -6,
	USB_INVALID_PARAM    = -7,
	USB_OTHER_ERROR      = -8
}UsbStatus;

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
		UsbConnect(void);
		void setUsb(UsbConfig* usb_config_);
		int openUsb(void);
		int writeUsb(const std::string& write_data_, EndPoint end_point_, uint32_t timeout_);
		int writeUsb(const uint8_t* wite_data_, uint32_t data_size_, EndPoint end_point_, uint32_t timeout_);
		int readUsb(uint8_t* read_data_, uint32_t data_size_, EndPoint end_point_, uint32_t timeout_);
		void closeUsb(void);
		void __attribute__((weak)) errorUsb(std::string error_str_);
		void __attribute__((weak)) infoUsb(std::string info_str_);
	private:
		libusb_context* context;
		libusb_device_handle* handle;
		UsbConfig* usb_config;
};

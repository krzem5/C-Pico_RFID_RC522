#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <hardware/watchdog.h>
#include <pico/bootrom.h>
#include <pico_usb_console/protocol.h>
#include <pico_usb_console/serial.h>
#include <rfid_rc522/rc522.h>



static void _input_callback(unsigned char length,const char* data){
	if (length==1&&*data=='q'){
		reset_usb_boot(0,0);
	}
}



int main(void){
	gpio_init(PICO_DEFAULT_LED_PIN);
	gpio_set_dir(PICO_DEFAULT_LED_PIN,GPIO_OUT);
	gpio_put(PICO_DEFAULT_LED_PIN,1);
	pico_usb_console_init();
	pico_usb_console_protocol_set_input_callback(_input_callback);
	rc522_t rc522={
		13,
		10,
		11,
		12,
		spi1
	};
	rc522_init(&rc522);
	watchdog_enable(500,0);
	while (1){
		pico_usb_console_update();
		pico_usb_console_protocol_update();
		watchdog_update();
		unsigned int uid=rc522_get_uid(&rc522);
		if (uid){
			pico_usb_console_protocol_send_log(0,"%x%x%x%x",uid>>24,uid>>16,uid>>8,uid);
		}
	}
	reset_usb_boot(0,0);
}

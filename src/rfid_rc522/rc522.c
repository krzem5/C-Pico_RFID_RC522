#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <rfid_rc522/rc522.h>



static inline unsigned char _read_register(const rc522_t* rc522,unsigned char regsiter){
	unsigned char write_buffer[2]={regsiter|0x80,0};
	unsigned char read_buffer[2];
	gpio_put(rc522->pin_csn,0);
	spi_write_read_blocking(rc522->spi,write_buffer,read_buffer,2);
	gpio_put(rc522->pin_csn,1);
	return read_buffer[1];
}



static inline void _read_register_array(const rc522_t* rc522,unsigned char regsiter,unsigned char* data,unsigned char length){
	regsiter|=0x80;
	gpio_put(rc522->pin_csn,0);
	spi_write_blocking(rc522->spi,&regsiter,1);
	spi_read_blocking(rc522->spi,regsiter,data,length);
	gpio_put(rc522->pin_csn,1);
}



static inline void _write_register(const rc522_t* rc522,unsigned char regsiter,unsigned char value){
	unsigned char write_buffer[2]={regsiter,value};
	gpio_put(rc522->pin_csn,0);
	spi_write_blocking(rc522->spi,write_buffer,2);
	gpio_put(rc522->pin_csn,1);
}



static inline void _write_register_array(const rc522_t* rc522,unsigned char regsiter,const unsigned char* data,unsigned char length){
	gpio_put(rc522->pin_csn,0);
	spi_write_blocking(rc522->spi,&regsiter,1);
	spi_write_blocking(rc522->spi,data,length);
	gpio_put(rc522->pin_csn,1);
}



static inline _Bool _was_exchange_successful(const rc522_t* rc522,unsigned char length){
	return ((_read_register(rc522,MFRC522_REGISTER_COMIRQ)&0x20)&&!_read_register(rc522,MFRC522_REGISTER_ERROR)&&!(_read_register(rc522,MFRC522_REGISTER_CONTROL)&0x07)&&_read_register(rc522,MFRC522_REGISTER_FIFOLEVEL)==length);
}



void rc522_init(const rc522_t* rc522){
	gpio_init(rc522->pin_csn);
	gpio_set_dir(rc522->pin_csn,GPIO_OUT);
	gpio_set_function(rc522->pin_sck,GPIO_FUNC_SPI);
	gpio_set_function(rc522->pin_tx,GPIO_FUNC_SPI);
	gpio_set_function(rc522->pin_rx,GPIO_FUNC_SPI);
	gpio_put(rc522->pin_csn,1);
	sleep_ms(5);
	spi_init(rc522->spi,1000000);
	spi_set_format(rc522->spi,8,SPI_CPOL_0,SPI_CPHA_0,SPI_MSB_FIRST);
	if (_read_register(rc522,MFRC522_REGISTER_VERSION)!=0x92){
		return;
	}
	_write_register(rc522,MFRC522_REGISTER_COMMAND,0x0f);
	while (_read_register(rc522,MFRC522_REGISTER_COMMAND)&0x10);
	_write_register(rc522,MFRC522_REGISTER_MODE,0x28);
	_write_register(rc522,MFRC522_REGISTER_TXMODE,0x00);
	_write_register(rc522,MFRC522_REGISTER_RXMODE,0x00);
	_write_register(rc522,MFRC522_REGISTER_TXASK,0x40);
	_write_register(rc522,MFRC522_REGISTER_MODWIDTH,0x26);
	_write_register(rc522,MFRC522_REGISTER_RFCFG,0x70);
	_write_register(rc522,MFRC522_REGISTER_TMODE,0x00);
	_write_register(rc522,MFRC522_REGISTER_TXCONTROL,0x83);
}



unsigned int rc522_get_uid(const rc522_t* rc522){
	_write_register(rc522,MFRC522_REGISTER_COMMAND,0x00);
	_write_register(rc522,MFRC522_REGISTER_COMIRQ,0x7f);
	_write_register(rc522,MFRC522_REGISTER_FIFOLEVEL,0x80);
	_write_register(rc522,MFRC522_REGISTER_FIFODATA,0x26);
	_write_register(rc522,MFRC522_REGISTER_BITFRAMING,0x07);
	_write_register(rc522,MFRC522_REGISTER_COMMAND,0x0c);
	_write_register(rc522,MFRC522_REGISTER_BITFRAMING,0x87);
	sleep_us(450);
	if (!_was_exchange_successful(rc522,2)){
		return 0;
	}
	unsigned char write_buffer[2]={0x93,0x20};
	_write_register(rc522,MFRC522_REGISTER_COMMAND,0x00);
	_write_register(rc522,MFRC522_REGISTER_COMIRQ,0x7f);
	_write_register(rc522,MFRC522_REGISTER_FIFOLEVEL,0x80);
	_write_register_array(rc522,MFRC522_REGISTER_FIFODATA,write_buffer,2);
	_write_register(rc522,MFRC522_REGISTER_BITFRAMING,0x00);
	_write_register(rc522,MFRC522_REGISTER_COMMAND,0x0c);
	_write_register(rc522,MFRC522_REGISTER_BITFRAMING,0x80);
	sleep_us(800);
	if (!_was_exchange_successful(rc522,5)){
		return 0;
	}
	unsigned char read_buffer[5];
	_read_register_array(rc522,MFRC522_REGISTER_FIFODATA,read_buffer,5);
	if ((read_buffer[0]^read_buffer[1]^read_buffer[2]^read_buffer[3])!=read_buffer[4]){
		return 0;
	}
	return (read_buffer[0]<<24)|(read_buffer[1]<<16)|(read_buffer[2]<<8)|read_buffer[3];
}

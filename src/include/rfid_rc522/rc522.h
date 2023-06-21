#ifndef _RFID_RC522_RC522_H_
#define _RFID_RC522_RC522_H_ 1
#include <hardware/spi.h>



#define MFRC522_REGISTER_COMMAND 0x02
#define MFRC522_REGISTER_COMIRQ 0x08
#define MFRC522_REGISTER_ERROR 0x0c
#define MFRC522_REGISTER_FIFODATA 0x12
#define MFRC522_REGISTER_FIFOLEVEL 0x14
#define MFRC522_REGISTER_CONTROL 0x18
#define MFRC522_REGISTER_BITFRAMING 0x1a
#define MFRC522_REGISTER_MODE 0x22
#define MFRC522_REGISTER_TXMODE 0x24
#define MFRC522_REGISTER_RXMODE 0x26
#define MFRC522_REGISTER_TXCONTROL 0x28
#define MFRC522_REGISTER_TXASK 0x2a
#define MFRC522_REGISTER_MODWIDTH 0x48
#define MFRC522_REGISTER_RFCFG 0x4c
#define MFRC522_REGISTER_TMODE 0x54
#define MFRC522_REGISTER_VERSION 0x6e



typedef struct _RC522{
	const unsigned char pin_csn;
	const unsigned char pin_sck;
	const unsigned char pin_tx;
	const unsigned char pin_rx;
	spi_inst_t* spi;
} rc522_t;



void rc522_init(const rc522_t* rc522);



unsigned int rc522_get_uid(const rc522_t* rc522);



#endif

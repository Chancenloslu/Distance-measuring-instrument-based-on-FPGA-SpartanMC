#include "ourdisplay.h"
#include "display.h"
#include <stdio.h>
#include "peripherals.h"
#include <string.h>
//#include "i2c_master.h"


#define SPI_MASTER_STAT_FILL 	0x00007
//#define SPI_MASTER_STAT_INT 	0x00008
//#define SPI_MASTER_STAT_SS_ON 	0x00010
//#define SPI_MASTER_STAT_SS_SET 	0x00020

#define PRESCLAR 299 //(PERI_CLK / (5*DES_SCL))-1
#define I2C_SLAVE_ADDRESS 0xE0 //0x70 << 1
//************************************************************
//			 			SPI TREIBER
//************************************************************
/*
 * Aktivierung der I2C-Master
**/
void spi_peri_enable(){
	//spi_master_enable(&SPI_MASTER);//maybe 2nd method
	SPI_MASTER.spi.spi_control &= 0x00000;		//clear each bit
	SPI_MASTER.spi.spi_control |= SPI_MASTER_CTRL_EN;
	SPI_MASTER.spi.spi_control &= ~SPI_MASTER_CTRL_BITCNT;
	SPI_MASTER.spi.spi_control |= (9<<8);		// 9-bit register
	SPI_MASTER.spi.spi_control |= (1 << 13);	//frequency: 60M/(4*3) = 5M
	//spi_master_set_div(&SPI_MASTER, 128);
	//while(!(SPI_MASTER.spi.spi_status & SPI_MASTER_STAT_INT));
}

/*
 * Auswahl der Slave-Adresse
**/
void spi_peri_select(){
	SPI_MASTER.spi.spi_control |= (1 << 4); //address
}

/*
 * Deaktivierung der Slave-Adresse
**/
void spi_peri_deselect(){
	SPI_MASTER.spi.spi_control &= ~SPI_MASTER_CTRL_SLAVE;
}

/*
 * Senden von Daten an das OLED-Display
**/
void spi_peri_write(unsigned int data){
	while(SPI_MASTER.spi.spi_status & SPI_MASTER_STAT_FILL);  //wait until the FIFO is empty
	SPI_MASTER.spi.spi_data_out = data;
	//printf(data);
}

//************************************************************
//			 			I2C TREIBER
//************************************************************


/*
 * Das Lesen von Firmwareregister, um die Inhalt von REG 0 zu erhalten
**/
unsigned int ReadFirmware(){
	//state = 1;
	I2C_MASTER.data[0] = I2C_SLAVE_ADDRESS; // slave address 
	I2C_MASTER.data[1] = 0;			//Register nummer in slave
	I2C_MASTER.cmd = I2C_CMD_STA | (2 << 3);
	while(I2C_MASTER.stat & I2C_STA_TIP); 	//wait for not busy
	while(I2C_MASTER.stat & I2C_STA_NO_ACK);	//wait for slave's ackownledgement
	I2C_MASTER.cmd |= I2C_CMD_STO;
	
	I2C_MASTER.data[0] = I2C_SLAVE_ADDRESS + 1; //the last bit set 1
	I2C_MASTER.cmd = I2C_CMD_STA | (2 << 3) | I2C_CMD_RD;	
	while(I2C_MASTER.stat & I2C_STA_TIP); 	//wait for not busy
	while(I2C_MASTER.stat & I2C_STA_NO_ACK);	//wait for slave's ackownledgement
	I2C_MASTER.cmd |= I2C_CMD_STO;
	printf("firmware:%u\n", I2C_MASTER.data[1]);
	
	
	return I2C_MASTER.data[1];
}

/*
 * Aktivierung der I2C-Master
**/
void i2c_peri_enable(){
	I2C_MASTER.ctrl &= 0x0000;
	I2C_MASTER.ctrl = I2C_CTRL_EN | PRESCLAR;
	I2C_MASTER.ctrl |= I2C_CTRL_INTR_EN;
	
	
}

/*
 * wenn die Inhalt von REG 0 nicht mehr 0xFF ist
 * wird die Initialisierung durchgefÃ¼hrt, um die Leseanforderung zu senden
**/
void i2c_peri_read_inital(){
	//state = 2;
	I2C_MASTER.data[0] = I2C_SLAVE_ADDRESS; // slave address 
	I2C_MASTER.data[1] = 2;			//Register nummer in slave
	I2C_MASTER.cmd = I2C_CMD_STA | (2 << 3) | I2C_CMD_STO;
	
	//while(I2C_MASTER.stat & I2C_STA_TIP);
	printf("wait until slave acknoledge\n");
	//printf("state= %u\n", state);
	//while(I2C_MASTER.stat & I2C_STA_NO_ACK);
	printf("read Initialisierung fertig.\n");
	
}

/*
 * Senden des Lesenbefehls 
 * empfange die Werte von REG 2 UND 3
**/
unsigned int i2c_peri_read(){
	
	if(I2C_MASTER.stat & I2C_STA_NO_ACK) {
		//printf("slave nicht gefunden. Initialisierung noch einmal.\n");
		i2c_peri_read_inital();
	}
	
	I2C_MASTER.data[0] = I2C_SLAVE_ADDRESS + 1; //the last bit set 1
	I2C_MASTER.cmd = I2C_CMD_STA | (3 << 3) | I2C_CMD_RD | I2C_CMD_STO;

	printf("start to wait");
	while(I2C_MASTER.stat & I2C_STA_TIP);	//wait for not busy
	printf("wait until transfer complete");
	while(I2C_MASTER.stat & I2C_STA_NO_ACK);	//wait for slave's ackownledgement
	printf("wait until slave acknoledge22222\n");
	unsigned int register2 = I2C_MASTER.data[1];
	unsigned int register3 = I2C_MASTER.data[2];
	printf("register2: %u, register3: %u \n", register2, register3);
	//printf("state= %u\n", state);
	return (register2 * 256) + register3;
}
/*
void i2c_peri_write_initial(){
	
	I2C_MASTER.data[0] = I2C_SLAVE_ADDRESS; // slave address
	I2C_MASTER.data[0] = 0;			//Register nummer in slave
	I2C_MASTER.cmd = I2C_CMD_STA | (2 << 3);
	if(I2C_MASTER.stat & I2C_STA_NO_ACK) //"wait until slave acknoledge register nummer\n"
	{
		return;
	}
	//printf("wirte initialisierung fertig.\n");
}
* */

/*
 * Schreibenbefehl
 * @Data der zu sendende Befehl
 * 
**/
void i2c_peri_write(unsigned int Data){
	I2C_MASTER.data[0] = I2C_SLAVE_ADDRESS; // slave address
	I2C_MASTER.data[1] = 0;			//Register nummer in slave
	I2C_MASTER.data[2] = Data;
		I2C_MASTER.cmd = I2C_CMD_STA | (3 << 3) | I2C_CMD_STO;
		while(I2C_MASTER.stat & I2C_STA_NO_ACK);	//wait for slave's ackownledgement
	/*if(I2C_MASTER.stat & I2C_STA_NO_ACK) //"wait until slave acknoledge register nummer\n"
	{
		return;
	}*/
	//printf("wirte data: %u \n", Data);
}

/*
 * Die Initialisierung der OLED-Display und SPI-Master
**/
void display_init(){
	char *greet = "Hello_World!";
	char *data = "Die_Entfernung:";
	//display configuration
	spi_peri_enable();
	spi_peri_select();
	OLED_Init_25664();
	Checkerboard_25664();
	Show_String_25664(1, greet, 10, 0);
	Show_String_25664(1, data, 10, 12);
}

/*
 * Die Initialisierung der I2C-Master
**/
void ultraschall_init(){
	i2c_peri_enable();
	//i2c_peri_write_initial();
	i2c_peri_write(0x51); //messsenvorgang starten

}

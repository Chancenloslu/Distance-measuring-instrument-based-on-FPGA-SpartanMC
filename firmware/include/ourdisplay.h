#ifndef __OURDISPLAY_H
#define __OURDISPLAY_H

#include <spi.h>
#include <peripherals/spi_master.h>
#include <peripherals/spi_slave.h>
#include <peripherals/i2c_master.h>
//extern int state;
void spi_peri_enable();
void spi_peri_select();
void spi_peri_deselect();
void spi_peri_write();

void ReadFirmware_init();
unsigned int ReadFirmware();
void i2c_peri_enable();
void i2c_peri_read_inital();
unsigned int i2c_peri_read();
void i2c_peri_write();
void i2c_peri_write_initial();

void ultraschall_init();
void display_init();

#endif

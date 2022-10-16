#include <stdio.h>
#include "peripherals.h"
#include <debugging.h>
#include "ourdisplay.h"
#include <moduleParameters.h>
#include "sleep.h"
#include "display.h"
#include "interrupt.h"
#include <stdlib.h>
#include <peripherals/compare.h>
#include <peripherals/timer.h>

DEFINE_SLEEP_MS(sleep_ms, SB_SPARTANMC_FREQUENCY)

FILE * stdout = &UART_LIGHT_FILE;
unsigned int distances[5];
int	state = 0;
int index = 0;
unsigned int firmware;
unsigned int dataReturn;
char str[5];
char *cm = "_____cm";
int timer_flag;

/*
 * Medianfilterung, um die Ausgabe zu filtern
 * @arr Ein Array, das fünf aufeinanderfolgende Entfernungen speichert
 * */
unsigned int MidFilter(unsigned int arr[5]){
	short isSorted;
	int temp;
	int length = sizeof(arr[5]) / sizeof(unsigned int);
	for(int i=0; i<length-1; i++){
        isSorted = 1;  			//assume the remaining elements are sorted
        for(int j=0; j<length-1-i; j++){
            if(arr[j] > arr[j+1]){
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
                isSorted = 0;  			//if exchange happens => the array is unordered
            }
        }
        if(isSorted) break; 			//if no change => array is ordered
    }
    return arr[length/2];
}

/*
 * Interrupt
 * */
 
ISR(0)(){
		//printf("interrupzt state");
		
		//second version
		switch(state){
			case 0: 
				state = 1;
				firmware = ReadFirmware(); // ReadFirmware
				//printf("Die State ist:%u\n",state);
				break;
			case 1: // judge if the Reg0 = 0xFF
				if(firmware == 0xFF){
					state = 0;
					firmware = ReadFirmware();
				}else{
					state = 2;
					i2c_peri_read_inital();
				}
				break;
			case 2:
				state = 0;
				dataReturn = i2c_peri_read();
				if(index < 5){
					distances[index] = dataReturn;
					index++;
				}else {
					index = 0;
					dataReturn = MidFilter(distances);
					printf("Die Distanz ist:%u\n", dataReturn);
					//itoa(dataReturn, str,10);
					snprintf(str, 5, "%u", dataReturn);
					//timer_flag = 1;
				}
				break;
			default:
				printf("unknown state: %u\n", state);
				state = 0;
				break;
		}

	
}



void main() {
	printf("*************New Round start***************\n");
	interrupt_enable();
	display_init();
	timer_flag = 0;
	//char *str2 = "123456";

	printf("*************New Round start***************\n");
	while(1){
		//ultraschall_init();
		
		/*second version: statemachine in INterrupt*/
		i2c_peri_enable();
		i2c_peri_write(0x51);
		
		/*first version with sleep function, this can perfectly run*/
		/*
		i2c_peri_enable();
		i2c_peri_write(0x51);
		sleep_ms(65); //Sleep for 65 ms
		
		i2c_peri_read_inital();
		dataReturn = i2c_peri_read();
		if(index < 5){
			distances[index] = dataReturn;
			index++;
		}else {
			index = 0;
			dataReturn = MidFilter(distances);
			printf("Die Distanz ist:%u\n", dataReturn);
			
			//itoa(dataReturn, str,10);
			snprintf(str, 5, "%u", dataReturn);
		}*/
		
			
			Show_String_25664(1, cm, 40, 12);
			Show_String_25664(1, str, 40, 12);
	}
	
	spi_peri_deselect();
}

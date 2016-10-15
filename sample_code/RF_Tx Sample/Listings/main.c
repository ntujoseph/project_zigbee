#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "autonet.h"

int main(void)
{
	uint8_t tx_msg[256];
	uint8_t tx_length;
	uint8_t count = 0;
	uint8_t Type;
	uint16_t Addr;
	uint8_t radio_channel;
	uint16_t radio_panID;
	
	Type = Type_Light;
	Addr = 0x0001;
	radio_channel = 25;
	radio_panID = 0x0007;
	
	Initial(Addr, Type, radio_channel, radio_panID);
	setTimer(1,1000,UNIT_MS);
	
	
	
	while(1){
		if(checkTimer(1)){		
			
			tx_msg[0] = count;
			count ++;
			count %=2;
			tx_length = 1;
			RF_Tx(0x0003,tx_msg,tx_length);
			
			//for (i=0;i<=1000;i++); // short delay
		}
	}
}

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "autonet.h"

int main(void)
{
	uint8_t rcvd_msg[20];
	uint8_t rcvd_payload[20];
	uint8_t rcvd_length;
	uint8_t rcvd_payloadLength;
	uint8_t rcvd_rssi;
	uint8_t output_array[10];
  uint8_t n;
	uint8_t Type;
	uint16_t Addr;
	uint8_t radio_channel;
	uint16_t radio_panID;

	Type = Type_Light;
	Addr = 0x0002;
	radio_channel = 25;
	radio_panID = 0x0007;

	Initial(Addr,Type, radio_channel, radio_panID);

	while(1){
		if(RF_Rx(rcvd_msg, &rcvd_length, &rcvd_rssi)){
			getPayloadLength(&rcvd_payloadLength, rcvd_msg);
			getPayload(rcvd_payload, rcvd_msg, rcvd_payloadLength);
			//n=sprintf((char *)output_array,"%d\r\n",rcvd_payload[0]);
			//get relay state
		//	n=sprintf((char *)output_array,"%d\r\n",getSwitchState);
			
			if (rcvd_payload[0]==0x1) //light on
				 setGPIO(2,1);
			else if (rcvd_payload[0]==0x0) //light off
				 setGPIO(2,0);
		//	COM2_Tx(output_array,3);
		}
	}
}

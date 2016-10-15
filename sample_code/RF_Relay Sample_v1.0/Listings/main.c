#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "autonet.h"

#define PRINT_BUFSIZE 64

int main(void)
{
	uint8_t rcvd_msg[20];
	uint8_t rcvd_payload[20];
	uint8_t rcvd_length;
	uint8_t rcvd_payloadLength;
	uint8_t rcvd_rssi;
	uint8_t output_array[PRINT_BUFSIZE];
  uint8_t len;
	uint8_t Type;
	uint16_t Addr;
	uint8_t radio_channel;
	uint16_t radio_panID;
  	
	Type = Type_Light;
	Addr = 0x0003;
	radio_channel = 25;
	radio_panID = 0x0007;
  
 
	
	Initial(Addr,Type, radio_channel, radio_panID);
	
	len=snprintf((char *)output_array,PRINT_BUFSIZE,"starging Zigbee Relay......\r\n");
	COM2_Tx(output_array,len);

	while(1){
		if(RF_Rx(rcvd_msg, &rcvd_length, &rcvd_rssi)){
			getPayloadLength(&rcvd_payloadLength, rcvd_msg);
			getPayload(rcvd_payload, rcvd_msg, rcvd_payloadLength);
			sprintf((char *)output_array,"%d\r\n",rcvd_payload[0]);
			//get relay state
			COM2_Tx(output_array,3);
		  sprintf((char *)output_array,"%d\r\n",getSwitchState());
		  COM2_Tx(output_array,3);
			
			if (getSwitchState()==1) 
		    RF_Tx(0xFFFF,rcvd_payload,1);

			
		}
	}
}

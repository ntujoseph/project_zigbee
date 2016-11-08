#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "autonet.h"
#define PRINT_BUFSIZE 64

#define ROLE 1 //1: sender; 2: receiver
void debug_print(char *s);
int main(void) { 
 
 
  uint8_t Type = 0x01; 
  uint8_t radio_channel = 25; 
	uint8_t r,n;
  uint16_t radio_panID = 0x0007; 
	int degree;
  uint8_t output_array[PRINT_BUFSIZE]={0};
  uint8_t Num; 
  uint16_t addr[10];   
  uint16_t value[10]; 
	uint16_t addr_array[2]={0};
	int len;
		uint8_t rcvd_msg[20];
	uint8_t rcvd_payload[20];
	uint8_t rcvd_length;
	uint8_t rcvd_payloadLength;
		uint8_t rcvd_rssi;
	
		#if ROLE==1

	  uint16_t Addr = 0x00F2; 
	  Initial(Addr, Type, radio_channel, radio_panID); 
	  len=sprintf((char *)output_array,"I'm sender,addr=%x\r\n",Addr);
	   debug_print(output_array);
	  #elif ROLE==2  //receiver
	   uint16_t Addr = 0x00F1; 
	   Initial(Addr, Type, radio_channel, radio_panID); 
		 len=sprintf((char *)output_array,"I'm receiver,addr=%x\r\n",Addr);
	   debug_print(output_array);
     #endif 
 
  setTimer(1, 1000, UNIT_MS); 
    

	
while(1) { 
  
	 update_group_info(); 
  
    if(checkTimer(1)) { 
			
			#if ROLE==1
			
	  
			 n=get_LOS_device(addr_array,1); 	//comx	 
			 sprintf((char *)output_array,"COM1:n=%d,addr=%x\r\n",n,addr_array[0]);
		   debug_print(output_array);
			
	   
			 r=getSwitchState();
			 sprintf((char *)output_array,"SwitchState=%d\r\n",r);
		   debug_print(output_array);
			 if (r==1) setGPIO(1,1);
			 else  setGPIO(1,0);
			
			if (n>0) {	
		   RF_Tx(addr_array[0],&r,1);
		  //RF_Tx(0xFFFF,&r,1);
			}
			#elif ROLE==2  //receiver
    
		    if(RF_Rx(rcvd_msg, &rcvd_length, &rcvd_rssi)){
			getPayloadLength(&rcvd_payloadLength, rcvd_msg);
			getPayload(rcvd_payload, rcvd_msg, rcvd_payloadLength);
			len=sprintf((char *)output_array,"%d\r\n",rcvd_payload[0]);
		  COM2_Tx(output_array,len);
	
			if (rcvd_payload[0]==0x1) //light on
				 setGPIO(2,1);
			else if (rcvd_payload[0]==0x0) //light off
				 setGPIO(2,0);
	
		}
       #endif
			
    } 
 } 

 }

 void debug_print(char *s)
{
	  int len=strlen(s);
	  COM2_Tx(s,len);


}

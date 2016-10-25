#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "autonet.h"
#define PRINT_BUFSIZE 64

int main(void) { 
 
  uint16_t Addr = 0x00F1; 
  uint8_t Type = 0x01; 
  uint8_t radio_channel = 25; 
	uint8_t r;
  uint16_t radio_panID = 0x0007; 
	int degree;
  uint8_t output_array[PRINT_BUFSIZE]={0};
  uint8_t Num; 
  uint16_t addr[10];   
  uint16_t value[10]; 
	int len;
 
  Initial(Addr, Type, radio_channel, radio_panID); 
  setTimer(1, 3000, UNIT_MS); 

while(1) { 
  
    update_group_info(); 
    if(checkTimer(1)) { 
			
			 r=get_direction(&degree); 
		   len=sprintf((char *)output_array,"%d\r\n",degree);
		   COM2_Tx(output_array,len);
	
		
      Num = Group_Diff(addr, value, 0x00,degree, 30); 
			 len=sprintf((char *)output_array,"Num=%d\r\n",Num);
		   COM2_Tx(output_array,len);
      if(Num!=0) {    // matching device(s) exist(s) 
        // do something 
				 setGPIO(2,1);
      } else {              // No matching device 
        // do something
         setGPIO(2,0);				
      } 
			
    } 
 } 

 }

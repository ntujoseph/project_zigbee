#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "autonet.h"

#define ROLE 0 //  1: transmitter,  0: receiver
#define MY_DEVICE_ADDR  0x0008   //mainly for transmitter
#define MY_DEVICE_ID  MY_DEVICE_ADDR   //should be (0~7)

#define DEBUG 0
#define TARGET_ADDR 0x00F1   //receiver address
#define MAX_PACKET_COUNT 1000
#define MAX_RECV_COUNT 100
#define PAN_ID  0x0007
#define RADIO_CHANNEL  25
#define PRINT_BUFSIZE 128
#define RCV_BUFSIZE 128
//-------------------
void debug_print(char *s);
void show_report(uint16_t id);

typedef struct _packet
{
  uint16_t dev_id __attribute__((packed)); //id=0,1,2,3,...,7
  uint16_t seq_no __attribute__((packed));
	uint8_t data[60] __attribute__((packed));
	
}Packet;

	  

typedef struct _record {
  
	uint16_t dev_id;
	uint16_t last_seq_no;
	uint16_t packet_count;
	uint16_t t1; //the time of the first packet
	uint16_t t2; 
	uint8_t  finish;
	
} Record;

Record report[10]={0};

//------------------


int main(void)
{
	
	uint8_t rcvd_msg[RCV_BUFSIZE];
	uint8_t rcvd_payload[RCV_BUFSIZE];
	uint8_t rcvd_length;
	uint8_t tx_msg[256];
	uint8_t tx_length;
	uint16_t count=0;
	uint8_t rcvd_payloadLength;
	uint8_t rcvd_rssi;
  uint8_t len;
  Packet * pkt;
	uint8_t Type;
	uint8_t id;
	uint16_t Addr;
	uint8_t radio_channel;
	uint16_t radio_panID;
	uint16_t timer_count=0;
  uint8_t output_array[PRINT_BUFSIZE]={0}; 	
	
	Packet mypacket;
	Type = Type_Light;
	Addr = MY_DEVICE_ADDR;
	radio_channel = RADIO_CHANNEL;
	radio_panID = PAN_ID;

	
	
#if ROLE==1   //transmitter

	
	Initial(Addr,Type, radio_channel, radio_panID);

	debug_print("Starging Zigbee Relay......\r\n");
	

   
	    debug_print("I'm Transmitter\r\n");
  
    mypacket.dev_id=MY_DEVICE_ID; //should increment by one for differenct device(0~7)
	  mypacket.seq_no=count;

	while(count<=MAX_PACKET_COUNT){
	
			RF_Tx(TARGET_ADDR,(uint8_t *)&mypacket,sizeof(Packet));
      mypacket.seq_no=++count;			

	}

#else 
	  
  
	// if you are receiver ,your device address should be TARGET_ADDR
	Addr=TARGET_ADDR;  
	Initial(Addr,Type, radio_channel, radio_panID); 
	setTimer(1, 100, UNIT_MS);  
  debug_print("I'm receiver\r\n");
	
	while(1){
		
		 if(checkTimer(1)) { 
          timer_count++;
			    //sprintf((char *)output_array,"%d\r\n",timer_count);
			    //debug_print(output_array);
      } 
		
	
		if(RF_Rx(rcvd_msg, &rcvd_length, &rcvd_rssi)){
			
			getPayloadLength(&rcvd_payloadLength, rcvd_msg);
			getPayload(rcvd_payload, rcvd_msg, rcvd_payloadLength);
			//dump
		 //len=sprintf((char *)output_array,"%d\r\n",rcvd_payload[0]);
     //debug_print(output_array);
			
			pkt=(Packet *)rcvd_payload;
			//record to statistical table
		  id=pkt->dev_id;
			
			if (report[id].packet_count<MAX_RECV_COUNT) {			
			
			if(report[id].packet_count==0) {
					report[id].dev_id=id;
			  report[id].t1=timer_count; //time for the first one
			}
			
		
			report[id].packet_count++;
		
			report[id].last_seq_no=pkt->seq_no;		
	  	report[id].t2=timer_count; // now
			#if DEBUG==1
		  sprintf((char *)output_array,"%d %d %d %d %d\r\n",id,pkt->seq_no,report[id].packet_count,timer_count,report[id].finish);
		  debug_print(output_array);			
      #endif
		  } else if (report[id].packet_count==MAX_RECV_COUNT && report[id].finish==0) {				
		 	    show_report(id);
				  report[id].finish=1;
			
	    } 
	
	  }  //if rx available
	} //edn while(1)

#endif		
	
		

} //end main



void debug_print(char *s)
{
	  int len=strlen(s);
	  COM2_Tx(s,len);


}

void show_report(uint16_t id)
{

	char output_array[PRINT_BUFSIZE]={0}; 	
		sprintf((char *)output_array,"ID COUNT SEQ T1	T2\r\n");
		debug_print(output_array);
		
	
	//for(id=0;id<8;id++){
	
	sprintf((char *)output_array,"%2d %4d %4d %4d %4d\r\n",
		 report[id].dev_id,report[id].packet_count,report[id].last_seq_no,report[id].t1,report[id].t2);
	debug_print(output_array);
	

}









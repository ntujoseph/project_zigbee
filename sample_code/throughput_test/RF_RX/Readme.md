
### 1個收送7個送

* Receiver address: 0x00F1

* ID 0:  Transmitter address: 0x0001
* ID 1:  Transmitter address: 0x0002
* ID 2:  Transmitter address: 0x0003
* ID 3:  Transmitter address: 0x0004
* ID 4:  Transmitter address: 0x0005
* ID 5:  Transmitter address: 0x0006
* ID 6:  Transmitter address: 0x0007


### 封包格式

dev_id | seq_no | data
-------|--------| -------------
2byte  | 2byte  | 60byte

```c
typedef struct _packet
{
  uint16_t dev_id __attribute__((packed)); //id=0,1,2,3,...,7
  uint16_t seq_no __attribute__((packed));  
   uint8_t data[60] __attribute__((packed));
	
}Packet;
```
**記錄每一個人的throughput統計表**

```c
typedef struct _record {
  
	uint16_t dev_id;
	uint16_t last_seq_no;
	uint16_t packet_count;
	uint16_t t1;  //the time of the first packet
	uint16_t t2; 
	uint8_t  finish;
} Record;
```
如:


ID COUNT SEQ T1 T2   
0  400  568   53  110


# 計算每個Dvice的throughput

throughput=(packet_count*64)/(T2-T1)*100ms  <br>
packet_count=MAX_RECV_COUNT(we set to 400)

#include <SoftwareSerial.h>

EspSoftwareSerial::UART Port[8];

#define beep1 0x11
#define beep3 0x13
#define ledoff 0x20
#define ledr 0x21
#define ledy 0x22
#define ledry 0x23
#define ledg 0x24
#define ledrg 0x25
#define ledyg 0x26
#define ledryg 0x27
#define rfidSym 0x40
#define startSym 0x43
#define restartSym 0xff

uint8_t recvStatus=0;
bool doRestart[8];

void initRS422() {
  Port[0].begin(9600,SWSERIAL_8N1,1,2,false);
  Port[1].begin(9600,SWSERIAL_8N1,4,5,false);
  Port[2].begin(9600,SWSERIAL_8N1,6,7,false);
  Port[3].begin(9600,SWSERIAL_8N1,16,17,false);
  Port[4].begin(9600,SWSERIAL_8N1,18,21,false);
  Port[5].begin(9600,SWSERIAL_8N1,38,39,false);
  Port[6].begin(9600,SWSERIAL_8N1,40,41,false);
  Port[7].begin(9600,SWSERIAL_8N1,42,47,false); }

void rs422Worker() {
  static uint32_t rs422SendTimer[8], rs422RecvTimer[8];
  static int command[8]={rfidSym}, bytes[8]={0}, receive[8]={0}, check[8]={0}, buffer[8][3]={0};

  for (int port=0;port<8;port++) {

    if (millis()>=rs422SendTimer[port]) {
      if (doAlarm[port]==1) { command[port]=ledr; doAlarm[port]=2; }
      if (doRestart[port]==1) { command[port]=restartSym; doRestart[port]=0; }
      Port[port].write(command[port]);
      if (command[port]==ledg) { command[port]=beep1; rs422SendTimer[port]=millis()+100; }
      else if (command[port]==ledr) { command[port]=beep3; rs422SendTimer[port]=millis()+100; }
      else if (command[port]==beep1) { command[port]=rfidSym; rs422SendTimer[port]=millis()+900; }
      else if (command[port]==beep3) { command[port]=rfidSym; rs422SendTimer[port]=millis()+900; }
      else if (command[port]==rfidSym) { command[port]=ledy; rs422SendTimer[port]=millis()+100; }
      else { command[port]=rfidSym; rs422SendTimer[port]=millis()+100; } }

    while (Port[port].available()) {
      if (bytes[port]==0) {
        receive[port]=Port[port].peek();
        if (receive[port]==0x1) { Port[port].read(); rs422RecvTimer[port]=millis()+10000; }
        else if (receive[port]==rfidSym) { Port[port].read(); bytes[port]=-1; check[port]=rfidSym; }
        else if (receive[port]==startSym) { Port[port].read(); bytes[port]=-1; }
        else { Port[port].read(); } }
      else if (bytes[port]==-1 && Port[port].available()) { bytes[port]=Port[port].read(); check[port]^=bytes[port]; }
      while (bytes[port]>0 && Port[port].available()) { int n=Port[port].read(); check[port]^=n; bytes[port]--; if (bytes[port]<3) { buffer[port][bytes[port]]=n; }
        if (bytes[port]==0) {
          if (receive[port]==rfidSym) { int value=(String(buffer[port][2],HEX).toInt()*100)+String(buffer[port][1],HEX).toInt(); uint16_t m=checkRFID(value,port);
            if ((m & 0xff)>0 && check[port]==0) { for (int o=0;o<8;o++) { if (bitRead(m,o)==1) { Log.print(1,"Port: %i RFID read: %i OK\r\n",o,value); command[o]=ledg; doMessage(0,value,o); setPin(o,HIGH,m>>8); } } }
            else { Log.print(1,"Port: %i RFID read: %i Checksum: %s denied\r\n",port,value,String(check[port],HEX).c_str()); command[port]=ledr; doMessage(1,value,port); } }
          else if (receive[port]==startSym) { Log.print(1,"Port: %i Reader Started\r\n",port); command[port]=beep1; doMessage(4,0,port); } } } }

    if (millis()>=rs422RecvTimer[port] && bitRead(recvStatus,port)==1) { bitClear(recvStatus,port); Log.print(1,"Port: %i Reader Offline\r\n",port); doMessage(2,0,port); }
    if (millis()<rs422RecvTimer[port] && bitRead(recvStatus,port)==0) { bitSet(recvStatus,port); Log.print(1,"Port: %i Reader Online\r\n",port); doMessage(3,0,port); } } }

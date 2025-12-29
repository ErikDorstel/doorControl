#include <Wire.h>
#include <MCP23017.h>

MCP23017 mcp=MCP23017(0x20,48);

int mcpInPin[8]={0,1,2,3,8,9,10,11};
int mcpOutPin[8]={4,5,6,7,12,13,14,15};
uint32_t mcpInTimer[8];
uint32_t mcpOutTimer[8];
uint32_t mcpIntervalTimer[8];
uint8_t mcpIntervalStatus[8];
uint8_t doAlarm[8];
uint8_t alarmFlags=0;

void initMCP() {
  Wire.begin();
  mcp.Init();
  mcp.setPortMode(0b11110000,A,INPUT_PULLUP); mcp.setPort(0b00000000,A);
  mcp.setPortMode(0b11110000,B,INPUT_PULLUP); mcp.setPort(0b00000000,B); }

void doorInterval(int pin) {
  if (mcpIntervalStatus[pin]==1) {
    mcpIntervalStatus[pin]=2;
    mcpIntervalTimer[pin]=millis()+1000;
    if (pin<4) { mcp.setPin(mcpOutPin[pin],A,HIGH); } else if (pin<8) { mcp.setPin(mcpOutPin[pin]-8,B,HIGH); } }
  else if (mcpIntervalStatus[pin]==2) {
    mcpIntervalStatus[pin]=1;
    mcpIntervalTimer[pin]=millis()+5000;
    if (pin<4) { mcp.setPin(mcpOutPin[pin],A,LOW); } else if (pin<8) { mcp.setPin(mcpOutPin[pin]-8,B,LOW); } } }

void setPin(int pin, bool value, int options) {
  if (pin<4) { if ((options & 2)==2 && value==HIGH) { mcpIntervalTimer[pin]=millis(); mcpIntervalStatus[pin]=1; } else { mcp.setPin(mcpOutPin[pin],A,value); } }
  else if (pin<8) { if ((options & 2)==2 && value==HIGH) { mcpIntervalTimer[pin]=millis(); mcpIntervalStatus[pin]=1; } else { mcp.setPin(mcpOutPin[pin]-8,B,value); } }
  if (value) { if ((options & 2)==2) { mcpOutTimer[pin]=millis()+60000UL; } else { mcpOutTimer[pin]=millis()+5000; } Log.print(1,"Port: %i Output High\r\n",pin); doMessage(5,0,pin); }
  else { mcpIntervalTimer[pin]=0; mcpIntervalStatus[pin]=0; mcpOutTimer[pin]=0; Log.print(1,"Port: %i Output Low\r\n",pin); doMessage(6,0,pin); } }

bool getPin(int pin) {
  bool value=0;
  if (bitRead(readerInput[0].ports,pin)==0) { return value; }
  if (pin<4) { value=mcp.getPin(mcpInPin[pin],A); }
  else if (pin<8) { value=mcp.getPin(mcpInPin[pin]-8,B); }
  if (value && mcpInTimer[pin]==0) { mcpInTimer[pin]=millis(); Log.print(1,"Port: %i Input Open\r\n",pin); doMessage(7,0,pin); }
  else if (value && millis()>=mcpInTimer[pin]+10000UL && doAlarm[pin]==2) { mcpInTimer[pin]=millis(); doAlarm[pin]=1; }
  else if (value && millis()>=mcpInTimer[pin]+600000UL) { mcpInTimer[pin]=millis(); Log.print(1,"Port: %i Input Open Alarm\r\n",pin); doAlarm[pin]=1; bitSet(alarmFlags,pin); doMessage(8,0,pin); }
  else if ((!value) && mcpInTimer[pin]>0) { mcpInTimer[pin]=0; Log.print(1,"Port: %i Input Closed\r\n",pin); doAlarm[pin]=0; bitClear(alarmFlags,pin); doMessage(9,0,pin); }
  return value; }

void mcpWorker() {
  for (int n=0;n<8;n++) {
    if (millis()>mcpOutTimer[n] && mcpOutTimer[n]>0) { setPin(n,LOW,0); }
    if (millis()>=mcpIntervalTimer[n] && mcpIntervalTimer[n]>0) { doorInterval(n); }
    getPin(n); } }

EthernetUDP alarmUDP;

void alarmWorker() {
  static uint32_t alarmTimer;
  static uint8_t attempt=0;
  if (checkEth()!=2) { return; }

  if (millis()>=alarmTimer || alarmFlag==1) {
    alarmFlag=0; attempt++; attempt%=20;
    if (attempt<15) { alarmTimer=millis()+1000; } else { alarmTimer=millis()+60000UL; }
    alarmUDP.begin(9999);
    if (alarmUDP.beginPacket("255.255.255.255",9999)==1) {
      alarmUDP.write(ethMAC,6);
      for (int n=0;n<8;n++) { alarmUDP.write(alarmFlags[n]); }
      alarmUDP.endPacket();
      Log.print(2,"Input Open UDP Status sent: ");
      for (int n=0;n<8;n++) { Log.write(2,alarmFlags[n]+48); }
      Log.print(2,"\r\n"); } }

  if (alarmUDP.parsePacket()==6) {
    Log.print(2,"Input Open UDP Status reply: received\r\n");
    attempt=0; alarmTimer=millis()+60000UL; } }

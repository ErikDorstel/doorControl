EthernetUDP alarmUDP;

void alarmWorker() {
  static uint32_t alarmTimer;
  if (checkEth()!=2) { return; }

  if (millis()>=alarmTimer) { alarmTimer=millis()+60000UL;
    alarmUDP.begin(9999);
    if (alarmUDP.beginPacket("255.255.255.255",9999)==1) {
      alarmUDP.write(ethMAC,6);
      for (int n=0;n<8;n++) { alarmUDP.write(alarmFlags[n]); }
      alarmUDP.endPacket();
      Log.print(1,"Input Open UDP Status sent: ");
      for (int n=0;n<8;n++) { Log.write(1,alarmFlags[n]+48); }
      Log.print(1,"\r\n"); } } }

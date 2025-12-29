EthernetUDP alarmUDP;

void alarmWorker() {
  static uint32_t alarmTimer;
  if (checkEth()!=2) { return; }

  if (millis()>=alarmTimer) { alarmTimer=millis()+60000UL;
    alarmUDP.begin(9999);
    if (alarmUDP.beginPacket("255.255.255.255",9999)==1) {
      alarmUDP.write(ethMAC,6);
      alarmUDP.write(alarmFlags);
      alarmUDP.endPacket();
      Log.print(1,"Alarm sent Status: %i\r\n",alarmFlags); } } }

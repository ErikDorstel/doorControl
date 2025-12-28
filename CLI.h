String cmdLine="";

void help() {
  Log.print(0,"debug [0-1]\r\n");
  Log.print(0,"get version\r\n");
  Log.print(0,"get status\r\n");
  Log.print(0,"clear status\r\n");
  Log.print(0,"get configuration\r\n");
  Log.print(0,"set dhcp\r\n");
  Log.print(0,"set ip [static IP]\r\n");
  Log.print(0,"set gw [static Gateway]\r\n");
  Log.print(0,"set dns [static DNS]\r\n");
  Log.print(0,"set nm [static Netmask]\r\n");
  Log.print(0,"set ntp [Hostname]\r\n");
  Log.print(0,"set password [Password]\r\n");
  Log.print(0,"get rfid [RFID]\r\n");
  Log.print(0,"get profile [Profile]\r\n");
  Log.print(0,"set port [Port]\r\n");
  Log.print(0,"check rfid [RFID] [Port]\r\n");
  Log.print(0,"get messages\r\n");
  Log.print(0,"restart port [Port]\r\n");
  Log.print(0,"restart ethernet\r\n");
  Log.print(0,"restart cpu\r\n");
  Log.print(0,"get flash\r\n");
  Log.print(0,"read flash\r\n");
  Log.print(0,"write flash\r\n");
  Log.print(0,"erase flash\r\n");
  Log.print(0,"read data\r\n");
  Log.print(0,"read default\r\n");
  Log.print(0,"write data\r\n");
  Log.print(0,"erase data\r\n");
  Log.print(0,"exit\r\n");
  Log.print(0,"help\r\n"); }

void doParse() {
  cmdLine.trim();
  if (cmdLine!="") { Log.print(0,"\r\n"); }
  String value=cmdLine.substring(cmdLine.lastIndexOf(" ")+1);
  if (cmdLine.startsWith("deb")) { Log.debug=value.toInt(); Log.print(0,"Debug Level: %i\r\n",Log.debug); }
  else if (cmdLine.startsWith("get ver")) { Log.print(0,"doorControl Version 0.9\r\n"); }
  else if (cmdLine.startsWith("get stat")) {
    Log.print(0,"Eth Status: "); if (checkEth()>0) { Log.print(0,"Up (%i)",checkEth()); } else { Log.print(0,"Down (%i)",checkEth()); }
    if (checkEth()>1) { Log.print(0,"   IP: %s",Ethernet.localIP().toString().c_str()); }
    Log.print(0,"   Events Up: %i",upEvents);
    Log.print(0,"   Down: %i\r\n",downEvents);
    Log.print(0,"Reader Ports: "); for (int n=0;n<8;n++) { Log.write(0,bitRead(recvStatus,n)+48); }
    Log.print(0,"   Input: "); for (int n=0;n<8;n++) { Log.write(0,getPin(n)+48); }
    Log.print(0,"   Enable: "); for (int n=0;n<8;n++) { Log.write(0,bitRead(readerInput[0].ports,n)+48); } Log.print(0,"\r\n");
    Log.print(0,"NTP Hostname: %s",ntpHost.c_str());
    Log.print(0,"   Address: %s",timeServer.toString().c_str());
    Log.print(0,"   Next: %i\r\n",ntpTimer<millis()?0:(ntpTimer - millis())/1000);
    Log.print(0,"NTP Epoch: %i",rtc.now().unixtime());
    Log.print(0,"   Day: %i",dayOfWeek(rtc.now().unixtime()));
    Log.print(0,"   Daily Epoch: %i\r\n",epochOfDay(rtc.now().unixtime()));
    Log.print(0,"TELNET Session: %i",sessionActive);
    if (sessionActive) { Log.print(0,"   IP: %s",telnetSession.remoteIP().toString().c_str()); }
    if (sessionActive) { Log.print(0,"   Auth: %i",isAuth); }
    Log.print(0,"   Debug: %i\r\n",Log.debug);
    Log.print(0,"Heap Total: %d",ESP.getHeapSize());
    Log.print(0,"   Free: %d",ESP.getFreeHeap());
    Log.print(0,"   Used: %d\r\n",ESP.getHeapSize()-ESP.getFreeHeap());
    Log.print(0,"PSRAM Total: %d",ESP.getPsramSize());
    Log.print(0,"   Free: %d",ESP.getFreePsram());
    Log.print(0,"   Used: %d\r\n",ESP.getPsramSize()-ESP.getFreePsram());
    Log.print(0,"Uptime: %i days %s hours\r\n",millis()/86400000,String((double)(millis()%86400000)/3600000.0,2).c_str()); }
  else if (cmdLine.startsWith("clear stat")) { upEvents=0; downEvents=0; Log.print(0,"Statistics cleared\r\n"); }
  else if (cmdLine.startsWith("get conf")) {
    Log.print(0,"Eth DHCP: %i\r\n",ethDHCP);
    Log.print(0,"Eth IP: %s\r\n",ethIP.c_str());
    Log.print(0,"Eth GW: %s\r\n",ethGW.c_str());
    Log.print(0,"Eth DNS: %s\r\n",ethDNS.c_str());
    Log.print(0,"Eth NM: %s\r\n",ethNM.c_str());
    Log.print(0,"NTP Hostname: %s\r\n",ntpHost.c_str());
    Log.print(0,"TELNET Password: xxxx\r\n"); }
  else if (cmdLine.startsWith("set dhcp")) { ethDHCP=!ethDHCP; Log.print(0,"Eth DHCP: %i\r\n",ethDHCP); }
  else if (cmdLine.startsWith("set ip")) { if (value=="ip") { ethIP=""; } else { ethIP=value; } Log.print(0,"Eth IP: %s\r\n",ethIP.c_str()); }
  else if (cmdLine.startsWith("set gw")) { if (value=="gw") { ethGW=""; } else { ethGW=value; } Log.print(0,"Eth GW: %s\r\n",ethGW.c_str()); }
  else if (cmdLine.startsWith("set dns")) { if (value=="dns") { ethDNS=""; } else { ethDNS=value; } Log.print(0,"Eth DNS: %s\r\n",ethDNS.c_str()); }
  else if (cmdLine.startsWith("set nm")) { if (value=="nm") { ethNM=""; } else { ethNM=value; } Log.print(0,"Eth NM: %s\r\n",ethNM.c_str()); }
  else if (cmdLine.startsWith("set ntp")) { if (value=="ntp") { ntpHost=""; } else { ntpHost=value; } Log.print(0,"NTP Hostname: %s\r\n",String(ntpHost).c_str()); }
  else if (cmdLine.startsWith("set pass")) { if (value=="pass" || value=="password") { telnetPass="access"; } else { telnetPass=value; } Log.print(0,"TELNET Password: xxxx\r\n"); }
  else if (cmdLine.startsWith("get rfid")) { if (value!="rfid") { int m=value.toInt(); if (m>=0 && m<10000) {
    for (int n=0;n<dataLength.locked;n++) { if (locked[n].rfid==m) { Log.print(0,"RFID: %i Locked: 1\r\n",m); } }
    for (int n=0;n<dataLength.rfid;n++) { if (rfid[n].rfid==m) { Log.print(0,"RFID: %i Profile: %i\r\n",m,rfid[n].profile); } } } } }
  else if (cmdLine.startsWith("get prof")) { if (value!="prof" && value!="profile") { int m=value.toInt(); if (m>=0 && m<5000) { for (int o=0;o<dataLength.profile;o++) { if (profile[o].profile==m) {
    Log.print(0,"Profile: %i Ports: ",m); for (int n=0;n<8;n++) { Log.write(0,bitRead(accessProfile[profile[o].accessProfile].ports,n)+48); }
    Log.print(0," Multi: %i Interval: %i\r\n",accessProfile[profile[o].accessProfile].options & 1,(accessProfile[profile[o].accessProfile].options & 2)>>1);
    Log.print(0,"   fromDay: %i toDay: %i ",restrictionProfile[profile[o].restrictionProfile].fromDay,restrictionProfile[profile[o].restrictionProfile].toDay);
    Log.print(0,"fromEpoch: %i toEpoch: %i\r\n",restrictionProfile[profile[o].restrictionProfile].fromEpoch,restrictionProfile[profile[o].restrictionProfile].toEpoch); } } } } }
  else if (cmdLine.startsWith("set po")) { if (value!="po" && value!="port") { int n=value.toInt(); if (n>=0 && n<8) { Log.print(0,"Done.\r\n"); setPin(n,HIGH,0); } } }
  else if (cmdLine.startsWith("check rfid")) { int m=cmdLine.substring(cmdLine.indexOf(" ",cmdLine.indexOf(" ")+1)+1).toInt(); int n=value.toInt(); if (m>=0 && m<10000) { if (n>=0 && n<8) {
    Log.print(0,"RFID: %i Port: %i Ports: ",m,n); uint16_t p=checkRFID(m,n); for (int o=0;o<8;o++) { Log.write(0,bitRead(p,o)+48); }
    Log.print(0,"\r\n  Multi: %i Interval: %i\r\n",bitRead(p,8),bitRead(p,9)); } } }
  else if (cmdLine.startsWith("get mess")) { Log.print(0,"Messages stored: %i\r\n",dataLength.message); for (int n=0;n<dataLength.message;n++) {
    Log.print(0,"Epoch: %i Port: %i Command: (%i) ",message[n].epoch,message[n].port,message[n].command); Log.print(0,messageText[message[n].command],message[n].rfid); Log.print(0,"\r\n"); } }
  else if (cmdLine.startsWith("restart port")) { if (value!="port" && value.toInt()<8) { doRestart[value.toInt()]=1; Log.print(0,"Port: %i Reader Restart sent\r\n",value.toInt()); } }
  else if (cmdLine.startsWith("restart eth")) { if (checkEth()>0) { configEth(); ntpNow(); } }
  else if (cmdLine.startsWith("restart cpu")) { if (telnetSession.connected()) { telnetSession.stop(); } ESP.restart(); }
  else if (cmdLine.startsWith("get flash")) { getFlash(); }
  else if (cmdLine.startsWith("read flash")) { readFlash(); }
  else if (cmdLine.startsWith("write flash")) { writeFlash(); }
  else if (cmdLine.startsWith("erase flash")) { eraseFlash(); }
  else if (cmdLine.startsWith("read data")) { dataRead(); }
  else if (cmdLine.startsWith("read default")) { defaultData(); }
  else if (cmdLine.startsWith("write data")) { dataWrite(); }
  else if (cmdLine.startsWith("erase data")) { dataErase(); }
  else if (cmdLine.startsWith("exit")) { Log.debug=0; if (telnetSession.connected()) { telnetSession.stop(); } }
  else if (cmdLine.startsWith("help")) { help(); }
  Log.print(0,"> "); Log.needCR=true; }

void cliWorker() {
  if (Serial.available()) {
    char serialByte=Serial.read();
    if (serialByte==127) { Log.write(0,serialByte); cmdLine.remove(cmdLine.length()-1); }
    else if (serialByte==10 || serialByte==13) { Log.needCR=false; Log.print(0,"\r\n"); doParse(); cmdLine=""; }
    else { Log.write(0,serialByte); cmdLine+=String(serialByte); } }

  if (telnetSession.available() && isAuth) {
    char telnetByte=telnetSession.read();
    if (isIAC==3) { isIAC=0; }
    if (telnetByte==255 && (!isIAC)) { isIAC++; }
    else if (telnetByte==255 && isIAC) { isIAC=0; }
    else if (isIAC) { isIAC++; }
    else if (telnetByte==127) { Serial.write(telnetByte); cmdLine.remove(cmdLine.length()-1); }
    else if (telnetByte==10) {}
    else if (telnetByte==13) { Log.needCR=false; Serial.println(); doParse(); cmdLine=""; }
    else { Serial.write(telnetByte); cmdLine+=String(telnetByte); } } }

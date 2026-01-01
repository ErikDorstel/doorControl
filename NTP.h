#include <Dns.h>
DNSClient dns;
EthernetUDP ntpUDP;

#include "RTClib.h"
RTC_DS3231 rtc;

String ntpHost="pool.ntp.org";
uint32_t ntpTimer=0;
IPAddress timeServer;
uint8_t packetBuffer[48];

int dayOfWeek(unsigned long t) { int n=((t/86400)+4)%7; if (n==0) { n=7; } return n; }
uint32_t epochOfDay(unsigned long t) { return t%86400; }

void initRTC() {
  rtc.begin();
  Log.print(0,"RTC: %i\r\n",rtc.now().unixtime()); }

void ntpNow() { ntpTimer=0; }

void ntpWorker() {
  if (checkEth()!=2) { return; }

  if (millis()>=ntpTimer) { ntpTimer=millis()+5000;
    dns.begin(Ethernet.dnsServerIP());
    if (dns.getHostByName(ntpHost.c_str(),timeServer,1500)!=1) { return; }
    ntpUDP.begin(8888);
    memset(packetBuffer,0,48);
    packetBuffer[0] = 0b11100011;   //Set LI, Version, Mode
    packetBuffer[1] = 0;            //Set Stratum, or type of clock
    packetBuffer[2] = 6;            //Set Polling Interval
    packetBuffer[3] = 0xEC;         //Set Peer Clock Precision
    //Set to 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    if (ntpUDP.beginPacket(timeServer,123)==1) {
      ntpUDP.write(packetBuffer,48);
      ntpUDP.endPacket();
      Log.print(1,"NTP: Request sent to: %s\r\n",ntpHost.c_str()); } }

  int udpBytes=ntpUDP.parsePacket();
  if (udpBytes==48) {
    ntpUDP.read(packetBuffer,48);
    uint32_t highWord = word(packetBuffer[40], packetBuffer[41]);
    uint32_t lowWord = word(packetBuffer[42], packetBuffer[43]);
    uint32_t epoch = (highWord << 16 | lowWord) - 2208988800UL;
    rtc.adjust(epoch);
    ntpTimer=millis()+600000UL;
    Log.print(1,"NTP: Reply received from %s: %i\r\n",ntpHost.c_str(),epoch); }
  else { for (int n=0;n<udpBytes;n++) { ntpUDP.read(); } } }

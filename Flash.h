#include <Preferences.h>
#include <nvs_flash.h>

Preferences flash;

void getFlash() {
  flash.begin("ACCESS",true);
  Log.print(0,"Eth DHCP: %i\r\n",flash.getBool("ethDHCP",true));
  Log.print(0,"Eth IP: %s\r\n",flash.getString("ethIP","192.168.100.100").c_str());
  Log.print(0,"Eth GW: %s\r\n",flash.getString("ethGW","192.168.100.1").c_str());
  Log.print(0,"Eth DNS: %s\r\n",flash.getString("ethDNS","192.168.100.1").c_str());
  Log.print(0,"Eth NM: %s\r\n",flash.getString("ethNM","255.255.255.0").c_str());
  Log.print(0,"NTP Hostname: %s\r\n",String(flash.getString("ntpHost","")).c_str());
  Log.print(0,"TELNET Password: xxxx\r\n");
  flash.end(); }

void readFlash() {
  flash.begin("ACCESS",true);
  ethDHCP=flash.getBool("ethDHCP",true);
  ethIP=flash.getString("ethIP","192.168.100.100");
  ethGW=flash.getString("ethGW","192.168.100.1");
  ethDNS=flash.getString("ethDNS","192.168.100.1");
  ethNM=flash.getString("ethNM","255.255.255.0");
  ntpHost=flash.getString("ntpHost","pool.ntp.org");
  telnetPass=flash.getString("telnetPass","access");
  flash.end();
  Log.print(0,"Flash: read\r\n");
  if (checkEth()>0) { configEth(); ntpNow(); } }

void writeFlash() {
  flash.begin("ACCESS",false);
  flash.putBool("ethDHCP",ethDHCP);
  flash.putString("ethIP",ethIP);
  flash.putString("ethGW",ethGW);
  flash.putString("ethDNS",ethDNS);
  flash.putString("ethNM",ethNM);
  flash.putString("ntpHost",ntpHost);
  flash.putString("telnetPass",telnetPass);
  flash.end();
  Log.print(0,"Flash: written\r\n"); }

void eraseFlash() {
  nvs_flash_erase(); nvs_flash_init();
  Log.print(0,"Flash: erased\r\n"); }

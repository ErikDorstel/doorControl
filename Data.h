#include <FS.h>
#include <LittleFS.h>

struct dataLengthStruct { uint16_t rfid=0; uint16_t locked=0; uint16_t profile=0; uint16_t accessProfile=0; uint16_t restrictionProfile=0; uint16_t readerInput=0; uint16_t message=0; } dataLength;

struct rfidStruct { uint16_t rfid=0xffff; uint16_t profile=0; };
rfidStruct *rfid=(rfidStruct*)heap_caps_malloc(sizeof(rfidStruct)*20000,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

struct lockedStruct { uint16_t rfid=0xffff; };
lockedStruct *locked=(lockedStruct*)heap_caps_malloc(sizeof(lockedStruct)*1000,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

struct profileStruct { uint16_t profile=0xffff; uint8_t accessProfile=0; uint8_t restrictionProfile=0; };
profileStruct *profile=(profileStruct*)heap_caps_malloc(sizeof(profileStruct)*5000,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

struct accessProfileStruct { uint8_t ports=0; uint8_t multi=0; };
accessProfileStruct *accessProfile=(accessProfileStruct*)heap_caps_malloc(sizeof(accessProfileStruct)*256,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

struct restrictionProfileStruct { uint8_t fromDay=0; uint8_t toDay=0; uint32_t fromEpoch=0; uint32_t toEpoch=0; };
restrictionProfileStruct *restrictionProfile=(restrictionProfileStruct*)heap_caps_malloc(sizeof(restrictionProfileStruct)*256,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

struct readerInputStruct { uint8_t ports=0; };
readerInputStruct *readerInput=(readerInputStruct*)heap_caps_malloc(sizeof(readerInputStruct)*1,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

struct messageStruct { uint8_t command=0xff; uint32_t epoch=0; uint16_t rfid=0; uint8_t port=0; };
messageStruct *message=(messageStruct*)heap_caps_malloc(sizeof(messageStruct)*50000,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

File file;

char messageText[][25]={"RFID read: %i OK","RFID read: %i denied","Reader Offline","Reader Online","Reader Started",
  "Output High","Output Low","Input Open","Input Open Alarm","Input Closed"};

void dataWrite() {
  file=LittleFS.open("/dataLength.struct",FILE_WRITE); int n=file.write((byte*)&dataLength,sizeof(dataLengthStruct)); file.close();
  file=LittleFS.open("/rfid.struct",FILE_WRITE); n+=file.write((byte*)rfid,sizeof(rfidStruct)*20000); file.close();
  file=LittleFS.open("/locked.struct",FILE_WRITE); n+=file.write((byte*)locked,sizeof(lockedStruct)*1000); file.close();
  file=LittleFS.open("/profile.struct",FILE_WRITE); n+=file.write((byte*)profile,sizeof(profileStruct)*5000); file.close();
  file=LittleFS.open("/accessProfile.struct",FILE_WRITE); n+=file.write((byte*)accessProfile,sizeof(accessProfileStruct)*256); file.close();
  file=LittleFS.open("/restrictionProfile.struct",FILE_WRITE); n+=file.write((byte*)restrictionProfile,sizeof(restrictionProfileStruct)*256); file.close();
  file=LittleFS.open("/readerInput.struct",FILE_WRITE); n+=file.write((byte*)readerInput,sizeof(readerInputStruct)*1); file.close();
  Log.print(0,"Data written: %i\r\n",n); }

void dataRead() {
  file=LittleFS.open("/dataLength.struct"); int n=file.read((byte*)&dataLength,sizeof(dataLengthStruct)); file.close();
  file=LittleFS.open("/rfid.struct"); n+=file.read((byte*)rfid,sizeof(rfidStruct)*20000); file.close();
  file=LittleFS.open("/locked.struct"); n+=file.read((byte*)locked,sizeof(lockedStruct)*1000); file.close();
  file=LittleFS.open("/profile.struct"); n+=file.read((byte*)profile,sizeof(profileStruct)*5000); file.close();
  file=LittleFS.open("/accessProfile.struct"); n+=file.read((byte*)accessProfile,sizeof(accessProfileStruct)*256); file.close();
  file=LittleFS.open("/restrictionProfile.struct"); n+=file.read((byte*)restrictionProfile,sizeof(restrictionProfileStruct)*256); file.close();
  file=LittleFS.open("/readerInput.struct"); n+=file.read((byte*)readerInput,sizeof(readerInputStruct)*1); file.close();
  Log.print(0,"Data read: %i\r\n",n); }

void defaultData() {
  dataLength.rfid=5;
    rfid[0].rfid=1234; rfid[0].profile=21;
    rfid[1].rfid=1234; rfid[1].profile=31;
    rfid[2].rfid=3272; rfid[2].profile=21;
    rfid[3].rfid=3272; rfid[3].profile=31;
    rfid[4].rfid=1111; rfid[4].profile=21;
  dataLength.locked=1;
    locked[0].rfid=1111;
  dataLength.profile=2;
    profile[0].profile=21; profile[0].accessProfile=0; profile[0].restrictionProfile=0;
    profile[1].profile=31; profile[1].accessProfile=1; profile[1].restrictionProfile=0;
  dataLength.accessProfile=2;
    accessProfile[0].ports=0b00110011; accessProfile[0].multi=0;
    accessProfile[1].ports=0b11001100; accessProfile[1].multi=1;
  dataLength.restrictionProfile=1;
    restrictionProfile[0].fromDay=1; restrictionProfile[0].toDay=7;
    restrictionProfile[0].fromEpoch=0; restrictionProfile[0].toEpoch=86399;
  dataLength.readerInput=1;
    readerInput[0].ports=0b11000000;
  Log.print(0,"Default Data: read\r\n"); }

void dataErase() {
  dataLength.rfid=0; dataLength.locked=0; dataLength.profile=0; dataLength.accessProfile=0; dataLength.restrictionProfile=0; dataLength.readerInput=0;
  for (int n=0;n<20000;n++) { rfid[n].rfid=0xffff; rfid[n].profile=0; }
  for (int n=0;n<1000;n++) { locked[n].rfid=0xffff; }
  for (int n=0;n<5000;n++) { profile[n].profile=0xffff; profile[n].accessProfile=0; profile[n].restrictionProfile=0; }
  for (int n=0;n<256;n++) { accessProfile[n].ports=0; accessProfile[n].multi=0; }
  for (int n=0;n<256;n++) { restrictionProfile[n].fromDay=0; restrictionProfile[n].toDay=0; restrictionProfile[n].fromEpoch=0; restrictionProfile[n].toEpoch=0; }
  for (int n=0;n<1;n++) { readerInput[n].ports=0; }
  Log.print(0,"Data: erased\r\n"); }

uint8_t checkRFID(int value, int port) {
  for (int n=0;n<dataLength.locked;n++) { if (locked[n].rfid==value) { return 0; } }
  int currentDay=dayOfWeek(rtc.now().unixtime());
  uint32_t currentEpoch=epochOfDay(rtc.now().unixtime());
  for (int n=0;n<dataLength.rfid;n++) {
    if (rfid[n].rfid==value) {
      for (int o=0;o<dataLength.profile;o++) {
        if (rfid[n].profile==profile[o].profile) {
          if (bitRead(accessProfile[profile[o].accessProfile].ports,port)==1) {
            if (currentDay>=restrictionProfile[profile[o].restrictionProfile].fromDay && currentDay<=restrictionProfile[profile[o].restrictionProfile].toDay) {
              if (currentEpoch>=restrictionProfile[profile[o].restrictionProfile].fromEpoch && currentEpoch<=restrictionProfile[profile[o].restrictionProfile].toEpoch) {
                if (accessProfile[profile[o].accessProfile].multi==0) { return pow(2,port); }
                else { return accessProfile[profile[o].accessProfile].ports; } } } } } } } }
  return 0; }

void doMessage(uint8_t command, uint16_t rfid, uint8_t port) {
  message[dataLength.message].command=command;
  message[dataLength.message].epoch=rtc.now().unixtime();
  message[dataLength.message].rfid=rfid;
  message[dataLength.message].port=port;
  dataLength.message++; }

void initData() { LittleFS.begin(true); }

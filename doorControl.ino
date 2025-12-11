#include "Log.h"
#include "Eth.h"
#include "NTP.h"
#include "Data.h"
#include "TELNET.h"
#include "MCP.h"
#include "RS422.h"
#include "Flash.h"
#include "CLI.h"

void setup() {
  initEth();
  initRTC();
  initData();
  initTELNET();
  initMCP();
  initRS422();
  readFlash();
  dataRead(); }

void loop() { rs422Worker(); cliWorker(); ethWorker(); telnetWorker(); ntpWorker(); mcpWorker(); }

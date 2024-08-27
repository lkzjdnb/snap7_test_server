/*=============================================================================|
|  PROJECT SNAP7                                                         1.4.0 |
|==============================================================================|
|  Copyright (C) 2013, 2014 Davide Nardella                                    |
|  All rights reserved.                                                        |
|==============================================================================|
|  SNAP7 is free software: you can redistribute it and/or modify               |
|  it under the terms of the Lesser GNU General Public License as published by |
|  the Free Software Foundation, either version 3 of the License, or           |
|  (at your option) any later version.                                         |
|                                                                              |
|  It means that you can distribute your commercial software linked with       |
|  SNAP7 without the requirement to distribute the source code of your         |
|  application and without the requirement that your application be itself     |
|  distributed under LGPL.                                                     |
|                                                                              |
|  SNAP7 is distributed in the hope that it will be useful,                    |
|  but WITHOUT ANY WARRANTY; without even the implied warranty of              |
|  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               |
|  Lesser GNU General Public License for more details.                         |
|                                                                              |
|  You should have received a copy of the GNU General Public License and a     |
|  copy of Lesser GNU General Public License along with Snap7.                 |
|  If not, see  http://www.gnu.org/licenses/                                   |
|==============================================================================|
|                                                                              |
|  New Server Example (1.1.0)                                                  |
|  Here we set ReadEventCallback to get in advance which area the client needs |
|  then we fill this area with a counter.                                      |
|  The purpose is to show how to modify an area before it be trasferred to the |
|  client                                                                      |
|                                                                              |
|=============================================================================*/
#include "snap7.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

TS7Server *Server;

// Here we use the callback to show the log, this is not the best choice since
// the callback is synchronous with the client access, i.e. the server cannot
// handle futher request from that client until the callback is complete.
// The right choice is to use the log queue via the method PickEvent.

#define NB_DB 256
#define DB_SIZE 256

char* regs[NB_DB];

void S7API EventCallBack(void *usrPtr, PSrvEvent PEvent, int Size) {
  // print the event
  printf("%s | ", SrvEventText(PEvent).c_str());
  int sum = 0;
  for(int i = 0; i < PEvent->EvtParam4; i++){
    sum |= (regs[PEvent->EvtParam2][PEvent->EvtParam3 + i] & 0xFF) << ((PEvent->EvtParam4 - 1 - i) * 8);
    printf("%x ", regs[PEvent->EvtParam2][PEvent->EvtParam3 + i]);
  }
  // for(int i = 0; i < 15; i++){
  //   for(int j = 0; j < 15; j++)
  //     printf("%02d ", regs[i][j]);
  //   printf("\n");
  // }
  printf("%d\n", sum);
  fflush(stdout);
};

// The read event callback is called multiple times in presence of multiread var
// request
void S7API ReadEventCallBack(void *usrPtr, PSrvEvent PEvent, int Size) {
  // print the read event
  printf("%s\n", SrvEventText(PEvent).c_str());
  fflush(stdout);
};

void S7API WriteEventCallBack(void *usrPtr, PSrvEvent PEvent, int Size) {
  // print the read event
  printf("%s\n", SrvEventText(PEvent).c_str());
  fflush(stdout);
};

int main(int argc, char *argv[]) {
  int Error;
  Server = new TS7Server;

  // Share some resources with our virtual PLC
  for(int i = 0; i < NB_DB; i++){
    regs[i] = (char*)malloc(DB_SIZE * sizeof(char));
    // regs[i][12] = i;
    Server->RegisterArea(srvAreaDB, i, regs[i], DB_SIZE);
  }

  // We mask the read event to avoid the double trigger for the same event
  // Server->SetEventsMask(~evcDataRead);
  Server->SetEventsCallback(EventCallBack, NULL);
  // Set the Read Callback
  Server->SetReadEventsCallback(ReadEventCallBack, NULL);
  // Start the server onto the default adapter.
  // To select an adapter we have to use Server->StartTo("192.168.x.y").
  // Start() is the same of StartTo("0.0.0.0");
  Error = Server->Start();
  if (!Error == 0) 
    printf("%s\n", SrvErrorText(Error).c_str());

  for (;;);

  Server->Stop(); // <- not strictly needed, every server is stopped on deletion
                  //    and every client is gracefully disconnected.
  delete Server;
}

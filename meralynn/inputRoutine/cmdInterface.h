#ifndef CMDINTERFACE_H
#define CMDINTERFACE_H

const int arrSize = 12;
static int cnt    = 0;
char inputArray[arrSize];

void printBuffer();
void assignCommand();
void store();
void retreive();
void erase();
void files();
void freespace();
void run ();
void list();
void suspend();
void resume();
void kill();


typedef struct {
  char commandName[arrSize];
  void *func;
  //  int commandId;
  //  String stub;
} commandType;


static commandType command[] = {
  {"store", &store},
  {"retreive", &retreive},
  {"erase", &erase},
  {"files", &files},
  {"freespace", &freespace},
  {"run", &run},
  {"list", &list},
  {"suspend", &suspend},
  {"resume", &resume},
  {"kill", &kill}
};
#endif

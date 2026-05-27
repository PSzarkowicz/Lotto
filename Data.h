
#ifndef LOTTO_DATA_H
#define LOTTO_DATA_H

#include "defs.h"
#include <time.h>
#include <cstdlib>
#include <cstring>
#include <stdio.h>

int ReadData(Player*** pAllPlayers, const char* sFileName);
void FreeMem(Player*** pTab, int nPlayersNo);

#endif //LOTTO_DATA_H
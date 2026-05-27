
#include "Print.h"

#include <chrono>
#include <stdio.h>

void PrintSex(FILE* fout, Player* p);
void PrintDate(FILE* fout, Player* p);
void PrintHour(FILE* fout, Player* p);
void PrintBets(FILE* fout, int** pBets, int nBetsCount);

void PrintResults(int* pNums49, Player** pPlayers, int nPlayersNo) {
    FILE* fout = fopen("wyniki.txt", "w");
    if (!fout) return;

    for(int i = 0; i < nPlayersNo; ++i) {
        fprintf(fout, "* %s %s   ",pPlayers[i]->Sname, pPlayers[i]->Fname);
        PrintSex(fout, pPlayers[i]);
        PrintHour(fout, pPlayers[i]);
        PrintDate(fout, pPlayers[i]);
        fprintf(fout,"%.11s",pPlayers[i]->Pesel);
        fprintf( fout, "\n%.4s %.2s %.2s %.3s %.2s %.2s %.8s %.16s  ", pPlayers[i]->Acc.accSwift.Institution,
            pPlayers[i]->Acc.accSwift.Country,pPlayers[i]->Acc.accSwift.Location,
            pPlayers[i]->Acc.accSwift.Branch,pPlayers[i]->Acc.accIban.Country,
            pPlayers[i]->Acc.accIban.Control,pPlayers[i]->Acc.accIban.Bank,
            pPlayers[i]->Acc.accIban.AccNum );
        fprintf(fout, "\nZaklady:\n");
        PrintBets(fout, pPlayers[i]->pBets, pPlayers[i]->BetsMax);
        fprintf(fout, "---------------------------\n");
    }

    fprintf(fout, "\n ------ STATYSTYKA ( liczba -> ile raz wystepuje ) -------\n");
    for(int i = 0; i < LOTTO_NUMS; i++) {
            fprintf( fout, "%d -> %d | ", i+1, pNums49[i+1]);
            if ((i+1)%10 == 0) fprintf(fout, "\n");
    }

    fclose(fout);
}

void PrintSex(FILE* fout, Player* p) {
    switch (p->Sex) {
        case MALE: fprintf(fout, " *Mezczyzna*   "); break;
        case FEMALE: fprintf(fout, " *Kobieta*   "); break;
    }
}

void PrintDate(FILE* fout, Player* p) {
    switch(p->BetsDate.weekDay) {
        case Mon: fprintf(fout, "Mon, "); break;
        case Tue: fprintf(fout, "Tue, "); break;
        case Wed: fprintf(fout, "Wed, "); break;
        case Thu: fprintf(fout, "Thu, "); break;
        case Fri: fprintf(fout, "Fri, "); break;
        case Sat: fprintf(fout, "Sat, "); break;
        case Sun: fprintf(fout, "Sun, "); break;
    }
    fprintf(fout, "%02d/", p->BetsDate.nDay);
    switch(p->BetsDate.nMonth) {
        case Jan: fprintf(fout, "Jan/"); break;
        case Feb: fprintf(fout, "Feb/"); break;
        case Mar: fprintf(fout, "Mar/"); break;
        case Apr: fprintf(fout, "Apr/"); break;
        case May: fprintf(fout, "May/"); break;
        case Jun: fprintf(fout, "Jun/"); break;
        case Jul: fprintf(fout, "Jul/"); break;
        case Aug: fprintf(fout, "Aug/"); break;
        case Sep: fprintf(fout, "Sep/"); break;
        case Oct: fprintf(fout, "Oct/"); break;
        case Nov: fprintf(fout, "Nov/"); break;
        case Dec: fprintf(fout, "Dec/"); break;
    }
    fprintf(fout, "%d ", p->BetsDate.nYear);
}

void PrintHour(FILE* fout, Player* p) {
    fprintf(fout, " %02d:%02d ", p->BetsHour.nHour, p->BetsHour.nMin);
}

void PrintBets(FILE* fout, int** pBets, int nBetsCount) {
    for (int i = 0; i < nBetsCount; i++ ) {
        int* p = *pBets++;
        if (p == NULL) continue;
        for (int j = 0; j < BET_LEN; j++ ) {
            fprintf(fout, "%d\t ", *p++);
        }
        fprintf(fout, "\n");
    }
}
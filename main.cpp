/*=============================================
Obliczanie statystyk losowanych liczb z zakladow Lotto
W pliku we sa po gwiazdce nazwisko i imie gracza litera plci
potem konto bankowe (SWIFT), potem godizna i data zawarcia
zakladu i numer pesel
Od nowej linii sa zaklady


*Banaszak   Lolita  f    BPHKPLPKXXX   PL61123400110011000012300678  12:34   16/12/2016 01234567890
 1 10 23 30 38  47
 9 24 26 33 40  48
 8 14 26 33 47  41
 7 12 15 16 41  48
 6 14 20 27 28  40
=============================================*/

#include "defs.h"  // // definicje typow i stalych
#include "Print.h"    // interfejs modulu drukowania
#include "Data.h"     // interfejs modulu wczytywania danych
#include <memory.h>
#include <stdio.h>
#include <malloc/malloc.h>

void HeapCheck();
void CalcStat(int* pNums49, Player** pPlayers, int nPlayersNo);
void Sort(Player** pPlayers, int nPlayersNo); // sortuje po nazwisko + imie (jesli nazwisko ident)

int main( int argc, char* argv[] )
{
    // Parametrem programu jest plik tekstowy z zakladami
    if (argc != 2) {
        printf("Usage: %s  ERROR\n",argv[0]);
        return 1;
    }
    // definicja dynamicznej tablicy typu Player** (tabl wsk do graczy)   - NULL
    Player** pPlayers = NULL;
    // zdefiniowac zmienna dla pamiatania ilosci wczytanych danych (ilosci gracczy) 0
    int nPlayersNum = 0;
    nPlayersNum = ReadData(&pPlayers, argv[1]);
  //  if( ilosc_wczytanych danych == 0  ) (funkcja czytajaca zwraca ilosc przeczytanyh
    if (nPlayersNum == 0) {
        printf("No players found\n");
        return 2;
    }
    HeapCheck();
   //         nawet jesli podczas czytania wystapil blad)


    // definicja tablicy dla pamiatania statystyk dla wyboru 6 z 49                 (tak jak w Lotto)
    int pNums49[LOTTO_NUMS+1] = { 0 };
    //przetwarzanie danych
    // obliczenie statystyk
    CalcStat(pNums49, pPlayers, nPlayersNum);
    // posortowac ze wzgledu na nazwisko i imie (przestawiac wskazniki w tablicy)
    Sort(pPlayers, nPlayersNum);
    // wydrukowac dane do pliku - wyniki.txt
    PrintResults(pNums49, pPlayers, nPlayersNum);

    //zwolnic pamiec
    FreeMem(&pPlayers, nPlayersNum);
    ///HeapCheck(); --   mozna dodac ta funkcje - mozna skopiowac z helpow
    HeapCheck();
    return 0;
}


void CalcStat( int* pNumsAll,  Player** pPlayers, int nPlayersNo )
{
    Player** pCurrPlayer = pPlayers;
    for (int i = 0; i < nPlayersNo; i++ ) {
        Player* g = *pCurrPlayer++;

        if (g != NULL && g->pBets != NULL) {
            int** pAllBets = (int**)g->pBets;
            for (int j = 0; j < g->BetsMax; j++ ) {
                int* p = pAllBets[j];

                if (p != NULL) {
                    for (int k = 0; k < BET_LEN; k++ ) {
                        pNumsAll[*p++]++;
                    }
                }
            }
        }
    }
}
void Sort(Player** pPlayers, int nPlayersNo) {
    if (pPlayers == NULL || nPlayersNo < 2) return;


    for( int i = 0; i < nPlayersNo - 1; i++ ) {

        int iTemp = i;

        Player* min = pPlayers[i];

        for( int j = i + 1; j < nPlayersNo; j++ ) {

            int cmpSurname = strcmp(pPlayers[j]->Sname, min->Sname);

            if ( cmpSurname < 0 || (cmpSurname == 0 && strcmp(pPlayers[j]->Fname, min->Fname) < 0) ) {

                min = pPlayers[j];
                iTemp = j;
            }
        }

        pPlayers[iTemp] = pPlayers[i];
        pPlayers[i] = min;
    }
}

void HeapCheck(){

    malloc_zone_t *zone = malloc_default_zone();

    if (!malloc_zone_check(zone)) {
        printf("heap Error!\n");
    } else {
        printf("heap is fine!\n");
    }

    return;
}
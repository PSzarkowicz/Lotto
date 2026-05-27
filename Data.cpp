
#include "Data.h"

#include <chrono>

#include "defs.h"

int AllocPlayerAddMem(Player*** pTab, int nCurrSize);  //realok dla graczy
int AllocBetAddMem(int*** pBets, int nCurrSize);  // realokacja dla zakladow
int SetSex(Player* p, char c);
int SetDate(Player* p, int d, int m, int y);
int SetHour(Player* p, int hh, int mm);


int AllocPlayerAddMem( Player*** pTab, int nCurrSize )
{
    // uzywa funkcji "realloc()" i memset()
    // przy czym jesli sie nie powiodlo
    // to nie mozna stracic do tej pory czytanych danych i program ma dzialac dalej
    // z tym ze zwraca wtedy wartosc zero
    // w przypadku powodzenia realokacji zwraca o ile realokowano!!!
    Player** oldpTab = *pTab;
    if( (*pTab = (Player**)realloc( *pTab, (nCurrSize + ALLOCPLAYER) * sizeof(Player*)  ) ) == NULL ) {
        *pTab = oldpTab;
        return 0;
    }
    memset( *pTab+nCurrSize, 0, ALLOCPLAYER*sizeof(Player*)  );

    return ALLOCPLAYER;
}

int AllocBetAddMem( int*** pBets, int nCurrSize )
{
    // uzywa funkcji "realloc()" i memset()
    // przy czym jesli sie nie powiodlo
    // to nie mozna stracic do tej pory czytanych danych i program ma dzialac dalej
    // z tym ze zwraca wtedy wartosc zero
    // w przypadku powodzenia realokacji zwraca o ile realokowano!!!
    int** oldpBets = *pBets;
    if( (*pBets = (int**)realloc( *pBets, (nCurrSize + ALLOCBET) * sizeof(int*)  ) ) == NULL ) {
        *pBets = oldpBets;
        return 0;
    }
    memset( *pBets+nCurrSize, 0, ALLOCBET*sizeof(int*)  );

    return ALLOCBET;

}



int ReadData( Player*** pAllPlayers, const char* sFileName )
{
    // otwiera plik do odczytu z badaniem bledu otwarcie - gdy blad to funkcja\
    // i zwraca zero
    FILE* fin = fopen(sFileName,"r");
    if (!fin) return 0;

    // zdefiniowac zmienna liczaca ilosc wczytanych danych (poczatkowo 0) oraz
    int data_count = 0;
    // zmienna aktualny maxym rozmiar tablicy pAllPalyers (poczatkowo 0 )
    int max_players = 0;
    // zrobic kopie  parametru *pAllPalyers  typu Player**  o nazwiee np pDraws
    Player** pDraws = *pAllPlayers;

    while( !feof( fin ) )
    {
        // jesli trzeba to realokowac  (ilosc wczytanych danych == maxym rozmiar)
        // dodac do zmiennej maxym rozmiar wynik funkcji realokujacej (3- ilosc o ktora realokujemy)
        if ( data_count == max_players ) {
           max_players+= AllocPlayerAddMem( pAllPlayers, max_players );
           pDraws = *pAllPlayers;
        }
        // if( (ilosc wczytanych danych == maxym rozmiar)  to koniec (przerw petli bo realloc sie nie powiodlo)
        if ( data_count == max_players ){
            fclose( fin );
            return data_count;
        }
        // wczytac kolejnego gracza (linia zawsze zaczyna sie od znaku '*')
        (*pAllPlayers)[data_count] = (Player*)calloc(1, sizeof(Player));
        if ((*pAllPlayers)[data_count] == NULL) {
            fclose(fin);
            return data_count;
        }
        // wczytywac jedna instrukcja fscanf wszystkie dane opisujace gracza
        // przy czym dane dot. plci i daty i czasu do zmiennych pomocniczych
        // ktore beda parametrami funkcji SetSex() i SetDate() i SetHour()
        char sex_char;
        int d; int m; int y;
        int hh; int mm;
        char star;
        if (fscanf(fin, " %c %s %s %c %4c%2c%2c%3c %2c%2c%8c%16c %d:%d %d/%d/%d %11c",
           &star, (*pAllPlayers)[data_count]->Sname,
           (*pAllPlayers)[data_count]->Fname, &sex_char,
           (*pAllPlayers)[data_count]->Acc.accSwift.Institution,
           (*pAllPlayers)[data_count]->Acc.accSwift.Country,
           (*pAllPlayers)[data_count]->Acc.accSwift.Location,
           (*pAllPlayers)[data_count]->Acc.accSwift.Branch,
           (*pAllPlayers)[data_count]->Acc.accIban.Country,
           (*pAllPlayers)[data_count]->Acc.accIban.Control,
           (*pAllPlayers)[data_count]->Acc.accIban.Bank,
           (*pAllPlayers)[data_count]->Acc.accIban.AccNum,
           &hh, &mm, &d, &m, &y,
           (*pAllPlayers)[data_count]->Pesel) != 18) {
            printf("ERROR ReadData");
            break;
        }
       // (obliczyc brakujace dane jak np dzien tygodnia - uzyc TYLKO odpowiednich funkcji bibliotecznych)
      // jesli nie powiodlo sie ustawic plci lub daty, czau to wyjscie z funkcji z
      // odpowiednim komunikatem z dotychczas
      // wczytanymi danymi o przerwac
      //
      //// Wywołujemy funkcje ustawiające, przekazując wczytane zmienne pomocnicze
        if (!SetSex( (*pAllPlayers)[data_count], sex_char ) || !SetDate( (*pAllPlayers)[data_count], d, m, y )
            || !SetHour ((*pAllPlayers)[data_count], hh, mm) ) {
            printf("ERROR: Niepoprawne dane dla gracza %s %s!\n",
                (*pAllPlayers)[data_count]->Sname, (*pAllPlayers)[data_count]->Fname);
            fclose(fin);
            return data_count;
        }
     // czytanie zakladow  - realokacja co 2 zaklady
        int bets_loaded = 0;
        while (1) {
            char next_chr;
            while (fscanf(fin, "%c", &next_chr) == 1 && (next_chr == ' ' || next_chr == '\t' || next_chr == '\n'));
            if (feof(fin)) break;  // jak sie plik konzcy

            if (next_chr == '*') { //tzn ze bedzie nazwisko imie
                ungetc(next_chr,fin);
                break;
            }

            ungetc(next_chr,fin); // zwracam pierwszy bet

            if (bets_loaded == (*pAllPlayers)[data_count]->BetsMax) {
               (*pAllPlayers)[data_count]->BetsMax += AllocBetAddMem( &(*pAllPlayers)[data_count]->pBets, bets_loaded);
            }
            int* single_bet = (int*)calloc(1, BET_LEN * sizeof(int));

            for (int i = 0; i < BET_LEN; i++) fscanf(fin, "%d", &single_bet[i]); //pobieram caly bet

            (*pAllPlayers)[data_count]->pBets[bets_loaded] = single_bet; // wklejam bet w tablice
            bets_loaded++;

        }

        // Zwiększamy licznik całkowity poprawnie wczytanych graczy
        data_count++;
        }


    // przywrocic wskaznik *pAllPalyers;
    *pAllPlayers = pDraws;
    //zamknac plik
    fclose(fin);
  //  return ilosc_wczytanych_danych;
    return data_count;
}

int SetSex( Player* p, char c )
{
    //jesli literka c NIE jest f F m lub M to zwaraca 0 a jesli OK to 1
    // uzywa nazw zdefiniowanych w typie wyliczeniowym  (switch)
    //  Do pola struktury wstawic odpowiednia wart typu wyliczeniowgo
    switch( c ) {
        case 'm':
        case 'M': p->Sex = MALE; return 1;
        case 'f':
        case 'F': p->Sex = FEMALE; return 1;
        default: return 0;
    }

    }
//---------------------------------------------------
int SetDate( Player* p, int d, int m, int y )
{
    // zprawdza czy d jest z przewdzialu od 1 do 31
    // m od 1 do 12
    // y wieksze od 1990
    if (d < 1 || d > 31) return 0;   // Dzień musi być między 1 a 31
    if (m < 1 || m > 12) return 0;   // Miesiąc między 1 a 12
    if (y <= 1990) return 0;          // Rok większy niż 1990
    // w przeciwnym przyp zwraca 0
    // ma obliczyc nazwe dnia tygodnia  (time.h) (switch)  - fukcje biblioteczne WYLACZNIE
    p->BetsDate.nYear = y;
    p->BetsDate.nDay = d;
    p->BetsDate.nMonth = (MonthNames)(m - 1);



    tm timeinfo = {0};
    timeinfo.tm_mday = d;
    timeinfo.tm_mon  = m - 1;
    timeinfo.tm_year = y - 1900;
    mktime(&timeinfo);

    switch ( timeinfo.tm_wday ) {
        case 0: p->BetsDate.weekDay = Sun; break;
        case 1: p->BetsDate.weekDay = Mon; break;
        case 2: p->BetsDate.weekDay = Tue; break;
        case 3: p->BetsDate.weekDay = Wed; break;
        case 4: p->BetsDate.weekDay = Thu; break;
        case 5: p->BetsDate.weekDay = Fri; break;
        case 6: p->BetsDate.weekDay = Sat; break;
    }

    // uzywa nazw zdefiniowanych w typach wyliczeniowych
    return 1;
}

//---------------------------------------------------
int SetHour( Player* p, int hh, int mm )
{
    // zprawdza czy hh jest z przewdzialu od 0 do 23
    // mm od 0 do 59
    if( hh < 0 || hh > 23 ) return 0;
    if( mm < 0 || mm > 59 ) return 0;
    p->BetsHour.nHour = hh;
    p->BetsHour.nMin = mm;

    return 1;
}

void FreeMem( Player*** pTab, int nPlayersNo ) {

    for (int i = 0; i < nPlayersNo; ++i) {
        Player* p = (*pTab)[i];

        if (p != NULL) {
            if (p->pBets != NULL) {
                for (int j = 0; j < p->BetsMax; ++j) {
                    if (p->pBets[j] != NULL) {
                        free(p->pBets[j]);
                    }
                }
                free(p->pBets);
            }
            free(p);
        }
    }
    free(*pTab);
    *pTab = NULL;
}


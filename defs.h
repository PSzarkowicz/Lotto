#ifndef DEFS_H
#define DEFS_H

typedef enum { MALE, FEMALE }    SexType;
typedef enum { Mon, Tue, Wed, Thu, Fri, Sat, Sun }    DayNames;   // three letter obriviation
typedef enum {Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec}  MonthNames; // three letter obriviation

//definicje WSZYSTKICH stalych!

//SWIFT
#define SWIFT_LEN 11
#define SWIFT_INST_LEN 4
#define SWIFT_COUNTRY_LEN 2
#define SWIFT_LOC_LEN 2
#define SWIFT_BRANCH_LEN 3

//IBAN
#define IBAN_LEN 28
#define IBAN_COUNTRY_LEN 2
#define IBAN_CTRL_LEN 2
#define IBAN_BNK_LEN 8
#define IBAN_ACC_LEN 16

#define MAXBETS      6   // ilosc zakladow
#define BET_LEN      6 // ilosc liczb w jednym zakladzie
#define LOTTO_NUMS 49
#define MAX_LANE 10

#define ALLOCPLAYER  3   // liczba realokowanych graczy za jednym razem ( realloc() )
#define ALLOCBET     2   // liczba realokowanych zakladow dla danego gracza

#define MAXNAME      30  //dlugosc nazwiska -30 znakow - string, Imie analogicznie
#define PESEL_LEN    11

//-----definicje typow strukturalnych dla numeru konta bankowego (SWIFT oraz IBAN) 

//SWIFT (lacznie 11 znakow - wszystkie pola tablicami znakowymi - NIE STRINGI!!!):
//kod instytucji (zwykle banku); (4 zxnaki)
//kod kraju; (2 znaki)
//kod lokalizacji lub miejscowości; (2 znaki)
//kod oddziału; (3 znaki)
typedef struct {
    char Institution[SWIFT_INST_LEN];
    char Country[SWIFT_COUNTRY_LEN];
    char Location[SWIFT_LOC_LEN];
    char Branch[SWIFT_BRANCH_LEN];
}SWIFT;

//IBAN (wszystkie pola tablicami znakowymi - NIE STRINGI!!!):
//2-literowy kod kraju
//2 znaki (cyfry)  kontrolne IBAN
//8 znakow (cyfry) numer rozliczeniowy banku
//16 znaki (cyfry) account number
typedef struct {
    char Country[IBAN_COUNTRY_LEN];
    char Control[IBAN_CTRL_LEN];
    char Bank[IBAN_BNK_LEN];
    char AccNum[IBAN_ACC_LEN];
}IBAN;

//Typ strukturalny (Account) ktory ma dwa pola typow SWIFT i IBAN
typedef struct {
    SWIFT accSwift;
    IBAN accIban;
} Account;


//definicja dla typu strukturalnego  daty: DateType (4 pola: nDay, weekDay, nMonth, nYear)
typedef struct {
    int nDay;
    DayNames weekDay; // jest szasna ze musza byc typy wyliczeniowe tu
    MonthNames nMonth;
    int nYear;
}DateType;

//definicja typu struktur. dla czasu : TimeType (2 pola: nHour, nMin)
typedef struct {
    int nHour;
    int nMin;
}TimeType;

//definicja typu strukturalnego dla gracza: Player
//SName - nazwisko  (string)  //char SName[MAXNAME+1]
//FName - imie      (string)
//Pesel  - pesel  (tablica znakowa - nie string!!!!!)
//BetsHour - godzina zawarcia zakladow (TimeType )
//BetsDate - data zawarcia zakladow  (DateType)
//Sex    - plec (typu SexType)
//Acc    - (typu Account)
//BetsMax - rozmiar tabl pBets      (obliczane podczas wczytywania)
//pBets  - tablica wskaznikow do zakladow (dynamicznie alokowana podczas wczytywania - realloc())
         //Jesli na koncu nie ma zakladu to element tabl jest NULL
typedef struct {
    char Sname[MAXNAME+1];
    char Fname[MAXNAME+1];
    char Pesel[PESEL_LEN];
    TimeType BetsHour;
    DateType BetsDate;
    SexType Sex;
    Account Acc;
    int BetsMax;
    int** pBets;
}Player;



#endif //DEFS_H
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define ROZMIAR			15					//domyslnie plansza 15 X 15		
//okresla dlugosc boku planszy (plansza jest kwadratowa pod wzgledem liczby komorek w pionie i poziomie), 
//aby mozna bylo wyznaczyc dokladny srodek, liczba musi byc NIEPARZYSTA!
//aby mozna bylo zagrac komfortowo, nalezy ustawic plansze nie mniejsza niz 7 X 7
//ze wzgledu na wspolrzedne umieszczone na ramie planszy najlepiej grac na polu max 26 X 26

#define SPACJA			32					//ASCII spacji
#define BACKSPACE		8					//ASCII backspace
#define GORNY_LEWY_X	80					//tu zaczyna sie plansza (domyœlnie 80)
#define GORNY_LEWY_Y	2					//tu zaczyna sie plansza (domyœlnie 2)

#define LEGENDA_GORNY_LEWY_X	3			//tu zaczyna sie legenda (domyœlnie 3)	
#define LEGENDA_GORNY_LEWY_Y	2			//tu zaczyna sie legenda (domyœlnie 2)

#define LEGENDA_ROZMIAR_X		50			//legenda rozmiar x
#define LEGENDA_ROZMIAR_Y		30			//legenda rozmiar y

#define PUSTA_PLANSZA			0			//tryb wprowadzania nowych slow na plannsze
#define NIEPUSTA_PLANSZA		1			//tryb wprowadzania nowych slow na plannsze

#define POZIOM					0			//orientacja pozioma
#define PION					1			//orientacja pionowa

#define NIE						0			//nie
#define TAK						1			//tak

#define PKT_GORNY_LEWY_X		80			//tu zaczynaja sie wyswietlac punkty (domyœlnie 80) 	
#define PKT_GORNY_LEWY_Y		20			//tu zaczynaja sie wyswietlac punkty (domyœlnie 20)

#define PKT_ROZMIAR_X			17			//punkty rozmiar x
#define PKT_ROZMIAR_Y			5			//punkty rozmiar y

#define RANDOM_SEED				300			//liczba wygenerowanych ziaren

struct plansza
{
	short bonus_za_slowo;		// 1 lub 2 lub 3
	short bonus_za_litere;		// 1 lub 2 lub 3
	short srodek;				// czy pole jest srodkiem planszy, 0 - nie, 1 - tak
	char litera;				// litera czyli znak na polu
};

struct litery
{
	char		znak;			//jaka to litera
	short		punkty;			//punkty za litere
	short		ilosc;			//lacznie ile jest liter
	short		w_puli;			//ile jest jeszcze w puli

};

//kolor ostatniej litery
//numer indeksu jesli byla w puli
struct ostatnia_litera
{
	int  kolor;
	int  indeks;
};

struct punktacja
{
	int gracz;
	int komputer;
};

/*STRUKTURA SLUZACA DO STWORZENIA LISTY (DO KTOREJ LADUJE SIE POTEM SLOWNIK)*/
struct element {
	struct element *nastepny;		//wskaŸnik na kolejny element listy
	char *slowo;				//przechowywana wartoœæ
};


void druk_plansza(plansza p[][ROZMIAR]);
void druk_legenda(void);
void init(plansza p[][ROZMIAR], litery tab_litery[]);
litery* dobierz_litery(char litera, litery tab_litery[]);
void litery_start_los(int *licznik, litery tab_litery[], char litery_gracz[]);
void renderuj(void);
void wypisz_wsp(int x, int y);
int ruch_kursorem_slowo(litery tab_litery[], punktacja *pkt, ostatnia_litera ost_lit[], plansza p[][ROZMIAR], int *x, int *y, int ile_lit, char slowo_na_plansze[], int tryb);
char los_litera(int *licznik, litery tab_litery[]);
void jakie_litery_ma_gracz(char litery_gracz[]);
void druk_plansza_legenda_tlo(plansza p[][ROZMIAR]);
void wstaw_slowo(punktacja *pkt, plansza p[][ROZMIAR], litery tab_litery[], char litery_gracz[]);
int ile_liter_w_puli(litery tab_litery[]);
void dod_lit_na_p(litery tab_litery[], punktacja *pkt, ostatnia_litera ost_lit[], plansza p[][ROZMIAR], int tryb, char slowo_na_plansze[]);
int ile_w_slowie_liter(char slowo_na_plansze[]);
int czy_moz_dod_lit(plansza p[][ROZMIAR], ostatnia_litera ost_lit[], int ile_lit, char slowo_na_plansze[]);
void nie_mozna_umiescic_takiego_slowa(void);
int esc_wcisniety(int tryb);
void wyswietlanie_slowa_ruchy_kursorem(plansza p[][ROZMIAR], char litera, int x, int y);
int proba_wstawienia_na_plansze(plansza p[][ROZMIAR], ostatnia_litera ost_lit[], int ile_lit, char slowo_na_plansze[], int x, int y);
int zapamietana_orientacja(int orient);
void nie_mozna_umiescic_tego_slowa_tutaj(void);
char* wskaznik_litery_gracz(char* lit_gracz, int tryb);
void koniec_liter(punktacja pkt);
void wymiana(int *licznik, litery tab_litery[], char litery_gracz[]);
void oddaj_litery(char litera, litery tab_litery[]);
void druk_punkty(punktacja pkt);
int dodaj_punkty(litery tab_litery[], plansza p[][ROZMIAR], ostatnia_litera ost_lit[], int ile_lit, char slowo_na_plansze[], int x, int y);
short pobierz_punkty(char litera, litery tab_litery[]);
void generuj_losowe(void);
int get_random(int *go);
void error(void);
char* podaj_nazwe(void);
void wait_enter(void);
int szukaj(element *glowa, char *szukaj);
void dodaj(element *poprzedni, char *napis);
void slowo_nie_znalezione(void);
int sprawdz_w_slowniku(plansza p[][ROZMIAR], ostatnia_litera ost_lit[], int ile_lit, char slowo_na_plansze[], int x, int y);
element* head(element *glowa, int tryb);
int wczytywanie_gry_ostrzezenie(void);
int tryb_nauki_on_off(int tryb);
void tryb_nauki_slownik(punktacja pkt, char slowo_do_slownika[]);
void tryb_nauki_pkt(litery tab_litery[], plansza p[][ROZMIAR], ostatnia_litera ost_lit[], int ile_lit, char slowo_na_plansze[], int x, int y);
void usun_cala_liste(void);
void min_dwie_litery(void);

#endif
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "functions.h"
#include "conio2.h"

void wstaw_slowo(punktacja *pkt, plansza p[][ROZMIAR], litery tab_litery[], char litery_gracz[])
{
	struct ostatnia_litera ost_lit[ROZMIAR];
	int j = 0;	//indeks pomocniczy, uzywany do zarzadzania danymi w strukturze
	int flaga = 1;	//warunek petli
	int f4 = 0;		//obsluga F4
	int czy_uzyte[ROZMIAR] = {};	//czy litera z puli gracza juz uzyta
	char slowo[ROZMIAR + 1];
	char slowo_na_plansze[ROZMIAR + 1];		//ostateczne slowo do umieszczenia
	char slowo_do_slownika[ROZMIAR + 1];	//w tej zmiennej wysylane jest slowo do slownika (do sprawdzenia)
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 27);
	cputs("Wprowadz slowo: ");
	while (flaga)
	{
		int zn = getch();			//pobieranie znaku
		if (zn == 0x30) continue;		//nie mozna uzywac '0', ten znak jest uzywany do oznaczania braku kafelka
		if (zn == 0x00)
		{
			if ((f4 == 1) && (getch() == 0x3e))				f4 = 0;
			else if ((f4 == 0) && (getch() == 0x3e))		f4 = 1;
		}
		else if (zn == 0x08)			//backspace
		{
			if (j > 0)
			{
				if (ost_lit[j - 1].kolor == GREEN)
				{
					ost_lit[j - 1].kolor = 0;
					czy_uzyte[ost_lit[j - 1].indeks] = 0;
					ost_lit[j - 1].indeks = -1;
				}
				else if (ost_lit[j - 1].kolor == RED)
				{
					ost_lit[j - 1].kolor = 0;
				}
				slowo[j - 1] = SPACJA;
				putch(BACKSPACE);
				putch(SPACJA);
				putch(BACKSPACE);
				j = j - 1;
			}
			if (tryb_nauki_on_off(2) == 1)
			{
				int pozycja_x = wherex();
				int pozycja_y = wherey(); 
				slowo[j] = '\0';
				strcpy(slowo_do_slownika, slowo);
				tryb_nauki_slownik(*pkt, slowo_do_slownika);
				gotoxy(pozycja_x, pozycja_y);
			}
		}
		else if ((j < ROZMIAR) && (((zn >= 97) && (zn <= 122)) || (zn == 35)))
		{
			/*WPROWADZANIE LITER I PUSTYCH KAFELKOW*/
			if (f4 == 1)	//obsluga F4
			{
				textbackground(RED);
				putch(zn);
				slowo[j] = zn;
				ost_lit[j].kolor = RED;
				ost_lit[j].indeks = -1;
				j++;
				textbackground(BLUE);
			}
			else
			{
				/*F4 WYLACZONE*/
				for (int i = 0; i < ROZMIAR; i++)
				{
					if ((zn == litery_gracz[i]) && (czy_uzyte[i] == 0))
					{
						textbackground(GREEN);
						putch(zn);
						slowo[j] = zn;
						czy_uzyte[i] = 1;
						ost_lit[j].kolor = GREEN;
						ost_lit[j].indeks = i;
						j++;
						textbackground(BLUE);
						break;
					}
					else if (i == (ROZMIAR - 1))
					{
						textbackground(RED);
						putch(zn);
						slowo[j] = zn;
						ost_lit[j].kolor = RED;
						ost_lit[j].indeks = -1;
						j++;
						textbackground(BLUE);
					}
				}
			}
			if (tryb_nauki_on_off(2) == 1)
			{
				int pozycja_x = wherex();
				int pozycja_y = wherey();
				slowo[j] = '\0';
				strcpy(slowo_do_slownika, slowo);
				tryb_nauki_slownik(*pkt, slowo_do_slownika);
				gotoxy(pozycja_x, pozycja_y);
			}
		}
		else if (zn == 0x0d) //enter
		{
			slowo[j] = '\0';
			int ile_pustych = 0;
			strcpy(slowo_na_plansze, slowo);
			for (int i = 0; i < ROZMIAR; i++)
			{
				if (slowo_na_plansze[i] == '#')
				{
					/*PUSTE KAFELKI*/
					druk_legenda();
					gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 27 + ile_pustych);
					cputs("Jaka litere ma oznaczac # ?  Wybor: ");
					while (1)
					{
						zn = getch();
						if ((zn >= 97) && (zn <= 122)) break;
					}
					putch(zn);
					slowo_na_plansze[i] = zn;
					ile_pustych++;
				}
			}
			druk_legenda();
			flaga = 0;
		}
	}
	if (j < 2)  min_dwie_litery();
	else
	{
		if ((p[ROZMIAR / 2][ROZMIAR / 2].srodek == 1) && (p[ROZMIAR / 2][ROZMIAR / 2].litera == SPACJA))
		{
			//gdy srodek pusty
			int ile_czerwonych = 0;
			for (int i = 0; i < ile_w_slowie_liter(slowo_na_plansze); i++)
			{
				if (ost_lit[i].kolor == RED) ile_czerwonych++;
			}
			if (ile_czerwonych > 0) nie_mozna_umiescic_takiego_slowa();
			//sprawdzenie oraz
			//wstawienia (jesli tylko slowo spelnia warunki)
			else dod_lit_na_p(tab_litery, pkt, ost_lit, p, PUSTA_PLANSZA, slowo_na_plansze);
		}
		else
		{
			/*NIEPUSTA PLANSZA*/
			/*SPRAWDZENIE WARUNKOW ORAZ EWENTUALNE WSTAWIENIE SLOWA*/
			int czy_mozna_dodac_litery = 0;
			czy_mozna_dodac_litery = czy_moz_dod_lit(p, ost_lit, ile_w_slowie_liter(slowo_na_plansze), slowo_na_plansze);
			if (czy_mozna_dodac_litery == TAK)
			{
				dod_lit_na_p(tab_litery, pkt, ost_lit, p, NIEPUSTA_PLANSZA, slowo_na_plansze);
			}
			else if (czy_mozna_dodac_litery == NIE)
			{
				nie_mozna_umiescic_takiego_slowa();
			}
		}
	}
}

int czy_moz_dod_lit(plansza p[][ROZMIAR], ostatnia_litera ost_lit[], int ile_lit, char slowo_na_plansze[])
{
	/*SZCZEGOLOWA ANALIZA WSZYSTKICH MOZLIWOSCI*/
	esc_wcisniety(2);
	char czerwone_litery[ROZMIAR];
	int ile_czerwonych = 0;
	for (int i = 0, j = 0; i < ile_lit; i++)
	{
		if (ost_lit[i].kolor == RED)
		{
			czerwone_litery[j] = slowo_na_plansze[i];
			j++;
			ile_czerwonych++;
		}
	}
	int czerwone_znalezione[ROZMIAR] = {};
	for (int x = 0; x < ile_czerwonych; x++)
	{
		for (int i = 0; i < ROZMIAR; i++)
		{
			for (int j = 0; j < ROZMIAR; j++)
			{
				if (p[i][j].litera == czerwone_litery[x])
				{
					czerwone_znalezione[x] = TAK;
					break;
				}
			}
		}
	}
	for (int x = 0; x < ile_czerwonych; x++)
	{
		if (czerwone_znalezione[x] == 0)	return NIE;
		else if (czerwone_znalezione[x] == TAK) 	continue;
	}
	/*AZ DO TEGO MOMENTU FUNKCJA SPRAWDZA CZY NA PLANSZY ZNAJDUJA SIE*/
	/*LITERY KTORE WYBRAL GRACZ, A KTORYCH NIE MIAL (CZERWONE LITERY)*/

	/*FUNCJA (OD TEGO MIEJSCA) SPRAWDZA CZY W JAKIKOWLIEK SPOSOB*/
	/*MOZNA ULOZYC PODANE LITERY NA PLANSZY*/
	int ile_wypelnionych_pol = 0;
	for (int i = 0; i < ROZMIAR; i++)
	{
		for (int j = 0; j < ROZMIAR; j++)
		{
			if (p[i][j].litera != SPACJA) ile_wypelnionych_pol++;
		}
	}
	if (((ROZMIAR*ROZMIAR) - ile_wypelnionych_pol) >= ile_lit)
	{
		int zajetych = 0;
		if (ile_czerwonych == 0)
		{/*JESLI GRACZ KORZYSTA TYLKO Z POSIADANYCH LITER*/			
		 /*POZIOM*/
			zajetych = 0;
			for (int i = 0; i < ROZMIAR; i++)
			{
				for (int j = 0; j < (ROZMIAR - ile_lit + 1); j++)
				{
					zajetych = 0;
					for (int l = 0; l < ile_lit; l++)
					{
						if (p[i][j + l].litera != SPACJA) zajetych++;
						if (p[i][j + l].litera != SPACJA)
						{
							if (p[i][j + l].litera != slowo_na_plansze[l]) break;
						}
						if ((zajetych > 0) && (zajetych < ile_lit) && (l == (ile_lit - 1))) return TAK;
					}
				}
			}

			/*PION*/
			zajetych = 0;
			for (int i = 0; i < (ROZMIAR - ile_lit + 1); i++)
			{
				for (int j = 0; j < ROZMIAR; j++)
				{
					zajetych = 0;
					for (int l = 0; l < ile_lit; l++)
					{
						if (p[i + l][j].litera != SPACJA) zajetych++;
						if (p[i + l][j].litera != SPACJA)
						{
							if (p[i + l][j].litera != slowo_na_plansze[l]) break;
						}
						if ((zajetych > 0) && (zajetych < ile_lit) && (l == (ile_lit - 1))) return TAK;
					}
				}
			}

			return NIE;
		}
		else /*JESLI GRACZ KORZYSTA NIE TYLKO Z SAMYCH POSIADANYCH LITER*/
		{
			/*POZIOM*/
			zajetych = 0;
			for (int i = 0; i < ROZMIAR; i++)
			{
				for (int j = 0; j < (ROZMIAR - ile_lit + 1); j++)
				{
					zajetych = 0;
					for (int l = 0; l < ile_lit; l++)
					{
						if (p[i][j + l].litera != SPACJA) zajetych++;
						if (ost_lit[l].kolor == RED)
						{
							if (slowo_na_plansze[l] != p[i][j + l].litera) break;
						}
						else if (ost_lit[l].kolor == GREEN)
						{
							if ((p[i][j + l].litera != SPACJA) && (p[i][j + l].litera != slowo_na_plansze[l])) break;
						}
						if ((zajetych > 0) && (zajetych < ile_lit) && (l == (ile_lit - 1))) return TAK;
					}
				}
			}
			/*PION*/
			zajetych = 0;
			for (int i = 0; i < (ROZMIAR - ile_lit + 1); i++)
			{
				for (int j = 0; j < ROZMIAR; j++)
				{
					zajetych = 0;
					for (int l = 0; l < ile_lit; l++)
					{
						if (p[i + l][j].litera != SPACJA) zajetych++;
						if (ost_lit[l].kolor == RED)
						{
							if (slowo_na_plansze[l] != p[i + l][j].litera) break;
						}
						else if (ost_lit[l].kolor == GREEN)
						{
							if ((p[i + l][j].litera != SPACJA) && (p[i + l][j].litera != slowo_na_plansze[l])) break;
						}
						if ((zajetych > 0) && (zajetych < ile_lit) && (l == (ile_lit - 1))) return TAK;
					}
				}
			}
			return NIE;
		}
	}
	else
	{
		return NIE;
	}
}

void dod_lit_na_p(litery tab_litery[], punktacja *pkt, ostatnia_litera ost_lit[], plansza p[][ROZMIAR], int tryb, char slowo_na_plansze[])
{
	/*WYBRANIE MIEJSCA NA PLANSZY I WSTAWIANIE*/
	int ile = ile_w_slowie_liter(slowo_na_plansze);
	int x = GORNY_LEWY_X + 1;
	int y = GORNY_LEWY_Y + 1;
	gotoxy(GORNY_LEWY_X + 1, GORNY_LEWY_Y + 1);
	for (int i = 0; i < ile; i++)
	{
		textbackground(BLACK);
		gotoxy(x + i, y);
		wyswietlanie_slowa_ruchy_kursorem(p, slowo_na_plansze[i], (x + i - GORNY_LEWY_X - 1), (y - GORNY_LEWY_Y - 1));
	}
	while (1)
	{
		if (ruch_kursorem_slowo(tab_litery, pkt, ost_lit, p, &x, &y, ile, slowo_na_plansze, tryb))
		{
			druk_plansza(p);
			break;
		}
		gotoxy(x, y);
	}
}

int proba_wstawienia_na_plansze(plansza p[][ROZMIAR], ostatnia_litera ost_lit[], int ile_lit, char slowo_na_plansze[], int x, int y)
{
	/*SPRAWDZENIE WSZYSTKICH WARUNKOW*/
	/*W TYM PRZYPADKU JUZ DLA KONKRETNEGO WYZNACZONEGO MIEJSCA*/

	int orientacja = zapamietana_orientacja(2);
	char czerwone_litery[ROZMIAR];
	int ile_czerwonych = 0;
	for (int i = 0, j = 0; i < ile_lit; i++)
	{
		if (ost_lit[i].kolor == RED)
		{
			czerwone_litery[j] = slowo_na_plansze[i];
			j++;
			ile_czerwonych++;
		}
	}
	/*USTALENIE LICZBY CZERWONYCH LITER - TZN. TAKICH, KTORYCH NIE MIAL GRACZ*/

	/*FUNCJA (OD TEGO MIEJSCA) SPRAWDZA CZY*/
	/*MOZNA ULOZYC PODANE LITERY NA PLANSZY W WYZNACZONYM MIEJSCU*/

	int zajetych = 0;
	if (ile_czerwonych == 0)
	{/*JESLI GRACZ KORZYSTA TYLKO Z POSIADANYCH LITER*/

		zajetych = 0;
		if (orientacja == POZIOM)
		{
			for (int i = 0; i < ile_lit; i++)
			{
				if (p[(y - (GORNY_LEWY_Y + 1))][(x - (GORNY_LEWY_X + 1) + i)].litera != SPACJA)
				{
					zajetych++;
					if (p[(y - (GORNY_LEWY_Y + 1))][(x - (GORNY_LEWY_X + 1) + i)].litera != slowo_na_plansze[i]) return NIE;
				}
				if ((zajetych > 0) && (zajetych < ile_lit) && (i == (ile_lit - 1))) return TAK;
			}
			return NIE;
		}
		else if (orientacja == PION)
		{
			for (int i = 0; i < ile_lit; i++)
			{
				if (p[(y - (GORNY_LEWY_Y + 1) + i)][(x - (GORNY_LEWY_X + 1))].litera != SPACJA)
				{
					zajetych++;
					if (p[(y - (GORNY_LEWY_Y + 1) + i)][(x - (GORNY_LEWY_X + 1))].litera != slowo_na_plansze[i]) return NIE;
				}
				if ((zajetych > 0) && (zajetych < ile_lit) && (i == (ile_lit - 1))) return TAK;
			}
			return NIE;
		}
	}
	else /*JESLI GRACZ KORZYSTA NIE TYLKO Z SAMYCH POSIADANYCH LITER*/
	{
		zajetych = 0;
		if (orientacja == POZIOM)
		{
			for (int i = 0; i < ile_lit; i++)
			{
				if (p[(y - (GORNY_LEWY_Y + 1))][(x - (GORNY_LEWY_X + 1) + i)].litera != SPACJA) zajetych++;
				if (ost_lit[i].kolor == RED)
				{
					if (slowo_na_plansze[i] != p[(y - (GORNY_LEWY_Y + 1))][(x - (GORNY_LEWY_X + 1) + i)].litera) return NIE;
				}
				else if (ost_lit[i].kolor == GREEN)
				{
					if ((p[(y - (GORNY_LEWY_Y + 1))][(x - (GORNY_LEWY_X + 1) + i)].litera != SPACJA) && (p[(y - (GORNY_LEWY_Y + 1))][(x - (GORNY_LEWY_X + 1) + i)].litera != slowo_na_plansze[i])) return NIE;
				}
				if ((zajetych > 0) && (zajetych < ile_lit) && (i == (ile_lit - 1))) return TAK;
			}
			return NIE;
		}
		else if (orientacja == PION)
		{
			for (int i = 0; i < ile_lit; i++)
			{
				if (p[(y - (GORNY_LEWY_Y + 1) + i)][(x - (GORNY_LEWY_X + 1))].litera != SPACJA) zajetych++;
				if (ost_lit[i].kolor == RED)
				{
					if (slowo_na_plansze[i] != p[(y - (GORNY_LEWY_Y + 1) + i)][(x - (GORNY_LEWY_X + 1))].litera) return NIE;
				}
				else if (ost_lit[i].kolor == GREEN)
				{
					if ((p[(y - (GORNY_LEWY_Y + 1) + i)][(x - (GORNY_LEWY_X + 1))].litera != SPACJA) && (p[(y - (GORNY_LEWY_Y + 1) + i)][(x - (GORNY_LEWY_X + 1))].litera != slowo_na_plansze[i])) return NIE;
				}
				if ((zajetych > 0) && (zajetych < ile_lit) && (i == (ile_lit - 1))) return TAK;
			}
			return NIE;
		}
	}
	return 0;
}

int ruch_kursorem_slowo(litery tab_litery[], punktacja *pkt, ostatnia_litera ost_lit[], plansza p[][ROZMIAR], int *x, int *y, int ile_lit, char slowo_na_plansze[], int tryb)
{
	/*PORUSZANIE SIE ZE SLOWEM PO PLANSZY*/
	/*WSTAWIANIE SLOWA*/
	/*SPRAWDZENIE WARUNKOW DLA WYBRANEJ POZYCJI*/
	/*USUNIECIE WYBRANYCH LITER Z PULI PO WSTAWIENIU SLOWA*/
	/*OBSLUGA KLAWISZA ESC*/

	char slowo_licz_pkt[ROZMIAR + 1];
	int state = 0;
	static int orientacja;
	if ((orientacja != PION) && (orientacja != POZIOM)) orientacja = POZIOM;
	wypisz_wsp(*x, *y);
	int zn;
	zn = getch();

	if ((zn == 0x6f) || ((zn == 0) && (orientacja == POZIOM)) || ((zn == 0) && (orientacja == PION)))
	{
		zapamietana_orientacja(orientacja);	//zapisanie aktualnej orientacji slowa

		/*ZMIANA ORIENTACJI PO WCISNIECIU O*/
		if (zn == 0x6f)
		{
			if (orientacja == POZIOM)
			{
				if (*y <= (GORNY_LEWY_Y + ROZMIAR - ile_lit + 1))
					orientacja = PION;
			}
			else
			{
				if (*x <= (GORNY_LEWY_X + ROZMIAR - ile_lit + 1))
					orientacja = POZIOM;
			}
		}

		/*RUCH - ORIENTACJA POZIOMA*/
		else if ((zn == 0) && (orientacja == POZIOM))
		{
			int zn = getch();
			if (zn == 0x48)
			{
				if (*y > (GORNY_LEWY_Y + 1))
				{
					*y = *y - 1;
				}
			}
			else if (zn == 0x50)
			{
				if (*y < (GORNY_LEWY_Y + ROZMIAR))
				{
					*y = *y + 1;
				}
			}
			else if (zn == 0x4b)
			{
				if (*x > (GORNY_LEWY_X + 1))
				{
					*x = *x - 1;
				}
			}
			else if (zn == 0x4d)
			{
				if (*x <= (GORNY_LEWY_X + ROZMIAR - ile_lit))
				{
					*x = *x + 1;
				}
			}
		}

		/*RUCH - ORIENTACJA PIONOWA*/
		else if ((zn == 0) && (orientacja == PION))
		{
			int zn = getch();
			if (zn == 0x48)
			{
				if (*y > (GORNY_LEWY_Y + 1))
				{
					*y = *y - 1;
				}
			}
			else if (zn == 0x50)
			{
				if (*y <= (GORNY_LEWY_Y + ROZMIAR - ile_lit))
				{
					*y = *y + 1;
				}
			}
			else if (zn == 0x4b)
			{
				if (*x > (GORNY_LEWY_X + 1))
				{
					*x = *x - 1;
				}
			}
			else if (zn == 0x4d)
			{
				if (*x < (GORNY_LEWY_X + ROZMIAR))
				{
					*x = *x + 1;
				}
			}
		}

		zapamietana_orientacja(orientacja);

		if (tryb_nauki_on_off(2) == 1)
		{
			int pozycja_x = wherex();
			int pozycja_y = wherey();
			tryb_nauki_pkt(tab_litery, p, ost_lit, ile_lit, slowo_na_plansze, *x, *y);
			gotoxy(pozycja_x, pozycja_y);
		}

	}

	/*ENTER*/
	else if (zn == 0x0d)
	{
		if (esc_wcisniety(2) == 0)
		{
			if (zapamietana_orientacja(orientacja) != 2)
			{

				if (tryb == PUSTA_PLANSZA)
				{
					for (int i = 0; i < ile_lit; i++)
					{
						if (orientacja == POZIOM)
						{
							if (p[(*y - (GORNY_LEWY_Y + 1))][(*x - (GORNY_LEWY_X + 1) + i)].srodek == 1)
							{
								break;
							}
						}
						else if (orientacja == PION)
						{
							if (p[*y - (GORNY_LEWY_Y + 1) + i][*x - (GORNY_LEWY_X + 1)].srodek == 1)
							{
								break;
							}
						}
						if (i == (ile_lit - 1))
						{
							nie_mozna_umiescic_tego_slowa_tutaj();
							state = 1;
							return state;
						}
					}
				}
				else if (tryb == NIEPUSTA_PLANSZA)
				{
					if (proba_wstawienia_na_plansze(p, ost_lit, ile_lit, slowo_na_plansze, *x, *y) == NIE)
					{
						nie_mozna_umiescic_tego_slowa_tutaj();
						state = 1;
						return state;
					}
				}

				if (sprawdz_w_slowniku(p, ost_lit, ile_lit, slowo_na_plansze, *x, *y) == NIE)
				{
					slowo_nie_znalezione();
					state = 1;
					return state;
				}

				pkt->gracz += dodaj_punkty(tab_litery, p, ost_lit, ile_lit, slowo_na_plansze, *x, *y);

				/*PETLA PONIZEJ USUWA POLOZONE NA PLANSZY LITERY Z PULI LITER GRACZA*/
				char* wsk_lit_gracz = NULL;
				wsk_lit_gracz = wskaznik_litery_gracz(wsk_lit_gracz, 1);
				for (int i = 0; i < ile_lit; i++)
				{
					if (ost_lit[i].kolor == GREEN)
					{
						if (orientacja == POZIOM)
						{
							if (p[*y - (GORNY_LEWY_Y + 1)][*x - (GORNY_LEWY_X + 1) + i].litera == SPACJA)
							{
								wsk_lit_gracz[ost_lit[i].indeks] = '0';
							}
						}
						else if (orientacja == PION)
						{
							if (p[*y - (GORNY_LEWY_Y + 1) + i][*x - (GORNY_LEWY_X + 1)].litera == SPACJA)
							{
								wsk_lit_gracz[ost_lit[i].indeks] = '0';
							}
						}
					}
				}

				if (orientacja == POZIOM)
				{
					for (int i = 0; i < ile_lit; i++)
					{
						p[*y - (GORNY_LEWY_Y + 1)][*x - (GORNY_LEWY_X + 1) + i].litera = slowo_na_plansze[i];
					}
				}
				else if (orientacja == PION)
				{
					for (int i = 0; i < ile_lit; i++)
					{
						p[*y - (GORNY_LEWY_Y + 1) + i][*x - (GORNY_LEWY_X + 1)].litera = slowo_na_plansze[i];
					}
				}
				state = 1;
				return state;
			}
		}
	}

	/*ESC*/
	else if (zn == 0x1b)
	{
		if (esc_wcisniety(1))
		{
			state = 1;
			return state;
		}
	}

	druk_plansza(p);
	/*WYDRUK PO PRZESUNIECIU KURSORA, TUZ PO ODSWIEZENIU PLANSZY*/
	if (orientacja == POZIOM)
	{
		for (int i = 0; i < ile_lit; i++)
		{
			textbackground(BLACK);
			gotoxy(*x + i, *y);
			wyswietlanie_slowa_ruchy_kursorem(p, slowo_na_plansze[i], (*x + i - GORNY_LEWY_X - 1), (*y - GORNY_LEWY_Y - 1));
		}
	}
	else if (orientacja == PION)
	{
		for (int i = 0; i < ile_lit; i++)
		{
			textbackground(BLACK);
			gotoxy(*x, *y + i);
			wyswietlanie_slowa_ruchy_kursorem(p, slowo_na_plansze[i], (*x - GORNY_LEWY_X - 1), (*y + i - GORNY_LEWY_Y - 1));
		}
	}
	state = 0;
	return state;
}

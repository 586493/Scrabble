#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "functions.h"
#include "conio2.h"

int ile_liter_w_puli(litery tab_litery[])
{
	int ile_liter = 0;
	for (int i = 0; i < 27; i++)
	{
		ile_liter += tab_litery[i].w_puli;
	}
	return ile_liter;
}

int ile_w_slowie_liter(char slowo_na_plansze[])
{
	int x = 0;
	for (int i = 0; i < ROZMIAR; i++)
	{
		if ((slowo_na_plansze[i]>=97)&&(slowo_na_plansze[i]<=122))
		{
			x++;
		}
	}
	return x;
}

void error(void)
{
	//if (plik == NULL)
	renderuj();
	gotoxy(2, 2);
	cputs("Error !");
	gotoxy(2, 4);
	cputs("Zamknij program [ENTER]");
	while (1)
	{
		if (getch() == 0x0d) exit(0);
	}
}

void wait_enter(void)
{
	/*OCZEKIWANIE NA WCISNIECIE ENTERA*/
	/*PRZYDATNE NP., GDY WYSWIETLA SIE JAKIS KOMUNIKAT*/
	/*W TAKIEJ SYTUACJI TO GRACZ DECYDUJE, KIEDY CHCE KONTYNUOWAC,*/
	/*KIEDY KOMUNIKAT MA ZNIKNAC*/
	while (1)
	{
		if (getch() == 0x0d) break;
	}
}

int tryb_nauki_on_off(int tryb)
{
	/*FUNKCJA ZAPAMIETUJE CZY TRYB NAUKI JEST WLACZONY*/
	static int x;
	if (tryb == 1)	//ON
	{
		x = 1;

	}
	else if (tryb == 0)	//OFF
	{
		x = 0;
	}
	return x;
}

void tryb_nauki_slownik(punktacja pkt, char slowo_do_slownika[])
{
	druk_punkty(pkt);
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 26);
	struct element* h = head(NULL, 0);
	if (szukaj(h, slowo_do_slownika) == NIE)
	{
		textcolor(RED);
		cputs("W slowniku: NIE");
	}
	else
	{
		textcolor(GREEN);
		cputs("W slowniku: TAK");
	}
	textcolor(WHITE);
}

void tryb_nauki_pkt(litery tab_litery[], plansza p[][ROZMIAR], ostatnia_litera ost_lit[], int ile_lit, char slowo_na_plansze[], int x, int y)
{
	druk_legenda();
	/*ZLICZANIE PUNKTOW ZA ULOZONE SLOWA*/
	int punkty = 0;
	int iloczyn = 1;
	punkty += dodaj_punkty(tab_litery, p, ost_lit, ile_lit, slowo_na_plansze, x, y);
	char punkty_napis[35];
	char rozpiska[25];
	sprintf(punkty_napis, "Lacznie punktow: %d ", punkty);
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 26);
	cputs(punkty_napis);

	/*STWORZENIE ROZPISKI*/
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 27);
	
	int orientacja = zapamietana_orientacja(2);
	if (orientacja == POZIOM)
	{
		for (int i = 0; i < ile_lit; i++)
		{
			/*LITERY*/
			sprintf(rozpiska, "(%c%d)", slowo_na_plansze[i], pobierz_punkty(slowo_na_plansze[i], tab_litery));
			cputs(rozpiska);
			if (p[y - (GORNY_LEWY_Y + 1)][x - (GORNY_LEWY_X + 1) + i].bonus_za_litere > 1)
			{
				sprintf(rozpiska, "*%d", p[y - (GORNY_LEWY_Y + 1)][x - (GORNY_LEWY_X + 1) + i].bonus_za_litere);
				cputs(rozpiska);
			}	
			putch('+');
		}
		putch(BACKSPACE);
		putch(SPACJA);
		gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 28);
		cputs("Bonusy za slowo: ");
		for (int i = 0; i < ile_lit; i++)
		{
			/*SLOWO*/
			sprintf(rozpiska, "%d*", p[y - (GORNY_LEWY_Y + 1)][x - (GORNY_LEWY_X + 1) + i].bonus_za_slowo);
			cputs(rozpiska);
			iloczyn *= p[y - (GORNY_LEWY_Y + 1)][x - (GORNY_LEWY_X + 1) + i].bonus_za_slowo;
		}
		putch(BACKSPACE);
		sprintf(rozpiska, "=%d", iloczyn);
		cputs(rozpiska);
	}
	else if (orientacja == PION)
	{
		for (int i = 0; i < ile_lit; i++)
		{
			/*LITERY*/
			sprintf(rozpiska, "(%c%d)", slowo_na_plansze[i], pobierz_punkty(slowo_na_plansze[i], tab_litery));
			cputs(rozpiska);
			if (p[y - (GORNY_LEWY_Y + 1) + i][x - (GORNY_LEWY_X + 1)].bonus_za_litere > 1)
			{
				sprintf(rozpiska, "*%d", p[y - (GORNY_LEWY_Y + 1) + i][x - (GORNY_LEWY_X + 1)].bonus_za_litere);
				cputs(rozpiska);
			}
			putch('+');
		}
		putch(BACKSPACE);
		putch(SPACJA);
		gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 28);
		cputs("Bonusy za slowo: ");
		for (int i = 0; i < ile_lit; i++)
		{
			/*SLOWO*/
			sprintf(rozpiska, "%d*", p[y - (GORNY_LEWY_Y + 1) + i][x - (GORNY_LEWY_X + 1)].bonus_za_slowo);
			cputs(rozpiska);
			iloczyn *= p[y - (GORNY_LEWY_Y + 1) + i][x - (GORNY_LEWY_X + 1)].bonus_za_slowo;
		}
		putch(BACKSPACE);
		sprintf(rozpiska, "=%d", iloczyn);
		cputs(rozpiska);
	}

}

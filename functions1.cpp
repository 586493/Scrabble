#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "functions.h"
#include "conio2.h"

void druk_plansza(plansza p[][ROZMIAR])
{
	/*RYSOWANIE RAMKI PLANSZY*/
	for (int i = GORNY_LEWY_X, a = -1; i < (GORNY_LEWY_X + ROZMIAR + 2); i++, a++)
	{
		gotoxy(i, GORNY_LEWY_Y);
		textbackground(MAGENTA);
		if ((i > GORNY_LEWY_X) && (i < (GORNY_LEWY_X + ROZMIAR + 1)))
		{
			putch(65 + a);
		}
		else
		{
			putch(SPACJA);
		}
	}
	for (int i = GORNY_LEWY_Y, a = -1; i < (GORNY_LEWY_Y + ROZMIAR + 2); i++, a++)
	{
		gotoxy(GORNY_LEWY_X, i);
		textbackground(MAGENTA);
		if ((i > GORNY_LEWY_Y) && (i < (GORNY_LEWY_Y + ROZMIAR + 1)))
		{
			putch(65 + a);
		}
		else
		{
			putch(SPACJA);
		}
	}
	for (int i = GORNY_LEWY_X; i < (GORNY_LEWY_X + ROZMIAR + 2); i++)
	{
		gotoxy(i, GORNY_LEWY_Y + ROZMIAR + 1);
		textbackground(MAGENTA);
		putch(SPACJA);
	}
	for (int i = GORNY_LEWY_Y; i < (GORNY_LEWY_Y + ROZMIAR + 2); i++)
	{
		gotoxy(GORNY_LEWY_X + ROZMIAR + 1, i);
		textbackground(MAGENTA);
		putch(SPACJA);
	}
	/*AZ DO TEGO MIEJSCA TRWA RYSOWANIE WYMAGANEJ RAMKI PLANSZY*/

	/*TU ZACZYNA SIE RYSOWANIE WNETRZA*/
	for (int y = (GORNY_LEWY_Y + 1), i = 0; (y < (GORNY_LEWY_Y + ROZMIAR + 1)) && (i < ROZMIAR); y++, i++)
	{
		for (int x = (GORNY_LEWY_X + 1), j = 0; (x < (GORNY_LEWY_X + ROZMIAR + 1)) && (j < ROZMIAR); x++, j++)
		{
			gotoxy(x, y);
			if (p[i][j].srodek == 1)	//srodek
			{
				textbackground(LIGHTMAGENTA);
			}
			else
			{

				if ((p[i][j].srodek == 0) && (p[i][j].bonus_za_litere == 2) && (p[i][j].bonus_za_slowo == 1))
				{
					textbackground(LIGHTCYAN);	//bonus x2 za litere
				}
				else if ((p[i][j].srodek == 0) && (p[i][j].bonus_za_litere == 3) && (p[i][j].bonus_za_slowo == 1))
				{
					textbackground(LIGHTBLUE);	//bonus x3 za litere
				}
				else if ((p[i][j].srodek == 0) && (p[i][j].bonus_za_litere == 1) && (p[i][j].bonus_za_slowo == 2))
				{
					textbackground(LIGHTGREEN);	//bonus x2 za slowo
				}
				else if ((p[i][j].srodek == 0) && (p[i][j].bonus_za_litere == 1) && (p[i][j].bonus_za_slowo == 3))
				{
					textbackground(GREEN);	//bonus x3 za slowo
				}
				else if ((p[i][j].srodek == 0) && (p[i][j].bonus_za_litere == 1) && (p[i][j].bonus_za_slowo == 1))
				{
					textbackground(LIGHTGRAY);	//zwykle pole
				}
			}
			textcolor(BLACK);
			putch(p[i][j].litera);
			textcolor(WHITE);
		}
	}
	/*TU KONCZY SIE RYSOWANIE WNETRZA PLANSZY*/
}

void druk_legenda(void)
{
	/*TU ZACZYNA SIE RYSOWANIE LEGENDY*/
	for (int i = LEGENDA_GORNY_LEWY_Y; i < (LEGENDA_GORNY_LEWY_Y + LEGENDA_ROZMIAR_Y); i++)
	{
		for (int j = LEGENDA_GORNY_LEWY_X; j < (LEGENDA_GORNY_LEWY_X + LEGENDA_ROZMIAR_X); j++)
		{
			gotoxy(j, i);
			textbackground(BLUE);
			putch(SPACJA);
		}
	}
	for (int i = 1; i <= 23; i++)
	{
		gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + i);
		switch (i)
		{
		case 1:		cputs("M***** S******* ******");
			break;
		case 3:		cputs("Funkcjonalnosci:");
			break;
		case 4:		cputs("a, b, c, d, e, f, g, h, i, j, k");
			break;
		case 6:		cputs("Skroty klawiszowe: ");
			break;
		case 7:		cputs("strzalki - przemieszczanie sie kursorem,");
			break;
		case 8:		cputs("q - wyjscie,");
			break;
		case 9:		cputs("n - nowa gra, t - tryb nauki,");
			break;
		case 10:	cputs("ENTER - potwierdzenie wyboru, koniec ruchu,");
			break;
		case 11:	cputs("esc - anulowanie akcji,");
			break;
		case 12:	cputs("i - wstawianie nowego slowa,");
			break;
		case 13:	cputs("backspace - kasowanie wpisanej litery,");
			break;
		case 14:	cputs("o - zmiana orientacji umieszczanego slowa,");
			break;
		case 15:	cputs("w - wymiana kafelkow,");
			break;
		case 16:	cputs("[1,7] - zaznaczanie kafelkow do wymiany,");
			break;
		case 17:	cputs("s - zapisanie gry, l - wczytanie gry");
			break;
		case 18:	cputs("Dodatkowo: ");
			break;
		case 19:	cputs("F5 - refresh, np. przy zmianie rozmiaru ekranu");
			break;
		case 20:	cputs("F4 - traktuj litery jako 'czerwone' (ON/OFF)");
			break;
		case 22:	cputs("Biezaca pozycja kursora: ");
			break;
		case 23:	cputs("Kursor niewidoczny");
			break;
		}
	}
	/*TU KONCZY SIE RYSOWANIE LEGENDY*/
}

void druk_punkty(punktacja pkt)
{
	/*WYPISYWANIE PUNKTACJI NA EKRAN*/
	char gracz[11];
	//char komp[11];
	sprintf(gracz, "%d", pkt.gracz);
	//sprintf(komp, "%d", pkt.komputer);
	for (int y = PKT_GORNY_LEWY_Y; y < (PKT_GORNY_LEWY_Y + PKT_ROZMIAR_Y); y++)
	{
		for (int x = PKT_GORNY_LEWY_X; x < (PKT_GORNY_LEWY_X + PKT_ROZMIAR_X); x++)
		{
			gotoxy(x, y);
			textbackground(BLUE);
			putch(SPACJA);
		}
	}
	gotoxy(PKT_GORNY_LEWY_X + 1, PKT_GORNY_LEWY_Y + 1);
	cputs("Punktacja");
	gotoxy(PKT_GORNY_LEWY_X + 1, PKT_GORNY_LEWY_Y + 3);
	cputs("Gracz: "); cputs(gracz);
	//gotoxy(PKT_GORNY_LEWY_X + 1, PKT_GORNY_LEWY_Y + 4);
	//cputs("Komputer: "); cputs(komp);
}

void renderuj(void)
{
	/*POKOLOROWANIE TLA ORAZ NASTAWIENIE KOLORU CZCIONKI*/
	textbackground(CYAN);
	clrscr();
	textcolor(WHITE);
}

void wypisz_wsp(int x, int y)
{
	/*WYSWIETLANIE POZYCJI KURSORA, KIEDY JEST TO POTRZEBNE*/
	for (int i = (LEGENDA_GORNY_LEWY_Y + 22); i < (LEGENDA_GORNY_LEWY_Y + 25); i++)
	{
		for (int j = (LEGENDA_GORNY_LEWY_X + 1); j < (LEGENDA_GORNY_LEWY_X + LEGENDA_ROZMIAR_X); j++)
		{
			gotoxy(j, i);
			textbackground(BLUE);
			putch(SPACJA);
		}
	}
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 22);
	cputs("Biezaca pozycja kursora: ");
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 23);
	cputs("Os pozioma: ");
	putch(65 + x - GORNY_LEWY_X - 1);
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 24);
	cputs("Os pionowa: ");
	putch(65 + y - GORNY_LEWY_Y - 1);
}

void druk_plansza_legenda_tlo(plansza p[][ROZMIAR])
{
	/*DRUKOWANIE WIELU RZECZY NA RAZ*/
	/*PRZYDATNE W NIEKTORYCH SYTUACJACH*/
	renderuj();
	druk_plansza(p);
	druk_legenda();
	_setcursortype(_NOCURSOR);;
}

void init(plansza p[][ROZMIAR], litery tab_litery[])
{
	/* 'ZEROWANIE' PLANSZY*/
	for (int i = 0; i < ROZMIAR; i++)
	{
		for (int j = 0; j < ROZMIAR; j++)
		{
			p[i][j].bonus_za_litere = 1;
			p[i][j].bonus_za_slowo = 1;
			p[i][j].litera = SPACJA;		//SPACJA, czyli na poczatek brak liter
			p[i][j].srodek = 0;
		}
	}

	p[ROZMIAR / 2][ROZMIAR / 2].srodek = 1;
	//srodek planszy, tu zaczyna sie gra

	/*ROZMIESZCZENIE POL SPECJALNYCH*/
	for (int i = 0; i < ROZMIAR; i++)
	{
		for (int j = 0; j < ROZMIAR; j++)
		{
			if (p[i][j].srodek == 0)
			{
				if (i == j)
				{
					p[i][j].bonus_za_litere = 2;	/*BONUS ZA LITERE X2*/
				}
				else if ((j == ROZMIAR / 2) && (i % 2 == 0) && ((i <= 3) || (i >= ROZMIAR - 4)))
				{
					p[i][j].bonus_za_litere = 3;	/*BONUS ZA LITERE X3*/
				}
				else if (i == ROZMIAR - j - 1)
				{
					p[i][j].bonus_za_slowo = 2;		/*BONUS ZA SLOWO X2*/
				}
				else if ((i == ROZMIAR / 2) && (j % 2 == 0) && ((j <= 3) || (j >= ROZMIAR - 4)))
				{
					p[i][j].bonus_za_slowo = 3;		/*BONUS ZA SLOWO X3*/
				}
			}
		}
	}

	struct litery bufor;
	FILE *plik;
	plik = fopen("litery.txt", "r");
	if (plik == NULL) 
	{ 
		renderuj();
		gotoxy(2, 2);
		cputs("Nie udalo sie wczytac pliku litery.txt !");
		gotoxy(2, 4);
		cputs("Zamknij program [ENTER]");
		while (1)
		{
			if (getch() == 0x0d) exit(0);
		}
	}
	else
	{
		int i = 0;
		/*POBRANIE Z PLIKU INFORMACJI O LITERACH*/
		while (!feof(plik))
		{
			fscanf(plik, "{'%c',%hd,%hd,%hd}\n", &bufor.znak, &bufor.punkty, &bufor.ilosc, &bufor.w_puli);

			tab_litery[i].znak = bufor.znak;
			tab_litery[i].punkty = bufor.punkty;
			tab_litery[i].ilosc = bufor.ilosc;
			tab_litery[i].w_puli = bufor.w_puli;
			i++;
		}
		fclose(plik);
	}
}

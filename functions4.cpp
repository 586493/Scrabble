#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "functions.h"
#include "conio2.h"

litery* dobierz_litery(char litera, litery tab_litery[])
{
	/*DOBIERANIE LITER*/
	/*JESLI UDA SIE ZNALEZC LITERE 'char litera' ZWRACA WSKAZNIK*/
	/*ORAZ ODEJMUJE LITERE Z PULI*/
	struct litery *lit = NULL;
	for (int i = 0; i < 27; i++)
	{
		if ((tab_litery[i].znak == litera) && (tab_litery[i].w_puli > 0))
		{
			(tab_litery[i].w_puli) = (tab_litery[i].w_puli) - 1;

			lit = &tab_litery[i];
			break;
		}
	}
	return  lit;
}

short pobierz_punkty(char litera, litery tab_litery[])
{
	/*FUNKCJA OBLICZA ILE LITER OTRZYMUJE SIE*/
	/*ZA 'char litera'*/
	short pkt;
	for (int i = 0; i < 27; i++)
	{
		if (tab_litery[i].znak == litera)
		{
			pkt = tab_litery[i].punkty;
			return  pkt;
		}
	}
	return 1;
}

void litery_start_los(int *licznik, litery tab_litery[], char litery_gracz[])
{
	/*LOSUJE LITERY, UZUPELNIA DO 7 LITER*/
	char znak;
	for (int i = 0; i < 7; i++)
	{
		while (1)
		{
			if (litery_gracz[i] != '0') break;
			struct litery *l;
			znak = los_litera(licznik, tab_litery);	//wylosowanie
			if (znak == SPACJA) return;	//nie udalo sie wylosowac
			l = dobierz_litery(znak, tab_litery);
			if (l != NULL)
			{
				litery_gracz[i] = l->znak;
				break;
			}
		}
	}
}

void jakie_litery_ma_gracz(char litery_gracz[])
{
	/*WYPISUJE LITERY GRACZA*/
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 25);
	cputs("Litery gracza:");
	for (int i = 0; i < 7; i++)
	{
		putch(SPACJA);
		if (litery_gracz[i] == '0') putch(SPACJA);
		else putch(litery_gracz[i]);
	}
}

void generuj_losowe(void)
{
	/*FUNKCJA GENERUJE ZIARNA, KTORE NASTEPNIE ZAPISUJE W PLIKU .BIN*/
	/*ZIARNA ZAPISYWANE Z GORY I DO TEGO W ZEWNETRZNYM PLIKU*/
	/*ABY ZAPEWNIC IDENTYCZNE LOSOWANIE PO KAZDYM ZAPISIE I WCZYTANIU GRY*/
	FILE* plik;
	int r;
	plik = fopen("random.bin", "wb");
	if (plik == NULL) { error(); }
	else
	{
		for (size_t i = 0; i < (RANDOM_SEED); i++)
		{
			r = rand();
			fwrite(&r, sizeof(int), 1, plik);
		}
	}
	fclose(plik);
}

int get_random(int *go)
{
	int r = 0;
	static FILE* plik;
	plik = fopen("random.bin", "rb");
	if (plik == NULL) { error(); }
	for (int i = 0; i < ((*go) % RANDOM_SEED); i++)
	{
		fread(&r, sizeof(int), 1, plik);	//POBRANIE ZIARNA (RANDOM SEED)
	}
	(*go)++;
	/*GO (CZYLI ZMIENNA LICZNIK Z PLIKU GLOWNEGO) MOWI O TYM*/
	/*KTORE KOLEJNE ZIARNO NALEZY POBRAC Z PLIKU .BIN*/
	fclose(plik);
	return r;
}

char los_litera(int *licznik, litery tab_litery[])
{
	/*ALGORYTM LOSUJACY LITERY*/
	/*IMITUJE LOSOWANIE Z WORECZKA PODCZAS NORMALNEJ GRY W SCRABBLE*/
	/*PRAWDOPODOBIENSTWO WYLOSOWANIA KONKRETNEJ LITERY UZALEZNIONE*/
	/*OD JEJ AKTUALNEJ LICZBY W PULI*/
	char litera = SPACJA;
	int lacznie_liter = ile_liter_w_puli(tab_litery);
	if (lacznie_liter > 0)
	{
		int ktora = 0;
		int r = get_random(licznik) % lacznie_liter;

		for (int i = 0; i < 27; i++)
		{
			if (tab_litery[i].w_puli == 0) continue;
			if ((r >= ktora) && (r < (ktora + tab_litery[i].w_puli)))
			{
				litera = tab_litery[i].znak;
			}
			ktora += tab_litery[i].w_puli;
		}
	}
	return litera;
}

void nie_mozna_umiescic_takiego_slowa(void)
{
	druk_legenda();
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 26);
	cputs("Nie mozna umiescic takiego slowa !!!");
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 27);
	cputs("Kontynuuj [ENTER]");
	wait_enter();
}

void min_dwie_litery(void)
{
	druk_legenda();
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 26);
	cputs("Za malo liter, aby ulozyc slowo !!!");
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 27);
	cputs("Kontynuuj [ENTER]");
	wait_enter();
}

void nie_mozna_umiescic_tego_slowa_tutaj(void)
{
	druk_legenda();
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 26);
	cputs("Nie mozna umiescic w tym miejscu tego slowa !!!");
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 27);
	cputs("Kontynuuj [ENTER]");
	wait_enter();
}

void slowo_nie_znalezione(void)
{
	druk_legenda();
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 26);
	cputs("Nie wszystkie slowa odnaleziono w slowniku !!!");
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 27);
	cputs("Kontynuuj [ENTER]");
	wait_enter();
}

int wczytywanie_gry_ostrzezenie(void)
{
	int zn;
	druk_legenda();
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 25);
	cputs("Czy na pewno chcesz wczytac zapisana gre?");
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 26);
	cputs("(Niezapisany stan gry zostanie utracony.)");
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 27);
	cputs("[1] Tak		[2] Nie");
	while (1)
	{
		zn = getch();
		if ((zn == 49) || (zn == 50)) break;
	}
	if (zn == 49) return TAK;
	else return NIE;
}

void koniec_liter(punktacja pkt)
{
	/*SEKWENCJA KONCOWA GRY*/
	char napis[30];
	druk_legenda();
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 26);
	cputs("W puli zabraklo liter!");
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 27);
	sprintf(napis, "Wynik gracza: %d pkt", pkt.gracz);
	cputs(napis);
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 28);
	cputs("Kontynuuj [ENTER]");
	wait_enter();
}

int esc_wcisniety(int tryb)
{
	/*FUNKCJA ZAPAMIETUJE CZY WCISNIETO ESC W CZASIE UMIESZCZANIA SLOWA*/
	static int x;
	if (tryb == 1) //ON, wcisniety
	{
		x = 1;
	}
	if (tryb == 2)	//OFF
	{
		x = 0;
	}
	return x;
}

void wyswietlanie_slowa_ruchy_kursorem(plansza p[][ROZMIAR], char litera, int x, int y)
{
	/*WYSWIETLENIE DANEGO SLOWA W KONKRETNYM MIEJSCU W CZASIE*/
	/*RUSZANIA KURSOREM*/
	if (p[y][x].litera == SPACJA)
	{
		textcolor(GREEN);
		putch(litera);
	}
	else if (p[y][x].litera != SPACJA)
	{
		if (p[y][x].litera == litera)
		{
			textcolor(GREEN);
			putch(litera);
		}
		else
		{
			textcolor(RED);
			putch(litera);
		}
	}
	textcolor(WHITE);
}

char* podaj_nazwe(void)
{
	/*FUNKCJA POBIERA NAZWE A NASTEPNIE DOKLEJA .BIN*/
	/*ZWRACA GOTOWY DO WSTAWIENIA STRING*/
	int j = 0;
	const int dlugosc = 102;
	char bufor[dlugosc];	//bez .bin
	char bufor2[dlugosc];	//z doklejonym .bin
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 26);
	cputs("Podaj nazwe: ");
	while (1)
	{
		int zn = getch();
		if ((zn == 0x08) && (j > 0)) //backspace
		{
			j--;
			bufor[j] = '\0';
			putch(BACKSPACE);
			putch(SPACJA);
			putch(BACKSPACE);
		}
		else if ((j < (dlugosc - 1)) && (((zn >= 97) && (zn <= 122)) || ((zn >= 48) && (zn <= 57))))
		{
			/*WPROWADZANIE Z KLAWIATURY LITER I CYFR*/
			bufor[j] = zn;
			j++;
			bufor[j] = '\0';
			putch(zn);
		}
		else if ((zn == 0x0d) && (j > 0))
		{
			/*ZATWIERDZENIE ENTEREM*/
			break;
		}
	}
	sprintf(bufor2, "%s.bin", bufor);	//doklejenie .bin
	char* nazwa = (char*)malloc(sizeof(char)*(strlen(bufor2) + 1));
	//alokacja odpowiedniej ilosci pamieci (z uwzglednieniem '\0')
	strcpy(nazwa, bufor2); //kopiowanie stringa
	return nazwa;
}

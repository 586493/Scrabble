#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include"conio2.h"
#include"functions.h"

int main(int argc, char *argv[])
{
	/*ZALADOWANIE SLOWNIKA*/
	FILE *plik;
	struct element *poczatek;
	struct element el_1;
	poczatek = &el_1;
	poczatek->nastepny = NULL;
	char *str = (char*)malloc(sizeof(char)*41);
	if (argc > 1)
	{
		plik = fopen(argv[1], "r");
	}
	else
	{
		plik = fopen("scrabble_words_dictionary.txt", "r");
	}	
	if (plik == NULL) { error(); }
	while (1)
	{
		int liczba_wczytanych = fscanf(plik, "%40s\n", str);
		if (liczba_wczytanych <= 0) break;
		dodaj(poczatek, str);
	}
	fclose(plik);
	free(str);
	head(poczatek, 1);
	/*W TYM MIEJSCU KONCZY SIE LADOWANIE SLOWNIKA*/

	srand(time(NULL));
	int zn = 1;							//zmienna do wczytywania znakow z klawiatury
	struct punktacja pkt;				//struktura przechowujaca punkty 
	struct plansza p[ROZMIAR][ROZMIAR];	//struktura zawierajaca info o planszy
	struct litery tab_litery[27];		//struktura zawierajaca info o literach				
	int licznik = 0;					//licznik mowi o tym, ktore ziarno (random seed) z pliku pobrac
	char* name;							//name jest uzywany do nadawania nazwy zapisom


	#ifndef __cplusplus
		Conio2_Init();
	#endif
	
	settitle("MS");

	do
	{
		pkt.gracz = 0;
		pkt.komputer = 0;
		char litery_gracz[7] = { '0','0','0','0','0','0','0' };
		wskaznik_litery_gracz(litery_gracz, 0);	//przechowuje wsk na liery gracza
		init(p, tab_litery);		//inicjalizacja wszystkiego, 'wyzerowanie' niektorych parametrow gry
		int klawiatura_l = 0;		//czy bylo wcisniete l
		licznik = 0;				//licznik mowi o tym, ktore ziarno (random seed) z pliku pobrac
		druk_plansza_legenda_tlo(p);	//drukowanie kilku rzeczy

		/*jezeli gracz wlaczyl nowa gre lub wczytal gre w trakcie grania, to nie pobieraj nowego znaku*/
		if ((zn != 'n') && (zn != 'l'))
		{
			zn = getch();
		}

		/*WCZYTYWANIE*/
		if (zn == 'l')
		{
			FILE* plik;			//save
			FILE* plik_random;	//ziarna
			druk_legenda();
			gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 25);
			cputs("Wczytanie stanu gry");
			char* n = podaj_nazwe();
			plik = fopen(n, "rb");
			plik_random = fopen("random.bin", "wb");
			if ((plik == NULL) || (plik_random == NULL))	//error alert
			{
				druk_legenda();
				gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 27);
				cputs("Nie udalo sie wczytac zapisu! [ENTER]");
				wait_enter();	//czekaj na enter
				zn = 1;		//znak bedzie wczytany od nowa
			}
			else
			{
				klawiatura_l = 1;	//wcisnieto l
				for (size_t i = 0; i < ROZMIAR; i++)
				{
					for (size_t j = 0; j < ROZMIAR; j++)
					{
						fread(&p[i][j], sizeof(struct plansza), 1, plik);
					}
				}
				fread(&pkt, sizeof(struct punktacja), 1, plik);
				for (int i = 0; i < 27; i++) fread(&tab_litery[i], sizeof(struct litery), 1, plik);
				fread(litery_gracz, (sizeof(char) * 7), 1, plik);
				fread(&licznik, sizeof(int), 1, plik);
				
				int r;	/*POBRANIE Z SAVE'A ZIAREN ORAZ ICH ZAPIS*/
				for (size_t i = 0; i < (RANDOM_SEED); i++)
				{
					fread(&r, sizeof(int), 1, plik);
					fwrite(&r, sizeof(int), 1, plik_random);
				}
				fclose(plik);
				fclose(plik_random);
				free(n);

				wskaznik_litery_gracz(litery_gracz, 0);
				druk_plansza_legenda_tlo(p);
				/*PO POMYSLNYM ZALADOWANIU SAVE'A URUCHAMIA GRE, TAK JAKBY*/
				/*BYLA NOWA, ALE Z WCZYTANYMI PARAMETRAMI*/
				zn = 'n';	
			}
		}

		switch (zn)
		{
		case 0x00:		
			/*F5 - refresh (praktyczny dodatek), przydatny przy zmianie rozmiaru okna*/
			/*po zmianie rozmiaru okna wystarczy wcisnac F5, aby wszystko*/
			/*wyswietlalo sie poprawnie w oknie o nowym rozmiarze*/
			_setcursortype(_NOCURSOR);;
			if (getch() == 0x3f)
			{
				druk_plansza_legenda_tlo(p);
			}
			break;
		case 'n':
			/*wcisniecie n - nowa (zwykla) gra*/
			if(klawiatura_l == 0) generuj_losowe();	
			//jesli nie wcisnieto l (nie wczytano gry) tworzy 
			//nowe ziarno (random seed)

			_setcursortype(_NOCURSOR);;
			litery_start_los(&licznik, tab_litery, litery_gracz);
			jakie_litery_ma_gracz(litery_gracz);
			//losowanie liter i wypisanie posiadanych liter						
			int x = GORNY_LEWY_X + 1;		//wspolrzedne
			int y = GORNY_LEWY_Y + 1;		//wspolrzedne
			//nastawienie x oraz y na wspolrzedne poczatka planszy (lewy, gorny rog)
			while (zn != 'q')	
			{
				_setcursortype(_NOCURSOR);
				jakie_litery_ma_gracz(litery_gracz);

				//jesli esc byl wcisniety, to zamiast pobrac nowy znak 
				//oznaczajcy pewien tryb gry, przechodzi sie od razu
				//do ponownego ulozenia slowa
				if (esc_wcisniety(0) == 1)
				{
					zn = 'i';
				}
				else
				{
					zn = getch();
				}

				switch (zn)
				{
				case 0:
					_setcursortype(_NOCURSOR);	/*REFRESH - F5*/
					if (getch() == 0x3f)
					{
						druk_punkty(pkt);
						druk_plansza_legenda_tlo(p);
					}
					break;
				case 0x69:	/*WSTAWIANIE SLOW*/
					druk_punkty(pkt);
					druk_legenda();
					jakie_litery_ma_gracz(litery_gracz);
					wstaw_slowo(&pkt, p, tab_litery, litery_gracz);
					litery_start_los(&licznik, tab_litery, litery_gracz);
					druk_punkty(pkt);
					druk_legenda();
					/*GDY ZABRAKNIE LITER*/
					if (ile_liter_w_puli(tab_litery) == 0)
					{
						koniec_liter(pkt);
						zn = 'm';
						//m nie ma specjalnej funkcji, po prostu w sytuacji, gdy koncza sie
						//litery, zrywa petle odpowiedzialna za gre, ale nie zrywa petli 
						//zewnetrznej - mozna wiec skonczyc te gre, ale nie zamykac programu
					}

					break;
				case 0x77:
					/*WYMIANA LITER*/
					wymiana(&licznik, tab_litery, litery_gracz);
					druk_legenda();
					break;

				case 0x74:
					/*T - WLACZANIE/WYLACZANIE TRYBU NAUKI*/
					druk_legenda();
					gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 27);
					if (tryb_nauki_on_off(2) == 1)	//byl wlaczony
					{
						tryb_nauki_on_off(0);	//wylaczanie (OFF)
						textcolor(RED);
						cputs("Tryb nauki [OFF]");

					}
					else if (tryb_nauki_on_off(2) == 0)	//nie byl wlaczony
					{
						tryb_nauki_on_off(1);	//wlaczanie (ON)
						textcolor(GREEN);
						cputs("Tryb nauki [ON]");
						
					}
					textcolor(WHITE);
					break;

					/*ZAPIS*/
				case 0x73:
					FILE* plik;		//save
					FILE* plik_random;		//ziarna
					druk_legenda();
					gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 25);
					cputs("Zapisanie stanu gry");
					name = podaj_nazwe();
					plik = fopen(name, "wb");	//stworzenie zapisu pod nazwa z 'char* n'
					plik_random = fopen("random.bin", "rb");	//pobranie ziaren
					if ((plik == NULL)||(plik_random == NULL)) //error alert
					{ 
						gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 27);
						cputs("Nie udalo sie!   [ENTER]");
						wait_enter();
					}
					else
					{
						gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 27);
						cputs("Gra zapisana!   [ENTER]");
						wait_enter();

						for (size_t i = 0; i < ROZMIAR; i++)
						{
							for (size_t j = 0; j < ROZMIAR; j++)
							{
								fwrite(&p[i][j], sizeof(struct plansza), 1, plik);
							}
						}
						fwrite(&pkt, sizeof(struct punktacja), 1, plik);
						for (int i = 0; i < 27; i++) fwrite(&tab_litery[i], sizeof(struct litery), 1, plik);
						fwrite(litery_gracz, (sizeof(char) * 7), 1, plik);
						fwrite(&licznik, sizeof(int), 1, plik);
						int r;
						for (size_t i = 0; i < (RANDOM_SEED); i++)
						{
							/*POBRANIE Z PLIKU ZIAREN ORAZ ICH ZAPIS*/
							fread(&r, sizeof(int), 1, plik_random);
							fwrite(&r, sizeof(int), 1, plik);
						}
					}
						fclose(plik);
						fclose(plik_random);
						free(name);
						druk_legenda();
					break;

					/*WCZYTANIE GRY - OSTRZEZENIE O WYCHODZENIU Z NIEZAPISANEJ GRY*/
				case 0x6c:
					if (wczytywanie_gry_ostrzezenie() == TAK) zn = 'l';
					else zn = 1;
					druk_legenda();
					break;

				}

				if ((zn == 'q') || (zn == 'n') || (zn == 'l') || (zn == 'm')) break;
				/*ZERWANIE PETLI ODPOWIEDZIALNEJ ZA GRE*/
				/*q - ZAMKNIECIE PROGRAMU*/
				/*n - NOWA GRA*/
				/*l - WCZYTYWANIE SAVE'A*/
				/*m - GDY KONCZA SIE LITERY*/
			}
			break;
		}

	} while (zn != 'q');

	usun_cala_liste();

	_setcursortype(_NORMALCURSOR);

	return 0;
}

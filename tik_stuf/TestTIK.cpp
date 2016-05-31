// TestTIK.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <tchar.h>
#include <iostream>

// Struktura opisuj�ca wiersz Modelu �r�d�a informacji (MZI) 
struct TElement
{
	// Indeks symbolu w MZI
	int Indeks;
	// Kod symbolu wyst�puj�cego w odczytanym tek�cie
	int SYMBOL;
	// Liczba wyst�pie� symbolu w odczytanym tek�cie
	int Liczba;
};

// Struktura opisauj�ca wiersz w Tabeli drzewa kodowania 
struct TWezel
{
	// Kod ojca
	int Ojciec;
	// Kod potomka 0 (po lewej) 
	int Potomek0;
	// Kod potomka 1 (po prawej)
	int Potomek1;
};


// Struktura opisujaca wiersz Tabeli kodowej
struct TKodSymbolu
{
	// Kod symbolu
	int SYMBOL;
	// Kod Huffmana symbolu ale podany jako liczba ca�kowita bez znaku - �atwo�� implementacji
	unsigned int KOD;
	// D�ugo�� kodu Huffmana 
	unsigned int DlgKOD;
};

// Utworzenie Modelu �r�d�a informacji (MZI)
// ARGUMENTY:
//   nazwaPlikuWej - nazwa pliku wej�ciowego, gdze znajduje si� tekst,
//   nazwaPlikuMZI - nazwa pliku do kt�rego zostanie zapisany MZI
//   N - parametr wyj�ciowy okre�laj�cy liczb� wierszy MZI
// WYNIK FUNKCJI:
//   MZI - model �r�d�a informacji (MZI)
TElement* UtworzMZI(char *nazwaPlikuWej, char *nazwaPlikuMZI, int &N)
{
	// Liczba symboli odczytanych z pliku
	int ileOdczytano = 0;
	// Symbol odczytany z pliku
	char znakOdczytany;
	// Liczba r�znych symboli odczytanych z pliku
	int liczbaSymboli = 0;	
	// Tablica statyczna dla wszystkich znak�w ASCII
	char TabASCII[256];
	// Struktura opisuj�ca plik
	FILE *PLIK;
	
	// wyzerowanie tablicy znak�w ASCII - wykorzystanie funkcji, kt�ra zeruje
	// wszystkie bajty obszaru pami�ci o rozmiarze "256 * sizeof(char)",
	// pocz�wszy od adresu "TabASCII"
	memset(TabASCII, 0, 256 * sizeof(char));

	// komunikat diagnostyczny - wypisanie tekstu na ekranie
	printf("Odczytuje plik wejsciowy (%s)...\n\n", nazwaPlikuWej);
	// Otwarcie pliku wej�ciowego do odczytu - odczytanie tekstu
	PLIK = fopen(nazwaPlikuWej, "rt");
	
	while (!feof(PLIK))
	{
		// odczytanie symbolu z pliku. Je�eli "ileOdczytano == -1" tzn. osi�gni�to koniec pliku
		// w zmiennej "znakOdczytany" znajduje si� kod symbolu
		ileOdczytano = fscanf(PLIK, "%c", &znakOdczytany);
		if (ileOdczytano  > 0)
		{
			// komunikat diagnostyczny - wypisanie tekstu na ekranie
			printf("%c", znakOdczytany);
			// je�eli zawarto�� kom�rki tabeli jest r�wna zero tzn. odczytano nowy symbol
			if (TabASCII[znakOdczytany] == 0)
				liczbaSymboli++;
			// zliczanie symboli
			TabASCII[znakOdczytany] ++;
		}
	}
	// zamkni�cie pliku
	fclose(PLIK);
	// komunikat diagnostyczny - wypisanie tekstu na ekranie
	printf("\n\nZakonczono odczyt pliku wejsciowego...\n\n");

	// Nadanie wartosci zmiennej "N", kt�ra b�dzie zwr�cona na zewn�trz funkcji.
	// Sp�jrz na nag�owek funkcji "UtworzMZI".
	N = liczbaSymboli;
		
	// UWAGA operator "new" pozwala w tym przypadku na dynamiczne utworzenie modelu �r�d�a informacji,
	// tzn. jednowymiarowej tablicy struktur o N elementach
	
	// Model �r�d�a informacji
	TElement *MZI = new TElement[N];
	// Indeks wiersza MZI
	int n = -1;
	// Zmienna pomocnicza, kt�ra pomaga ustali� w�a�ciwy wiersz dla symbolu wstawianego do MZI
	int k;
	// Kod symbolu
	int kodZnaku;
	// Liczba wyst�ie� symbolu
	int liczbaWystapien;

	// Przegl�danie tabicy znak�w ASCII i wstawianie symboli do MZI na w�a�ciwe miejsce
	// (wstawianie z sortowaniem)
	for (int idxASCII = 0; idxASCII < 256; idxASCII++)
	{
		// je�eli w tablicy znak�w ASCII jaka� pozycja jest wi�ksza od zera,
		// tzn. odczytano z pliku znak o kodzie "idxASCII".
		// Warto�� przechowywana w tabicy jest r�wna liczbie jego wyst�pienia w pliku wej�ciowym
		if (TabASCII[idxASCII] > 0)
		{
			// zapami�tanie kodu ASCII symbolu
			kodZnaku = idxASCII;
			// zapami�tanie liczby wyst�pie� znaku ASCII
			liczbaWystapien = TabASCII[idxASCII];

			// zwi�kszenie numeru wierszy tablicy MZI
			++n;

			// Rozuni�cie tablicy MZI tak, aby symbol o najmniejszej liczbie wyst�pie� 
			// by� w wierszu "n". Nowy symbola nale�y wpisac do wiersza "k" - na w�a�ciwe miejsce.  
			k = n;
			while ((k > 0) && (MZI[k - 1].Liczba < liczbaWystapien))
			{
				MZI[k].SYMBOL = MZI[k - 1].SYMBOL;
				MZI[k].Liczba = MZI[k - 1].Liczba;
				--k;
			}
			MZI[k].SYMBOL = kodZnaku;
			MZI[k].Liczba = liczbaWystapien;
		}
	}
	
	// komunikat diagnostyczny - wypisanie tekstu na ekranie
	printf("MODEL ZRODLA INFORMACJI:\n");
	printf("%s\t%s\t%s\n", "Indeks", "Symbol", "Liczba wystapien");
	// otworzenie pliku gdzie zostanie zapisany MZI
	PLIK = fopen(nazwaPlikuMZI, "wt");
	for (int idx = 0; idx < N; idx++)
	{
		MZI[idx].Indeks = idx;

		kodZnaku = MZI[idx].SYMBOL;
		liczbaWystapien = MZI[idx].Liczba;
		// Zapis MZI do pliku
		// 1 - indeks symbolu
		// 2 - kod symbolu
		// 3 - liczba wyst�pie� symbolu w tek�cie
		fprintf(PLIK, "%d %d %d\n", idx, kodZnaku, liczbaWystapien);
		// komunikat diagnostyczny - wypisanie tekstu na ekranie
		printf("%d\t%c\t%d\n", idx, kodZnaku, liczbaWystapien);
	}
	// zamkni�cie pliku
	fclose(PLIK);
	// komunikat diagnostyczny - wypisanie tekstu na ekranie
	printf("\n\n******************************************\n\n");
	// zwr�cenie MZI do miejsca wywo�ania funkcji
	return MZI;
}

// Utworzenie tablicy drzewa kodowania
// ARGUMENTY:
//   nazwaPliku - nazwa pliku do kt�rego zostanie zapisana tablica drzewa kodowania
//   MZI - model �r�d�a informacji (MZI)
//   N - liczba wierszy MZI
// WYNIK FUNKCJI:
//   TDK - tablica drzewa kodowania
TWezel* UtworzTDK(char * nazwaPliku, TElement *MZI, int N)
{
	// Struktura opisuj�ca plik
	FILE *PLIK;
	// Zmienna pomocnicza, kt�ra pomaga ustali� w�a�ciwy wiersz dla symbolu wstawianego do kopii MZI
	int k;
	// Kod symbolu pomocniczego
	int SymbolPomocniczy;
	// liczba wyst�ie� symbolu pomocniczego
	int LiczbaPomocniczego;
	// Indeks w�z�a ostaniego
	int idxPotomek0;
	// Indeks w�z�a przedostaniego
	int idxPotomek1;
	// Tabela drzewa kodowania (TDK) 
	TWezel* TDK;
	// Kopia modelu �r�d�a informacji (kopia MZI)
	TElement *kopiaMZI;
	
	// Dynamiczne utworzenie tabeli drzewa kodowania
	TDK = new TWezel[N - 1];
	// Dynamiczne utworzenie kopii modelu �r�d�a informacji
	kopiaMZI = new TElement[N];
	// Utworzenie kopii MZI za pomoc� fukcji, kt�ra kopiuje blok pami�ci
	memcpy_s(kopiaMZI, N*sizeof(TElement), MZI, N*sizeof(TElement));	
	
	for (int idx = 0; idx < N - 1; idx++)
	{
		// Utworzenie w�z�a pomocniczego. Kod w�z�a jest zawsze ujemny i nale�y do przedzia�u <-2N+2, -N>
		SymbolPomocniczy = -1 * (idx + N);
		// Okre�lenie ostaniego w�z�a w kopii MZI
		idxPotomek0 = N - idx - 1;
		// Okre�lenie przedostaniego w�z�a w kopii MZI
		idxPotomek1 = N - idx - 2;
		
		// Liczba wyst�pie� w�z�a pomocniczego
		LiczbaPomocniczego = kopiaMZI[idxPotomek0].Liczba + kopiaMZI[idxPotomek1].Liczba;
		
		// Zapisanie do TDK symbolu rodzica		
		TDK[idx].Ojciec = SymbolPomocniczy;
		// kod potomka 0 - je�eli ostatni symbol w kopii MZI jest ujemny to jako Potomek0 podaje si�
		// jego symbol (liczba ujemna), w przeciwnym przypadku indeks symbolu w MZI (liczba dodatnia)
		TDK[idx].Potomek0 = 
			kopiaMZI[idxPotomek0].SYMBOL < 0 ? kopiaMZI[idxPotomek0].SYMBOL : kopiaMZI[idxPotomek0].Indeks;
		// kod potomka 1 - je�eli przedostatni symbol w kopii MZI jest ujemny to jako Potomek1 podaje si�
		// jego symbol (liczba ujemna), w przeciwnym przypadku indeks symbolu w MZI (liczba dodatnia)
		TDK[idx].Potomek1 =
			kopiaMZI[idxPotomek1].SYMBOL < 0 ? kopiaMZI[idxPotomek1].SYMBOL : kopiaMZI[idxPotomek1].Indeks;

		
		// Rozuni�cie tablicy kopiaMZI tak, aby symbol o najmniejszej liczbie wyst�pie� 
		// by� w ostatnim wierszu a nowy symbol wpisac do wiersza "k" - na w�a�ciwe miejsce.
		// W ka�dej iteracji liczba wierszy tablicy (kopiaMZI) zminejsza si� o jeden.
		k = N - idx - 2;
		while ((k - 1 >= 0) && (kopiaMZI[k - 1].Liczba < LiczbaPomocniczego))
		{
			kopiaMZI[k].Indeks = kopiaMZI[k - 1].Indeks;
			kopiaMZI[k].SYMBOL = kopiaMZI[k - 1].SYMBOL;
			kopiaMZI[k].Liczba = kopiaMZI[k - 1].Liczba;
			k--;
		}		
		kopiaMZI[k].SYMBOL = SymbolPomocniczy;
		kopiaMZI[k].Liczba = LiczbaPomocniczego;
	}
	// usuni�cie kopii MZI
	delete kopiaMZI;

	// komunikat diagnostyczny - wypisanie tekstu na ekranie
	printf("TABELA DRZEWA KODOWANIA:\n");
	printf("%s\t%s\t%s\n", "Ojciec", "Potmek_0", "Potomek_1");

	// otworzenie pliku do zapisu - zapisanie TDK do pliku
	PLIK = fopen(nazwaPliku, "wt");
	for (int idx = 0; idx < N - 1; idx++)
	{
		// zapisanie wiersza TDK do pliku
		fprintf(PLIK, "%d %d %d\n", TDK[idx].Ojciec, TDK[idx].Potomek0, TDK[idx].Potomek1);
		
		// komunikaty diagnostyczne - wypisanie tekstu na ekranie
		printf("%d", TDK[idx].Ojciec);
		printf("\t");
		if (TDK[idx].Potomek0 < 0)
			printf("%d", TDK[idx].Potomek0);
		else
			printf("%c", MZI[TDK[idx].Potomek0].SYMBOL);
		printf("\t\t");
		if (TDK[idx].Potomek1 < 0)
			printf("%d", TDK[idx].Potomek1);
		else
			printf("%c", MZI[TDK[idx].Potomek1].SYMBOL);
		printf("\n");
	}
	// zamkni�cie pliku
	fclose(PLIK);
	printf("\n\n******************************************\n\n");
	// zwr�cenie TDK do miejsca wywo�ania funkcji
	return TDK;
}


// Utworzenie tablicy kodowej
// ARGUMENTY:
//   nazwaPliku - nazwa pliku do kt�rego zostanie zapisana tablica kodowa
//   MZI - model �r�d�a informacji (MZI)
//   TDK - tablica drzewa kodowania
//   N - liczba wierszy MZI
// WYNIK FUNKCJI:
//   TK - tablica kodowa
TKodSymbolu* UtworzTK(char * nazwaPliku, TElement *MZI, TWezel *TDK, int N)
{
	// Struktura opisuj�ca plik
	FILE *PLIK;
	// Tablica kodowa symboli (TK)
	TKodSymbolu *TK;
	// Tablica kodowa pomocniczych symboli kodowych (pomocnicza TK)
	TKodSymbolu *PomTK;

	// indeks symbolu rodzica - zawsze w pomocniczej TK
	int idxRodzica;
	// indeks potomka
	int idxPotomka;
	// symbol potomka
	int symbolPotomka;
	// kod symbolu rodzica
	int kodRodzica;
	// d�ugp�� kodu rodzica
	int dlgKoduRodzica;

	// Dynamiczny przydzia� pami�ci dla Tabeli kodowej
	TK = new TKodSymbolu[N];
	// Dynamiczny przydzia� pami�ci dla pomocniczej Tabeli kodowej
	PomTK = new TKodSymbolu[N - 1];

	// Algorytm za czynamy od ostaniego wiersza TDK.
	// W kolumnie "Ojciec" znajduje si� korze� drzewa.
	// Informacje o korzeniu wpisujemy do pomocniczej TK
	PomTK[N - 2].SYMBOL = TDK[N - 2].Ojciec;
	PomTK[N - 2].KOD = 0;
	PomTK[N - 2].DlgKOD = 0;
	// kodujemy symbole zaczynamy od ostaniego wiersza TDK a ko�czymy na wierszy pierwszym
	for (int idxTDK = N - 2; idxTDK >= 0; idxTDK--)
	{
		// okre�lamu indeks symbolu rodzica - zawsze b�dzie to symbol pomocniczy
		idxRodzica = -N - TDK[idxTDK].Ojciec;
		// odczytujemy z pomocniczej TK kod rodzica
		kodRodzica = PomTK[idxRodzica].KOD;
		// odczytujemy z pomocniczej TK d�ugo�� kodu rodzica
		dlgKoduRodzica = PomTK[idxRodzica].DlgKOD;
		
		if (TDK[idxTDK].Potomek0 < 0)
		{
			// okre�lenie indeksu i symbolu potomka - kod potomka ujemny
			idxPotomka = -N - TDK[idxTDK].Potomek0;
			symbolPotomka = TDK[idxTDK].Potomek0;

			// uzupe�nienie informacji w pomocniczej TK - kod potomka ujemny
			PomTK[idxPotomka].SYMBOL = symbolPotomka;
			// Poni�sza instrukcja jest r�wnowa�na dopisaniu zera po prawe stronie.
									PomTK[idxPotomka].KOD = (kodRodzica << 1) + 0;
						PomTK[idxPotomka].DlgKOD = dlgKoduRodzica + 1;
		}
		else
		{
						idxPotomka = TDK[idxTDK].Potomek0;
			symbolPotomka = MZI[TDK[idxTDK].Potomek0].SYMBOL;

			// uzupe�nienie informacji w TK - kod potomka dodatni
			TK[idxPotomka].SYMBOL = symbolPotomka;
			// Poni�sza instrukcja jest r�wnowa�na dopisaniu zera po prawe stronie.
			// Przesuwamy kod rodzica o jedn� pozycj� w lewo (operacja na bitach)
			// i dopisujemy zero po prawej stronie.
			TK[idxPotomka].KOD = (kodRodzica << 1) + 0;
			// okre�lenie d�ugo�ci kodu potomka
			TK[idxPotomka].DlgKOD = dlgKoduRodzica + 1;
		}

		if (TDK[idxTDK].Potomek1 < 0)
		{
			// okre�lenie indeksu i symbolu potomka - kod potomka ujemny
			idxPotomka = -N - TDK[idxTDK].Potomek1;
			symbolPotomka = TDK[idxTDK].Potomek1;
			
			// uzupe�nienie informacji w pomocniczej TK - kod potomka ujemny
			PomTK[idxPotomka].SYMBOL = symbolPotomka;
			// Poni�sza instrukcja jest r�wnowa�na dopisaniu jedynki po prawe stronie.
			// Przesuwamy kod rodzica o jedn� pozycj� w lewo (operacja na bitach)
			// i dopisujemy jedynk� po prawej stronie.
			PomTK[idxPotomka].KOD = (kodRodzica << 1) + 1;
			// okre�lenie d�ugo�ci kodu potomka
			PomTK[idxPotomka].DlgKOD = dlgKoduRodzica + 1;
		}
		else
		{
			// okre�lenie indeksu i symbolu potomka - kod potomka dodatni
			idxPotomka = TDK[idxTDK].Potomek1;
			symbolPotomka = MZI[TDK[idxTDK].Potomek1].SYMBOL;
			
			// uzupe�nienie informacji w TK - kod potomka dodatni
			TK[idxPotomka].SYMBOL = symbolPotomka;
			// Poni�sza instrukcja jest r�wnowa�na dopisaniu jedynki po prawe stronie.
			// Przesuwamy kod rodzica o jedn� pozycj� w lewo (operacja na bitach)
			// i dopisujemy jedynk� po prawej stronie.
			TK[idxPotomka].KOD = (kodRodzica << 1) + 1;
			// okre�lenie d�ugo�ci kodu potomka
			TK[idxPotomka].DlgKOD = dlgKoduRodzica + 1;
		}
	}
	// Usuni�cie pomocniczej TK
	delete PomTK;

	// komunikaty diagnostyczne - wypisanie tekstu na ekranie
	printf("TABELA KODOWA:\n");
	printf("%s\t%s\t%s\n", "Symbol", "Kod (hex)", "Dlg. kodu");

	// Otwarcie pliku do zapisau TK
	PLIK = fopen(nazwaPliku, "wt");

	for (int idx = 0; idx < N; idx++)
	{
		// Zapisanie do pliku wiersza tabeli kodowej
		// 1 - kod symbolu
		// 2 - Kod Huffmana symbolu
		// 3 - Dlugo�� kodu Huffmana symbolu
		fprintf(PLIK, "%d %d %d\n", TK[idx].SYMBOL, TK[idx].KOD, TK[idx].DlgKOD);
		// komunikat diagnostyczny - wypisanie tekstu na ekranie
		printf("%c\t0x%X\t\t%d\n", TK[idx].SYMBOL, TK[idx].KOD, TK[idx].DlgKOD);
	}
	// Zamkni�cie pliku
	fclose(PLIK);
	// komunikat diagnostyczny - wypisanie tekstu na ekranie
	printf("\n\n******************************************\n\n");
	// zwr�cenie TK do miejsca wywo�ania funkcji
	return TK;
}

int SzukajZnaku(TKodSymbolu *TK, int N, char symbol) {
	for (int i = 0; i < N; i++) {
		if ((int)symbol == TK[i].SYMBOL) return i;
	}
	return -1;
}

void Kompr(char *NazwaPlikuWyn, char *NazwaPlikuZr, TKodSymbolu *TK, int N) {
	char buff[256];
	int K = 0, id, LICZBA = 0, MASKA, ILE = 0;
	unsigned char ZNAK, tmp;
	//Odczyt tekstu �r�d�owego
	FILE *plikWej = fopen(NazwaPlikuZr, "rt");
	while (fscanf(plikWej, "%c", &tmp) != -1) {
		if(tmp > 0)
			buff[K++] = tmp;
	}
	fclose(plikWej);

	FILE *plikWyj = fopen(NazwaPlikuWyn, "wb");
	for (int idx = 0; idx < K; idx++) {
		id = SzukajZnaku(TK, N, buff[idx]);
		if (id != -1) {
			LICZBA = LICZBA << TK[id].DlgKOD;
			LICZBA = LICZBA + TK[id].KOD;
			ILE = ILE + TK[id].DlgKOD;
			if (ILE >= 8) {
				MASKA = 0x00FF << (ILE - 8);
				ZNAK = (LICZBA & MASKA) >> (ILE - 8);
				fwrite(&ZNAK, sizeof(ZNAK), 1, plikWyj);
				LICZBA = LICZBA & (~MASKA);
				ILE = ILE - 8;
			}
		}
	}
	if (ILE > 0) {
		MASKA = 0x00FF >> (8 - ILE);
		ZNAK = (LICZBA & MASKA) << (8 - ILE);
		fwrite(&ZNAK, sizeof(ZNAK), 1, plikWyj);
		ZNAK = 8 - ILE;
	}
	else
		ZNAK = 0;
	fwrite(&ZNAK, sizeof(ZNAK), 1, plikWyj);
	fclose(plikWyj);
}

void deKompr(char *NzPlikWej, char *NzPlikWyj, TWezel *TDK, int N, TKodSymbolu *TK) {
	unsigned char buff[256], STOP, SYMBOL, ZNAK;
	int i = 0, DLG = 0, idxTDK = N-2, KodBin;
	FILE *PlikWej = fopen(NzPlikWej, "rb");
	FILE *PlikWyj = fopen(NzPlikWyj, "wt");
	while (fread(&buff[i++], sizeof(unsigned char), 1, PlikWej)) DLG++;
	fclose(PlikWej);
	for (int idx = 0; idx < DLG - 1; idx++) {
		SYMBOL = buff[idx];
		STOP = idx < DLG - 2 ? 1 : 1 << buff[DLG - 1];
		for (unsigned char MASKA = 128; MASKA >= STOP; MASKA >>= 1) {
			if ((MASKA & SYMBOL) > 0) {
				if (TDK[idxTDK].Potomek1 >= 0)
					KodBin = TK[TDK[idxTDK].Potomek1].SYMBOL;
				else
					KodBin = TDK[idxTDK].Potomek1;
			}
			else {
				if (TDK[idxTDK].Potomek0 >= 0)
					KodBin = TK[TDK[idxTDK].Potomek0].SYMBOL;
				else
					KodBin = TDK[idxTDK].Potomek0;
			}
			if (KodBin < 0) idxTDK = -1 * (N + KodBin);
			else {
				ZNAK = KodBin;
				fprintf(PlikWyj, "%c", ZNAK);
				idxTDK = N - 2;
			}
		}
	}
	fclose(PlikWyj);
}

int main()
{
	// Liczba wierszy modelu �r�d�a informacji
	int N = 0;
	
	//Utworzenie modelu �r�d�a informacji
	TElement *MZI = UtworzMZI("PlikWej.txt", "MZI.txt", N);

	// Utworzenie tabeli drzewa kodowania
	TWezel *TDK = UtworzTDK("TDK.txt", MZI, N);

	// Utworzenie tabeli kodowej
	TKodSymbolu *TK = UtworzTK("TK.txt", MZI, TDK, N);

	//Kompresja
	Kompr("PlikWyj.txt", "PlikWej.txt", TK, N);

	//Dekompresja
	deKompr("PlikWyj.txt", "dekomp.txt", TDK, N, TK);
	delete TK;
	delete TDK;
	delete MZI;

	system("pause");

    return 0;
}


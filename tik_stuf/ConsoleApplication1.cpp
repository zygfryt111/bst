/* HUFFMAN, kompresja i dekompresja
Testowane na bitmapie ze zdjeciem pana Huffmana i mp4-ce z 43. odcinkiem Dragon
Ball Super z wynikiem pozytywnym */


#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



#define INPUT_FILENAME "plikwejsciowy.txt"

#define CODE_TREE_OUTPUT_FILENAME "tabelakodowa.txt"

#define CODE_TABLE_OUTPUT_FILENAME "skompresowanedane.txt"

#define ENCODED_OUTPUT_FILENAME "zdekompresowane.huf"

#define DECODED_OUTPUT_FILENAME "in.dec.txt"

#define READ_BUFFER_SIZE 256

#define WRITE_BUFFER_SIZE 256

#define NUM_BYTE_VALUES 256

// Struktura opisujaca wpis modelu zrodla informacji
typedef struct { // Struktura bez nazwy...
	short symbol; // Wartosc symbolu, lub pozniej ujemny identyfikator rodzica
	long count;   // Liczba wystapien symbolu w pliku
} source_model_entry; /* ...ale przez typedef bedzie sie do niej mozna odwolywac
					  jako do typu "source_model_entry" */
					  // Struktura opisujaca model zrodla informacji
typedef struct {
	/* Tablica wpisow. Roznych symboli nigdy nie bedzie wiecej niz roznych
	mozliwych wartosci bajtu */
	source_model_entry entries[NUM_BYTE_VALUES];
	short length; /* Liczba roznych symboli w modelu, czyli dlugosc powyzszej
				  tablicy. Gdyby zrezygnowac z tej struktury i uzyc samej
				  tablicy entries[], to rozmiar i trzeba by bylo przekazywac
				  do kazdej funkcji korzystajacej z modelu, co zasmiecaloby
				  niepotrzebnie ich sygnatury */
} source_model;

// Element tablicowej postaci drzewa kodowania
typedef struct {
	short parent;      // Identyfikator rodzica
	short children[2]; // Identyfikatory / wartosci bajtu dwojki dzieci
} code_tree_node;
// Struktura przechowujaca drzewo kodowania w postaci tablicy
typedef struct {
	code_tree_node *nodes; /* Deklarujemy tablice jako wskaznik na element, bo nie
						   znamy jeszcze jej rozmiaru, chociaz mozna by bylo tak
						   zrobic bo znamy rozmiar maksymalny */
	short length;          // Rozmiar powyzszej tablicy
} code_tree;

// Wiersz tabeli kodowej
typedef struct {
	short symbol; // Wartosc bajtu (dla tekstu - kod ASCII)
	char *code;   // Kod huffmana. jako wskaznik na char, bo bedzie zmiennej dlugosci
} code_table_row;
// Tabela kodowa
typedef struct {
	code_table_row *rows; // Tablica wierszy
	short length;         // Dlugosc tablicy - liczba wierszy
} code_table;

/* Bufor wykorzysytywany do zapisu danych do pliku. Zapisywanie bajt po bajcie
moze byc mniej wydajne, wiec chcemy zbierac w buforze bajty w liczbie
WRITE_BUFFER_SIZE i zapisywac calosc gdy bufor sie zapelni */
typedef struct {
	/* Tablica bajtow bufora. Unsigned char, bo bajty przyjmuja wartosci 0-255.
	Zwykly char tez by sie sprawdzil, ale potem przy diagnostycznym wypisywaniu
	zawartosci bufora na ekran dostawalibysmy ujemne liczby i byloby niefajnie */
	unsigned char data[WRITE_BUFFER_SIZE];
	int num_buffered_bytes; // Liczba bajtow zapisanych do bufora
							/* Uchwyt pliku, do ktorego bufor po zapelnieniu zapisze swoje dane. Moznaby go
							przekazywac oddzielnie jako argument, ale tak wychodzi schludniej */
	FILE* file;
} write_buffer;

/* Deklaracje funkcji tutaj, bo jak jest main na gorze to fajnie jest wtedy.
Inaczej kompilator by narzekal ze nie zna sygnatur tych funkcji */
/* Wrapper na fopen, ktory po probie otworzeniu pliku sprawdza, czy sie udalo.
Jezeli nie, wypisuje komunikat i zamyka program. Jezeli tak, to zwraca uchwyt */
FILE* fopen_except(const char*, const char* mode);
// Zlicza wystapienia symboli w pliku, zwraca tablice ze zliczeniami
int* count_symbols(FILE*);
// Tworzy model zrodla z tablicy wystapien symboli
source_model* create_source_model(int[]);
// Porownuje wpisy modelu zrodla po liczbie zliczen
int compare_source_model_entries_by_count_desc(const void*, const void*);
// Wykorzystuje powyzsza funkcje do posortowania modelu zrodla po liczbie zliczen
void sort_source_model(source_model*);
// Tworzy z modelu zrodla drzewo kodowania w postaci tablicy
code_tree* create_code_tree(source_model*);
// Wypisuje do strumienia drzewo kodowania w postaci tabelki
void fprint_code_tree(FILE*, code_tree*);
// Tworzy z drzewa kodowania tabele kodowa
code_table* create_code_table(code_tree*);
// Wypisuje do strumienia tabele kodowa
void fprint_code_table(FILE*, code_table*);
// Wyszukuje w tabeli kodowej symbol i zwraca jego kod huffmana
char* get_code_for_symbol_from_code_table(short, code_table*);
// Koduje plik na podstawie tabeli kodowej i zapisuje wynik
void encode(FILE*, code_table*, FILE*);
// Dekoduje plik na podstawie drzewa kodowania i zapisuje wynik
void decode(FILE*, code_tree*, FILE*);
// Zapisuje bajt do bufora zapisu
void write_byte_to_buffer(unsigned char, write_buffer*);
// Zapisuje zawartosc bufora zapisu do pliku
void write_buffer_to_file(write_buffer*);

int main(int argc, char *argv[]) {
	// Wczytujemy plik wejsciowy do kompresji
	FILE *input = fopen_except(INPUT_FILENAME, "rb");
	printf("Plik '%s' zostal wczytany\n", INPUT_FILENAME);

	// Zliczamy liczby wystapien symboli w pliku. Wynik otrzymujemy w postaci tablicy
	int *sc = count_symbols(input);

	// Majac liczby wystapien mozemy utworzyc model zrodla informacji
	source_model *sm = create_source_model(sc);

	/* Zgodnie z wymaganiami sortujemy model zrodla informacji. Nie musimy tworzyc
	kopii i zwracac nowego. Wystarczy, ze przekazemy wskaznik i zrobimy to w
	miejscu */
	sort_source_model(sm);

	// Z posortowanego modelu zrodla informacji tworzymy drzewo kodowania
	code_tree *ctr = create_code_tree(sm);

	// Otwieramy plik, w ktorym ma sie znalezc drzewo kodowe
	FILE *ctr_output = fopen_except(CODE_TREE_OUTPUT_FILENAME, "wb");
	fprint_code_tree(ctr_output, ctr); // Wpisujemy drzewo do pliku ctr_output
	printf("Tablica drzewa kodowania zostala zapisana w pliku '%s'\n",
		CODE_TREE_OUTPUT_FILENAME);
	fclose(ctr_output); // Pamietamy o zamknieciu pliku

						// Z drzewa kodowania wyznaczamy tabele kodowa
	code_table *ctb = create_code_table(ctr);

	// Tabele kodowa zapisujemy do pliku, analogicznie jak wyzej drzewo
	FILE *ctb_output = fopen_except(CODE_TABLE_OUTPUT_FILENAME, "wb");
	fprint_code_table(ctb_output, ctb);
	printf("Tablica kodowa zostala zapisana w pliku '%s'\n",
		CODE_TABLE_OUTPUT_FILENAME);
	fclose(ctb_output);

	// Otwieramy plik, ktory zawieral bedzie zakodowane dane
	FILE *encoded_output = fopen_except(ENCODED_OUTPUT_FILENAME, "wb");
	// Kodujemy plik wejsciowy przy pomocy tabeli kodowej i zapisujemy do pliku
	encode(input, ctb, encoded_output);
	printf("Plik wejsciowy zostal skompresowany do pliku '%s'\n",
		ENCODED_OUTPUT_FILENAME);

	// Plik wejsciowy zakodowany. Przy dekodowaniu nie potrzebujemy oryginalu
	fclose(input);

	/* Teraz pora na dekodowanie. Warto zaznaczyc, ze w kompletnym programie
	mielibysmy oddzielne uruchomienia programu dla kompresji i dekompresji
	(rozrozniane parametrami uruchomieniowymy, ewentualnie poprzez opcje w menu),
	a drzewo kodowe byloby dopisywane do pliku skompresowanego i stamtad
	odczytywane (obecnie korzystamy z drzewa, ktore nadal egzystuje sobie w
	pamieci po tym jak utworzylismy je przy kompresji */
	// Potrzebny bedzie plik zakodowany
	FILE *encoded_input = fopen_except(ENCODED_OUTPUT_FILENAME, "rb");
	// I wyjsciowy plik, w ktorym zapiszemy dane zdekodowane
	FILE *decoded_output = fopen_except(DECODED_OUTPUT_FILENAME, "wb");
	// Dekodujemy plik zakodowany przy pomocy drzewa kodowania i zapisujemy do pliku
	decode(encoded_input, ctr, decoded_output);
	printf("Plik '%s' zostal zdekompresowany do pliku '%s'\n",
		ENCODED_OUTPUT_FILENAME, DECODED_OUTPUT_FILENAME);
	fclose(encoded_input);

	/* Czyscimy smieci, bo to nie Java i profesjonalna smieciarka firmy Oracle
	nie przyjedzie. IBM-u tez nie */
	free(sm);
	free(ctr->nodes);
	free(ctr);
	free(ctb->rows);
	free(ctb);

	getchar();
	exit(EXIT_SUCCESS); // Wychodzimy z kodem zadowolonym
}

/* Wrapper na fopen, ktory po probie otworzeniu pliku sprawdza, czy sie udalo.
Wejscie: nazwa pliku, tryb
Wyjscie: uchwyt otworzonego pliku lub brak i efekt uboczny w postaci wypisania
komunikatu o bledzie i wyjscia z programu z kodem negatywnym */
FILE* fopen_except(const char* filename, const char* mode) {
	FILE *fp = fopen(filename, mode);

	if (!fp) { // Jezeli uchwyt pusty
		fprintf(stderr, "Nie mozna otworzyc pliku %s\n", filename);
		exit(EXIT_FAILURE);
	}

	return fp;
}

/* Zlicza wystapienia symboli w pliku, zwraca tablice ze zliczeniami
Wejscie: uchwyt pliku
Wyjscie: tablica zliczen
*/
int* count_symbols(FILE *fp) {
	/* Alokujemy miejsce na tablice zliczen. NUM_BYTE_VALUES wierszy (256, bo tyle
	roznych symboli moglo wystapic w pliku - indeks w tablicy odpowiada wartosci
	bajtu danego symbolu) po sizeof(int) bajtow. Calloc w przeciwienstwie do
	malloca inicjuje pamiec do 0. Nie chcemy miec tam smieci, bo bedziemy
	odrzucac symbole z zerowa liczba zliczen. */
	int *sc = (int*)calloc(NUM_BYTE_VALUES, sizeof(int));

	// Bufor dla bajtow odczytanych z pliku wejsciowego
	unsigned char buffer[READ_BUFFER_SIZE];
	while (!feof(fp)) { // Dopoki nie napotkamy na koniec pliku
						/* Czytamy do bufora READ_BUFFER_SIZE paczek o wielkosci sizeof(unsigned char)
						z pliku fp. Dostajemy liczbe przeczytanych bajtow, bo moze sie zdarzyc, ze
						nie zapelnimy calego bufora zanim dojdziemy do konca pliku */
		size_t bytes_read = fread(buffer, sizeof(unsigned char), READ_BUFFER_SIZE, fp);

		for (int i = 0; i < bytes_read; i++) { // Dla kazdego odczytanego bajtu
											   /* Pamietamy, ze indeks w tablicy zliczen odpowiada wartosci bajtu symbolu.
											   Jezeli wiec zwiekszymy liczbe zliczen pod indeksem rownym wartosci bajtu
											   napotkanego symbolu, to zliczymy wystapienie tego symbolu */
			sc[buffer[i]]++;
		}
	}

	return sc; // Zwracamy wskaznik na pierwszy element, czyli tablice
}

/* Tworzy na podstawie tablicy wystapien model zrodla i go zwraca
Wejscie: tablica zliczen
Wyjscie: model zrodla informacji */
source_model* create_source_model(int *sc) {
	// Alokujemy miejsce na strukture przechowywujaca model zrodla
	source_model *sm = (source_model*)malloc(sizeof(source_model));
	sm->length = 0; // Inicjujemy rozmiar na 0

					/* Iterujemy po wszystkich mozliwych wartosciach bajtu. Wystarczylby nawet
					unsigned char zamiast shorta, ale nie ma wiekszej roznicy */
	for (short i = 0; i < NUM_BYTE_VALUES; i++) {
		/* Tworzymy wpis w modelu zrodla tylko gdy liczba zliczen danego symbolu jest
		wieksza od 0 */
		if (sc[i] > 0) {
			/* Pamietajmy, ze indeks nowego wiersza tablicy bedzie zawsze rowny jej
			dlugosci (np. dlugosc = 1? - znaczy, ze mamy jeden element pod indeksem 0,
			a wiec nowy wstawimy pod indeksem 1, co rowne jest dlugosci) */
			/* Indeksy w tablicy zliczen odpowiadaja wartosciom symboli, wiec stad
			zapisujemy symbol */
			sm->entries[sm->length].symbol = i;
			/* Liczba zliczen to z kolei wartosc zapisana pod tym indeksem */
			sm->entries[sm->length].count = sc[i];
			sm->length++; // Dodalismy wpis, odnotowujemy to w zmiennej przech. dlugosc.
		}
	}

	return sm;
}

/* Porownuje wpisy modelu zrodla po liczbie zliczen.
Wejscie: dwa wskazniki na elementy dowolnego typu
Wyjscie: wynik porownania
Taka sygnatura jest wymagana przez funkcje qsort. */
int compare_source_model_entries_by_count_desc(const void *se1, const void *se2) {
	/* Mamy wskazniki na cokolwiek, ale wiemy, ze sa tam elementy modelu zrodla.
	Zeby kompilator dal nam stamtad wydobyc nasza liczbe zliczen, z nim tez
	musimy sie podzielic ta informacja, stad te rzutowania */
	int c1 = ((source_model_entry*)se1)->count;
	int c2 = ((source_model_entry*)se2)->count;

	// -1 gdy c1 > c2, 0 gdy c1 = c2, 1 gdy c1 < c2 (bo sortujemy malejaco!)
	return -((c1 > c2) - (c1 < c2));
}

/* Sortuje model zrodla informacji malejaco wg liczby zliczen symbolu
Wejscie: tablica zliczen
Wyjscie: brak, tablica zliczen modyfikowana jako efekt uboczny */
void sort_source_model(source_model *sm) {
	/* Wykorzystujemy funkcje qsort z biblioteki standardowej. Funkcja ta jest
	zadeklarowana w stdlib.h */
	qsort(sm->entries, // Wskaznik na tablice
		sm->length,  // Dlugosc tablicy
		sizeof(sm->entries[0]), // Rozmiar elementu. Wszystkie maja jednakowy
		&compare_source_model_entries_by_count_desc); // Wsk. na f. porownujaca
}

/* Tworzy z modelu zrodla drzewo kodowania w postaci tablicy
Wejscie: model zrodla informacji
Wyjscie: drzewo kodowania w postaci tablicy */
code_tree* create_code_tree(source_model *sm) {
	// Standardowo alokujemy miejsce w pamieci
	code_tree* ctr = (code_tree*)malloc(sizeof(code_tree));
	// Wiemy, ze tablica bedzie miala o jeden wpis mniej niz model zrodla...
	ctr->length = sm->length - 1;
	// ...wiec alokujemy miejsce w pamieci na tyle wpisow
	ctr->nodes = (code_tree_node*)malloc(ctr->length * sizeof(code_tree_node));

	/* 'i' jedzie po modelu zrodla od konca, tj. od symbolu o najmniejszej czestosci
	'j' jedzie po naszej nowej tablicy drzewa kodowania od poczatku */
	for (int i = sm->length - 1, j = 0; i > 0; i--, j++) {
		// Wzor na identyfikator rodzica. Zaczynamy numerowac od -(l. symboli)
		ctr->nodes[j].parent = -(sm->length + j);
		/* Sklejamy dwa symbole o najmniejszej czestosci, czyli ten, ktory obecnie
		przetwarzamy i ten powyzej niego */
		ctr->nodes[j].children[0] = sm->entries[i].symbol;
		ctr->nodes[j].children[1] = sm->entries[i - 1].symbol;

		/* Zastepujemy (i-1)-szy symbol rodzicem sklejonych powy¿ej symboli.
		Liczba zliczen dla symbolu sklejonego bedzie suma sklejonych symboli */
		sm->entries[i - 1].symbol = ctr->nodes[j].parent;
		sm->entries[i - 1].count = sm->entries[i].count + sm->entries[i - 1].count;
		/* Zerujemy liczbe zliczen i-tego, niepotrzebnego juz (bo wszedl w sklad
		symbolu sklejonego) wpisu, zeby nam przypadkiem nie wskoczyl na znaczace
		miejsce po posortowaniu. Symbol moze sobie zostac w pamieci, bo tamtym
		obszarem tablicy juz sie nie bedziemy zajmowac (iterujemy od konca do
		poczatku), a potem i tak wszystko zwolnimy. */
		sm->entries[i].count = 0;

		/* Sortujemy model zrodla. Wystarczyloby przebabelkowac nowy, sklejony symbol
		do gory, ale jestemy leniwi i w sumie roznica w wydajnosci jest minimalna
		albo nie ma jej wcale */
		sort_source_model(sm);
	}

	return ctr;
}

/* Wypisuje do strumienia drzewo kodowania w postaci tabelki
Wejscie: strumien wyjsciowy, drzewo kodowania
Wyjscie: brak, efekt uboczny w postaci wpisania drzewa do strumienia */
void fprint_code_tree(FILE* stream, code_tree *ctr) {
	fprintf(stream, "ojciec potomek0 potomek1\n"); // Naglowek
	for (int i = 0; i < ctr->length; i++) { // Dla kazdego elementu
											// Wartosci rodzica i obojga dzieci, oddzielone spacjami i zakonczone nowa l.
		fprintf(stream, "%d %d %d\n", ctr->nodes[i].parent,
			ctr->nodes[i].children[0],
			ctr->nodes[i].children[1]);
	}
}

/* Tworzy tabele kodowa na podstawie drzewa kodowania
Wejscie: drzewo kodowania
Wyjscie: tabela kodowa */
code_table* create_code_table(code_tree *ctr) {
	code_table* ctb = (code_table*)malloc(sizeof(code_table));
	/* Dlugosc drzewa kodowania byla rowna dlugosci modelu zrodla pomniejszonej o 1.
	W tabeli kodowej bedzie tyle samo symboli co w modelu zrodla, wiec odwracamy
	operacje */
	ctb->length = ctr->length + 1;
	ctb->rows = (code_table_row*)malloc(ctb->length * sizeof(code_table_row));

	/* Pomocnicza tabela kodowa, ktora bedzie przechowywala przedrostki huffmana
	symboli-rodzicow */
	code_table* actb = (code_table*)malloc(sizeof(code_table));
	actb->length = ctr->length;
	actb->rows = (code_table_row*)malloc(actb->length * sizeof(code_table_row));

	/* Teoretyczna maksymalna dlugosc slowa kodowego jest (chyba) rowna liczbie
	symboli pomniejszonej o 1 */
	int max_code_length = ctb->length - 1;

	// Zaczynamy wypelnianie pomocniczej TK od konca, czyli korzenia drzewa kodowania
	code_table_row* actb_last_row = &actb->rows[actb->length - 1];
	/* Korzen jest rodzicem w ostatnim wpisie w drzewie kodowania, wiec przepisujemy
	stamtad jego kod */
	actb_last_row->symbol = ctr->nodes[ctr->length - 1].parent;
	/* Przedrostek korzenia jest pusty, wiec wystarczy zaalokowac miejsce na jeden
	bajt zerowy konczacy ciag znakow i go tam wpisac */
	actb_last_row->code = (char*)malloc(sizeof(char));
	actb_last_row->code[0] = '\0';

	/* Po drzewie kodowania jedziemy od konca, a po tworzonej tabeli kodowej od
	poczatku */
	for (int ctr_idx = ctr->length - 1, ctb_idx = 0; ctr_idx >= 0; ctr_idx--) {
		/* Wzorek na indeks rodzica obecnego elementu drzewa kodowania w pomocniczej
		tabeli kodowej */
		int parent_actb_index = -ctr->length - ctr->nodes[ctr_idx].parent - 1;
		/* Pobieramy kod tego rodzica z tabeli pomocniczej. Bedziemy go uzywac do
		wyznaczenia kodu jego dzieci. Kod jest stringiem */
		char *parent_code = actb->rows[parent_actb_index].code;
		// Dlugosc kodu rodzica. Funkcja strlen() z string.h zwraca dlugosc stringa
		int parent_code_length = strlen(parent_code);
		for (int i = 0; i < 2; i++) { // Dla kazdego dziecka
									  // Pobieramy symbol dziecka z drzewa
			short symbol = ctr->nodes[ctr_idx].children[i];

			/* Tworzymy wskaznik na wiersz tabeli kodowej, do ktorego wpiszemy dziecko.
			Jezeli dziecko jest niesklejonym ("prawdziwym") symbolem, czyli ma
			wartosc nieujemna (odpowiadajaca wartosci bajtu symbolu), to bierzemy
			obecny wiersz z glownej tabeli kodowej i przy okazji popychami indeks na
			wiersz nastepny.
			Jezeli dziecko jest symbolem sklejonym, czyli ma wartosc ujemna, to
			wpiszemy go do tabeli pomocniczej. Indeks obliczamy na podstawie wzoru
			zastosowanego juz wyzej.
			Ponizsze wyrazenie to operator warunkowy trojargumentowy. To, co
			przypiszemy do zmiennej target_row zalezy od wyniku wyrazenia
			(symbol >= 0). Jezeli bedzie prawdziwe, to przypiszemy wynik wyrazenia po
			znaku zapytania, a jezeli nie - wynik wyrazenia po dwukropku. */
			code_table_row* target_row =
				symbol >= 0 ? &ctb->rows[ctb_idx++]
				: &actb->rows[-ctr->length - symbol - 1];

			// Ustawiamy symbol w wyznaczonym wierszu
			target_row->symbol = symbol;
			/* Alokujemy miejsce na kod. Bedzie zajmowal tyle co kod rodzica
			(przedrostek) plus jeden znak ('0' lub '1') */
			target_row->code = (char*)malloc((parent_code_length + 1) * sizeof(char));
			// Najpierw kopiujemy kod rodzica. Funkcja kopiowania strcpy() ze string.h
			strcpy(target_row->code, parent_code);
			/* Na kolejnym miejscu (indeks rowny dlugosci przekopiowanego ciagu)
			wstawiamy '0' dla lewego dziecka (czyli indeks rowny 0) lub '1' dla
			prawego (czyli indeks rowny 1) */
			target_row->code[parent_code_length] = i == 0 ? '0' : '1';
			// Konczymy ciag znakow bajtem zerowym
			target_row->code[parent_code_length + 1] = '\0';
		}
	}

	/* Zwalniamy pamiec po tabeli pomocniczej, ktorej nie bedziemy juz potrzebowali.
	Wiersze byly zadeklarowane jako wskaznik, wiec musimy je zwolnic oddzielnie.
	*/
	free(actb->rows);
	free(actb);

	return ctb; // Zwracamy utworzona tabele kodowa
}

/* Wypisuje do strumienia tabele kodowa
Wejscie: strumien wyjsciowy, tabela kodowa
Wyjscie: brak, efekt uboczny w postaci wpisania tabeli do strumienia */
void fprint_code_table(FILE* stream, code_table *ctb) {
	fprintf(stream, "symbol kod\n");
	for (int i = 0; i < ctb->length; i++) {
		fprintf(stream, "%d %s\n", ctb->rows[i].symbol, ctb->rows[i].code);
	}
}

/* Wyszukuje w tabeli kodowej symbol i zwraca jego kod huffmana
Wejscie: symbol (wartosc bajtu), tabela kodowa
Wyjscie: kod huffmana (wskaznik na tablice znakow) */
char* get_code_for_symbol_from_code_table(short symbol, code_table* ctb) {
	// TODO: przyspieszyc wykorzystujac BST
	for (int i = 0; i < ctb->length; i++) { // Przeszukujemy tabele kodowa
		if (ctb->rows[i].symbol == symbol) { // Jezeli natrafimy na poszukiwany sym....
			return ctb->rows[i].code; // ...zwracamy jego kod
		}
	}

	return NULL; // Jezeli nic nie znalezlismy, zwracamy NULL
}

/* Koduje plik wejsciowy przy pomocy tabeli kodowej i zapisuje wynik do pliku
Wejscie: uchwyt na oryginalny plik wejsciowy, tabela kodowa,
uchwyt na plik wyjsciowy
Wyjscie: brak, efekt uboczny w postaci wpisania zakodowanych danych do pliku */
void encode(FILE* input, code_table* ctb, FILE* encoded_output) {
	// Uzywane kasety przewijamy do poczatku przed kolejnym uzyciem. Pliki tez
	rewind(input);

	// Tworzymy bufor, ktorego uzyjemy do zapisu
	write_buffer* wb = (write_buffer*)malloc(sizeof(write_buffer));
	wb->num_buffered_bytes = 0; // Inicjujemy liczbe zbuforowanych bajtow
	wb->file = encoded_output; /* Podajemy uchwyt do pliku, do ktorego bedziemy
							   zapisywac */

							   // Bedziemy tez czytac z oryginalnego pliku, wiec przyda sie drugi bufor
	unsigned char read_buffer[READ_BUFFER_SIZE];
	// Do tego bajtu wprowadzac bedziemy bity oczekujace na zapis
	unsigned char bit_buffer = 0;
	// A tutaj bedziemy sledzic ile bitow juz wprowadzilismy
	short num_buffered_bits = 0;
	while (!feof(input)) { // Dopoki nie dojedziemy do konca pliku wejsciowego
						   // Czytamy paczke READ_BUFFER_SIZE bajtow do read_buffer z pliku input
		size_t bytes_read = fread(read_buffer,
			sizeof(unsigned char),
			READ_BUFFER_SIZE,
			input);
		for (int i = 0; i < bytes_read; i++) { // Dla kazdego odczytanego bajtu
											   // Bajt jest symbolem do zakodowania. W tabeli kodowej wyszukujemy jego kod
			char *code = get_code_for_symbol_from_code_table(read_buffer[i], ctb);

			/* Jedziemy od poczatku do konca kodu. Kod jest ciagiem znakow, wiec jego
			koniec zwieszczy bajt zerowy */
			for (int j = 0; code[j] != '\0'; j++) {
				/* Przesuwamy bufor bitow o bit w lewo, zeby nie nadpisac poprzednio
				zapisanego bitu (nie dotyczy pierwszego obiegu). Na nowym miejscu
				bedziemy mieli 0 (tak dziala przesuniecie logiczne) */
				bit_buffer <<= 1;
				/* Jezeli na obecnym miejscu w kodzie mamy jedynke, to trzeba ja ustawic
				(obecnie mamy zero). */
				if (code[j] == '1') {
					/* Ustawiamy bit na najmniej znaczacym miejscu poprzez OR bufora z
					00000001 */
					bit_buffer |= 1;
				}
				num_buffered_bits++; // Zaznaczamy ¿e dopisalismy bit

				if (num_buffered_bits == 8) { // Jezeli bufor bitow jest pelny
											  // Zapisujemy nowy bajt do glownego bufora zapisu...
					write_byte_to_buffer(bit_buffer, wb);
					// ...i zerujemy bufor bitow zeby byl gotowy na kolejne dane
					bit_buffer = 0;
					num_buffered_bits = 0;
				}
			}
		}

	}

	/* Koniec petli. przetowrzylismy juz cale wejscie, jednak w buforze bitow moga
	byc jeszcze bity-sieroty ktore nieuzbieraly nam sie w caly bajt */
	if (num_buffered_bits > 0) { // Jezeli mamy takie bity...
								 /* ...to mamy je na miejscach najmniej znaczacych, a chcemy na najbardziej
								 znaczacych, wiec przesuwamy... */
		bit_buffer <<= 8 - num_buffered_bits;
		write_byte_to_buffer(bit_buffer, wb); // ...i zapisujemy ostatni bajt
	}

	/* A na koniec jeszcze bajt z liczba bitow uzytych w ostatnim bajcie kodowym,
	ktora bedzie niezbedna przy dekodowaniu */
	write_byte_to_buffer(num_buffered_bits, wb);

	/* Jezeli glowny bufor zapisu ma jeszcze oczekujace bajty, to to jest ten moment,
	w je zapiszemy konczac caly watek */
	if (wb->num_buffered_bytes > 0) {
		write_buffer_to_file(wb);
	}

	// Zwalniamy pamiec po buforze zapisu...
	free(wb);

	// ...i plik wyjsciowy
	fclose(encoded_output);
}

/* Dekoduje plik zakodowany przy pomocy drzewa kodowania i zapisuje do pliku
Wejscie: uchwyt na zakodowany plik wejsciowy, drzewo kodowania,
uchwyt na plik wyjsciowy
Wyjscie: brak, efekt uboczny w postaci wpisania zdekodowanych danych do pliku */
void decode(FILE* input, code_tree* ctr, FILE* decoded_output) {
	/* Chcemy sprawdzic zapisana wczesniej informacje o liczbie bitow
	wykorzystanych w ostatnim bajcie kodowym, ktora sie niebawem przyda.
	Znajduje sie ona w ostatnim bajcie pliku, wiec do tego miejsca przewijamy
	tez nasza kastete... */
	fseek(input, -1L, SEEK_END);
	// ...i pobieramy bajt...
	short num_last_code_byte_used_bits = fgetc(input);
	/* ...co rownoczesnie przewinie nam plik do konca, a ftell wie o ile lacznie
	przewinelismy i nam nawet, zgodnie z nazwa, powie, a my z tego bedziemy mieli
	dlugosc zakodowanego pliku w bajtach */
	unsigned long input_file_size = (unsigned long)ftell(input);
	rewind(input); // Jestemy na koncu, przewijamy na poczatek

				   // Alokujemy sobie bufor zapisu, analogicznie jak przy kompresji
	write_buffer* wb = (write_buffer*)malloc(sizeof(write_buffer));
	wb->num_buffered_bytes = 0;
	wb->file = decoded_output; // Bufor prosimy o wyrzucanie sie do pliku wyj.

							   // Bedziemy czytac plik zakodowany, potrzebujemy bufora
	unsigned char read_buffer[READ_BUFFER_SIZE];

	/* Bedziemy poruszac sie po drzewie kodowania od korzenia, a potem po dzieciach,
	w zaleznosci od odczytanego bitu (0 -> lewe dziecko, 1 -> prawe). Potrzebujemy
	wskaznika na korzen, ktory jest ostatnim elementem drzewa. */
	code_tree_node* ctr_node = &ctr->nodes[ctr->length - 1];
	// Tutaj przechowywac bedziemy aktualny identyfikator / wartosc bajtu symbolu
	short symbol;

	// Zliczamy na ktorym bajcie pliku jestesmy, zeby wiedziec kiedy przestac czytac
	unsigned long current_byte_global_idx = 0;
	// Dopoki nie osiagniemy konca pliku (nie osiagniemy, ale przezorny costam costam)
	while (!feof(input)) {
		// Czytamy bajty do bufora
		size_t bytes_read = fread(read_buffer,
			sizeof(unsigned char),
			READ_BUFFER_SIZE,
			input);
		/* Jedziemy po przeczytanych bajtach dopoki albo sie nie skoncza, albo nie
		dojdziemy do przedostatniego bajtu calego pliku (w ostatnim mamy bajt
		informacyjny, ktory przeczytalismy juz wyzej) */
		for (int i = 0, num_bits_to_read = 8;
		i < bytes_read && current_byte_global_idx < input_file_size - 1;
			i++, current_byte_global_idx++) {
			// Przepisujemy sobie odczytany bajt
			unsigned char byte = read_buffer[i];

			// Jezeli jestesmy na przedostatnim bajcie
			if (current_byte_global_idx == input_file_size - 2) {
				/* Ustawiamy zmienna iteracyjna, dzieki czemu przeczytamy tylko tyle bitow
				z ostatniego bajtu ile powinnismy, nie koniecznie wszystkie */
				num_bits_to_read = num_last_code_byte_used_bits;
				/* Przesuwamy bity w prawo, bo mamy je na miejscach najbardziej znaczacych,
				a czytamy z miejsc najmniej znaczacych */
				byte >>= 8 - num_bits_to_read;
			}

			// Jedziemy po bitach naszego bajtu, od najbardziej znaczacego.
			for (int j = num_bits_to_read - 1; j >= 0; j--) {
				/* Zmienna 'j' okresla przesuniecie bajtu, wiec np. dla bajtu 1010000 i
				poczatkowego j=7, przeksztalcimy nasz bajt na postac 00000001 i
				odczytamy wartosc ostatniego bitu ANDujac go z 00000001. Dla kolejnych
				dwoch obiegow odczytamy kolejno 0 i 1. */
				char bit = (byte >> j) & 1;

				/* Pierwsze dziecko odpowiada wartosci '0', a drugie '1', co sie
				szczesliwie pokrywa z indeksami w tablicy dzieci, totez mozemy nasz
				symbol (czyli wartosc bajtu lub ujemny identyfikator w przypadku
				symbolu sklejonego) odczytac nastepujaco: */
				symbol = ctr_node->children[bit];
				if (symbol >= 0) { // Jezeli niesklejony symbol
								   // Mamy kompletny kod, znalezlismy koncowy symbol, zapisujemy go do buf.
					write_byte_to_buffer(symbol, wb);
					// Wracamy do korzenia drzewa, aby rozpoczac odkodowywanie nast. ciagu
					ctr_node = &ctr->nodes[ctr->length - 1];
				}
				else { // Jezeli sklejony
					   /* W drzewie idziemy do dziecka wybranego przez odczytany bit. Wzorek
					   juz znany */
					ctr_node = &ctr->nodes[-ctr->length - symbol - 1];
				}
			}
		}
	}

	/* Koniec petli, odczytalismy juz wszystko. W buforze mogly jednak pozostac
	niezapisane jeszcze do pliku bajty. Jezeli takie sa, to zapisujemy je teraz*/
	if (wb->num_buffered_bytes > 0) {
		write_buffer_to_file(wb);
	}

	free(wb);

	fclose(decoded_output);
}

/* Zapisuje bajt do bufora zapisu. W przypadku zapelnienia bufora wywoluje zapis
jego zawartosci do pliku wyjsciowego.
Wejscie: wartosc bajtu, wskaznik na bufor zapisu
Wyjscie: brak, efekt uboczny w postaci zapisania bajtu w buforze, mozliwy
efekt uboczny w postaci zapisania zawartosci bufora do pliku */
void write_byte_to_buffer(unsigned char byte, write_buffer* wb) {
	// Zapisujemy bajt na nastepnym wolnym miejscu...
	wb->data[wb->num_buffered_bytes] = byte;
	wb->num_buffered_bytes++; // ...i odnotowujemy ten fakt

							  // Jezeli bufor jest pelny
	if (wb->num_buffered_bytes == WRITE_BUFFER_SIZE) {
		write_buffer_to_file(wb); // Zapisujemy zawartosc bufora do pliku
	}
}

/* Zapisuje zawartosc bufora zapisu do pliku
Wejscie: bufor zapisu
Wyjscie: brak, efekt uboczny w postaci zapisania zawartosci bufora do pliku
*/
void write_buffer_to_file(write_buffer* wb) {
	/* Zapisujemy z wb->data porcje rozmiaru sizeof(unsigned char) w liczbie
	wb->num_buffered_bytes do pliku wb->file */
	fwrite(wb->data, sizeof(unsigned char), wb->num_buffered_bytes, wb->file);
	/* Zerujemy licznik. Komorek zerowac nie musimy, bo i tak nie zapiszemy wiecej
	niz mamy w liczniku, a tyle ile bedzie w liczniku zawsze wczesniej nadpiszemy
	*/
	wb->num_buffered_bytes = 0;
}

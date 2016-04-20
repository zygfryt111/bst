/* TIK lab. 1.
 * 
 * Napisa� program, kt�ry odczytuje plik tekstowy i wykonuje nast�puj�ce
 * operacje:
 *   1. Podaje znaki u�yte w pliku.
 *   2. Zlicza wyst�pienie ka�dego znaku w pliku wej�ciowym.
 *   3. Wynik zapisuje do pliku wyj�ciowego.
 *
 *
 * Wystarczy�oby pewnie obs�ugiwa� tylko zwyk�y 7-bitowy ASCII. Tutaj jest te�
 * rozszerzony, bo niczego to nie komplikuje.
 * Testowane und dzia�a na:
 *   - GCC 5.3.0, Linux 4.3.5 x64, standard C11 (-std=c11);
 *   - GCC 3.4.2 mingw-special [Dev-C++ 4.9.9.2], Windows XP x32, standard C99
 *     (Dev-C++: Tools -> Compiler... -> "Add the following..." -> "-std=c99");
 *     Dev-C++ z jakiego� powodu nie potrafi poprawnie odczyta� tego pliku
 *     (kodowanie UTF-8) i wy�wietla krzaki w miejsce polskich znak�w. Podobny
 *     problem ma windowsowy command prompt przy wy�wietlaniu napis�w.
 */

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#define INPUT_FILENAME "in.txt"
#define OUTPUT_FILENAME "out.txt"
// rozmiar bufora, do kt�rego wczytywana jest porcja danych z pliku wej�ciowego
#define BUFFER_SIZE 256
// liczba znak�w w rozszerzonym ASCII
#define EXTENDED_ASCII_SIZE 256

// para znak-liczba wyst�pie�
typedef struct {
  unsigned char symbol;
  int count;
} symbol_entry;

void count_symbols(FILE*, int[]);
int create_symbol_entries(int[], symbol_entry*);
int compare_entries_by_count_desc(const void*, const void*);
char* get_printable_representation(unsigned char);
void print_summary(FILE*, symbol_entry*, int);

int main(int argc, char *argv[]) {
  int i;

  FILE *input = fopen(INPUT_FILENAME, "r");
  if (input == NULL) {
    fprintf(stderr, "Nie mo�na otworzy� pliku %s\n", INPUT_FILENAME);
    exit(EXIT_FAILURE);
  }
  printf("Wczytano plik %s\n\n", INPUT_FILENAME);

  // liczby wyst�pie� w pliku ka�dego ze znak�w rozszerzonego ASCII
  int symbol_counts[EXTENDED_ASCII_SIZE] = {0}; 
  
  // wype�nia symbol_counts na podstawie pliku
  count_symbols(input, symbol_counts);

  // pary znak-liczba wyst�pie� dla znak�w z niezerow� liczb� wystapie�
  symbol_entry *entries = malloc(EXTENDED_ASCII_SIZE * sizeof(unsigned char));

  // wype�nia entries na podstawie tablicy symbol_counts
  int entries_size = create_symbol_entries(symbol_counts, entries);

  // wykonuje alg. quicksort wg f. por�wnawczej compare_entries_by_count_desc
  qsort(entries, 
        entries_size, 
        sizeof(*entries), 
        &compare_entries_by_count_desc);

  // wypisuje podsumowanie na ekran
  print_summary(stdout, entries, entries_size);

  fclose(input);

  FILE *output = fopen(OUTPUT_FILENAME, "w");
  if (input == NULL) {
    fprintf(stderr, "Nie mo�na stworzy�/otworzy� pliku %s\n", OUTPUT_FILENAME);
    exit(EXIT_FAILURE);
  }

  print_summary(output, entries, entries_size);
  printf("\nZapisano podsumowanie do pliku %s\n", OUTPUT_FILENAME);

  fclose(output);

  free(entries);

  exit(EXIT_SUCCESS);
}

// zlicza l. wyst�pie� ka�dego ze znak�w w pliku i wpisuje do tablicy
void count_symbols(FILE *fp, int *symbol_counts) {
  unsigned char buffer[BUFFER_SIZE];

  while (!feof(fp) && !ferror(fp)) {
    size_t bytes_read = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, fp);

    for (int i = 0; i < bytes_read; i++) {
      symbol_counts[buffer[i]]++;
    }
  }
}

// tworzy wpisy dla znak�w na podstawie tablicy zlicze� i zwraca liczb� wpis�w
int create_symbol_entries(int symbol_counts[], symbol_entry *entries) {
  int entries_size = 0;

  for (int i = 0; i < EXTENDED_ASCII_SIZE; i++) {
    if (symbol_counts[i] > 0) {
      entries[entries_size].symbol = i;
      entries[entries_size].count = symbol_counts[i];
      entries_size++;
    }
  }

  return entries_size;
}

// por�wnuje obiekty symbol_entry malej�co wed�ug liczby zlicze�
int compare_entries_by_count_desc(const void *s1, const void *s2) {
  int c1 = ((symbol_entry*) s1)->count;
  int c2 = ((symbol_entry*) s2)->count;

  // 1 gdy c1 < c2, -1 gdy c1 > c2, 0 gdy c1 = c2
  return -((c1 > c2) - (c1 < c2));
}

// zwraca napis z wy�wietlaln� reprezentacj� znaku przy okazji padduj�c z lewej 
// spacjami �eby si� tabelka nie rozje�d�a�a  
char* get_printable_representation(unsigned char symbol) {
  // nadaje oznaczenia powszechniejszym znakom niedrukowalnym
  switch (symbol) {
    case '\t': return "  TB"; break;
    case '\n': return "  LF"; break;
    case '\r': return "  CR"; break;
    case ' ':  return "  SP"; break;
  }

  static char str[4];

  if (isprint(symbol)) {
    // znak drukowalny mo�na wypisa�
    str[0] = ' '; str[1] = ' '; str[2] = ' ';
    str[3] = symbol;
  } else {
    // dla znak�w niedrukowalnych nieobs�u�onych w switchu zwraca napis z ich
    // warto�ci� heksow�
    sprintf(str, "0x%x", symbol);
  }

  return str;
}

// drukuje tabel� podsumowuj�c� do strumienia
void print_summary(FILE *stream, symbol_entry *entries, int entries_size) {
  fprintf(stream, "-----------------------------------------\n"
                  "| Oznaczenia:                           |\n"
                  "| TB   - tabulator                      |\n"
                  "| LF   - line feed                      |\n"
                  "| CR   - powr�t karetki                 |\n"
                  "| SP   - spacja                         |\n"
                  "| 0x.. - inny znak niedrukowalny (hex)  |\n"
                  "-----------------------------------------\n"
                  "| L. wyst�pie� | Kod ASCII (dec) | Znak |\n"
                  "-----------------------------------------\n");

  for (int i = 0; i < entries_size; i++) {
    symbol_entry entry = entries[i];
    unsigned char symbol = entry.symbol;
    fprintf(stream, "|%13d |%16d | %s |\n", 
                    entry.count,
                    symbol, 
                    get_printable_representation(symbol));
  }

  fprintf(stream, "-----------------------------------------\n");
}

#include <stdio.h>
#include <time.h>

int czytajPlik(unsigned int tab[], FILE *filePointer);
void wyswietl(unsigned int tab[], int rozm);
void bubble(unsigned int tab[], int rozm);
void insert(unsigned int tab[], int rozm);
void bubbleStraznik(unsigned int tab[], int rozm);
void shell(unsigned int tab[], int rozm);
void quick(unsigned int tab [], int low, int high);
void pisz(unsigned int tab[], int rozm);

int main(){
    unsigned int tab[100000];
    double czas;
    clock_t start, end;
    FILE *filePointer;
    char fileNameIn[30];
    char wybor;
    int rozm;
    int low = 0;
    int high = 0;

    printf("ALGORYTMY SORTOWANIA \n\nOpis plikow: \n   Pliki losXXX.txt to liczby losowe \n   Pliki sort_m/sort_r to liczby posortowane malejaco/rosnaco \n   Parametry ostRand/pieRand to pliki posortowane z losowa pierwsza/ostatnia liczba \n");
    printf("Podaj plik wejsciowy (razem z rozszerzeniem)\nPlik musi znajdowac sie w katalogu z programem: \n");
    gets(fileNameIn);
    printf("Nazwa podanego pliku wejsciowego: %s\n", fileNameIn);
    if((filePointer=fopen(fileNameIn, "rb"))==NULL){
                                       //printf("Blad przy otwieraniu pliku %s\n",fileNameIn);
                                       perror(fileNameIn);
                                       return 0;
                                       }
    rozm = czytajPlik(tab, filePointer);
    high = rozm - 1;
    fclose(filePointer);
    printf("Wybierz algorytm sortowania, ktory chcesz uzyc: \n1 - BubbleSort \n2 - InsertionSort \n3 - BubbleSort ze straznikiem \n4 - ShellSort \n5 - Quicksort \n");
    scanf("%c",&wybor);
    switch(wybor){
                  case '1':
                       start = clock();
                       bubble(tab, rozm);
                       end = clock();
                       pisz(tab, rozm);
                       break;
                  case '2':
                       start = clock();
                       insert(tab, rozm);
                       end = clock();
                       pisz(tab, rozm);
                       break;
                  case '3':
                       start = clock();
                       bubbleStraznik(tab, rozm);
                       end = clock();
                       pisz(tab, rozm);
                       break;
                  case '4':
                       start = clock();
                       shell(tab, rozm);
                       end = clock();
                       pisz(tab, rozm);
                       break;
                  case '5':
                       start = clock();
                       printf("Sortowanie...");
                       quick(tab, low, high);
                       end = clock();
                       pisz(tab, rozm);
                       break;
                  default:
                       printf("Wybrales zly argument.");
                       break;
                       }
    czas = (double)((float)end - (float)start)/CLOCKS_PER_SEC;
    printf("\nWynik sortowania zostal zapisany do pliku wynik.txt \n");
    printf("Czas sortowania pliku %s algorytmem %c wynosi %.5lf sekund.\a", fileNameIn, wybor, czas);
//    wyswietl(tab, rozm);
    getch();
}

int czytajPlik(unsigned int tab[], FILE *filePointer){
     int k;
     int i=0;
     printf("Odczytywanie pliku...\n");
     while (fscanf(filePointer,"%d", &k)!=EOF){
           tab[i++]=k;
     }
     return i;
}

/* void wyswietl(unsigned int tab[], int rozm){
     int i = 0;
     for(i=0; i<rozm; i++){
              printf("%d\n",tab[i]);
              }
} */

void bubble(unsigned int tab[], int rozm){
  printf("Sortowanie...");
  int i = (rozm - 1);
  int k;
  unsigned int temp;
  for (i; i>0; i--)
    for (k=1; k<=i; k++)
      if (tab[k-1] > tab[k]){
              temp = tab[k-1];
              tab[k-1] = tab[k];
              tab[k] = temp;
              }
}



void insert(unsigned int tab[], int rozm){
     printf("Sortowanie...");
     unsigned int temp;
     int i, k;
     for(i=1; i<rozm; i++){
		temp = tab[i];
		k = i-1;
		while(temp<tab[k] && k>=0){
			tab[k+1] = tab[k];
			k--;
		}
		tab[k+1] = temp;
	}
}

void bubbleStraznik(unsigned int tab[], int rozm){
    printf("Sortowanie...");
    unsigned int temp;
    int p = 0;
    for(int j = rozm - 1; j > 0; j--)
    {
    p = 1;
    for(int i = 0; i < j; i++)
    {
       if(tab[i] > tab[i + 1])
      {
        temp = tab[i];
        tab[i] = tab[i+1];
        tab[i+1] = temp;
        p = 0;
    }

      }
      if(p!=0) break;

  }

}

void quick(unsigned int tab [], int low, int high)
{
int pivot, i, j, temp;
    if (low < high)
    {
        pivot = low;
        i = low;
        j = high;
        while (i < j)
        {
            while (tab[i] <= tab[pivot] && i <= high)
            {
                i++;
            }
            while (tab[j] > tab[pivot] && j >= low)
            {
                j--;
            }
            if (i < j)
            {
                temp = tab[i];
                tab[i] = tab[j];
                tab[j] = temp;
            }
        }
        temp = tab[j];
        tab[j] = tab[pivot];
        tab[pivot] = temp;
        quick(tab, low, j - 1);
        quick(tab, j + 1, high);
    }
}

void shell(unsigned int tab[], int rozm){
printf("Sortowanie...");
int h,i,j,x;
for(h = 1; h < rozm; h = 3 * h + 1);
  h /= 9;
  if(!h) h++;

  while(h)
  {
    for(j = rozm - h - 1; j >= 0; j--)
    {
      x = tab[j];
      i = j + h;
      while((i < rozm) && (x > tab[i]))
      {
        tab[i - h] = tab[i];
        i += h;
      }
      tab[i - h] = x;
    }
    h /= 3;
  }
}

void pisz(unsigned int tab[], int rozm){
     char fileNameOut[20] = "wynik.txt";
     FILE *filePointer;
     int i;
     if((filePointer=fopen(fileNameOut, "w"))==NULL){
                                       //printf("Blad przy otwieraniu pliku %s\n",fileNameOut);
                                       perror(fileNameOut);
                                       }
     else
     for(i=0;i<rozm;i++)
        fprintf(filePointer,"%d ",(int)tab[i]);
     fclose(filePointer);
}


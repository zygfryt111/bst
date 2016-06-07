#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
 
void bubbleSort(int* nr1, int* nr2, int* nr3, int n1, int n2, int n3){
    int temp;
    double czas;
    clock_t start, stop;
    
    start=clock();
    for(int i = 1; i < n1; i++) {
        for(int j = 0; j < n1-i; j++ ) {
            if(nr1[j] > nr1[j+1]) {
                temp = nr1[j];
                nr1[j] = nr1[j+1];
                nr1[j+1] = temp;
            }
        }
    }
    stop=clock();
    czas = ((float)stop - (float)start) / CLOCKS_PER_SEC;
    printf("\n(Bubble) Sortowanie dla n = 1000 zajelo: %lf s", czas);
    
    start=clock();
    for(int i = 1; i < n2; i++) {
        for(int j = 0; j < n2-i; j++ ) {
            if(nr2[j] > nr2[j+1]) {
                temp = nr2[j];
                nr2[j] = nr2[j+1];
                nr2[j+1] = temp;
            }
        }
    }
    stop=clock();
    czas = ((float)stop - (float)start) / CLOCKS_PER_SEC;
    printf("\n(Bubble) Sortowanie dla n = 10000 zajelo: %lf s", czas);
    
    start=clock();
    for(int i = 1; i < n3; i++) {
        for(int j = 0; j < n3-i; j++ ) {
            if(nr3[j] > nr3[j+1]) {
                temp = nr3[j];
                nr3[j] = nr3[j+1];
                nr3[j+1] = temp;
            }
        }
    }
    stop=clock();
    czas = ((float)stop - (float)start) / CLOCKS_PER_SEC;
    printf("\n(Bubble) Sortowanie dla n = 100000 zajelo: %lf s", czas);
}

void bubbleSortStraznik(int* nr1, int* nr2, int* nr3, int n1, int n2, int n3){
     int temp;
     double czas;
     clock_t start, stop;

     start=clock();
     for(int j = n1 - 1; j > 0; j--)
  {
    int p = 1;
    for(int i = 0; i < j; i++)
      if(nr1[i] > nr1[i + 1])
      {
        temp = nr1[j];
        nr1[j] = nr1[j+1];
        nr1[j+1] = temp;
        p = 0;
      }
    if(p) break;
  }
  stop=clock();
  czas = ((float)stop - (float)start) / CLOCKS_PER_SEC;
  printf("\n\n\n(Bubble ze straznikiem) Sortowanie dla n = 1000 zajelo: %lf s", czas);
    
    start=clock();
    for(int j = n2 - 1; j > 0; j--)
  {
    int p = 1;
    for(int i = 0; i < j; i++)
      if(nr2[i] > nr2[i + 1])
      {
        temp = nr2[j];
        nr2[j] = nr2[j+1];
        nr2[j+1] = temp;
        p = 0;
      }
    if(p) break;
  }
  stop=clock();
  czas = ((float)stop - (float)start) / CLOCKS_PER_SEC;
  printf("\n(Bubble ze straznikiem) Sortowanie dla n = 10000 zajelo: %lf s", czas);
    
    start=clock();
    for(int j = n3 - 1; j > 0; j--)
  {
    int p = 1;
    for(int i = 0; i < j; i++)
      if(nr3[i] > nr3[i + 1])
      {
        temp = nr3[j];
        nr3[j] = nr3[j+1];
        nr3[j+1] = temp;
        p = 0;
      }
    if(p) break;
  }
  stop=clock();czas = ((float)stop - (float)start) / CLOCKS_PER_SEC;
  printf("\n(Bubble ze straznikiem) Sortowanie dla n = 100000 zajelo: %lf s", czas);
}
 
void drukujTablice(int* nr1, int* nr2, int* nr3, int n1, int n2, int n3) {
    for(int i = 0; i < n1; ++i) {
        printf("[%d] = %d \n", i, nr1[i]);
    }
    for(int i = 0; i < n2; ++i) {
        printf("[%d] = %d \n", i, nr2[i]);
    }
    for(int i = 0; i < n3; ++i) {
        printf("[%d] = %d \n", i, nr3[i]);
    }
}
void zapiszPlik(int* nr1, int* nr2, int* nr3, int n1, int n2, int n3) {
    FILE *p = fopen("plik.txt", "w");
    if(p == NULL)
    {
         printf("Nie mozna otworzyc pliku");
         exit(1);
    }
    for(int i = 0; i < n1; ++i) {
        fprintf(p, "[%d] = %d \n", i, nr1[i]);
    }
    fprintf(p, "\n Nastepny punkt \n");
    for(int i = 0; i < n2; ++i) {
        fprintf(p, "[%d] = %d \n", i, nr2[i]);
    }
    fprintf(p, "\n Nastepny punkt \n");
    for(int i = 0; i < n3; ++i) {
        fprintf(p, "[%d] = %d \n", i, nr3[i]);
    }
     fclose(p);
}
 
int main(){        
    int n1 = 1000;
    int n2 = 10000;
    int n3 = 100000;
    
    printf("Program zaczyna dzialanie \n");
    Sleep(3000);
    
    int *tab1 = (int*)malloc(sizeof(int)*n1);
    int *tab2 = (int*)malloc(sizeof(int)*n2);
    int *tab3 = (int*)malloc(sizeof(int)*n3);
    
    srand(time(NULL));
    for(int i = 0; i < n1; ++i) {
        tab1[i] = rand()%1000;
    }
    for(int i = 0; i < n2; ++i) {
        tab2[i] = rand()%1000;
    }
    for(int i = 0; i < n3; ++i) {
        tab3[i] = rand()%1000;
    }
    drukujTablice(tab1, tab2, tab3, n1, n2, n3);
    printf("\nLiczby zostaly wygenerowane\nSortowanie...");
    bubbleSort(tab1, tab2, tab3, n1, n2, n3);
   
    Sleep(3000);
    printf("\nNacisnij klawisz, aby przjesc do sortowania ze straznikiem...");
    getch();
    for(int i = 0; i < n1; ++i) {
        tab1[i] = rand()%1000;
    }
    for(int i = 0; i < n2; ++i) {
        tab2[i] = rand()%1000;
    }
    for(int i = 0; i < n3; ++i) {
        tab3[i] = rand()%1000;
    }
    drukujTablice(tab1, tab2, tab3, n1, n2, n3);
    printf("\nLiczby zostaly ponownie wygenerowane\nSortowanie...");
    bubbleSortStraznik(tab1, tab2, tab3, n1, n2, n3);
    
    zapiszPlik(tab1, tab2, tab3, n1, n2, n3);

    if(tab1 != NULL) {    
        free(tab1);
    }
    if(tab2 != NULL) {    
        free(tab2);
    }
    if(tab3 != NULL) {    
        free(tab3);
    }
    printf("\n\nPosortowane tablice znajduja sie w pliku plik.txt\nProgram zakonczyl dzialanie, nacisnij jakis klawisz, aby zakonczyc \n");
    getch();
    return 0;
}

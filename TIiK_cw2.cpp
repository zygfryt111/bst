#include<stdlib.h>
#include <stdio.h>
#include <conio.h>
#define BUFSIZE 1000
#define kropka 46



struct symbol_zrodla 
{
       int symbol;
       int czestosc;
} model_zrodla[256]; 

struct galaz
{
       int ojciec;
       int potomek1;
       int potomek2;
} drzewo_kodowania[255];


struct element_kodu 
{
       int symbol;
       unsigned char slowo[4];
       int liczba_bitow;
} tablica_kodowania[512], tablica_kodu[256]; 

#include "fwlasne.bwc"  // tutaj s¹ porownaj() i nazwapliku()

int czytaj_model( char *nazwapliku_model, struct symbol_zrodla model_zrodla[])
{
int n,lsz,a,b;
    
    FILE *wskaznik_pliku=fopen(nazwapliku_model,"r"); 

    if (wskaznik_pliku == NULL)
	{
		printf("Nie mozna otworzyc pliku: %s  \n", nazwapliku_model);
		exit(EXIT_FAILURE);
	}
	else
	{
    printf( "Czytanie modelu zrodla z pliku %s \n", nazwapliku_model); 	
    n=0;
    lsz=0;
    
//Format zapisu do pliku byl nastepujacy
//fprintf(wskaznikpliku_msort, "  %d  %d\n", model_zrodla[i].symbol, model_zrodla[i].czestosc); 

    while (fscanf(wskaznik_pliku,"  %d  %d\n", &a, &b)!=EOF)
    {
      if (b!=0)
      {    
      model_zrodla[n].symbol=a;
      model_zrodla[n].czestosc=b;
      n++;
      lsz++;
      }
     }
    printf("liczba wszystkich symboli modelu: %d\n", lsz);
    }
    return (lsz);
}   

int zrob_drzewo(struct symbol_zrodla model_zrodla[], struct galaz drzewo_kodowania[],int liczba_symboli_modelu, char *nazwapliku_drzewo)
{
int liczba_symboli = liczba_symboli_modelu;
int licznik_symboli;
int licznik_symboli_dodanych = 0;
int n;

    licznik_symboli=liczba_symboli;
    for(int k=0; k<liczba_symboli-1; k++)
    {
     drzewo_kodowania[k].ojciec = 256+k;
     drzewo_kodowania[k].potomek1 = model_zrodla[liczba_symboli_modelu-1-k].symbol;
     drzewo_kodowania[k].potomek2 = model_zrodla[liczba_symboli_modelu-2-k].symbol;
     model_zrodla[liczba_symboli_modelu-2-k].symbol = 256+k;
     model_zrodla[liczba_symboli_modelu-2-k].czestosc = model_zrodla[liczba_symboli_modelu-1-k].czestosc + model_zrodla[liczba_symboli_modelu-2-k].czestosc;
     licznik_symboli--;    
     qsort(model_zrodla,licznik_symboli,sizeof(struct symbol_zrodla), porownaj);
     licznik_symboli_dodanych++;
     }
         
         
    FILE *wskaznikpliku_drzewo=fopen(nazwapliku_drzewo,"wb"); 
 	        
    if (wskaznikpliku_drzewo == NULL)
	    {
		printf("Nie mozna utworzyc pliku: \n", nazwapliku_drzewo);
		exit(EXIT_FAILURE);
	    }
 	else
        {
        for(int k=0; k<licznik_symboli_dodanych; k++)
          {
          //printf("%d %d %d \n", drzewo_kodowania[k].ojciec, drzewo_kodowania[k].potomek1, drzewo_kodowania[k].potomek2 );
          fprintf(wskaznikpliku_drzewo, "%d %d %d\n ", drzewo_kodowania[k].ojciec, drzewo_kodowania[k].potomek1, drzewo_kodowania[k].potomek2 );  
          } 
        }

        fclose(wskaznikpliku_drzewo); 
  return (licznik_symboli_dodanych);
}

int daj_tablice_kodowania(struct galaz drzewo_kodowania[], struct element_kodu tablica_kodowania[],int liczba_symboli_modelu, char *nazwa_tkodow)
{
int symbol1,symbol2;
int n, indeks_ojca, symbol_ojca;
int licznik_drzewa=liczba_symboli_modelu-2;
int licznik_kodu=0;
int dlugosc_ciagu=0;
int pozycja, ktory_bajt, ktory_bit;
unsigned char bajt, bajtek[4], jedynka=1;
    
    for(int k=0; k<2*liczba_symboli_modelu; k++)
    {
      for(int i=0; i<4; i++)
          tablica_kodowania[k].slowo[i]=0;
      tablica_kodowania[k].symbol=0;     
      tablica_kodowania[k].liczba_bitow=0;
    }
   
     tablica_kodowania[licznik_kodu].symbol=drzewo_kodowania[licznik_drzewa].ojciec;
     while(licznik_drzewa+1)
     {
          symbol_ojca=drzewo_kodowania[licznik_drzewa].ojciec; 
          tablica_kodowania[licznik_kodu+1].symbol=drzewo_kodowania[licznik_drzewa].potomek1;
          tablica_kodowania[licznik_kodu+2].symbol=drzewo_kodowania[licznik_drzewa].potomek2;
         
          indeks_ojca=-1;
          for (int i=0; i<licznik_kodu+1; i++)
             {
               //printf("%d  %d  %d\n", licznik_drzewa, symbol_ojca, tablica_kodowania[i].symbol); 
               if ((tablica_kodowania[i].symbol)==(symbol_ojca))
                {
                 indeks_ojca=i;
                 break;
                }
             }
             if (indeks_ojca==-1)
             {
                 printf("Blad tworzenie tablicy kodowania\n");
                 exit(EXIT_FAILURE);                                          
             }
   
          dlugosc_ciagu=tablica_kodowania[indeks_ojca].liczba_bitow;
          //Przepisanie ciagu ojca do potomkow    
          for (int i=0; i<4; i++)
                {
                tablica_kodowania[licznik_kodu+1].slowo[i]=tablica_kodowania[indeks_ojca].slowo[i];
                tablica_kodowania[licznik_kodu+2].slowo[i]=tablica_kodowania[indeks_ojca].slowo[i];
 
                }
           
          pozycja=dlugosc_ciagu;
          ktory_bajt=pozycja/8;
          ktory_bit=pozycja%8;
          jedynka=1;
          jedynka=jedynka<<7-ktory_bit;

          bajt=tablica_kodowania[indeks_ojca].slowo[ktory_bajt];
          bajt=bajt|jedynka;//dopisanie jedynki
          
          tablica_kodowania[licznik_kodu+1].slowo[ktory_bajt]=bajt;
          tablica_kodowania[licznik_kodu+1].liczba_bitow=dlugosc_ciagu+1;
          tablica_kodowania[licznik_kodu+2].liczba_bitow=dlugosc_ciagu+1;
          licznik_kodu+=2;
          licznik_drzewa--;
     }
     
    FILE *wskaznikpliku_tkodow=fopen(nazwa_tkodow,"wb"); 
    if (wskaznikpliku_tkodow == NULL)
	{
		printf("Nie mozna utworzyc pliku: %s \n", nazwa_tkodow);
		exit(EXIT_FAILURE);
	}
	else
    {
    //printf("Tablica kodowania \n\n");
    for(int k=0; k<2*liczba_symboli_modelu-1; k++)
        {
        for(int i=0; i<4; i++)   
          bajtek[i]=tablica_kodowania[k].slowo[i];
          //printf("%d  %d  %d  %d %d %d \n", tablica_kodowania[k].symbol, bajtek[0],bajtek[1],bajtek[2],bajtek[3],tablica_kodowania[k].liczba_bitow); 
          fprintf(wskaznikpliku_tkodow, " %d %d  %d  %d %d %d \n", tablica_kodowania[k].symbol, bajtek[0],bajtek[1],bajtek[2],bajtek[3],tablica_kodowania[k].liczba_bitow); 
        }
    }
    fclose(wskaznikpliku_tkodow); 
    
    return 0;
} 


int daj_tablice_kodu(struct element_kodu tablica_kodowania[], struct element_kodu tablica_kodu[],int liczba_symboli_modelu, char *nazwa_tkodu)
{
int symbol, licznik_kodu=0;
unsigned char bajtek[4];
  
     for (int k=0; k<2*liczba_symboli_modelu-1; k++)
     {
         symbol=tablica_kodowania[k].symbol;
         if (symbol<256) 
         {
             tablica_kodu[licznik_kodu].symbol=symbol;
             tablica_kodu[licznik_kodu].liczba_bitow=tablica_kodowania[k].liczba_bitow;
             for (int i=0; i<4; i++)
                tablica_kodu[licznik_kodu].slowo[i]=tablica_kodowania[k].slowo[i];
             licznik_kodu++;   
         }
     }
            
    FILE *wskaznikpliku_tkodu=fopen(nazwa_tkodu,"wb"); 
    if (wskaznikpliku_tkodu == NULL)
	{
		printf("Nie mozna utworzyc pliku: %s \n", nazwa_tkodu);
		exit(EXIT_FAILURE);
	}
	else
    {
     for(int k=0; k<liczba_symboli_modelu; k++)
        {
        for(int i=0; i<4; i++)   
          bajtek[i]=tablica_kodu[k].slowo[i];
        fprintf(wskaznikpliku_tkodu, " %d %d  %d  %d %d %d \n", tablica_kodu[k].symbol, bajtek[0],bajtek[1],bajtek[2],bajtek[3],tablica_kodu[k].liczba_bitow); 
        }
    }
    fclose(wskaznikpliku_tkodu); 
    
    return 0;
} 

int main(int argc, char *argv[])
{
    int liczba_symboli_modelu=0;
    int liczba_symboli_dodanych=0;
   	char *nazwa_pliku_in;
    char rozszerzenie_m[]= "model";
    char rozszerzenie_s[]= "modsort";
    char rozszerzenie_d[]= "tree";
    char rozszerzenie_tkodow[]= "coding";
    char rozszerzenie_tkodu[]= "code";
    char nazwapliku_model[24];
    char nazwa_modsort[24];
    char nazwapliku_drzewo[24];
    char nazwa_tkodow[24];
    char nazwa_tkodu[24];
   	
    int n;
   	
          
    if(argc==2)
      {
        nazwa_pliku_in=argv[1];
        printf("Parametry programu:  %d   %s \n",  argc, nazwa_pliku_in);
      }
	else
    {
   		printf("Zla liczba parametrow.  ","%d", argc);
		exit(EXIT_FAILURE);
    }

    n=nazwa_pliku(nazwa_pliku_in, rozszerzenie_m,nazwapliku_model);
    //printf("Nazwa pliku modelu:  %s \n", nazwapliku_model);
    n=nazwa_pliku(nazwa_pliku_in, rozszerzenie_s,nazwa_modsort);
    //printf("Nazwa pliku modelu po sortowaniu:  %s \n", nazwa_modsort);
 
    liczba_symboli_modelu=czytaj_model(nazwa_modsort, model_zrodla);
    printf("Liczba symboli modelu %d \n", liczba_symboli_modelu);
//    for (int n=0;n<256;n++)
//       printf("%d %d \n", model_zrodla[n].symbol, model_zrodla[n].czestosc);
    
    n=nazwa_pliku(nazwa_pliku_in, rozszerzenie_d,nazwapliku_drzewo);
      
    liczba_symboli_dodanych=zrob_drzewo(model_zrodla,drzewo_kodowania,liczba_symboli_modelu,nazwapliku_drzewo);
  
    if (liczba_symboli_dodanych!=0)
    {
     printf("Liczba symboli dodanych: %d\n", liczba_symboli_dodanych);
     printf("Drzewo kodowania wyznaczone i zapisane do pliku %s\n", nazwapliku_drzewo);
    }
    else
    {
     printf("Utworzenie drzewa kodowania nie powiodlo sie.\n");
     exit(EXIT_FAILURE);
    }

    n=nazwa_pliku(nazwa_pliku_in, rozszerzenie_tkodow,nazwa_tkodow);
    n = daj_tablice_kodowania(drzewo_kodowania, tablica_kodowania, liczba_symboli_modelu, nazwa_tkodow);

    if (n==0)
     printf("Tablica kodowania zapisana do pliku %s\n", nazwa_tkodow);
    else
    {
     printf("Utworzenie tablicy kodowania nie powiodlo sie.\n");
     exit(EXIT_FAILURE);
    }
    
    n=nazwa_pliku(nazwa_pliku_in, rozszerzenie_tkodu,nazwa_tkodu);
    n = daj_tablice_kodu( tablica_kodowania, tablica_kodu,liczba_symboli_modelu,nazwa_tkodu);

    if (n==0)
     printf("Tablica kodu zapisana do pliku %s\n", nazwa_tkodu);
    else
    {
     printf("Utworzenie tablicy kodu nie powiodlo sie.\n");
     exit(EXIT_FAILURE);
    }
 getch();
    return 0;
} 














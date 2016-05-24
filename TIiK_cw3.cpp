#include<stdlib.h>
#include <stdio.h>

#define BUFSIZE 1000
#define kropka 46




struct element_kodu 
{
       int symbol;
       unsigned char slowo[4];
       int liczba_bitow;
};

struct element_kodu tablica_kodu[256]; 


int nazwa_pliku(char *nazwa_pliku, char *rozszerzenie, char *nazwa_pliku_nowa) 
{
    int i,n;
   	char *w_nazwy;
    
    for (n=0; n<24; n++)
       nazwa_pliku_nowa[n]=0;

    n=0;
    w_nazwy=nazwa_pliku;
    nazwa_pliku_nowa[n]=*w_nazwy;
    while ((nazwa_pliku_nowa[n]!=kropka)&(nazwa_pliku_nowa[n]!='\0'))
           {
           n++;
           w_nazwy++;
           nazwa_pliku_nowa[n]=*w_nazwy;
           }
    n++;
    i=0;
    nazwa_pliku_nowa[n]=rozszerzenie[i];
    while (rozszerzenie[i]!='\0')
           {
           n++;
           i++;
           nazwa_pliku_nowa[n]=rozszerzenie[i];
           }         
    return 0;
}    

int czytaj_tabele_kodu( char *nazwa_tkodu, struct element_kodu tablica_kodu[])
{
int n, lsz;
int s,b[4],lb;
    
    FILE *wskaznik_pliku=NULL;
	wskaznik_pliku=fopen(nazwa_tkodu,"rb"); 

    if (wskaznik_pliku == NULL)
	{
		printf("Nie mozna otworzyc pliku: %s  \n", nazwa_tkodu);
		exit(EXIT_FAILURE);
	}
	else
	{
    n=0;
    lsz=0;
    printf("Odczyt tablicy kodu z pliku: %s  \n", nazwa_tkodu);    
//Format zapisu do pliku byl nastepujacy
//fprintf(wskaznikpliku_tkodu, " %d %d  %d  %d %d %d \n", tablica_kodu[k].symbol, bajtek[0],bajtek[1],bajtek[2],bajtek[3],tablica_kodu[k].liczba_bitow); 
//Argumenty fscan musza byc wskaznikami

    while (fscanf(wskaznik_pliku," %d %d  %d  %d %d %d \n", &s, &b[0],&b[1],&b[2],&b[3],&lb)!=EOF)
    {
      //printf(" %d %d  %d  %d %d %d \n", s, b[0],b[1],b[2],b[3],lb);     
      if (lb!=0)
      {    
      tablica_kodu[n].symbol=s;
      for (int i=0;i<4;i++)
        tablica_kodu[n].slowo[i]=(unsigned char)b[i];
      tablica_kodu[n].liczba_bitow=lb;  
      n++;
      lsz++;
      }
     }
    printf("liczba odczytanych symboli modelu: %d\n", lsz);
    }
    return (lsz);
}   


int kompresja(char *nazwa_pliku_in, char *nazwa_pliku_out, struct element_kodu tablica_kodu[],int liczba_symboli_modelu)
{
    unsigned char bufor_wejscia[BUFSIZE];
    unsigned char bufor_wyjscia[BUFSIZE];
    unsigned char symbol, dopisek, bajt_out;
    int n,suma=0;
    int licznik_symboli_kodowanych;
    int kursor_slowka, kursor_out;//pozycja bitu w slowie kodowym, pozycja bitu w bajcie out
    int liczba_bit_skod, liczba_wolne_bity, liczba_pozostale_w_slowku, numer_bajtu, numer_slowka;
    unsigned char maska1, maska2, maska, slowko[4];

    FILE *wskaznik_pliku_out=NULL;
	wskaznik_pliku_out=fopen(nazwa_pliku_out,"wb"); 

    if (wskaznik_pliku_out == NULL)
	{
		printf("Nie mozna otworzyc pliku: %s  \n", nazwa_pliku_in);
		exit(EXIT_FAILURE);
	}
    
 
    FILE *wskaznik_pliku_in=NULL;
	wskaznik_pliku_in=fopen(nazwa_pliku_in,"rb"); 

    if (wskaznik_pliku_in == NULL)
	{
		printf("Nie mozna otworzyc pliku: %s  \n", nazwa_pliku_in);
		exit(EXIT_FAILURE);
	}
	
    //printf( "Czytanie pliku %s porcjami do kodowania. \n", nazwa_pliku_in); 
    //printf( "Pisanie do pliku %s po kodowaniu porcji. \n", nazwa_pliku_out); 	
    
    numer_slowka=0;
    numer_bajtu=0;
    kursor_out=0;
    bajt_out=0;
    licznik_symboli_kodowanych=0;
    
    while(n=fread(bufor_wejscia,sizeof(unsigned char),BUFSIZE,wskaznik_pliku_in)) 
    {
    for(int i=0; i<n; i++)
      {
        symbol = bufor_wejscia[i];
        for (int m=0; m<liczba_symboli_modelu; m++)
        {
          if (symbol==tablica_kodu[m].symbol) 
           {
           for (int k=0; k<4; k++)
             slowko[k]=tablica_kodu[m].slowo[k];
           liczba_bit_skod=tablica_kodu[m].liczba_bitow; 
           suma=suma+liczba_bit_skod;
           kursor_slowka=0;
           numer_slowka=0;
           break;
           }
        }
        //printf("Symbol numer %d  kod dec %d  \n", licznik_symboli_kodowanych, symbol);
        while (liczba_bit_skod>0)
        {
           liczba_pozostale_w_slowku=8-kursor_slowka;
           if (liczba_pozostale_w_slowku>liczba_bit_skod)
              liczba_pozostale_w_slowku=liczba_bit_skod;
           liczba_wolne_bity=8-kursor_out;
           if (liczba_pozostale_w_slowku<liczba_wolne_bity)
               liczba_wolne_bity=liczba_pozostale_w_slowku;
           maska1=255>>kursor_slowka;
           maska2=255<<(8-(kursor_slowka+liczba_wolne_bity));
           maska=maska1&maska2;
           dopisek=slowko[numer_slowka]&maska;
           dopisek=dopisek<<kursor_slowka;
           dopisek=dopisek>>kursor_out;
           bajt_out=bajt_out|dopisek;
           kursor_out+=liczba_wolne_bity;   
           liczba_bit_skod-=liczba_wolne_bity;  
           kursor_slowka+=liczba_wolne_bity;
           liczba_pozostale_w_slowku-=liczba_wolne_bity;  
           if (liczba_pozostale_w_slowku==0)
           {
             numer_slowka++;  
             kursor_slowka=0;                             
             if (liczba_bit_skod>=8)
             liczba_pozostale_w_slowku=8;
             else
             liczba_pozostale_w_slowku=liczba_bit_skod;
           }                                        
          // printf("Bajt przetwarzany numer %d  kod hdec  %x  \n", numer_bajtu, bajt_out);
           if (kursor_out==8)
           {
           //printf("Bajt wyjsciowy numer %d  kod hdec  %x  \n", numer_bajtu, bajt_out);
           fwrite(&bajt_out,sizeof(unsigned char),1,wskaznik_pliku_out);
           numer_bajtu++;
           kursor_out=0;
           bajt_out=0;
           }
         }
         licznik_symboli_kodowanych+=1;
       }  
    } 
     
    printf("Bajt wyjsciowy numer %d  kod hdec  %x  \n", numer_bajtu, bajt_out);
    fwrite(&bajt_out,sizeof(unsigned char),1,wskaznik_pliku_out);
                              
    printf("Liczba zakodowanych znakow pliku kompresowanego: %d\n", licznik_symboli_kodowanych);
    printf("Liczba bajtow w pliku skompresowanym: %d\n", numer_bajtu+1);
    printf("Wskaznik upakowania: %5.1f  procent\n", 100*(float)(numer_bajtu+1)/(float)licznik_symboli_kodowanych);
    fclose(wskaznik_pliku_in); 
    fclose(wskaznik_pliku_out); 

    return (suma);
}


int main(int argc, char *argv[])
{
    int liczba_symboli_modelu=0;
    int liczba_symboli_dodanych=0;
   	char *nazwa_pliku_in;
    char rozszerzenie_m[]= "model";
    char rozszerzenie_s[]= "modsort";
    char rozszerzenie_mmod[]= "modmod";
    char rozszerzenie_d[]= "tree";
    char rozszerzenie_tkodow[]= "coding";
    char rozszerzenie_tkodu[]= "code";
    char rozszerzenie_out[]= "huffman";
    char nazwapliku_model[24];
    char nazwa_modsort[24];
    char nazwapliku_drzewo[24];
    char nazwa_modmod[24];
    char nazwa_tkodow[24];
    char nazwa_tkodu[24];
    char nazwa_pliku_out[24];
   	
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

    
    n=nazwa_pliku(nazwa_pliku_in, rozszerzenie_tkodu,nazwa_tkodu);
    liczba_symboli_modelu = czytaj_tabele_kodu( nazwa_tkodu, tablica_kodu);

    if (liczba_symboli_modelu>0)
     printf("Tablica kodu odczytana z pliku %s\n", nazwa_tkodu);
    else
    {
     printf("Odczytanie tablicy kodu nie powiodlo sie.\n");
     exit(EXIT_FAILURE);
    }
 
    n=nazwa_pliku(nazwa_pliku_in, rozszerzenie_out,nazwa_pliku_out);

    n=kompresja(nazwa_pliku_in, nazwa_pliku_out, tablica_kodu,liczba_symboli_modelu);

    
    if (n>0)
    {
      printf("Suma bitow kodu w pliku %s  %d  \n", nazwa_pliku_out, n);
      if (n%8!=0)
        n=n/8+1;
      printf("Liczba bajtow w pliku %s  %d  \n", nazwa_pliku_out, n);
    }
    else
      printf("Kodowanie nie powiodlo sie.\n");
  
    return 0;
    
} 

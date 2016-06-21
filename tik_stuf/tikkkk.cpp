#include<stdlib.h>
#include <stdio.h>
#include <conio.h>
#define BUFSIZE 1000
#define BUFSIZECRC 128
#define kropka 46
#define SIZECRC 4
#define degreegen 32
struct symbol_zrodla
{
       int symbol;
       int czestosc;
};
struct symbol_zrodla model_zrodla[256];
 
int porownaj(const void *operand1,const void *operand2)
{
    const struct symbol_zrodla *pointer1 = (const struct symbol_zrodla*) operand1;
    const struct symbol_zrodla *pointer2 = (const struct symbol_zrodla*) operand2;
    if((pointer1->czestosc)==(pointer2->czestosc)) return 0;
    else if((pointer1->czestosc)<(pointer2->czestosc))return 1;
    else return -1;
}
 
int nazwa_pliku(char *nazwa_pliku, char *rozszerzenie, char *nazwa_pliku_nowa)
{
    int i,n;
    char *w_nazwy;
   
    for (n=0; n<24; n++)
       nazwa_pliku_nowa[n]=0;
 
    n=0;
    w_nazwy=nazwa_pliku;
    nazwa_pliku_nowa[n]=*w_nazwy;
    while ((nazwa_pliku_nowa[n]!=kropka)&&(nazwa_pliku_nowa[n]!='\0'))
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
   
 
int wyznacz_model(char *nazwa_plik_dany,  char *nazwapliku_model, struct symbol_zrodla model_zrodla[], char *nazwa_ile)
{
    int n;
    int indeks = 0;
    int lwz = 0;
    int licznik_symboli_zrodla=0;
    struct symbol_zrodla model_pelny[256];
   
    unsigned char bufor_wejscia[BUFSIZE];
   
    for(int k=0; k<256; k++)
    {
     model_pelny[k].symbol=k;
     model_pelny[k].czestosc=0;
    }
   
 
    FILE *wskaznik_pliku=NULL;
    wskaznik_pliku=fopen(nazwa_plik_dany,"rb");
 
    if (wskaznik_pliku == NULL)
    {
        printf("Nie mozna otworzyc pliku: %s  \n", nazwa_plik_dany);
        exit(EXIT_FAILURE);
    }
    else
    //wczytuje dane z pliku blokami i wyznacza czêstoœci wystêpowania znaków
    {
    printf( "Czytanie pliku %s do wyznaczenia modelu. \n", nazwa_plik_dany);    
    while(n=fread(bufor_wejscia,sizeof(unsigned char),BUFSIZE,wskaznik_pliku))
    {
          for(int i=0; i<n; i++)
          {
            indeks = bufor_wejscia[i];
            model_pelny[indeks].czestosc++;
          }
            lwz+=n;
    }
    printf("liczba wszystkich znakow w pliku: %d\n", lwz);
    }
 
    FILE *wskaznik_ile=NULL;
    wskaznik_ile=fopen(nazwa_ile,"wb");
 
    if (wskaznik_ile == NULL)
    {
        printf("Nie mozna otworzyc pliku: %s  \n", nazwa_ile);
        exit(EXIT_FAILURE);
    }
    else
    {
      fprintf(wskaznik_ile, "%d\n", lwz);
      fclose(wskaznik_ile);    
    }    
 
 
 
    for(int i=0; i<256; i++)
      {
            if (model_pelny[i].czestosc!=0)
            {
               model_zrodla[licznik_symboli_zrodla].symbol=model_pelny[i].symbol;
               model_zrodla[licznik_symboli_zrodla].czestosc=model_pelny[i].czestosc;
               licznik_symboli_zrodla++;
             }
       }
 
    /*
	FILE *wskaznik_pliku_zapis=NULL;
    wskaznik_pliku_zapis=fopen(nazwapliku_model,"wb");
        if (wskaznik_pliku_zapis == NULL)
    {
        printf("Nie mozna utworzyc pliku: %s \n", nazwapliku_model);
        exit(EXIT_FAILURE);
    }
    else
    {
    //printf("Model zrodla \n\n");
    for(int i=0; i<licznik_symboli_zrodla; i++)
        {
        //printf("znak %c liczba (%d) liczebnosc  %d\n", model_zrodla[i].symbol,model_zrodla[i].symbol, model_zrodla[i].czestosc);
        fprintf(wskaznik_pliku_zapis, "%d  %d  \n", model_zrodla[i].symbol, model_zrodla[i].czestosc);  
        }
    }
 
    fclose(wskaznik_pliku); // zamyka plik
    */
    return (licznik_symboli_zrodla);
}
 
 
int sortuj_model(struct symbol_zrodla model_zrodla[],int liczba_symboli_modelu,char *nazwapliku_drzewo)
{  
    qsort(model_zrodla,liczba_symboli_modelu,sizeof(struct symbol_zrodla), porownaj);
   
    FILE *wskaznikpliku_msort=NULL;
    wskaznikpliku_msort=fopen(nazwapliku_drzewo,"wb");
    if (wskaznikpliku_msort == NULL)
    {
        printf("Nie mozna utworzyc pliku: %s \n", nazwapliku_drzewo);
        exit(EXIT_FAILURE);
    }
    else
    {
    //printf("Model zrodla po sortowaniu \n\n");
    for(int i=0; i<256; i++)
        {
        //printf("znak %c kod (%d) czestosc %d\n", model_zrodla[i].symbol,model_zrodla[i].symbol, model_zrodla[i].czestosc);
        fprintf(wskaznikpliku_msort, "  %d  %d\n", model_zrodla[i].symbol, model_zrodla[i].czestosc);
        }
    }
 
    fclose(wskaznikpliku_msort);
 
    return 0;
}
 
 
 
//funkcje z drugiego
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
     
    /*
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
   */
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
           
    /*
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
   */
    return 0;
}

//_______________________________________FUNKCJE Z 3 __________


int czytaj_tabele_kodu( char *nazwa_tkodu, struct element_kodu tablica_kodu[])
{
int n, lsz;
int s,b[4],lb;
    /*
	FILE *wskaznik_pliku=fopen(nazwa_tkodu,"rb"); 
    if (wskaznik_pliku == NULL)
	{   printf("Nie mozna otworzyc pliku: %s  \n", nazwa_tkodu);
		exit(EXIT_FAILURE);}
	else
    { n=0;
    lsz=0;
    printf("Odczyt tablicy kodu z pliku: %s  \n", nazwa_tkodu);    
//Format zapisu do pliku byl nastepujacy
//fprintf(wskaznikpliku_tkodu, " %d %d  %d  %d %d %d \n", tablica_kodu[k].symbol, bajtek[0],bajtek[1],bajtek[2],bajtek[3],tablica_kodu[k].liczba_bitow); 
//Argumenty fscan musza byc wskaznikami
    while (fscanf(wskaznik_pliku," %d %d  %d  %d %d %d \n", &s, &b[0],&b[1],&b[2],&b[3],&lb)!=EOF)
    { //printf(" %d %d  %d  %d %d %d \n", s, b[0],b[1],b[2],b[3],lb);     
      if (lb!=0)
      {    
      tablica_kodu[n].symbol=s;
      for (int i=0;i<4;i++)
        tablica_kodu[n].slowo[i]=(unsigned char)b[i];
      tablica_kodu[n].liczba_bitow=lb;  
      n++;
      lsz++;
      }}
    printf("liczba odczytanych symboli modelu: %d\n", lsz);
    }
    return (lsz);
    */
}   





int kompresja(char *nazwa_pliku_in, char *nazwa_pliku_out, struct element_kodu tablica_kodu[],int liczba_symboli_modelu)
{   unsigned char bufor_wejscia[BUFSIZE];
    unsigned char bufor_wyjscia[BUFSIZE];
    unsigned char symbol, dopisek, bajt_out;
    int n,suma=0;
    int licznik_symboli_kodowanych;
    int kursor_slowka, kursor_out;//pozycja bitu w slowie kodowym, pozycja bitu w bajcie out
    int liczba_bit_skod, liczba_wolne_bity, liczba_pozostale_w_slowku, numer_bajtu, numer_slowka;
    unsigned char maska1, maska2, maska, slowko[4];

    FILE *wskaznik_pliku_out=fopen(nazwa_pliku_out,"wb"); 
    if (wskaznik_pliku_out == NULL)
	{   printf("Nie mozna otworzyc pliku: %s  \n", nazwa_pliku_in);
		exit(EXIT_FAILURE);}
    
    FILE *wskaznik_pliku_in=NULL;
	wskaznik_pliku_in=fopen(nazwa_pliku_in,"rb"); 

    if (wskaznik_pliku_in == NULL)
	{	printf("Nie mozna otworzyc pliku: %s  \n", nazwa_pliku_in);
		exit(EXIT_FAILURE);	}
	
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
         }}
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
           {//printf("Bajt wyjsciowy numer %d  kod hdec  %x  \n", numer_bajtu, bajt_out);
           fwrite(&bajt_out,sizeof(unsigned char),1,wskaznik_pliku_out);
           numer_bajtu++;
           kursor_out=0;
           bajt_out=0;
         }}
         licznik_symboli_kodowanych+=1;
     }} 
    printf("Bajt wyjsciowy numer %d  kod hdec  %x  \n", numer_bajtu, bajt_out);
    fwrite(&bajt_out,sizeof(unsigned char),1,wskaznik_pliku_out);
                              
    printf("Liczba zakodowanych znakow pliku kompresowanego: %d\n", licznik_symboli_kodowanych);
    printf("Liczba bajtow w pliku skompresowanym: %d\n", numer_bajtu+1);
    printf("Wskaznik upakowania: %5.1f  procent\n", 100*(float)(numer_bajtu+1)/(float)licznik_symboli_kodowanych);
    fclose(wskaznik_pliku_in); 
    fclose(wskaznik_pliku_out); 
return (suma);}

//funkcje czwartego programu----------------------------------------------------------------------

int czytaj_drzewo( char *nazwapliku_drzewo, struct galaz drzewo_kodowania[])
{
int licz_galezie;
int pater,syn1,syn2;
    
    FILE *wskaznik_pliku=NULL;
	wskaznik_pliku=fopen(nazwapliku_drzewo,"rb"); 

    if (wskaznik_pliku == NULL)
	{
		printf("Nie mozna otworzyc pliku: %s  \n", nazwapliku_drzewo);
		exit(EXIT_FAILURE);
	}
	else
	{
    licz_galezie=0;
    printf("Odczyt drzewa kodowania z pliku: %s  \n", nazwapliku_drzewo);    
//Format zapisu do pliku byl nastepujacy
//fprintf(wskaznikpliku_drzewo, "%d %d %d\n ", drzewo_kodowania[k].ojciec, drzewo_kodowania[k].potomek1, drzewo_kodowania[k].potomek2 );  

    while (fscanf(wskaznik_pliku,"%d %d %d\n", &pater, &syn1,&syn2)!=EOF)
    {
      //printf("%d %d %d\n ", pater, syn1,syn2);     
 
      drzewo_kodowania[licz_galezie].ojciec=pater;
      drzewo_kodowania[licz_galezie].potomek1=syn1;
      drzewo_kodowania[licz_galezie].potomek2=syn2;
      licz_galezie++;
     }
    printf("liczba odczytanych galezi drzewa: %d\n", licz_galezie);
    }
    fclose(wskaznik_pliku);
    return (licz_galezie);
}   


int dekompresja(char *nazwa_pliku_skom, char *nazwa_pliku_dekom, struct galaz drzewo_kodowania[], int liczba_galezi_drzewa, int ile_symboli)
{
    unsigned char bajt_out, bit1, bajt_in;
    int m,n,suma_symboli=0,suma_bitow=0;
    int indeks_drzewa;
    int kursor_in;//pozycja bitu w bajcie wejsciowym
    int pater, syn1, syn0;;
    unsigned char maska=128;

    FILE *wskaznik_pliku_skom=NULL;
	wskaznik_pliku_skom=fopen(nazwa_pliku_skom,"rb"); 

    if (wskaznik_pliku_skom == NULL)
	{
		printf("Nie mozna otworzyc pliku: %s  \n", nazwa_pliku_skom);
		exit(EXIT_FAILURE);
	}
    
 
    FILE *wskaznik_pliku_dekom=NULL;
	wskaznik_pliku_dekom=fopen(nazwa_pliku_dekom,"wb"); 

    if (wskaznik_pliku_dekom == NULL)
	{
		printf("Nie mozna otworzyc pliku: %s  \n", nazwa_pliku_dekom);
		exit(EXIT_FAILURE);
	}
		
    printf( "Czytanie pliku skompresowanego %s po jednym bajcie. \n", nazwa_pliku_skom); 
    printf( "Po zdekodowaniu pisanie bajtu do pliku %s. \n", nazwa_pliku_dekom); 	
    
    suma_symboli=0;
    indeks_drzewa=liczba_galezi_drzewa-1;
    pater=drzewo_kodowania[indeks_drzewa].ojciec;
    syn1=drzewo_kodowania[indeks_drzewa].potomek1;
    syn0=drzewo_kodowania[indeks_drzewa].potomek2;
    
    //printf("Tyle ma byc bajtow po zdekodowaniu:  %d a tyle zdekodowano: %d\n", ile_bajtow, suma_symboli); 
            
    while(suma_symboli<ile_symboli) 
    {
    //printf("Tyle ma byc bajtow:  %d a tyle juz zdekodowano: %d\n", ile_bajtow, suma_symboli);                               
    n=fread(&bajt_in,sizeof(unsigned char),1,wskaznik_pliku_skom);             
    if (n==1)
      {
        for (int k=0;k<8; k++)
        {
            bit1=bajt_in&maska;
            //printf("Galaz %d %d %d  bajt_in %x bit1 %x\n",pater, syn1, syn0, bajt_in, bit1);
            bajt_in=bajt_in<<1;
            suma_bitow++;
            if (bit1)
              pater=syn1;
            else
              pater=syn0;
            
            if (pater<256)
            {
               bajt_out=pater;
               //printf("Galaz %d %d %d symbol %d\n",pater, syn1, syn0, bajt_out);
               fwrite(&bajt_out,sizeof(unsigned char),1,wskaznik_pliku_dekom);
               suma_symboli++;
               indeks_drzewa=liczba_galezi_drzewa-1;
               pater=drzewo_kodowania[indeks_drzewa].ojciec;
               syn1=drzewo_kodowania[indeks_drzewa].potomek1;
               syn0=drzewo_kodowania[indeks_drzewa].potomek2; 
               if (suma_symboli == ile_symboli)
               break;//wyjscie z for         
            }
            else
            {
            m=liczba_galezi_drzewa-1;
            while (m>=0)
            {
               if  (drzewo_kodowania[m].ojciec==pater)
               {
                 indeks_drzewa=m;
                 m=-1;                                   
               } 
               m=m-1;
            }        
            //pater=drzewo_kodowania[indeks_drzewa].ojciec;
            syn1=drzewo_kodowania[indeks_drzewa].potomek1;
            syn0=drzewo_kodowania[indeks_drzewa].potomek2;
            }//end else
          }//end for
        } //end if
      }//end while  

      printf("Liczba odczytanych bitow: %d, liczba zdekodowanych symboli %d.\n", suma_bitow, suma_symboli);       
      fclose(wskaznik_pliku_skom);
      fclose(wskaznik_pliku_dekom);
      
      return (suma_symboli);
}     
        
//funkcje z 5 - 6 laborek

int nowa_nazwa(char *nazwa_pliku, char *rozszerzenie, char *nazwa_pliku_nowa) 
{
    int i,n;
   	char *w_nazwy;
    
    for (n=0; n<24; n++)
       nazwa_pliku_nowa[n]=0;

    n=0;
    w_nazwy=nazwa_pliku;
    nazwa_pliku_nowa[0]=*w_nazwy;
    while ((nazwa_pliku_nowa[n]!=kropka)&&(nazwa_pliku_nowa[n]!='\0'))
           {
           n++;
           w_nazwy++;
           nazwa_pliku_nowa[n]=*w_nazwy;
           }
    n++;
    i=0;
    nazwa_pliku_nowa[n]=rozszerzenie[0];
    while (rozszerzenie[i]!='\0')
           {
           n++;
           i++;
           nazwa_pliku_nowa[n]=rozszerzenie[i];
           }         
    return 0;
}    

int dzielenie(char *nazwa_pliku_in, unsigned char reszta[], unsigned char wielomian[])
{
 unsigned char bufor_wejscia[BUFSIZECRC], kolejka[SIZECRC+BUFSIZECRC];   
 unsigned char start=0, koniec=0, maska=128, ostatni_bit, pierwszy_bit;
 int n,m;
 int licznik_bitow=0;
 int licznik_bajtow_in=0, licznik_buforu=0;
 
    for (int i=0; i<SIZECRC; i++)
       reszta[i]=0;
       
    for (int i=0; i<SIZECRC+BUFSIZECRC; i++)
       kolejka[i]=0;   
 
    FILE *wskaznik_pliku_in=NULL;
	wskaznik_pliku_in=fopen(nazwa_pliku_in,"rb"); 

    if (wskaznik_pliku_in == NULL)
	{
	  printf("Nie mozna otworzyc pliku: %s  \n", nazwa_pliku_in);
	  exit(EXIT_FAILURE);
	}

    while(n=fread(bufor_wejscia,sizeof(unsigned char),BUFSIZECRC,wskaznik_pliku_in)) 
    {
        //printf("Return fread %d\n",n);
        for (int i=0; i<n; i++)
                  kolejka[SIZECRC+i]=bufor_wejscia[i];
        licznik_bitow=8*n;
        licznik_bajtow_in+=n;
                
        while (licznik_bitow)
        {
          licznik_buforu=1+(licznik_bitow-1)/8; 
          //printf("Liczba bitow %d. Licznik buforu %d. \n", licznik_bitow, licznik_buforu);
          pierwszy_bit=maska&kolejka[0];    
         
          for (int i=0; i<SIZECRC+licznik_buforu; i++)
               {
                 kolejka[i]=(kolejka[i]<<1);
                 ostatni_bit=kolejka[i+1]>>7;
                 kolejka[i]=kolejka[i]|ostatni_bit; 
                 //printf("Bajt %d kolejki po przesunieciu: %#x\n", i, kolejka[i]);
               }
          if (pierwszy_bit!=0)  //jesli pierwszy bit jest jedynka
               for(int i=0; i<SIZECRC; i++)
                  {
                    kolejka[i]=kolejka[i]^wielomian[i];          
                    //printf("Bajt %d kolejki po xorowaniu: %#x\n", i, kolejka[i]);
                  }     
          licznik_bitow--;  
                   
        } // end while                    
    } // end while    
    
    //printf("Sukcesywne dopisywanie degreegen zer na koncu kolejki.\n");
    licznik_bitow=degreegen;
      
        while (licznik_bitow>0)
        {
          //printf("Liczba bitow %d\n", licznik_bitow);

          pierwszy_bit=maska&kolejka[0];    
          //printf("Pierwszy bit %#x\n", pierwszy_bit);  
          for (int i=0; i<SIZECRC; i++)
               {
                 kolejka[i]=(kolejka[i]<<1);
                 ostatni_bit=kolejka[i+1]>>7;
                 kolejka[i]=kolejka[i]|ostatni_bit; 
               }  
              //for(int i=0; i<SIZECRC+n; i++)
              //   printf("Po przesunieciu bajt %d kolejki: %#x\n", i, kolejka[i]);      
          //Zero na koncu jest dopisywane z definicji przesuniecia       
          if (pierwszy_bit!=0)  //jesli pierwszy bit jest jedynka                              
                 for(int i=0; i<SIZECRC; i++)
                 {
                    kolejka[i]=kolejka[i]^wielomian[i];          
                    //printf("Bajt %d kolejki po xorowaniu: %#x\n", i, kolejka[i]);
                 }     
          licznik_bitow--;       
   
        }// end while
 
 for(int i=0; i<SIZECRC; i++)
      reszta[i]=kolejka[i];          
                 
 return licznik_bajtow_in;   

}
/*
*******************************************************************M A I N***************************************************************************************
*******************************************************************M A I N***************************************************************************************
*******************************************************************M A I N***************************************************************************************
*******************************************************************M A I N***************************************************************************************
*******************************************************************M A I N***************************************************************************************
*/
 
int main(int argc, char *argv[])
{
	//**************************deklaracje*********************************
	int wybor;
	int i;
	int g;
	char plik_in[24];
   	char plik_out[24];
   	char plik_dek[24];
   	char plik_crc[24];
	//---------------------------------1 main
    int n;
    int liczba_symboli_modelu=0;
    char *nazwa_pliku_in;
    char rozszerzenie_m[]= "model";
    char rozszerzenie_s[]= "modsort";
    char rozszerzenie_ile[]="ile";
    char nazwa_modelu[24];
    char nazwa_modsort[24];
    char nazwa_ile[24];
    //---------------------------------2gi main
    int liczba_symboli_dodanych=0;
    char rozszerzenie_d[]= "tree";
    char rozszerzenie_tkodow[]= "coding";
    char rozszerzenie_tkodu[]= "code";
    char nazwapliku_model[24];
    char nazwapliku_drzewo[24];
    char nazwa_tkodow[24];
    char nazwa_tkodu[24];
    //----------------------------------3 main
    char rozszerzenie_mmod[]= "modmod";
    char rozszerzenie_out[]= "huffman";
    char nazwa_modmod[24];
    char nazwa_pliku_out[24];
     //----------------------------------4 main  
    char *nazwa_pliku_dekom;
    char *nazwa_pliku_skom;
    int liczba_galezi_drzewa=0;
    int ile_bajtow_ma_byc, ile_bajtow_zrobiono;
    //-----------------------------------5 i 6 main
     unsigned char wielomian[SIZECRC], reszta[SIZECRC];
//char n_plik[]="pliktestowy.txt";
char n_plik[24];
char CRC_rozszerzenie[]= "CRC";
char n_plik_CRC[24];
int stopien=32;
//wielomian[0]=0x04;
//wielomian[1]=0xc1;
//wielomian[2]=0x1d;
//wielomian[3]=0xb7;
wielomian[0]=0x00;
wielomian[1]=0x00;
wielomian[2]=0x00;
wielomian[3]=0x0b;
//----------------------------------------koniec deklaracji
//----------------------------------------1 main   	          

do
{
printf("\n1.Kompresja\n2.Dekompresja\n3.Obliczanie CRC\n4.Sprawdzanie poprawnosci CRC\n");
scanf("%d", &wybor);  
switch(wybor)
{
case 1:
{
	//************************************************************************************************************
	//--------------------------------------KOMPRESJA--------------------------------------------------------------
	//************************************************************************************************************
    printf("Podaj nazwe pliku do kompresji:\n");
    scanf("%s",plik_in);

        nazwa_pliku_in=plik_in;
        printf("Nazwa pliku do kompresji:  %s \n", nazwa_pliku_in);     
        nazwa_pliku(nazwa_pliku_in, rozszerzenie_m,nazwa_modelu);
        printf("Nazwa pliku modelu:  %s \n", nazwa_modelu); 
        nazwa_pliku(nazwa_pliku_in, rozszerzenie_ile,nazwa_ile);
        printf("Nazwa pliku z liczba bajtow w pliku kompresowanym:  %s \n", nazwa_ile);
        nazwa_pliku(nazwa_pliku_in, rozszerzenie_d,nazwapliku_drzewo);
        printf("Nazwa pliku modelu po sortowaniu:  %s \n", nazwapliku_drzewo); 
    
    liczba_symboli_modelu = wyznacz_model(nazwa_pliku_in, nazwa_modelu, model_zrodla, nazwa_ile);
 
    if (liczba_symboli_modelu!=0)
    {
        printf("Liczba symboli alfabetu wejsciowego: %d\n", liczba_symboli_modelu);                            
        printf("Model zrodla wyznaczony i zapisany do pliku %s\n", nazwa_modelu);
    }
    else
    {
        printf("Wyznaczanie modelu zrodla nie powiodlo sie.\n");
        exit(EXIT_FAILURE);
    }
    n = sortuj_model(model_zrodla,liczba_symboli_modelu, nazwapliku_drzewo);
    if (n==0)
     printf("Model zrodla po sortowaniu zapisany do pliku %s\n", nazwapliku_drzewo);
    else
    {
     printf("Sortowanie modelu zrodla nie powiodlo sie.\n");
     exit(EXIT_FAILURE);
    }
   
 
   
    //---------------------------------2gi main
 
    n=nazwa_pliku(nazwa_pliku_in, rozszerzenie_m,nazwapliku_model);
    //printf("Nazwa pliku modelu:  %s \n", nazwapliku_model);
    n=nazwa_pliku(nazwa_pliku_in, rozszerzenie_s,nazwa_modsort);
    //printf("Nazwa pliku modelu po sortowaniu:  %s \n", nazwa_modsort);
 
    liczba_symboli_modelu=czytaj_model(nazwapliku_drzewo, model_zrodla);
    printf("Liczba symboli modelu %d \n", liczba_symboli_modelu);
//    for (int n=0;n<256;n++)
//       printf("%d %d \n", model_zrodla[n].symbol, model_zrodla[n].czestosc);
   
    //n=nazwa_pliku(nazwa_pliku_in, rozszerzenie_d,nazwapliku_drzewo);
     
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
     // pocztaek 3ciego maina _______________________________________________________
      
    n=nazwa_pliku(nazwa_pliku_in, rozszerzenie_tkodu,nazwa_tkodu);
    liczba_symboli_modelu = czytaj_tabele_kodu( nazwa_tkodu, tablica_kodu);

    if (liczba_symboli_modelu>0)
     printf("Tablica kodu odczytana z pliku %s\n", nazwa_tkodu);
    else
    {printf("Odczytanie tablicy kodu nie powiodlo sie.\n");
     exit(EXIT_FAILURE);}
 
    n=nazwa_pliku(nazwa_pliku_in, rozszerzenie_out,nazwa_pliku_out);
    n=kompresja(nazwa_pliku_in, nazwa_pliku_out, tablica_kodu,liczba_symboli_modelu);
    
    if (n>0)
    { printf("Suma bitow kodu w pliku %s  %d  \n", nazwa_pliku_out, n);
      if (n%8!=0) n=n/8+1;
      printf("Liczba bajtow w pliku %s  %d  \n", nazwa_pliku_out, n);
    }
    else
      printf("Kodowanie nie powiodlo sie.\n");
      break;
} 
case 2:
	
	//************************************************************************************************************
	//--------------------------------------DEKOMPRESJA--------------------------------------------------------------
	//************************************************************************************************************	
	
{
      //------------------------------------------4 main
    printf("Podaj nazwe pliku do dekompresji:\n");
	scanf("%s",plik_out);
	printf("Podaj nazwe pliku zdekompresowanego:\n");
	scanf("%s",plik_dek);
    nazwa_pliku_skom=plik_out;
    nazwa_pliku_dekom=plik_dek;
    printf("Parametry programu:  %d   %s  %s \n",  argc, nazwa_pliku_skom, nazwa_pliku_dekom);
    
    n=nazwa_pliku(nazwa_pliku_skom, rozszerzenie_d,nazwapliku_drzewo);
    liczba_galezi_drzewa = czytaj_drzewo( nazwapliku_drzewo, drzewo_kodowania);

    if (liczba_galezi_drzewa>0)
     printf("Drzewo kodowania odczytane z pliku %s\n", nazwapliku_drzewo);
    else
    {
     printf("Odczytanie drzewa kodowania z pliku %s nie powiodlo sie.\n", nazwapliku_drzewo);
     exit(EXIT_FAILURE);
    }
   
    n=nazwa_pliku(nazwa_pliku_skom, rozszerzenie_ile,nazwa_ile);
    FILE *wskaznik_ile=NULL;
	wskaznik_ile=fopen(nazwa_ile,"rb"); 
    if (wskaznik_ile == NULL)
	{
		printf("Nie mozna otworzyc pliku: %s  \n", nazwa_ile);
		exit(EXIT_FAILURE);
	}
	else
	{
      printf("Odczyt, ile bajtow nalezy odkodowac, z pliku: %s\n", nazwa_ile);    
      n=fscanf(wskaznik_ile,"%d\n",&ile_bajtow_ma_byc);
      printf("Tyle bajtow nalezy odkodowac: %d\n", ile_bajtow_ma_byc);     
    }
    fclose(wskaznik_ile);
    
    ile_bajtow_zrobiono=dekompresja(nazwa_pliku_skom, nazwa_pliku_dekom,drzewo_kodowania,liczba_galezi_drzewa, ile_bajtow_ma_byc);
    
    
    if (ile_bajtow_zrobiono==ile_bajtow_ma_byc)
      printf("Liczba bajtow prawidlowa. Wynik dekompresji zapisany do pliku: %s. \n\n\n", nazwa_pliku_dekom);
    else
      printf("Liczba bajtow nieprawidlowa. Wynik dekompresji zapisany do pliku: %s. \n\n\n", nazwa_pliku_dekom);
      break;
}

case 3:
{
	//************************************************************************************************************
	//--------------------------------------C R C--------------------------------------------------------------
	//************************************************************************************************************
	
	
   //5 i 6 main

/*
for (int i=0; i<SIZECRC; i++)
   {
    wielomian[i]=0;
   }
*/


printf("Podaj plik, dla ktorego program ma wyznaczyc CRC:\n");
					scanf("%s",n_plik);
					int p;		
					p=nowa_nazwa (n_plik, CRC_rozszerzenie, n_plik_CRC);
					printf("Wartosc CRC zostanie zapisana w pliku %s\n", n_plik_CRC);
					p=dzielenie(n_plik, reszta, wielomian);
					printf("Liczba bajtow w pliku %d\n",n);
										
					// Samo CRC do pliku
													
					FILE *start;
					start=fopen(n_plik_CRC, "w");
				    for(int i=0; i<SIZECRC; i++) {       
				    fprintf(start ,"%c",reszta[i]);
                    }
					fclose(start); 
							            
							                     
					FILE *wskaznik_pliku_crc=NULL;
					wskaznik_pliku_crc=fopen(n_plik,"a"); 
							            
					for(int i=0; i<SIZECRC; i++) {
					fprintf(wskaznik_pliku_crc ,"%c",reszta[i]);
					}
					fclose(wskaznik_pliku_crc);
      
      
      break;
}

case 4:
{
	//************************************************************************************************************
	//--------------------------------------CRC SPRAWDZ--------------------------------------------------------------
	//************************************************************************************************************
	g=0;
	printf("Podaj nazwe pliku, dla ktore ma byc sprawdzone CRC:\n");
	scanf("%s",plik_crc);
	FILE *plik=NULL;
	plik=fopen(plik_crc,"rb");
	fclose(plik);
	n=dzielenie(plik_crc, reszta, wielomian);
	for(i=0; i<SIZECRC; i++){
	if (reszta[i]==0)
		g++;}
	if (g==SIZECRC)
		printf("Slowa kodowe CRC sa poprawne.\n");
	else 
		printf("Blad w pliku.\n");}
	break;
}

}while(1);

  getch();
    return 0;
}

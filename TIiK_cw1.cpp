#include<stdlib.h>
#include <stdio.h>
#include <conio.h>
#define BUFSIZE 1000
#define kropka 46
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
    return (licznik_symboli_zrodla);
} 


int sortuj_model(struct symbol_zrodla model_zrodla[],int liczba_symboli_modelu,char *nazwapliku_msort)
{  
    qsort(model_zrodla,liczba_symboli_modelu,sizeof(struct symbol_zrodla), porownaj);
    
    FILE *wskaznikpliku_msort=NULL;
	wskaznikpliku_msort=fopen(nazwapliku_msort,"wb"); 
    if (wskaznikpliku_msort == NULL)
	{
		printf("Nie mozna utworzyc pliku: %s \n", nazwapliku_msort);
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


int main(int argc, char *argv[])
{
    int n;
    int liczba_symboli_modelu=0;
   	char *nazwa_pliku_in;
    char rozszerzenie_m[]= "model";
    char rozszerzenie_s[]= "modsort";
    char rozszerzenie_ile[]="ile";
    char nazwa_modelu[24];
    char nazwa_modsort[24];
    char nazwa_ile[24];
  
           
    if(argc==2)
        { 
        nazwa_pliku_in=argv[1];             
        printf("Nazwa pliku do kompresji:  %s \n", nazwa_pliku_in);     
        nazwa_pliku(nazwa_pliku_in, rozszerzenie_m,nazwa_modelu);
        printf("Nazwa pliku modelu:  %s \n", nazwa_modelu); 
        nazwa_pliku(nazwa_pliku_in, rozszerzenie_ile,nazwa_ile);
        printf("Nazwa pliku z liczba bajtow w pliku kompresowanym:  %s \n", nazwa_ile);
        nazwa_pliku(nazwa_pliku_in, rozszerzenie_s,nazwa_modsort);
        printf("Nazwa pliku modelu po sortowaniu:  %s \n", nazwa_modsort); 
        }
	else
    {
   		printf("Zla liczba parametrow.  ","%d", argc);
		exit(EXIT_FAILURE);
    }
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
    n = sortuj_model(model_zrodla,liczba_symboli_modelu, nazwa_modsort);
    if (n==0)
     printf("Model zrodla po sortowaniu zapisany do pliku %s\n", nazwa_modsort);
    else
    {
     printf("Sortowanie modelu zrodla nie powiodlo sie.\n");
     exit(EXIT_FAILURE);
    }
  getch();
    return 0;
} 

#include<stdlib.h>
#include<stdio.h>
#define kropka 46
#define BUFOR 1000

struct element_zrodla
{
 int indeks_znaku;
 int ilosc_wystapien;
};
struct element_zrodla model[256];
int licznik_symboli=0;
int nazwa_pliku_Huffman(char *nazwa_pliku,char *rozszerzenie, char *nazwa_pliku_z_nowym_rozszerzeniem)
{
    int i=0;
    int n=0;
    for(i=0;i<35;i++)
    {
        nazwa_pliku_z_nowym_rozszerzeniem[i]=0;
    }
    while((nazwa_pliku[n]!=kropka)&&(nazwa_pliku[n]!='\0'))
    {
        n++;
    }
    for(i=0;i<n;i++)
    {
        nazwa_pliku_z_nowym_rozszerzeniem[i]=nazwa_pliku[i];
    }
    nazwa_pliku_z_nowym_rozszerzeniem[i]=kropka;
    n=0;
    i++;
    while(rozszerzenie[n]!='\0')
    {
        nazwa_pliku_z_nowym_rozszerzeniem[i]=rozszerzenie[n];
        i++;
        n++;
    }
    return 0;
}

int utworz_model_niepelny_Huffman(struct element_zrodla model[],struct element_zrodla model_pelny[256])
{
    int n=0,i;
    for(i=0;i<256;i++)
    {
        if(model_pelny[i].ilosc_wystapien>0)
        {
            model[n].indeks_znaku=model_pelny[i].indeks_znaku;
            model[n].ilosc_wystapien=model_pelny[i].ilosc_wystapien;
            n++;
        }
    }
    return n;
}
int oblicz_model_Huffman(char *nazwa_pliku, char *nazwa_pliku_model, char *nazwa_pliku_wynik, struct element_zrodla model[])
{
    int liczba_znakow=0;
    int n=0,k=0,j=0,i=0;
    unsigned char BUFOR_WEJSCIOWY[BUFOR];
    struct element_zrodla model_pelny[256];
    for(i=0;i<256;i++)
    {
        model_pelny[i].indeks_znaku=i;
        model_pelny[i].ilosc_wystapien=0;
    }
    FILE *plik;
    plik=fopen(nazwa_pliku,"rb");
    if(plik==NULL)
    {
        printf("Nie udalo sie otworzyc pliku zrodlowego\n");
    }
    else
    {
        printf("Odczytuje plik zrodlowy\n");
        while(n=fread(BUFOR_WEJSCIOWY, sizeof(unsigned char),BUFOR,plik))
        {
            for(j=0;j<n;j++)
            {
                model_pelny[BUFOR_WEJSCIOWY[j]].ilosc_wystapien++;
            }
            liczba_znakow+=n;
        }
        printf("W pliku wejsciowym bylo %d znakow\n",liczba_znakow);
        fclose(plik);
        plik=fopen(nazwa_pliku_wynik,"wb");
        if(plik==NULL)
        {
            printf("Nie mozna otworzyc pliku %d",nazwa_pliku_wynik);
            exit(EXIT_FAILURE);
        }
        else
        {
            fprintf(plik,"%d \n",liczba_znakow);
            fclose(plik);
        }
        licznik_symboli=utworz_model_niepelny_Huffman(model,model_pelny);
        plik=fopen(nazwa_pliku_model,"wb");
        if(plik==NULL)
        {
            printf("Nie mozna otworzyc pliku %d",nazwa_pliku_model);
            exit(EXIT_FAILURE);
        }
        else
        {
            for(k=0;k<licznik_symboli;k++)
            {
                fprintf(plik,"%d %d \n",model[k].indeks_znaku,model[k].ilosc_wystapien);
            }
            fclose(plik);
        }
    }
    return 0;
}

int funkcja_porownujaca(const void *element1,const void *element2)
{
    const struct element_zrodla *p1=(const struct element_zrodla*) element1;
    const struct element_zrodla *p2=(const struct element_zrodla*) element2;
    if((p1->ilosc_wystapien)==(p2->ilosc_wystapien))
    {
        return 0;
    }
    else if((p1->ilosc_wystapien)<(p2->ilosc_wystapien))
    {
        return 1;
    }
    else
    {
        return -1;
    }
}
int posortuj_model_Huffmana(struct element_zrodla model[],int liczba_elementow, char*plik_model_posortowany)
{
    int i=0;
    qsort(model,liczba_elementow,sizeof(struct element_zrodla),funkcja_porownujaca);
    FILE *plik;
    plik=fopen(plik_model_posortowany,"wb");
    if(plik==NULL)
    {
        printf("blad w trakcie otwierania pliku do zapisu posortowanego modelu\n");
        exit(EXIT_FAILURE);
    }

    else
    {
        for(i=0;i<liczba_elementow;i++)
        {
            fprintf(plik,"%d %d\n",model[i].indeks_znaku,model[i].ilosc_wystapien);

        }
        fclose(plik);
    }
}
struct galaz_drzewa
{
    int ojciec;
    int potomek1;
    int potomek2;
};

struct element_tablicy
{
    int symbol;
    unsigned char slowo[4];
    int liczba_bitow;
};

struct galaz_drzewa drzewo_kodowania[255];

struct element_tablicy tablica_kodowania[256];

struct element_tablicy tablica_kodowania_odczytana[256];



int utworz_drzewo(struct element_zrodla model[],struct galaz_drzewa drzewo[],int liczba_symboli, char* plik_drzewa, char* plik_mod_model)
{
    int licznik_pomocniczy = liczba_symboli;
    int licznik_symboli = liczba_symboli;
    int licznik_elementow_drzewa = 0;
    int licznik_dodanych = 0;
    int i=0;
    for(i=0;i<liczba_symboli-1;i++)
    {
        drzewo[i].ojciec = 256+i;
        drzewo[i].potomek1 = model[liczba_symboli-1-i].indeks_znaku;
        drzewo[i].potomek2 = model[liczba_symboli-2-i].indeks_znaku;
        model[liczba_symboli-2-i].indeks_znaku=256+i;
        model[liczba_symboli-2-i].ilosc_wystapien = model[liczba_symboli-2-i].ilosc_wystapien+model[liczba_symboli-1-i].ilosc_wystapien;
        licznik_symboli--;
        licznik_elementow_drzewa++;
        qsort(model,licznik_symboli,sizeof(struct element_zrodla),funkcja_porownujaca);
        licznik_dodanych++;
    }
    FILE *plik;
    plik=fopen(plik_mod_model,"wb");
    if(plik==NULL)
    {
        printf("Blad w trakcie otwierania pliku ze zmodyfikowanym modelem\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        for(i=0;i<liczba_symboli;i++)
        {
            fprintf(plik,"%d %d\n",model[i].indeks_znaku,model[i].ilosc_wystapien);
        }
        fclose(plik);
    }
    plik=fopen(plik_drzewa,"a+");
    if(plik==NULL)
    {
        printf("Blad w trakcie otwierania pliku z drzewem\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(plik,"%d \n",licznik_dodanych);
        for(i=0;i<licznik_elementow_drzewa;i++);
        {
            fprintf(plik,"%d %d %d\n",drzewo[i].potomek1,drzewo[i].potomek2);
        }
        fclose(plik);
    }
}
int dekompresja_Huffmana(char *nazwa_pliku_skom, char *nazwa_pliku_dekom, struct galaz_drzewa drzewo_kodowania[])
{
int n;
int m;
int ojciec;
int potomek1;
int potomek2;
int ilosc_symboli;
int rozmiar_drzewa;
int licznik_drzewa;
int suma_symboli;
int suma_bitow;
unsigned char bajt_wejsciowy;
unsigned char bajt_wyjsciowy;
unsigned char maska=128;
unsigned char bit;

FILE *plik_dek=NULL;
plik_dek=fopen(nazwa_pliku_dekom,"wb");
if(plik_dek==NULL)
{
    printf("Blad w trakcie otwierania pliku zdekodaowanego do zapisu\n");
    exit(EXIT_FAILURE);
}
FILE *plik=NULL;
plik=fopen(nazwa_pliku_skom,"rb");

if(plik==NULL)
{
              printf("Blad w trakcie otwierania pliku zdekodowanego do zapisu\n");
              exit(EXIT_FAILURE);
}
else
{
    fscanf(plik,"%d\n %d\n", &ilosc_symboli,&rozmiar_drzewa);
    licznik_drzewa=0;
    int i;
    for(i=0;i<rozmiar_drzewa;i++)
    {
            fscanf(plik,"%d\n %d\n",&ilosc_symboli,&rozmiar_drzewa);
            drzewo_kodowania[licznik_drzewa].ojciec=ojciec;
            drzewo_kodowania[licznik_drzewa].potomek1=potomek1;
            drzewo_kodowania[licznik_drzewa].potomek2=potomek2;
            licznik_drzewa++;
    }
    suma_symboli=0;
    suma_bitow=0;
    ojciec=drzewo_kodowania[rozmiar_drzewa-1].ojciec;
    potomek1=drzewo_kodowania[rozmiar_drzewa-1].potomek1;
    potomek2=drzewo_kodowania[rozmiar_drzewa-1].potomek2;

    while(suma_symboli < ilosc_symboli)

{
                   int j=0;
                   n=fread(&bajt_wejsciowy, sizeof(unsigned char), 1, plik);
                   if(n==1)
                   {
                      for(j=0; j<8; j++)
                      {
                               bit=bajt_wejsciowy&maska;
                               bajt_wejsciowy=bajt_wejsciowy << 1;
                               suma_bitow++;
                               if(bit)
                               {
                                      ojciec=potomek1;
                                      }
                                      else
                                      {
                                          ojciec=potomek2;
                                          }
                                          if(ojciec<256)
                                          {
                                                        bajt_wyjsciowy=ojciec;
                                                        fwrite(&bajt_wyjsciowy, sizeof(unsigned char), 1, plik_dek);
                                                        suma_symboli++;
                                                        licznik_drzewa=rozmiar_drzewa-1;
                                                        ojciec=drzewo_kodowania[rozmiar_drzewa-1].ojciec;
                                                        potomek1=drzewo_kodowania[rozmiar_drzewa-1].potomek1;
                                                        potomek2=drzewo_kodowania[rozmiar_drzewa-1].potomek2;
if (suma_symboli == ilosc_symboli)//jezeli zdekodowano juz wszystkie symbole
{
                 break;
}
}
else//jezeli kod ojca nie jest <256
{
    m=rozmiar_drzewa-1;//ustawienie zmiennej m
    while (m>=0)
    {
          if(drzewo_kodowania[m].ojciec==ojciec)//jezeli ojciec dla zmiennej m jest obecnym ojcem
          {
                                                         licznik_drzewa=m;//ustawienie zmiennej licznik_drzewa
                                                         m=m-1;//ustawienie m na m-1
          }
    }
    m=m-1;// ustawienie m na m-1
    potomek1=drzewo_kodowania[licznik_drzewa].potomek1;//ustawienie potomek1
    potomek2=drzewo_kodowania[licznik_drzewa].potomek2;//ustawienie potomek2
}
}
}
}
printf("Liczba odczytanych bitow: %d, liczba zdekodowanych symboli %d. \n",suma_bitow,suma_symboli);
fclose(plik);// zamkniecie plikow
fclose(plik_dek);
return(suma_symboli);//zwrocenie liczby zdekodowanych symboli
}
}

int utworz_tablice_kodowania(struct galaz_drzewa drzewo[], int liczba_symboli, struct element_tablicy tablica[], char *nazwa_pliku_tablicy, char* nazwa_pliku_tablicy_mod)
{
    int liczba_drzewa = liczba_symboli-2;
    int licznik_tablicy = 0;
    int ojciec;
    int indeks_ojca_tablica;
    int liczba_bajtow;
    int liczba_bitow;
    int jedynka = 1;
    int i = 0, j=0;
    unsigned char bajty[4];

    for(i=0;i<2*liczba_symboli;i++)
    {
        tablica[i].symbol=0;
        tablica[i].liczba_bitow=0;
        for(j=0;j<4;j++)
        {
            tablica[i].slowo[j]=0;
        }
    }
    tablica[licznik_tablicy].symbol=drzewo[liczba_drzewa].ojciec;
    while(liczba_drzewa+1)
    {
        ojciec = drzewo[liczba_drzewa].ojciec;
        tablica[licznik_tablicy+1].symbol=drzewo[liczba_drzewa].potomek1;
        tablica[licznik_tablicy+2].symbol=drzewo[liczba_drzewa].potomek2;
        indeks_ojca_tablica=-1;
        for(i=0;i<licznik_tablicy+1;i++)
        {
            if((tablica[i].symbol)==(ojciec))
            {
                indeks_ojca_tablica=i;
                break;
            }
        }
        if(indeks_ojca_tablica==-1)
        {
            printf("Blad tworzenia tablicy kodowania");
            exit(EXIT_FAILURE);
        }
        else
        {
            for(i=0;i<4;i++)
            {
                tablica[licznik_tablicy+1].slowo[i]=tablica[indeks_ojca_tablica].slowo[i];
                tablica[licznik_tablicy+2].slowo[i]=tablica[indeks_ojca_tablica].slowo[i];
            }
            liczba_bajtow = (tablica[indeks_ojca_tablica].liczba_bitow)/8;
            liczba_bitow = (tablica[indeks_ojca_tablica].liczba_bitow)%8;
            jedynka = 1;
            jedynka = jedynka<<7-liczba_bitow;
            jedynka = tablica[indeks_ojca_tablica].slowo[liczba_bajtow]|jedynka;
            tablica[licznik_tablicy+1].slowo[liczba_bajtow]=jedynka;
            tablica[licznik_tablicy+1].liczba_bitow=tablica[indeks_ojca_tablica].liczba_bitow+1;
            tablica[licznik_tablicy+2].liczba_bitow=tablica[indeks_ojca_tablica].liczba_bitow+1;
            licznik_tablicy = licznik_tablicy+2;
            liczba_drzewa--;
        }
    }
    FILE *plik;
    plik=fopen(nazwa_pliku_tablicy,"wb");
    if(plik==NULL)
    {
        printf("Blad w trakcie otwierania pliku z tablica kodowania\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        for(i=0;i<2*liczba_symboli-1;i++)
        {
            for(j=0;j<4;j++)
            {
                bajty[j]=tablica[i].slowo[j];
            }
            fprintf(plik,"%d %d %d %d %d %d\n",tablica[i].symbol,bajty[0],bajty[1],bajty[2],bajty[3],tablica[i].liczba_bitow);
        }
        fclose(plik);
    }
    plik=fopen(nazwa_pliku_tablicy_mod,"wb");
    if(plik==NULL)
    {
        printf("Blad w trakcie otwierania pliku z tablica kodowania\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        for(i=0;i<2*liczba_symboli-1;i++)
        {
            if(tablica[i].symbol<256)
            {
                for(j=0;j<4;j++)
                {
                    bajty[j]=tablica[i].slowo[j];
                }
                fprintf(plik,"%d %d %d %d %d %d\n",tablica[i].symbol,bajty[0],bajty[1],bajty[2],bajty[3],tablica[i].liczba_bitow);
            }
        }
        fclose(plik);
    }
}
int main(int argc, char *argv[])
{
    char wybor;
    char znak;
    char nazwa_pliku[30];
    char nazwa_pliku_modelu[35];
    char nazwa_pliku_mod_modelu[35];
    char nazwa_pliku_sort[35];
    char nazwa_pliku_drzewo[35];
    char rozszerzenie_modelu[]="model";
    char rozszerzenie_mod_modelu[]="modmodel";
    char rozszerzenie_sort[]="sort";
    char rozszerzenie_drzewo[]="tree";
        printf("Teoria Informacji i Kodowania\n");
    printf("Laboratoria 2015\n");
    printf("Program do kodowania i dekodowania Huffmana\n");
    printf("oraz obliczania CRC32\n");
    printf("**************************************\n\n\n\n");
    wybor=1;
    while(wybor==1 || wybor==2 || wybor==3 || wybor==4)
    {
        printf("1.Kompresja metoda Huffmana\n");
        printf("2.Dekompresja metoda Huffmana\n");
        printf("3.Obliczenie CRC32 dla pliku\n");
        printf("4.Sprawdzenie CRC32\n");
        printf("Inny znak - wyjscie \n");
        scanf("%d",&wybor);
        if(wybor==1)
        {
            printf("Podaj nazwe pliku do kompresji wraz z rozszerzeniem\n");
            scanf("%s",&nazwa_pliku);
            nazwa_pliku_Huffman(nazwa_pliku,rozszerzenie_modelu,nazwa_pliku_modelu);
            printf("Nazwa pliku z modelem: %s\n",nazwa_pliku_modelu);
            nazwa_pliku_Huffman(nazwa_pliku,rozszerzenie_sort,nazwa_pliku_sort);
            printf("Nazwa pliku z posortowanym modelem: %s \n",nazwa_pliku_sort);
            nazwa_pliku_Huffman(nazwa_pliku,rozszerzenie_modelu,nazwa_pliku_modelu);
            printf("Nazwa pliku ze zmodyfikowanym modelem: %s \n",nazwa_pliku_mod_modelu);
            nazwa_pliku_Huffman(nazwa_pliku,rozszerzenie_drzewo,nazwa_pliku_drzewo);
            printf("Nazwa pliku z drzewem kodowania: %s \n",nazwa_pliku_drzewo);
            oblicz_model_Huffman(nazwa_pliku,nazwa_pliku_modelu,nazwa_pliku_drzewo,model);
            posortuj_model_Huffmana(model,licznik_symboli,nazwa_pliku_sort);
        }
    }
}

int odczytaj_tablice_kodowania(struct element_tablicy tablica_kodowania[],char* plik_mod_tablicy)
{
    int dlugosc_slowa;
    int bajty[4];
    int kod_znaku;
    int liczba_znakow=0;
    int i=0;
    FILE *plik;
    plik=fopen(plik_mod_tablicy,"rb");
    if(plik==NULL)
    {
        printf("Blad otwarcia pliku ze zmodyfikowana tablica kodowania\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        while(fscanf(plik,"%d %d  %d  %d %d %d \n",&kod_znaku,&bajty[0],&bajty[1],&bajty[2],&bajty[3],&dlugosc_slowa)!=EOF)
        {
            if(dlugosc_slowa>0)
            {
                tablica_kodowania[liczba_znakow].symbol=kod_znaku;
                tablica_kodowania[liczba_znakow].liczba_bitow=dlugosc_slowa;
                for(i=0;i<4;i++)
                {
                    tablica_kodowania[liczba_znakow].slowo[i]=bajty[i];
                }
                liczba_znakow++;
            }
        }
        fclose(plik);
        printf("Odczytano %d znakow z pliku ze zmodyfikowana tablica kodowania\n",liczba_znakow);
    }
    return liczba_znakow;
}
int kompresja_Huffman(struct element_tablicy tablica_kodowania[],char* plik_wejsciowy, char* plik_wynikowy, int liczba_elementow_tablicy)
{
    int n;
    int liczba_bitow_znaku;
    int suma_bitow;
    int kursor;
    int kursor_out;
    int liczba_pozostalych;
    int liczba_wolne_bity;
    int liczba_zak_symboli;
    int numer_slowa;
    int numer_bajtu;
    int dopisek;
    int i=0,k=0,j=0;
    unsigned char maska;
    unsigned char znak;
    unsigned char bufor_wejsciowy[BUFOR];
    unsigned char bufor_wyjsciowy[BUFOR];
    unsigned char slowo_znaku[4];
    unsigned char bajt_out;
    FILE *plik;
    FILE *plik_out;
    plik_out = fopen(plik_wynikowy,"a+");
    if(plik_out==NULL)
    {
        printf("Nie udalo sie otworzyc pliku wynikowego\n");
        exit(EXIT_FAILURE);
    }
    plik = fopen(plik_wejsciowy,"rb");
    if(plik_out==NULL)
    {
        printf("Nie udalo sie otworzyc pliku wejsciowego\n");
        exit(EXIT_FAILURE);
    }
    kursor_out=0;
    numer_slowa=0;
    numer_bajtu=0;
    bajt_out=0;
    liczba_zak_symboli=0;
    while(n=fread(bufor_wejsciowy,sizeof(unsigned char),BUFOR,plik))
    {
        for(i=0;i<n;i++)
        {
            znak = bufor_wejsciowy[i];
            for(j=0;j<liczba_elementow_tablicy;j++)
            {
                if(znak==tablica_kodowania[j].symbol)
                {
                    for(k=0;k<4;k++)
                    {
                        slowo_znaku[k]=tablica_kodowania[j].liczba_bitow;
                    }
                    liczba_bitow_znaku=tablica_kodowania[j].liczba_bitow;
                    suma_bitow=suma_bitow+liczba_bitow_znaku;
                    kursor=0;
                    numer_slowa=0;
                    break;
                }
            }
        while(liczba_bitow_znaku>0)
        {
            liczba_pozostalych=8-kursor;
            if(liczba_pozostalych>liczba_bitow_znaku)
            {
                liczba_pozostalych=liczba_bitow_znaku;
            }
            liczba_wolne_bity=8-kursor_out;
            if(liczba_pozostalych<liczba_wolne_bity)
            {
                liczba_wolne_bity=liczba_pozostalych;
            }
            maska=((255>>kursor)&(255<<(8-(kursor+liczba_wolne_bity))));
            dopisek=slowo_znaku[numer_slowa]&maska;
            dopisek=dopisek<<kursor;
            dopisek=dopisek>>kursor_out;
            bajt_out=bajt_out|dopisek;
            kursor_out+=liczba_wolne_bity;
            liczba_bitow_znaku-=liczba_wolne_bity;
            kursor+=liczba_wolne_bity;
            liczba_pozostalych-=liczba_wolne_bity;
            if(liczba_pozostalych==0)
            {
                numer_slowa++;
                kursor=0;
                if(liczba_bitow_znaku>=8)
                {
                    liczba_pozostalych=8;
                }
                else
                {
                    liczba_pozostalych=liczba_bitow_znaku;
                }
            }
            if(kursor_out==8)
            {
                fwrite(&bajt_out,sizeof(unsigned char),1,plik_out);
                numer_bajtu++;
                kursor_out=0;
                bajt_out=0;
            }
        }
        liczba_zak_symboli++;
        }
    }
    printf("Bajt wyjsciowy numer%d kod hdec %x \n",numer_bajtu, bajt_out);
    fwrite(&bajt_out,sizeof(unsigned char),1,plik_out);

    printf("Liczba zakodowanych znakow pliku kompresowanego: %d\n", liczba_zak_symboli);
    printf("Liczba bajtow w pliku skompresowanym: %d\n", numer_bajtu+1);
    printf("Wskaznik upakowania: %5.1f procent\n", 100*(float)(numer_bajtu+1)/(float)liczba_zak_symboli);
    fclose(plik);
    fclose(plik_out);

    return suma_bitow;
}

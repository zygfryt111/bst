#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int tab[30],k=0,i=0;

struct wezel
{
    int value;
    struct wezel *l;
    struct wezel *r;
    }*korzen = NULL, *temp = NULL, *t2, *t1;

void usunAdd();
void usun();
void wstaw();
void wstawAdd(struct wezel**, int );
void stworz();
void szukajAdd(struct wezel *t,int data);
void szukaj(struct wezel *t);
//void szukajElement (struct wezel *t, 
void inorder(struct wezel *t);
void preorder(struct wezel *t);
void postorder(struct wezel *t);
float srednia(int przyk[]);
void push(int x);
float odchylenie(int kwiatuszek[]);


int flaga = 1;

int main() {
	//wezel*korzen = NULL, *n = NULL;
int dana, wybor;
char odp ='N';

while(1)
{
printf("\n\n");
printf("\nMenu programu:");
printf("\n1) Wstaw element do drzewa\n");
printf("2) Usun element z drzewa\n");   //usuwa razem z wezlem
printf("3) Przechodzenie inorder\n");
printf("4) Przechodzenie preorder\n");
printf("5) Przechodzenie postorder\n");
printf("6) Srednia\n");
printf("7) Odchylenie obciazone i nieobciazone\n");
printf("9) Zakoncz program\n");

printf("\nWpisz swoj wybor: ");
scanf("%d", &wybor);
system("cls");
    switch (wybor)
           {
    case 1:
        wstaw();
        printf("Wpisz wartosc ktora chcesz wstawic do tablicy");
        //scanf("%d", &dana);
        push(dana);
        //wstawAdd(&korzen, dana);

		break;

    case 2:
        usun();
        break;

    case 3:
        inorder(korzen);
        break;

    case 4:
        preorder(korzen);
        break;

    case 5:
        postorder(korzen);
        break;
        
    case 6:
         printf("\nSrednia wynosi %f",srednia(tab));
         break;
             
    case 7:
         printf("\nOdchylenie: ");
         odchylenie(tab);
         break;

    case 9:
        exit(0);

    default :
        printf("Blad, wybierz inna opcje");
        break;
            }
    
    }
    return 0;
}
//wstawianie wartosci do wezla
void wstaw()
{
    stworz();
    if (korzen == NULL)
        korzen = temp;
    else
    szukaj(korzen);
}
//utworz wezel
void stworz()
{
int data;

    printf("Wpisz wartosc dla tego wezla: ");
    scanf("%d", &data);
    temp = (struct wezel *)malloc(1*sizeof(struct wezel));
    temp->value = data;
    temp->l = temp->r = NULL;
}
//wyszukiwanie prawidlowej pozycji do wstawienia wezla, funkcja pomocnicza
void szukaj(struct wezel *t)
{
if ((temp->value > t->value) && (t->r != NULL))
    szukaj(t->r);
else if ((temp->value > t->value) && (t->r == NULL))
    t->r = temp;
else if ((temp->value < t->value) && (t->l != NULL))
    szukaj(t->l);
else if ((temp->value < t->value) && (t->l == NULL))
    t->l = temp;
}
//funkcja przechodzenia inorder
void inorder(struct wezel *t)
{
if (korzen == NULL)
{
    printf("Drzewo jest puste, nie ma co wyswietlic");
    return;
}
    if (t->l != NULL)
    inorder(t->l);
    printf("%d -> ", t->value);
    if (t->r != NULL)
    inorder(t->r);
}
//funkcja pomocnicza dla usuwania
void usun()
{
int data;
    if (korzen == NULL)
    {
        printf("Drzewo jest puste, brak elementow do usuniecia");
        return;
    }
    printf("Wpisz liczbe, ktora chcesz usunac: ");
    scanf("%d", &data);
    t1 = korzen;
    t2 = korzen;
    szukajAdd(korzen, data);
}
//funkcja przechodzenia preorder
void preorder(struct wezel *t)
{
    if (korzen == NULL)
    {
        printf("Drzewo jest puste, nie ma co wyswietlic");
        return;
    }
    printf("%d -> ", t->value);
    if (t->l != NULL)
        preorder(t->l);
    if (t->r != NULL)
        preorder(t->r);
}
// funkcja przechodzenia postorder
void postorder(struct wezel *t)
{
    if (korzen == NULL)
    {
        printf("Drzewo jest puste, nie ma co wyswietlic");
        return;
    }
    if (t->l != NULL)
        postorder(t->l);
    if (t->r != NULL)
        postorder(t->r);
        printf("%d -> ", t->value);
}
//szukanie pozycji dla wezla
void szukajAdd(struct wezel *t, int data)
{
    if ((data>t->value))
    {
        t1 = t;
        szukajAdd(t->r, data);
    }
    else if ((data < t->value))
    {
        t1 = t;
        szukajAdd(t->l, data);
    }
    else if ((data==t->value))
    {
        usunAdd(t);
    }

}
//funkcja usuwania
void usunAdd(struct wezel *t)
{
//usun jesli lisc
    if ((t->l == NULL) && (t->r == NULL))
    {
        if (t1->l == t)
        {
            t1->l = NULL;
        }
        else
        {
            t1->r = NULL;
        }
        t = NULL;
        free(t);
        return;
    }
//usun jesli dziecko po lewej
    else if ((t->r == NULL))
    {
    if (t1 == t)
    {
        korzen = t->l;
        t1 = korzen;
    }
    else if (t1->l == t)
    {
        t1->l = t->l;
    }
    else
    {
        t1->r = t->l;
    }
    t = NULL;
    free(t);
    return;
    }
//usun jesli dziecko po prawej
    else if (t->l == NULL)
    {
        if (t1 == t)
        {
            korzen = t->r;
            t1 = korzen;
        }
    else if (t1->r == t)
        t1->r = t->r;
    else
        t1->l = t->r;
        free(t);
        return;
    }
}

void push(int x)
{
     if(k >= 30)
     printf("Stos przepelniony\n");
     
     else 
     tab[k]=x;
     printf("%d",tab[k]);
     k++;
}

float srednia(int przyk[])
{
    float suma=0;
     int   n=30;
        
    float sr;
    
    for(i=0;i<30;i++)
    {
                     if(tab[i]!=0)
                     suma+=tab[i];
                     
                     else
                     n--;
                     }
    sr=suma/n;
    
    return (sr);
    
}

float odchylenie(int kwiatuszek[])
{
      float suma=0;
     int   n=30;
        
    float sr;
    
    for(i=0;i<30;i++)
    {
                     if(tab[i]!=0)
                     suma+=tab[i];
                     
                     else
                     n--;
                     }
    sr=suma/n;
    float rozn =0;
    float suma1 =0;
    float pot =0;
    float mng =0;
    float x =0;
    float c =0;
    float m =0;
    float c4=0,
          c1=0,
          c2=0,
          s1=0,
          zmien1=0,
          zmien2=0;
                  
    for(i=0;i<n;i++)
    {                
              rozn=tab[i]-sr;
              pot=rozn*rozn;
              suma1=suma1+pot;
              }
     if(n>1)
     {
           c=n-1; 
           m=1/c;
           mng=suma1*m;
           x=sqrt(mng);
           }
    
                        
      else{
      puts("za malo elementow");}
      
      printf("\nOdchylenie standardowe: %f",x);
      
      zmien1=4*n;
      c1=1/zmien1;
      printf("\nc1 wynosi %f",c1);
      zmien2=32*n*n;
      c2=7/zmien2;
      printf("\nc2 wynosi %f",c2);
      c4=1-c1-c2;
      printf("\nc4 wynosi %f",c4);
      
      s1=x/c4;
      printf("\ns1 wynosi %f",s1);
      //z jakiegos powodu jesli sie nie rozbije tego na rozne wartosci to program nie chce dzialac
      
      
      return 0;
    }

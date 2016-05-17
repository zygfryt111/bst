#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>

#define zakoncz 20

int tab[30],
    k=0,
    i=0;

typedef struct wezel 
{
        int data;
        struct wezel *oj, *ls, *ps;
}wezel;

void wstaw(wezel**, int ) ;
void preorder(wezel *) ;
void postorder(wezel *) ;
void inorder(wezel *) ;
wezel* max(wezel *); 
wezel* min(wezel *) ;
wezel* szukaj(wezel *, int );
wezel* nast(wezel*, int ) ;
wezel* poprz(wezel *, int ) ;
void usun(wezel**, wezel **);
float srednia(int przyk[]);
void push(int x);
float odchylenie(int kwiatuszek[]);


int main() {
	wezel*korzen = NULL, *n = NULL;
	int dana, choice,a;
	char odp ='N';
	
	//for(i=0;i<30;i++)
    //tab[i]=0;
	
	puts("Program operujacy na drzewie  BST. Co chcesz zrobic? : ");	
	
	do{
		puts("\n1.Dodaj element");
		puts("2.Szukaj");
		puts("3.In Order");
		puts("4.Preorder");
		puts("5.Postorder");
		puts("6.Wyszukiwanie najmniejszego elementu");
		puts("7.Wyszukiwanie najwiekszego elemntu");
		puts("8.Znajdowanie nastepnika wezla");
		puts("9.Znajodowanie poprzednika wezla");
		puts("10.Srednia wartosc klucza");
		puts("11.Odchylenie standardowe");
		puts("0.Usun element");
		puts("Zakoncz '20'");
		scanf("%d",&choice);
		
	switch(choice){
		case 1:
			do{
			puts("Wprowadz nowy element : ");
			scanf("%d", &dana);
			push(dana); //zapisanie elementu do listy
			
			
			wstaw(&korzen, dana);
			puts("Chcesz dodac kolejny element? (t/n)");
			odp=getch();
		}while(odp=='t');
		for(i=0;i<30;i++)
		printf(" %d ",tab[i]); //wyswietlenie zapisanych elementow w liscie
		
			break;
		case 2:
			printf("\nPodaj liczbe: ");
			scanf("%d", &dana);
			n = szukaj (korzen, dana);
			
			if(n) 
			puts("Dana jest w drzewie");
			
			else 
			puts("Danej liczby nie ma w drzewie");
			
			break;
		case 3:
			puts("Przejscie wszystkich elementow drzewa in order");
			inorder(korzen);
			printf("\n");
			break;

		case 4:
			puts("Przejscie wszystkich elementow drzewa preorder");
			preorder(korzen);
			printf("\n");
			break;
			
		case 5:
			puts("Przejscie wszystkich elementow drzewa postorder");
			postorder(korzen);
			printf("\n");
			break;
			
		case 6:
			if(n = min(korzen)) 
			printf("\nElement minimalny: %d\n", n->data);
			
			else 
			puts("Drzewo nie zostalo utworzone!");
			break;
			
		case 7:
			if(n = max(korzen)) 
			printf("\nElement maksymalny: %d\n", n->data);
			
			else 
			puts("Drzewo nie zostalo utworzone!");
			break;
			
		case 8:
			puts("Nastepnik ktorego wezla ma byc znaleziony?");
			scanf("%d", &dana);
			
			n = nast (korzen, dana);
			
			if(n) 
			printf("\nNastêpnikiem podanego wezla jest: %d", n->data);
			
			else 
			puts("Wezel nie ma nastepnika");
			break;
			
		case 9:
			puts("Poprzednik ktorego wezla ma byc znaleziony?");
			scanf("%d", &dana);
			
			n = poprz(korzen, dana);
			
			if(n) 
			printf("\nPoprzednikiem danego wezla jest: %d", n->data);
			
			else 
			puts("Wezel nie ma poprzednika");
			break;
			
		case 0:
			puts("Ktory wezel ma zostac usuniety?");
			scanf("%d", &dana);
			n = szukaj (korzen, dana);
			
			usun(&korzen, &n);
			puts("Element zostal usuniety");
			break;
			
		case 10:
             printf("\nSrednia wynsi %f",srednia(tab));
             break;
             
             case 11:
                  printf("\nOdchylenie: ");
                  odchylenie(tab);
                  break;
			
		case 12:
			return 0;
			break;
			
		default:
			printf("\nWybrana operacja nie istnieje!");
			break;
	}
	}while(choice != zakoncz);
	return 0;
}

void wstaw(wezel**korzen, int x) 
{
	if (*korzen == NULL) {
		*korzen = (wezel*)malloc(sizeof(wezel));
		(*korzen)->data = x;
		(*korzen)->oj = (*korzen)->ls = (*korzen)->ps = NULL;
	}
	
    else if ((*korzen)->data > x) {
		if ((*korzen)->ls) wstaw(&(*korzen)->ls, x);
		else {
			wezel *n = (wezel*)malloc(sizeof(wezel));
			n->data = x;
			n->ls = n->ps = NULL;
			n->oj = (*korzen);
			(*korzen)->ls = n;
		}
	}
	else 
    {
		if ((*korzen)->ps) wstaw(&(*korzen)->ps, x);
		else {
			wezel *n = (wezel*)malloc(sizeof(wezel));
			n->data = x;
			n->ls = n->ps = NULL;
			n->oj = (*korzen);
			(*korzen)->ps = n;
		}
	}
}


void preorder(wezel *korzen) 
{
	if (korzen) 
    {
		printf("%d ", korzen->data);
		preorder(korzen->ls);
		preorder(korzen->ps);
	}
}

void inorder(wezel *korzen) 
{
	if (korzen) 
    {
		inorder(korzen->ls);
		printf("%d ", korzen->data);
		inorder(korzen->ps);
	}
}

void postorder(wezel *korzen) 
{
	if (korzen) 
    {
		postorder(korzen->ls);
		postorder(korzen->ps);
		printf("%d ", korzen->data);
	}
}

wezel* max(wezel *korzen) 
{
	if (korzen) 
    {
		while (korzen->ps) 
        korzen = korzen->ps;
	}
	return korzen;
}

wezel* min(wezel *korzen) 
{
	if (korzen) 
    {
		while (korzen->ls) 
        korzen = korzen->ls;
	}
	return korzen;
}

wezel* szukaj(wezel *korzen, int x) {
	if(korzen== NULL) return korzen;
	while (korzen) 
    {
		if (korzen->data > x) 
        korzen = korzen->ls;
        
		else if (korzen->data < x) 
        korzen = korzen->ps;
        
		else break;
	}
	return korzen;
}

wezel* nast(wezel*korzen, int x) 
{
	wezel *n = szukaj (korzen, x);
	if (n) {
		if (n->ps)
			return min(n->ps);
			
		else 
        {
			wezel *pn = n->oj;
			while(pn && pn->ls != n)
            {
				n = pn;
				pn = n->oj;
			}
			return pn;
		}
	}
	return n;
}

wezel* poprz(wezel *korzen, int x) 
{
	wezel*n = szukaj (&(*korzen), x);
	if (n) 
    {
		if (n->ls)
			return max(n->ls);
		else 
        {
			wezel *pn = n->oj;
			while (pn && pn->ps != n) {
				n = pn;
				pn = n->oj;
			}
			return pn;
		}
	}
	return n;
}

void usun(wezel**korzen, wezel **n)
{
	if(*korzen == NULL || *n == NULL) return;
	if((*n)->ls == NULL && (*n)->ps == NULL){
		if((*n)->oj == NULL)
			(*korzen) = NULL;
			
		else if((*n)->oj->ls == (*n))
			(*n)->oj->ls = NULL;
			
		else
			(*n)->oj->ps = NULL;
		free(*n);
	}
	else if((*n)->ls == NULL || (*n)->ps == NULL)
    {
		if((*n)->ls == NULL){
			if((*n)->oj== NULL)
				(*korzen)=(*n)->ps;
				
			else if((*n)->oj->ls == (*n))
				(*n)->oj->ls = (*n)->ps;
				
			else
				(*n)->oj->ps = (*n)->ps;
		}
		
		else
        {
			if((*n)->oj == NULL)
				(*korzen)=(*n)->ls;
				
			else if((*n)->oj->ls == (*n))
				(*n)->oj->ls = (*n)->ls;
				
			else
				(*n)->oj->ps = (*n)->ls;	
		}
		free(*n);
	}
	
	else
    {
		wezel *tmp = NULL;
		tmp = min((*n)->ps);
		(*n)->data = tmp->data;
		usun(&(*korzen), &tmp);
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
    // printf("n= %d",n);  //sprawdzenie czy prwidlowo zostala obliczona liczba elementow   
    for(i=0;i<n;i++)
    {                
              rozn=tab[i]-sr;
              // printf("rozn= %f\n",rozn);
              pot=rozn*rozn;
             //  printf("pot= %f\n",pot);
              suma1=suma1+pot;
              // printf("suma1= %f\n",suma1);
              }
    // printf("\nsuma1 = %f",suma1);         
     // printf("\nn= %d",n); //prawdzenie czy n jest takie samo 
     if(n>1)
     {
           c=n-1; 
           m=1/c;
           mng=suma1*m;
           x=sqrt(mng);
         //  printf("mng= %f",mng);        
         //  printf("x= %f",x);
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
      
      
      return 0;
    }

      

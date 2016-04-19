#include <stdio.h>
#include <stdlib.h>

struct wezel
{
    int value;
    struct wezel *l;
    struct wezel *r;
    }*korzen = NULL, *temp = NULL, *t2, *t1;

void usunAdd();
void wstaw();
void usun();
void stworz();
void szukajAdd(struct wezel *t,int data);
void szukaj(struct wezel *t);
void inorder(struct wezel *t);
void preorder(struct wezel *t);
void postorder(struct wezel *t);

int flaga = 1;

void main()
{
int wybor;

printf("\nMenu programu:");
printf("\n1) Wstaw element do drzewa\n");
printf("2) Usun element z drzewa\n");   //usuwa razem z wezlem (chyba)
printf("3) Przechodzenie inorder\n");
printf("4) Przechodzenie preorder\n");
printf("5) Przechodzenie postorder\n");
printf("6) Zakoncz program\n");
while(1)
{
printf("\nWpisz swoj wybor: ");
scanf("%d", &wybor);
    switch (wybor)
    {
    case 1:
        wstaw();
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
        exit(0);

    default :
        printf("Blad, wybierz inna opcje");
        break;
            }
        }
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
//wyszukiwanie prawidlowej pozycji do wstawienia wezla
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
//inorder, funkcja jest rekurencyjna
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
//funkcja preorder
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
// funkcja postorder
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
//szukanie pozycji dla wezla (co?)
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

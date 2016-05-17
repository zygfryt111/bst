#include <iostream>
#include <string>

using namespace std;

string dodaj(string liczba_1, string liczba_2)
{
    

    int i = liczba_1.length() - 1;                                                //ilo�� znakow w liczba_1
    int j = liczba_2.length() - 1;                                                //ilo�� znak�w w liczba_2

    string wynik = "";                                                            //na pocz�tku zeruje wynik

    int p = 0;                                                                    //na pocz�tku zeruje przeniesienie

    while(i>=0 || j>=0)                                                           //sprawdzam czy ilo�� znak�w nie sko�czy�a si� w liczba_1 lub liczba_2
    {

        int w1, w2, w;

        if(i >= 0)                            
		     w1 = liczba_1[i]-48 ;                                                 //liczba kt�ra stoi na pozycji i w liczba_1 
	    else w1 = 0;
        
		if(j >= 0) 
		     w2 = liczba_2[j]-48 ;                                                 //liczba kt�ra stoi na pozycji j w liczba_2
	    else w2 = 0; 

        w = w1 + w2 + p;                                                           //dodaje dwie cyfry z tych samych miesjc wraz z przeniesienim i wynik zapisujemy w zmiennej z
        
		wynik = "0" + wynik;
        wynik[0] = 48 + w % 10;

        p = w / 10;                                                                //przeniesienie do nast�pnej kolumny

        i--; j--;                                                                  //przesuwamy si� na poprzedni znak
    } 

    if(p!=0) 
	wynik = "1" + wynik;                                                            //je�li przeniesienie wynosi 1, dopisujemy cyfr� 1 do wyniku

    return wynik;
}


string mnozenie(unsigned int liczba_1, string liczba_2)
{

  string wynik = "0";                                                                //zeruje wynik

  while(true)                                                                        //wykonujemy mno�enie, warunek zawsze spe�niony
  {
    if((liczba_1 & 1)!=0)   wynik = dodaj(wynik,liczba_2);                           //sprawdzam czy pierwszy bit liczby "liczba_1" jest r�ny od zera
    if((liczba_1 >>= 1)!=0) liczba_2 = dodaj(liczba_2, liczba_2);                    //sprawdzam czy liczba_1 po przesuni�ciu bitowym w prawo jest r�na od zera
	else break;                                                                      //je�li liczba jest r�wna zero dzia�anie programu zostanie przerwane
  } 

  return wynik;          
}
 
int main()
{
    int i,n;
    string silnia = "1";

    cin >> n;

    for(i = 1; i <= n; i++) silnia = mnozenie(i,silnia);                             //wywo�anie funckji mno�enia w p�tli for, wykona si� tyle razy dop�ki i<=n czyli petla wykona si� tak� ilo�� razy jak� silnie chcemy obliczy�

    cout << n << "! = " << silnia;                                                   //wypisanie wyniku
    cin >> i;
    return 0;
}

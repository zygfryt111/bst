#include <iostream>
#include <string>

using namespace std;

string dodaj(string liczba_1, string liczba_2)
{
    

    int i = liczba_1.length() - 1;                                                //iloœæ znakow w liczba_1
    int j = liczba_2.length() - 1;                                                //iloœæ znaków w liczba_2

    string wynik = "";                                                            //na pocz¹tku zeruje wynik

    int p = 0;                                                                    //na pocz¹tku zeruje przeniesienie

    while(i>=0 || j>=0)                                                           //sprawdzam czy iloœæ znaków nie skoñczy³a siê w liczba_1 lub liczba_2
    {

        int w1, w2, w;

        if(i >= 0)                            
		     w1 = liczba_1[i]-48 ;                                                 //liczba która stoi na pozycji i w liczba_1 
	    else w1 = 0;
        
		if(j >= 0) 
		     w2 = liczba_2[j]-48 ;                                                 //liczba która stoi na pozycji j w liczba_2
	    else w2 = 0; 

        w = w1 + w2 + p;                                                           //dodaje dwie cyfry z tych samych miesjc wraz z przeniesienim i wynik zapisujemy w zmiennej z
        
		wynik = "0" + wynik;
        wynik[0] = 48 + w % 10;

        p = w / 10;                                                                //przeniesienie do nastêpnej kolumny

        i--; j--;                                                                  //przesuwamy siê na poprzedni znak
    } 

    if(p!=0) 
	wynik = "1" + wynik;                                                            //jeœli przeniesienie wynosi 1, dopisujemy cyfrê 1 do wyniku

    return wynik;
}


string mnozenie(unsigned int liczba_1, string liczba_2)
{

  string wynik = "0";                                                                //zeruje wynik

  while(true)                                                                        //wykonujemy mno¿enie, warunek zawsze spe³niony
  {
    if((liczba_1 & 1)!=0)   wynik = dodaj(wynik,liczba_2);                           //sprawdzam czy pierwszy bit liczby "liczba_1" jest ró¿ny od zera
    if((liczba_1 >>= 1)!=0) liczba_2 = dodaj(liczba_2, liczba_2);                    //sprawdzam czy liczba_1 po przesuniêciu bitowym w prawo jest ró¿na od zera
	else break;                                                                      //jeœli liczba jest równa zero dzia³anie programu zostanie przerwane
  } 

  return wynik;          
}
 
int main()
{
    int i,n;
    string silnia = "1";

    cin >> n;

    for(i = 1; i <= n; i++) silnia = mnozenie(i,silnia);                             //wywo³anie funckji mno¿enia w pêtli for, wykona siê tyle razy dopóki i<=n czyli petla wykona siê tak¹ iloœæ razy jak¹ silnie chcemy obliczyæ

    cout << n << "! = " << silnia;                                                   //wypisanie wyniku
    cin >> i;
    return 0;
}

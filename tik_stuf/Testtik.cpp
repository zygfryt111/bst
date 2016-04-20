// Testtik.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

struct typElement
{
	char symbol;
	int num;
};


int main(int argc, _TCHAR* argv[])
{
	typElement tab[11];
	FILE *plik;
	plik = fopen("test.txt", "rt");
	int i = 0;

	char sign;
	while (!feof(plik))
	{
		sign = fgetc(plik);
		tab[i].symbol = sign;
		tab[i].num = 1;
		i++;
	}
	fclose(plik);
	return 0;
}


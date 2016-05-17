#include<stdio.h>
#include<stdlib.h>

struct nazwa{
       char symbol;
       int czestosc;      
};

void sortowanie(struct nazwa a[], int length)          //sortowanie b¹belkowe
{
	int i, j;
	struct nazwa temp;
    for (i = 0; i < length; i++)
    {for (j = 0; j < length - 1; j++)
        {if (a[j + 1].czestosc > a[j].czestosc)
            {
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }}}}
            
            
            
            
            
main()
{
    FILE *zw;
    char zp;
    int i=0, j=0;
    struct nazwa tab[256];
    for(i=0;i<256;i++){
    tab[i].czestosc=0;
    }
    char c;
    zw=fopen("y:\\123.txt","r");
    
    while(zp!=EOF){
           zp=fgetc(zw);
           tab[(int)zp].czestosc++;
    }
    
    fclose(zw);
     printf("KOD ZNAKU  CZESTOSC\n\n");
          for(i=0; i<256;i++){
          c = (char)i;
          if(tab[i].czestosc!=0){
          tab[i].symbol=c;
          j++;
          }
       
       sortowanie(tab, i);                    //sortuje tablice
       
          }
          i=0;
          
       while(i<j){                            //wypisywanie
     printf("%c\t%d\n", tab[i].symbol, tab[i].czestosc);
     i++;
     }
     
     printf("\n\n");          
     getch();
     
     return 0;
}

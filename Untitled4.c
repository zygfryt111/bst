#include<stdio.h>
#include<stdlib.h>

void sortowanie(int tab1[],int n)
{
  int a=0, i=0, j=0;
    for(i=0;i<n;i++)
    for(j=1;j<n-i;j++) 
    if(tab1[j-1]>tab1[j])
      a=tab1[j-1];
      tab1[j-1]=tab1[j];
      tab1[j]=a;
}


main()
{
    FILE *zw;
    char zp;
    int i=0, j=0;
    int tab[256];
    int tab1[256];
    for(i=0;i<256;i++){
    tab[i]=0;}
    for(i=0;i<256;i++){
    tab[i]=0;}
    }
    char c;
    zw=fopen("y:\\123.txt","r");
    
    while(zp!=EOF){
           zp=fgetc(zw);
           tab[(int)zp]++;
           tab1[(int)zp]++;
    }
    
    fclose(zw);
     printf("KOD ZNAKU  CZESTOSC\n\n");
          for(i=0; i<256;i++){
          c = (char)i;
          if(tab[i]!=0){
          printf("%c ", c);
          printf("%d\n ", tab[i]);
       
          }
          
    } 
     getch();
     return 0;
}

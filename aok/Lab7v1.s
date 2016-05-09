.data

licznik:    .word       109
licznikTB:  .word       50

tablica:   .space       440
Suma:      .double       4
TB:	   .space       200

.text
addi r9, r0, tablica  ; adres pierwszego elemetu tablicy do r9
addi r4,r0,105	      ; r4=105
lw r20, licznik       ; licznik petli do wykonania wys³any do r20
addi r1,r1,1	      ; r1=1
sw 0(r9), r4          ; zapisz wartoœæ r4 do tablicy na 2 miejsce
addi r2, r0, 0        ; r2=0


wyp_tablicy:
add r4, r4, r1        ; r4=105+1
subi r20, r20, #1     ; zmniejszenie licznika wykonañ pêtli
sw 4(r9), r4          ; zapisz wartoœæ r4 do tablicy na 2 miejsce
addi r9, r9, #4       ; zmiana wskaŸnika na tablicy na 2 element
bnez r20, wyp_tablicy ; zbadanie, czy wykonano zadan¹ liczbê razy


addi r10, r0, tablica ; adres pierwszego elemetu tablicy do r10
lw r19, licznikTB     ; licznik wykonañ pêtli do r20
addi r7, r0, TB       ; adres pierwszego elemetu TB do r7
addi r26, r0, 0       ; wyzerowanie r26, w którym bêdzie suma

TB_i_suma:
lw r5, 0(r10)         ; odczytaj 1 element tablicy do r1
lw r6, 4(r10)         ; odczytaj 2 element tablicy do r2
addi r10, r10, #4     ; modyfikacja wskaŸnika na nastêpne elementy
mult r15, r5, r6      ; mno¿enie pierwszego razy drugi
subi r19, r19, #1     ; zmniejszenie licznika wykonañ pêtli
addi r16,r15, 100     ; r16=r15+100
sw 0(r7), r16         ; zapisz wartoœæ r16 do TB na 1 miejsce
addi r7, r7, #4       ; zmiana wskaŸnika na TB na 2 element
add r26, r16, r26     ; dodaje kolejn¹ wartoœæ do sumy i zapisuje w r26
bnez r19, TB_i_suma   ; zbadanie, czy wykonano zadan¹ liczbê razy

movi2fp f10,r26
sd Suma, f10	      ; przes³anie wyniku sumy z D2 do zmiennej suma

;*** end
trap 0
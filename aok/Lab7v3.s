.data
nr: 		.word 15
skladnik: 	.word 120
liczbatab: 	.word 109
liczbaTB: 	.word 60

T: 		.space 440
TB: 		.space 240
suma: 		.double 4


.text
lw 	r1, nr
lw 	r2, skladnik
addi 	r10, r0, T
add 	r3, r1, r2 		;licze numer + skladnik
lw 	r20, liczbatab
sw 	0(r10), r3 		;zapisa³am do 0el tablicy swój numer(pierwsza iteracja)

wypelnianieT:
addi 	r3, r3, 1 		;kolejny element do wpisania do tablicy
sw 	4(r10), r3
subi 	r20, r20, 1 		;licznik petli-1 (ile razy ma sie wykonac jeszcze)
addi 	r10, r10, 4
bnez 	r20, wypelnianieT 	;czy wykonano petle zadana ilosc razy?

;powyzsze dziala


;wzor (T[i+3] * T[i+7]) + SK£ADNIK

adresowanie:
addi 	r10, r0, T        	;ponownie adres I elementu tablicy T do r10, bo ten wyzej juz jest nieaktualny (petla)
addi 	r11, r0, TB       	;adres Iel TB do r11
lw 	r19, liczbaTB       	;licznik wykonañ pêtli z TB do r19
addi 	r26, r0, 0
;to wyzej tez jest dobrze


wypelnianieTB:
lw 	r5, 12(r10)         	;el T do r5
lw 	r6, 28(r10)         	;el T do r6
addi 	r10, r10, #4     	;adres na nastêpny element
mult 	r15, r5, r6      	; mno¿enie pierwszego razy drugi
subi 	r19, r19, #1     	; zmniejszenie licznika wykonañ pêtli
add 	r16,r15, r2       	; dodawanie skladnika
sw 	0(r11), r16        	; zapisz wartoœæ r16 do TB
addi 	r11, r11, #4       	; zmiana wskaŸnika na TB na 2 element
add 	r26, r26, r16     	; dodaje kolejn¹ wartoœæ do sumy i zapisuje w r26

bnez 	r19,wypelnianieTB   	; zbadanie, czy wykonano zadan¹ liczbê razy




movi2fp f10,r26
cvti2d	f12,f10
sd 	suma, f12

trap 0
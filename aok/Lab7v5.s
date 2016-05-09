.data
tabT: 		.word 110
tabTB:  	.word 80
numer: 		.word 15
skladnik: 	.word 140


T: 		.space 440
TB: 		.space 320
suma: 		.double 0

.text
;r10 wskaznikT
addi r10, r0, T

;pierwszy element tablicy:
lw 	r1, numer
lw 	r2, skladnik
add 	r3, r1, r2 
subi    r10,r10,#4

;liczba wykonan  petli tablicaT
lw r8, tabT


LoopT:

sw   	4(r10), r3   ;zapis w pierwszym el. tablicy
subi 	r8, r8, 1    ;liczba wyk petli - 1
addi 	r10, r10, 4  ;kolejna komorka tablicy
addi 	r3, r3, 1    ;kolejny element
bnez 	r8, LoopT    ;petla do wyczerpania wykonan


;wskaznikT jeszcze raz
addi 	r10, r0, T

;wskaznikTB       
addi 	r12, r0, TB

;liczba wykonan petli TablicaTB       
lw 	r8, tabTB

;zerowanie sumy (w r25)       
addi 	r25, r0, 0

;r17 -- wskaznik na skladnik
addi    r17, r0, skladnik

;r18 -- skladnik
lw      r18, 0(r17)

TablicaTB:
;wczytywanie danych
lw 	r1, 8(r10)         
lw 	r2, 20(r10)        

;wykonywanie dzialan
add 	r3, r1, r2       
mult 	r4 , r3, r18
sw 	0(r12), r4
        ; r12: | r4 |   
addi 	r12, r12, #4 ; TB++

         
;suma      
add 	r25, r25, r4


;wskaznikT++
addi 	r10, r10, #4

;liczba wykonan petli--
subi 	r8, r8, #1
      
bnez 	r8, TablicaTB 
  

movi2fp	 	f1,r25
cvti2d  	f2,f1
sd 		suma,f2
trap 0
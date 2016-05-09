.data
tabT: 		.word 110
tabTB:  	.word 60
numer: 		.word 15
skladnik: 	.word 180


T: 		.space 440
TB: 		.space 240
suma: 		.double 0

.text
;wskaznikT
addi r10, r0, T

;Obliczanie 1-el
lw 	r1, numer
lw 	r2, skladnik
add 	r3, r1, r2 
subi    r10,r10,#4

;liczba wykonan  petli tablicaT
lw r8, tabT

TablicaT:

sw   	4(r10), r3
subi 	r8, r8, 1 
addi 	r10, r10, 4
addi 	r3, r3, 1 
bnez 	r8, TablicaT 


;wskaznikT
addi 	r10, r0, T

;wskaznikTB       
addi 	r12, r0, TB

;liczba wykonan petli TablicaTB       
lw 	r8, tabTB

;suma       
addi 	r25, r0, 0

TablicaTB:
;wczytywanie danych
lw 	r1, 8(r10)         
lw 	r2, 16(r10)        
lw 	r3, 24(r10)

;wykonywanie dzialan
mult 	r4,r1, r2       
mult 	r5,r4,r3
sw 	0(r12), r5
         
;suma      
add 	r25, r25, r5

;wskaznikTB++
addi 	r12, r12, #4

;wskaznikT++
addi 	r10, r10, #4

;liczba wykonan petli--
subi 	r8, r8, #1
      
bnez 	r8,TablicaTB 
  

movi2fp	 	f1,r25
cvti2d  	f2,f1
sd 		suma,f2
trap 0
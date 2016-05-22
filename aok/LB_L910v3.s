[(3.2*T[i] * T[i+3]*T[i+5]* T[i+6])- T[i+7] ] /
[T[i+3] ]


.data
	T:	.space 	880
	.align     4
	Twy:	.space 	800
	.align     4
	Suma:	.space  8
	a:	.double	104.9

.text
.global main

main:
; ---------------------------
	addi R2,R0,a
	ld F4,0(R2)
;----------------------------
	addi R6,R0,1
	movi2fp F0,R6
	cvti2d F2,F0
; ----------------------------
	addi R2,R0,T
;----------------------------
	addi R5,R0,7
;----------------------------
Petla1:
	movd F24,F22		; Ti+5 * Ti+4 * Ti+3
	multd F22,F20,F4		; Ti+6 * Ti+5 * Ti+4
	multd F20,F18,F4		; Ti+6 * Ti+5
	subi R5,R5,1
	movd F18,F4			; Ti+6
	movd F16,F14		; Ti+1
	movd F14,F12		; Ti+2
	movd F12,F10		; Ti+3
	movd F10,F8			; Ti+4
	movd F8,F6			; Ti+5
	movd F6,F4			; Ti+6

	sd 0(R2),F4
	addd F4,F4,F2
	addi R2,R2,8
	bnez R5,Petla1
; ----------------------------
	addi R3,R0,Twy
; ----------------------------
	subd F28,F22,F16 		; E = (Ti+6 * Ti+5 * Ti+4) - Ti+2
; ----------------------------
	addi R5,R0,100
; ----------------------------
	addd F26,F24,F14		; F = (Ti+5 * Ti+4 * Ti+3) + Ti+1
; ----------------------------
Petla2:
	divd F30,F28,F26		; E/F
	subi R5,R5,1
	subd F28,F22,F16 		; E = (Ti+6 * Ti+5 * Ti+4) - Ti+2
	addd F26,F24,F14		; F = (Ti+5 * Ti+4 * Ti+3) + Ti+1

	movd F24,F22		; Ti+5 * Ti+4 * Ti+3
	multd F22,F20,F4		; Ti+6 * Ti+5 * Ti+4
	multd F20,F18,F4		; Ti+6 * Ti+5
	movd F18,F4			; Ti+6

	movd F16,F14		; Ti+1
	movd F14,F12		; Ti+2
	movd F12,F10		; Ti+3
	movd F10,F8			; Ti+4
	movd F8,F6			; Ti+5
	movd F6,F4			; Ti+6

	sd 0(R3),F30
	addd F0,F0,F30

	sd 0(R2),F4

	addi R3,R3,8
	addi R2,R2,8


	addd F4,F4,F2
	bnez R5,Petla2
; ---------------------------- 
	divd F30,F28,F26		; E/F
; ----------------------------
	addi R5,R0,3
; ----------------------------
Petla3:
	sd 0(R2),F4
	subi R5,R5,1
	addd F4,F4,F2
	addi R2,R2,8
	bnez R5,Petla3
; ----------------------------
	sd 0(R3),F30
	sd Suma, F0
	trap 	0
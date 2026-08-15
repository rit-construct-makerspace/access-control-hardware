;--------------------------------------------------------
; File Created by SDCC : free open source ISO C Compiler
; Version 4.6.0 #16555 (MINGW64)
;--------------------------------------------------------
	.module main
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
	.globl _update_shift_register
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area DATA
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area INITIALIZED
;--------------------------------------------------------
; Stack segment in internal ram
;--------------------------------------------------------
	.area SSEG
__start__stack:
	.ds	1

;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area DABS (ABS)

; default segment ordering for linker
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area CONST
	.area INITIALIZER
	.area CODE

;--------------------------------------------------------
; interrupt vector
;--------------------------------------------------------
	.area HOME
__interrupt_vect:
	int s_GSINIT ; reset
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area GSINIT
	call	___sdcc_external_startup
	tnz	a
	jreq	__sdcc_init_data
	jp	__sdcc_program_startup
__sdcc_init_data:
; stm8_genXINIT() start
	ldw x, #l_DATA
	jreq	00002$
00001$:
	clr (s_DATA - 1, x)
	decw x
	jrne	00001$
00002$:
	ldw	x, #l_INITIALIZER
	jreq	00004$
00003$:
	ld	a, (s_INITIALIZER - 1, x)
	ld	(s_INITIALIZED - 1, x), a
	decw	x
	jrne	00003$
00004$:
; stm8_genXINIT() end
	.area GSFINAL
	jp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME
	.area HOME
__sdcc_program_startup:
	jp	_main
;	return from main will return to caller
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE
;	main.c: 23: void update_shift_register(uint8_t data) {
;	-----------------------------------------
;	 function update_shift_register
;	-----------------------------------------
_update_shift_register:
	push	a
;	main.c: 25: for (uint8_t i = 0; i < 8; i++) {
	clrw	x
00106$:
;	main.c: 28: PC_ODR |= (1 << DATA_PIN);  
	ld	a, 0x500a
;	main.c: 25: for (uint8_t i = 0; i < 8; i++) {
	push	a
	ld	a, xl
	cp	a, #0x08
	pop	a
	jrnc	00104$
;	main.c: 27: if (data & 0x80) {
	tnz	(0x01, sp)
	jrpl	00102$
;	main.c: 28: PC_ODR |= (1 << DATA_PIN);  
	or	a, #0x40
	ld	0x500a, a
	jra	00103$
00102$:
;	main.c: 30: PC_ODR &= ~(1 << DATA_PIN); 
	and	a, #0xbf
	ld	0x500a, a
00103$:
;	main.c: 33: PC_ODR |= (1 << CLK_PIN);   // Clock HIGH
	ld	a, 0x500a
	or	a, #0x80
	ld	0x500a, a
;	main.c: 34: PC_ODR &= ~(1 << CLK_PIN);  // Clock LOW
	ld	a, 0x500a
	and	a, #0x7f
	ld	0x500a, a
;	main.c: 36: data <<= 1; 
	sll	(0x01, sp)
;	main.c: 25: for (uint8_t i = 0; i < 8; i++) {
	incw	x
	jra	00106$
00104$:
;	main.c: 39: PC_ODR |= (1 << STORE_PIN);  // Store HIGH
	or	a, #0x20
	ld	0x500a, a
;	main.c: 40: PC_ODR &= ~(1 << STORE_PIN); // Store LOW
	bres	0x500a, #5
;	main.c: 41: }
	pop	a
	ret
;	main.c: 43: int main(void) {
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
	sub	sp, #5
;	main.c: 47: PB_DDR |= (1 << CONFIG_PIN1) | (1 << CONFIG_PIN2); 
	ld	a, 0x5007
	or	a, #0x30
	ld	0x5007, a
;	main.c: 48: PB_CR1 |= (1 << CONFIG_PIN1) | (1 << CONFIG_PIN2); // Push-pull
	ld	a, 0x5008
	or	a, #0x30
	ld	0x5008, a
;	main.c: 49: PB_ODR &= ~((1 << CONFIG_PIN1) | (1 << CONFIG_PIN2)); // Drive them LOW (stays here forever)
	ld	a, 0x5005
	and	a, #0xcf
	ld	0x5005, a
;	main.c: 52: PC_DDR &= ~(1 << ACCESS_PIN); 
	bres	0x500c, #3
;	main.c: 53: PC_CR1 |= (1 << ACCESS_PIN);  
	bset	0x500d, #3
;	main.c: 56: PC_DDR |= (1 << CLK_PIN) | (1 << DATA_PIN) | (1 << STORE_PIN); 
	ld	a, 0x500c
	or	a, #0xe0
	ld	0x500c, a
;	main.c: 57: PC_CR1 |= (1 << CLK_PIN) | (1 << DATA_PIN) | (1 << STORE_PIN); 
	ld	a, 0x500d
	or	a, #0xe0
	ld	0x500d, a
;	main.c: 60: PC_ODR &= ~((1 << CLK_PIN) | (1 << DATA_PIN) | (1 << STORE_PIN));
	ld	a, 0x500a
	and	a, #0x1f
	ld	0x500a, a
;	main.c: 63: update_shift_register(0xFF);
	ld	a, #0xff
	call	_update_shift_register
;	main.c: 66: uint8_t current_state = (PC_IDR & (1 << ACCESS_PIN)) ? 1 : 0;
	btjf	0x500b, #3, 00117$
	ld	a, #0x01
	.byte 0x21
00117$:
	clr	a
00118$:
	ld	(0x03, sp), a
;	main.c: 71: while(1) {
00110$:
;	main.c: 73: current_state = (PC_IDR & (1 << ACCESS_PIN)) ? 1 : 0;
	btjf	0x500b, #3, 00119$
	ld	a, #0x01
	.byte 0x21
00119$:
	clr	a
00120$:
;	main.c: 76: if (current_state != last_state) {
	ld	(0x04, sp), a
	cp	a, (0x03, sp)
	jreq	00110$
;	main.c: 79: for(volatile uint16_t i = 0; i < 2000; i++); 
	clrw	x
	ldw	(0x01, sp), x
00113$:
	ldw	x, (0x01, sp)
	cpw	x, #0x07d0
	jrnc	00101$
	ldw	x, (0x01, sp)
	incw	x
	ldw	(0x01, sp), x
	jra	00113$
00101$:
;	main.c: 82: if (((PC_IDR & (1 << ACCESS_PIN)) ? 1 : 0) == current_state) {
	btjf	0x500b, #3, 00121$
	ld	a, #0x01
	ld	(0x05, sp), a
	.byte 0xc5
00121$:
	clr	(0x05, sp)
00122$:
	ld	a, (0x04, sp)
	cp	a, (0x05, sp)
	jrne	00110$
;	main.c: 84: if (current_state == 1) {
	ld	a, (0x04, sp)
	dec	a
	jrne	00103$
;	main.c: 86: update_shift_register(0xC0); // 0xC0 = 1100 0000
	ld	a, #0xc0
	call	_update_shift_register
	jra	00104$
00103$:
;	main.c: 89: update_shift_register(0x3F); // 0x3F = 0011 1111
	ld	a, #0x3f
	call	_update_shift_register
00104$:
;	main.c: 93: last_state = current_state; 
	ld	a, (0x04, sp)
	ld	(0x03, sp), a
	jra	00110$
;	main.c: 97: }
	addw	sp, #5
	ret
	.area CODE
	.area CONST
	.area INITIALIZER
	.area CABS (ABS)

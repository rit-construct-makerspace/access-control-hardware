                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ISO C Compiler
                                      3 ; Version 4.6.0 #16555 (MINGW64)
                                      4 ;--------------------------------------------------------
                                      5 	.module main
                                      6 	
                                      7 ;--------------------------------------------------------
                                      8 ; Public variables in this module
                                      9 ;--------------------------------------------------------
                                     10 	.globl _main
                                     11 	.globl _update_shift_register
                                     12 ;--------------------------------------------------------
                                     13 ; ram data
                                     14 ;--------------------------------------------------------
                                     15 	.area DATA
                                     16 ;--------------------------------------------------------
                                     17 ; ram data
                                     18 ;--------------------------------------------------------
                                     19 	.area INITIALIZED
                                     20 ;--------------------------------------------------------
                                     21 ; Stack segment in internal ram
                                     22 ;--------------------------------------------------------
                                     23 	.area SSEG
      000001                         24 __start__stack:
      000001                         25 	.ds	1
                                     26 
                                     27 ;--------------------------------------------------------
                                     28 ; absolute external ram data
                                     29 ;--------------------------------------------------------
                                     30 	.area DABS (ABS)
                                     31 
                                     32 ; default segment ordering for linker
                                     33 	.area HOME
                                     34 	.area GSINIT
                                     35 	.area GSFINAL
                                     36 	.area CONST
                                     37 	.area INITIALIZER
                                     38 	.area CODE
                                     39 
                                     40 ;--------------------------------------------------------
                                     41 ; interrupt vector
                                     42 ;--------------------------------------------------------
                                     43 	.area HOME
      008000                         44 __interrupt_vect:
      008000 82 00 80 07             45 	int s_GSINIT ; reset
                                     46 ;--------------------------------------------------------
                                     47 ; global & static initialisations
                                     48 ;--------------------------------------------------------
                                     49 	.area HOME
                                     50 	.area GSINIT
                                     51 	.area GSFINAL
                                     52 	.area GSINIT
      008007 CD 80 FE         [ 4]   53 	call	___sdcc_external_startup
      00800A 4D               [ 1]   54 	tnz	a
      00800B 27 03            [ 1]   55 	jreq	__sdcc_init_data
      00800D CC 80 04         [ 2]   56 	jp	__sdcc_program_startup
      008010                         57 __sdcc_init_data:
                                     58 ; stm8_genXINIT() start
      008010 AE 00 00         [ 2]   59 	ldw x, #l_DATA
      008013 27 07            [ 1]   60 	jreq	00002$
      008015                         61 00001$:
      008015 72 4F 00 00      [ 1]   62 	clr (s_DATA - 1, x)
      008019 5A               [ 2]   63 	decw x
      00801A 26 F9            [ 1]   64 	jrne	00001$
      00801C                         65 00002$:
      00801C AE 00 00         [ 2]   66 	ldw	x, #l_INITIALIZER
      00801F 27 09            [ 1]   67 	jreq	00004$
      008021                         68 00003$:
      008021 D6 80 2C         [ 1]   69 	ld	a, (s_INITIALIZER - 1, x)
      008024 D7 00 00         [ 1]   70 	ld	(s_INITIALIZED - 1, x), a
      008027 5A               [ 2]   71 	decw	x
      008028 26 F7            [ 1]   72 	jrne	00003$
      00802A                         73 00004$:
                                     74 ; stm8_genXINIT() end
                                     75 	.area GSFINAL
      00802A CC 80 04         [ 2]   76 	jp	__sdcc_program_startup
                                     77 ;--------------------------------------------------------
                                     78 ; Home
                                     79 ;--------------------------------------------------------
                                     80 	.area HOME
                                     81 	.area HOME
      008004                         82 __sdcc_program_startup:
      008004 CC 80 69         [ 2]   83 	jp	_main
                                     84 ;	return from main will return to caller
                                     85 ;--------------------------------------------------------
                                     86 ; code
                                     87 ;--------------------------------------------------------
                                     88 	.area CODE
                                     89 ;	main.c: 23: void update_shift_register(uint8_t data) {
                                     90 ;	-----------------------------------------
                                     91 ;	 function update_shift_register
                                     92 ;	-----------------------------------------
      00802D                         93 _update_shift_register:
      00802D 88               [ 1]   94 	push	a
                                     95 ;	main.c: 25: for (uint8_t i = 0; i < 8; i++) {
      00802E 5F               [ 1]   96 	clrw	x
      00802F                         97 00106$:
                                     98 ;	main.c: 28: PC_ODR |= (1 << DATA_PIN);  
      00802F C6 50 0A         [ 1]   99 	ld	a, 0x500a
                                    100 ;	main.c: 25: for (uint8_t i = 0; i < 8; i++) {
      008032 88               [ 1]  101 	push	a
      008033 9F               [ 1]  102 	ld	a, xl
      008034 A1 08            [ 1]  103 	cp	a, #0x08
      008036 84               [ 1]  104 	pop	a
      008037 24 25            [ 1]  105 	jrnc	00104$
                                    106 ;	main.c: 27: if (data & 0x80) {
      008039 0D 01            [ 1]  107 	tnz	(0x01, sp)
      00803B 2A 07            [ 1]  108 	jrpl	00102$
                                    109 ;	main.c: 28: PC_ODR |= (1 << DATA_PIN);  
      00803D AA 40            [ 1]  110 	or	a, #0x40
      00803F C7 50 0A         [ 1]  111 	ld	0x500a, a
      008042 20 05            [ 2]  112 	jra	00103$
      008044                        113 00102$:
                                    114 ;	main.c: 30: PC_ODR &= ~(1 << DATA_PIN); 
      008044 A4 BF            [ 1]  115 	and	a, #0xbf
      008046 C7 50 0A         [ 1]  116 	ld	0x500a, a
      008049                        117 00103$:
                                    118 ;	main.c: 33: PC_ODR |= (1 << CLK_PIN);   // Clock HIGH
      008049 C6 50 0A         [ 1]  119 	ld	a, 0x500a
      00804C AA 80            [ 1]  120 	or	a, #0x80
      00804E C7 50 0A         [ 1]  121 	ld	0x500a, a
                                    122 ;	main.c: 34: PC_ODR &= ~(1 << CLK_PIN);  // Clock LOW
      008051 C6 50 0A         [ 1]  123 	ld	a, 0x500a
      008054 A4 7F            [ 1]  124 	and	a, #0x7f
      008056 C7 50 0A         [ 1]  125 	ld	0x500a, a
                                    126 ;	main.c: 36: data <<= 1; 
      008059 08 01            [ 1]  127 	sll	(0x01, sp)
                                    128 ;	main.c: 25: for (uint8_t i = 0; i < 8; i++) {
      00805B 5C               [ 1]  129 	incw	x
      00805C 20 D1            [ 2]  130 	jra	00106$
      00805E                        131 00104$:
                                    132 ;	main.c: 39: PC_ODR |= (1 << STORE_PIN);  // Store HIGH
      00805E AA 20            [ 1]  133 	or	a, #0x20
      008060 C7 50 0A         [ 1]  134 	ld	0x500a, a
                                    135 ;	main.c: 40: PC_ODR &= ~(1 << STORE_PIN); // Store LOW
      008063 72 1B 50 0A      [ 1]  136 	bres	0x500a, #5
                                    137 ;	main.c: 41: }
      008067 84               [ 1]  138 	pop	a
      008068 81               [ 4]  139 	ret
                                    140 ;	main.c: 43: int main(void) {
                                    141 ;	-----------------------------------------
                                    142 ;	 function main
                                    143 ;	-----------------------------------------
      008069                        144 _main:
      008069 52 05            [ 2]  145 	sub	sp, #5
                                    146 ;	main.c: 47: PB_DDR |= (1 << CONFIG_PIN1) | (1 << CONFIG_PIN2); 
      00806B C6 50 07         [ 1]  147 	ld	a, 0x5007
      00806E AA 30            [ 1]  148 	or	a, #0x30
      008070 C7 50 07         [ 1]  149 	ld	0x5007, a
                                    150 ;	main.c: 48: PB_CR1 |= (1 << CONFIG_PIN1) | (1 << CONFIG_PIN2); // Push-pull
      008073 C6 50 08         [ 1]  151 	ld	a, 0x5008
      008076 AA 30            [ 1]  152 	or	a, #0x30
      008078 C7 50 08         [ 1]  153 	ld	0x5008, a
                                    154 ;	main.c: 49: PB_ODR &= ~((1 << CONFIG_PIN1) | (1 << CONFIG_PIN2)); // Drive them LOW (stays here forever)
      00807B C6 50 05         [ 1]  155 	ld	a, 0x5005
      00807E A4 CF            [ 1]  156 	and	a, #0xcf
      008080 C7 50 05         [ 1]  157 	ld	0x5005, a
                                    158 ;	main.c: 52: PC_DDR &= ~(1 << ACCESS_PIN); 
      008083 72 17 50 0C      [ 1]  159 	bres	0x500c, #3
                                    160 ;	main.c: 53: PC_CR1 |= (1 << ACCESS_PIN);  
      008087 72 16 50 0D      [ 1]  161 	bset	0x500d, #3
                                    162 ;	main.c: 56: PC_DDR |= (1 << CLK_PIN) | (1 << DATA_PIN) | (1 << STORE_PIN); 
      00808B C6 50 0C         [ 1]  163 	ld	a, 0x500c
      00808E AA E0            [ 1]  164 	or	a, #0xe0
      008090 C7 50 0C         [ 1]  165 	ld	0x500c, a
                                    166 ;	main.c: 57: PC_CR1 |= (1 << CLK_PIN) | (1 << DATA_PIN) | (1 << STORE_PIN); 
      008093 C6 50 0D         [ 1]  167 	ld	a, 0x500d
      008096 AA E0            [ 1]  168 	or	a, #0xe0
      008098 C7 50 0D         [ 1]  169 	ld	0x500d, a
                                    170 ;	main.c: 60: PC_ODR &= ~((1 << CLK_PIN) | (1 << DATA_PIN) | (1 << STORE_PIN));
      00809B C6 50 0A         [ 1]  171 	ld	a, 0x500a
      00809E A4 1F            [ 1]  172 	and	a, #0x1f
      0080A0 C7 50 0A         [ 1]  173 	ld	0x500a, a
                                    174 ;	main.c: 63: update_shift_register(0x3F);
      0080A3 A6 3F            [ 1]  175 	ld	a, #0x3f
      0080A5 CD 80 2D         [ 4]  176 	call	_update_shift_register
                                    177 ;	main.c: 66: uint8_t current_state = (PC_IDR & (1 << ACCESS_PIN)) ? 1 : 0;
      0080A8 72 07 50 0B 03   [ 2]  178 	btjf	0x500b, #3, 00117$
      0080AD A6 01            [ 1]  179 	ld	a, #0x01
      0080AF 21                     180 	.byte 0x21
      0080B0                        181 00117$:
      0080B0 4F               [ 1]  182 	clr	a
      0080B1                        183 00118$:
      0080B1 6B 03            [ 1]  184 	ld	(0x03, sp), a
                                    185 ;	main.c: 71: while(1) {
      0080B3                        186 00110$:
                                    187 ;	main.c: 73: current_state = (PC_IDR & (1 << ACCESS_PIN)) ? 1 : 0;
      0080B3 72 07 50 0B 03   [ 2]  188 	btjf	0x500b, #3, 00119$
      0080B8 A6 01            [ 1]  189 	ld	a, #0x01
      0080BA 21                     190 	.byte 0x21
      0080BB                        191 00119$:
      0080BB 4F               [ 1]  192 	clr	a
      0080BC                        193 00120$:
                                    194 ;	main.c: 76: if (current_state != last_state) {
      0080BC 6B 04            [ 1]  195 	ld	(0x04, sp), a
      0080BE 11 03            [ 1]  196 	cp	a, (0x03, sp)
      0080C0 27 F1            [ 1]  197 	jreq	00110$
                                    198 ;	main.c: 79: for(volatile uint16_t i = 0; i < 2000; i++); 
      0080C2 5F               [ 1]  199 	clrw	x
      0080C3 1F 01            [ 2]  200 	ldw	(0x01, sp), x
      0080C5                        201 00113$:
      0080C5 1E 01            [ 2]  202 	ldw	x, (0x01, sp)
      0080C7 A3 07 D0         [ 2]  203 	cpw	x, #0x07d0
      0080CA 24 07            [ 1]  204 	jrnc	00101$
      0080CC 1E 01            [ 2]  205 	ldw	x, (0x01, sp)
      0080CE 5C               [ 1]  206 	incw	x
      0080CF 1F 01            [ 2]  207 	ldw	(0x01, sp), x
      0080D1 20 F2            [ 2]  208 	jra	00113$
      0080D3                        209 00101$:
                                    210 ;	main.c: 82: if (((PC_IDR & (1 << ACCESS_PIN)) ? 1 : 0) == current_state) {
      0080D3 72 07 50 0B 05   [ 2]  211 	btjf	0x500b, #3, 00121$
      0080D8 A6 01            [ 1]  212 	ld	a, #0x01
      0080DA 6B 05            [ 1]  213 	ld	(0x05, sp), a
      0080DC C5                     214 	.byte 0xc5
      0080DD                        215 00121$:
      0080DD 0F 05            [ 1]  216 	clr	(0x05, sp)
      0080DF                        217 00122$:
      0080DF 7B 04            [ 1]  218 	ld	a, (0x04, sp)
      0080E1 11 05            [ 1]  219 	cp	a, (0x05, sp)
      0080E3 26 CE            [ 1]  220 	jrne	00110$
                                    221 ;	main.c: 84: if (current_state == 1) {
      0080E5 7B 04            [ 1]  222 	ld	a, (0x04, sp)
      0080E7 4A               [ 1]  223 	dec	a
      0080E8 26 06            [ 1]  224 	jrne	00103$
                                    225 ;	main.c: 86: update_shift_register(0x00); 
      0080EA 4F               [ 1]  226 	clr	a
      0080EB CD 80 2D         [ 4]  227 	call	_update_shift_register
      0080EE 20 05            [ 2]  228 	jra	00104$
      0080F0                        229 00103$:
                                    230 ;	main.c: 89: update_shift_register(0x3F); 
      0080F0 A6 3F            [ 1]  231 	ld	a, #0x3f
      0080F2 CD 80 2D         [ 4]  232 	call	_update_shift_register
      0080F5                        233 00104$:
                                    234 ;	main.c: 93: last_state = current_state; 
      0080F5 7B 04            [ 1]  235 	ld	a, (0x04, sp)
      0080F7 6B 03            [ 1]  236 	ld	(0x03, sp), a
      0080F9 20 B8            [ 2]  237 	jra	00110$
                                    238 ;	main.c: 97: }
      0080FB 5B 05            [ 2]  239 	addw	sp, #5
      0080FD 81               [ 4]  240 	ret
                                    241 	.area CODE
                                    242 	.area CONST
                                    243 	.area INITIALIZER
                                    244 	.area CABS (ABS)

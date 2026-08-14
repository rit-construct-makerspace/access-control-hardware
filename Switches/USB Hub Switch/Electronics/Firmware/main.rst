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
      008007 CD 80 F3         [ 4]   53 	call	___sdcc_external_startup
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
                                     89 ;	main.c: 22: void update_shift_register(uint8_t data) {
                                     90 ;	-----------------------------------------
                                     91 ;	 function update_shift_register
                                     92 ;	-----------------------------------------
      00802D                         93 _update_shift_register:
      00802D 88               [ 1]   94 	push	a
                                     95 ;	main.c: 24: for (uint8_t i = 0; i < 8; i++) {
      00802E 5F               [ 1]   96 	clrw	x
      00802F                         97 00106$:
                                     98 ;	main.c: 27: PC_ODR |= (1 << DATA_PIN);  
      00802F C6 50 0A         [ 1]   99 	ld	a, 0x500a
                                    100 ;	main.c: 24: for (uint8_t i = 0; i < 8; i++) {
      008032 88               [ 1]  101 	push	a
      008033 9F               [ 1]  102 	ld	a, xl
      008034 A1 08            [ 1]  103 	cp	a, #0x08
      008036 84               [ 1]  104 	pop	a
      008037 24 25            [ 1]  105 	jrnc	00104$
                                    106 ;	main.c: 26: if (data & 0x80) {
      008039 0D 01            [ 1]  107 	tnz	(0x01, sp)
      00803B 2A 07            [ 1]  108 	jrpl	00102$
                                    109 ;	main.c: 27: PC_ODR |= (1 << DATA_PIN);  
      00803D AA 40            [ 1]  110 	or	a, #0x40
      00803F C7 50 0A         [ 1]  111 	ld	0x500a, a
      008042 20 05            [ 2]  112 	jra	00103$
      008044                        113 00102$:
                                    114 ;	main.c: 29: PC_ODR &= ~(1 << DATA_PIN); 
      008044 A4 BF            [ 1]  115 	and	a, #0xbf
      008046 C7 50 0A         [ 1]  116 	ld	0x500a, a
      008049                        117 00103$:
                                    118 ;	main.c: 32: PC_ODR |= (1 << CLK_PIN);   // Clock HIGH
      008049 C6 50 0A         [ 1]  119 	ld	a, 0x500a
      00804C AA 80            [ 1]  120 	or	a, #0x80
      00804E C7 50 0A         [ 1]  121 	ld	0x500a, a
                                    122 ;	main.c: 33: PC_ODR &= ~(1 << CLK_PIN);  // Clock LOW
      008051 C6 50 0A         [ 1]  123 	ld	a, 0x500a
      008054 A4 7F            [ 1]  124 	and	a, #0x7f
      008056 C7 50 0A         [ 1]  125 	ld	0x500a, a
                                    126 ;	main.c: 35: data <<= 1; 
      008059 08 01            [ 1]  127 	sll	(0x01, sp)
                                    128 ;	main.c: 24: for (uint8_t i = 0; i < 8; i++) {
      00805B 5C               [ 1]  129 	incw	x
      00805C 20 D1            [ 2]  130 	jra	00106$
      00805E                        131 00104$:
                                    132 ;	main.c: 38: PC_ODR |= (1 << STORE_PIN);  // Store HIGH
      00805E AA 20            [ 1]  133 	or	a, #0x20
      008060 C7 50 0A         [ 1]  134 	ld	0x500a, a
                                    135 ;	main.c: 39: PC_ODR &= ~(1 << STORE_PIN); // Store LOW
      008063 72 1B 50 0A      [ 1]  136 	bres	0x500a, #5
                                    137 ;	main.c: 40: }
      008067 84               [ 1]  138 	pop	a
      008068 81               [ 4]  139 	ret
                                    140 ;	main.c: 42: int main(void) {
                                    141 ;	-----------------------------------------
                                    142 ;	 function main
                                    143 ;	-----------------------------------------
      008069                        144 _main:
      008069 52 05            [ 2]  145 	sub	sp, #5
                                    146 ;	main.c: 46: PB_DDR |= (1 << CONFIG_PIN); 
      00806B 72 18 50 07      [ 1]  147 	bset	0x5007, #4
                                    148 ;	main.c: 47: PB_CR1 |= (1 << CONFIG_PIN); // Push-pull
      00806F 72 18 50 08      [ 1]  149 	bset	0x5008, #4
                                    150 ;	main.c: 48: PB_ODR &= ~(1 << CONFIG_PIN); // Drive it LOW (stays here forever)
      008073 72 19 50 05      [ 1]  151 	bres	0x5005, #4
                                    152 ;	main.c: 51: PC_DDR &= ~(1 << ACCESS_PIN); 
      008077 72 17 50 0C      [ 1]  153 	bres	0x500c, #3
                                    154 ;	main.c: 52: PC_CR1 |= (1 << ACCESS_PIN);  
      00807B 72 16 50 0D      [ 1]  155 	bset	0x500d, #3
                                    156 ;	main.c: 55: PC_DDR |= (1 << CLK_PIN) | (1 << DATA_PIN) | (1 << STORE_PIN); 
      00807F C6 50 0C         [ 1]  157 	ld	a, 0x500c
      008082 AA E0            [ 1]  158 	or	a, #0xe0
      008084 C7 50 0C         [ 1]  159 	ld	0x500c, a
                                    160 ;	main.c: 56: PC_CR1 |= (1 << CLK_PIN) | (1 << DATA_PIN) | (1 << STORE_PIN); 
      008087 C6 50 0D         [ 1]  161 	ld	a, 0x500d
      00808A AA E0            [ 1]  162 	or	a, #0xe0
      00808C C7 50 0D         [ 1]  163 	ld	0x500d, a
                                    164 ;	main.c: 59: PC_ODR &= ~((1 << CLK_PIN) | (1 << DATA_PIN) | (1 << STORE_PIN));
      00808F C6 50 0A         [ 1]  165 	ld	a, 0x500a
      008092 A4 1F            [ 1]  166 	and	a, #0x1f
      008094 C7 50 0A         [ 1]  167 	ld	0x500a, a
                                    168 ;	main.c: 62: update_shift_register(0xFF);
      008097 A6 FF            [ 1]  169 	ld	a, #0xff
      008099 CD 80 2D         [ 4]  170 	call	_update_shift_register
                                    171 ;	main.c: 65: uint8_t current_state = (PC_IDR & (1 << ACCESS_PIN)) ? 1 : 0;
      00809C 72 07 50 0B 03   [ 2]  172 	btjf	0x500b, #3, 00117$
      0080A1 A6 01            [ 1]  173 	ld	a, #0x01
      0080A3 21                     174 	.byte 0x21
      0080A4                        175 00117$:
      0080A4 4F               [ 1]  176 	clr	a
      0080A5                        177 00118$:
      0080A5 6B 03            [ 1]  178 	ld	(0x03, sp), a
                                    179 ;	main.c: 70: while(1) {
      0080A7                        180 00110$:
                                    181 ;	main.c: 72: current_state = (PC_IDR & (1 << ACCESS_PIN)) ? 1 : 0;
      0080A7 72 07 50 0B 03   [ 2]  182 	btjf	0x500b, #3, 00119$
      0080AC A6 01            [ 1]  183 	ld	a, #0x01
      0080AE 21                     184 	.byte 0x21
      0080AF                        185 00119$:
      0080AF 4F               [ 1]  186 	clr	a
      0080B0                        187 00120$:
                                    188 ;	main.c: 75: if (current_state != last_state) {
      0080B0 6B 04            [ 1]  189 	ld	(0x04, sp), a
      0080B2 11 03            [ 1]  190 	cp	a, (0x03, sp)
      0080B4 27 F1            [ 1]  191 	jreq	00110$
                                    192 ;	main.c: 78: for(volatile uint16_t i = 0; i < 2000; i++); 
      0080B6 5F               [ 1]  193 	clrw	x
      0080B7 1F 01            [ 2]  194 	ldw	(0x01, sp), x
      0080B9                        195 00113$:
      0080B9 1E 01            [ 2]  196 	ldw	x, (0x01, sp)
      0080BB A3 07 D0         [ 2]  197 	cpw	x, #0x07d0
      0080BE 24 07            [ 1]  198 	jrnc	00101$
      0080C0 1E 01            [ 2]  199 	ldw	x, (0x01, sp)
      0080C2 5C               [ 1]  200 	incw	x
      0080C3 1F 01            [ 2]  201 	ldw	(0x01, sp), x
      0080C5 20 F2            [ 2]  202 	jra	00113$
      0080C7                        203 00101$:
                                    204 ;	main.c: 81: if (((PC_IDR & (1 << ACCESS_PIN)) ? 1 : 0) == current_state) {
      0080C7 72 07 50 0B 05   [ 2]  205 	btjf	0x500b, #3, 00121$
      0080CC A6 01            [ 1]  206 	ld	a, #0x01
      0080CE 6B 05            [ 1]  207 	ld	(0x05, sp), a
      0080D0 C5                     208 	.byte 0xc5
      0080D1                        209 00121$:
      0080D1 0F 05            [ 1]  210 	clr	(0x05, sp)
      0080D3                        211 00122$:
      0080D3 7B 04            [ 1]  212 	ld	a, (0x04, sp)
      0080D5 11 05            [ 1]  213 	cp	a, (0x05, sp)
      0080D7 26 CE            [ 1]  214 	jrne	00110$
                                    215 ;	main.c: 83: if (current_state == 1) {
      0080D9 7B 04            [ 1]  216 	ld	a, (0x04, sp)
      0080DB 4A               [ 1]  217 	dec	a
      0080DC 26 07            [ 1]  218 	jrne	00103$
                                    219 ;	main.c: 85: update_shift_register(0xC0); // 0xC0 = 1100 0000
      0080DE A6 C0            [ 1]  220 	ld	a, #0xc0
      0080E0 CD 80 2D         [ 4]  221 	call	_update_shift_register
      0080E3 20 05            [ 2]  222 	jra	00104$
      0080E5                        223 00103$:
                                    224 ;	main.c: 88: update_shift_register(0x3F); // 0x3F = 0011 1111
      0080E5 A6 3F            [ 1]  225 	ld	a, #0x3f
      0080E7 CD 80 2D         [ 4]  226 	call	_update_shift_register
      0080EA                        227 00104$:
                                    228 ;	main.c: 92: last_state = current_state; 
      0080EA 7B 04            [ 1]  229 	ld	a, (0x04, sp)
      0080EC 6B 03            [ 1]  230 	ld	(0x03, sp), a
      0080EE 20 B7            [ 2]  231 	jra	00110$
                                    232 ;	main.c: 96: }
      0080F0 5B 05            [ 2]  233 	addw	sp, #5
      0080F2 81               [ 4]  234 	ret
                                    235 	.area CODE
                                    236 	.area CONST
                                    237 	.area INITIALIZER
                                    238 	.area CABS (ABS)

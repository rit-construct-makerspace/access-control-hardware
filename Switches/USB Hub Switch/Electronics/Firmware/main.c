#include <stdint.h>

// Port B Registers (For Configuration Pins)
#define PB_ODR *(volatile uint8_t *)0x5005 
#define PB_DDR *(volatile uint8_t *)0x5007 
#define PB_CR1 *(volatile uint8_t *)0x5008 

// Port C Registers (For Shift Register and ACCESS Pin)
#define PC_ODR *(volatile uint8_t *)0x500A 
#define PC_IDR *(volatile uint8_t *)0x500B 
#define PC_DDR *(volatile uint8_t *)0x500C 
#define PC_CR1 *(volatile uint8_t *)0x500D 

// --- PIN DEFINITIONS ---
#define ACCESS_PIN  3 // Port C
#define CLK_PIN     7 // Port C (SHCP - Shift Register Clock)
#define DATA_PIN    6 // Port C (DS - Serial Data Input)
#define STORE_PIN   5 // Port C (STCP - Storage Register Clock / Latch)
#define CONFIG_PIN1 4 // Port B
#define CONFIG_PIN2 5 // Port B

// Function to shift 8 bits to the 74HC595
void update_shift_register(uint8_t data) {
    // Shift out 8 bits, MSB (bit 7) first
    for (uint8_t i = 0; i < 8; i++) {
        
        if (data & 0x80) {
            PC_ODR |= (1 << DATA_PIN);  
        } else {
            PC_ODR &= ~(1 << DATA_PIN); 
        }
        
        PC_ODR |= (1 << CLK_PIN);   // Clock HIGH
        PC_ODR &= ~(1 << CLK_PIN);  // Clock LOW
        
        data <<= 1; 
    }
    
    PC_ODR |= (1 << STORE_PIN);  // Store HIGH
    PC_ODR &= ~(1 << STORE_PIN); // Store LOW
}

int main(void) {
    // --- SETUP ---
    
    // Configure PB4 and PB5 as outputs and set them LOW
    PB_DDR |= (1 << CONFIG_PIN1) | (1 << CONFIG_PIN2); 
    PB_CR1 |= (1 << CONFIG_PIN1) | (1 << CONFIG_PIN2); // Push-pull
    PB_ODR &= ~((1 << CONFIG_PIN1) | (1 << CONFIG_PIN2)); // Drive them LOW (stays here forever)

    // Configure PC3 (ACCESS) as an Input
    PC_DDR &= ~(1 << ACCESS_PIN); 
    PC_CR1 |= (1 << ACCESS_PIN);  
    
    // Configure PC5, PC6, PC7 as Outputs
    PC_DDR |= (1 << CLK_PIN) | (1 << DATA_PIN) | (1 << STORE_PIN); 
    PC_CR1 |= (1 << CLK_PIN) | (1 << DATA_PIN) | (1 << STORE_PIN); 
    
    // Ensure initial SPI clock/data/latch states are low
    PC_ODR &= ~((1 << CLK_PIN) | (1 << DATA_PIN) | (1 << STORE_PIN));

    // Set outputs 0-5 HIGH and 6-7 LOW on boot (0x3F = 0011 1111)
    update_shift_register(0x3F);

    // Read the starting state of the ACCESS pin
    uint8_t current_state = (PC_IDR & (1 << ACCESS_PIN)) ? 1 : 0;
    uint8_t last_state = current_state;
    
    // --- MAIN LOOP ---
    
    while(1) {
        // Read the current state of the ACCESS pin
        current_state = (PC_IDR & (1 << ACCESS_PIN)) ? 1 : 0;
        
        // If the state has changed since the last time we checked...
        if (current_state != last_state) {
            
            // Brief delay to debounce
            for(volatile uint16_t i = 0; i < 2000; i++); 
            
            // Read it again to confirm it wasn't just electrical noise
            if (((PC_IDR & (1 << ACCESS_PIN)) ? 1 : 0) == current_state) {
                
                if (current_state == 1) {
                    // Low to High transition: All outputs LOW (0x00 = 0000 0000)
                    update_shift_register(0x00); 
                } else {
                    // High to Low transition: 0-5 HIGH, 6-7 LOW (0x3F = 0011 1111)
                    update_shift_register(0x3F); 
                }
                
                // Save the new state
                last_state = current_state; 
            }
        }
    }
}
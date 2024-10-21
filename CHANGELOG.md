# Changelog

Click on a section to expand it and view changes

<details>
  <summary>Core Main Board</summary>

  ## V2.3.2 - September 2024
  * Added ethernet to the board - board can be hard-coded to WiFi or ethernet, or swap dynamically.
  * Added DIP switches for configuring common settings - buzzer volume, board mode, etc.
  * Replaced EEPROM with internal key:value encrypted storage on ESP32
  * Added AtTiny to handle frontend (button, LED, key switch) for faster response times, no worry about blocking network communications
  * Added footprints for integrated UPDI programmer, USB debugger, USB hub for faster development and deployment
  * Replaced board detection switches with more robust limit switches, makes for more reliable operation
  * Replaced connections for button and key switch with angled screw blocks for easier integration
  * Repositioned buzzer for better sound quality

  ## V2.3.1 - August 2024
  * Major change to board form factor
  * Internal development model - limited information available.
  
  ## V2.2.2 - July 2024
  * Bugfixes
  * Internal development model - limited information available.
  
  ## V2.2.1 - June 2024
  * Major overhaul to the design
  * Removed USB port, replaced with programming header
  * Removed help button, CBI LEDs
  * Added second DB9 connector
  * Replaced MicroSD card slot with DIP8 socket for configuration EEPROM
  * Added RS232 to DB9 connectors, for interfacing with computer or interface
  * Added data flow control routing, to direct UART connection between programming/RS232 DB9 1/RS232 DB9 2
  * Added 2 more soldered nuts to direct card on insertion
  * Decreased overall board size for tighter integrations
  
  ## V2.1.1 - May 2024
  * Initial public relaese
  
  ## V2.0.1 - May 2024
  * Internal version for internal customer discovery
</details>

<details>
  <summary>Core Interface Board</summary>

  ## V2.0.1 - June 2024
  * Initial release
</details>

<details>
  <summary>USB Hub Switch</summary>
  
  ## V2.0.2 - June 2024
  * Changed to easier-to-solder USB-B port
  * Increased spacing between USB port and DB9

  ## V2.0.1 - May 2024
  * Initial release
</details>

<details>
  <summary>KMX Switch</summary>
  
  ## V2.0.1 - June 2024
  * Initial release
</details>

<details>
  <summary>Signal Relay</summary>

  ## V2.0.2 - June 2024
  * Add USB port, fuse for powering system
  * Swapped to mostly SMD parts for easier assembly
  * Replaced relay with 220V-rated variant for directly interfacing with contactor signals
  
  ## V2.0.1 - May 2024
  * Initial release
</details>

<details>
  <summary>AC Relay</summary>

  ## V2.0.2 - June 2024
  * Flipped AC wiring of the relay
  * Replaced monolithic 5v regulator with Texas Instruments circuit
    * Higher efficency
    * Smaller footprint
    * Works with higher capacitance
  * Added double spade connectors for neutral, earth wiring
    * Makes it easier to wire plugs through the board
  * Added threaded standoffs for mounting the board
  
  ## V2.0.1 - May 2024
  * Initial release
</details>

<details>
  <summary>Contactor Relay</summary>

  ## V2.0.1 - June 2024
  * Initial Release
</details>

<details>
  <summary>USB Adapter</summary>
  
  ## V2.0.1 - June 2024
  * Initial release
  * Allows the Core to connect via USB to a computer through DB9
</details>

<details>
  <summary>EEPROM Programmer</summary>
  
  ## V2.0.1 - June 2024
  * Initial release
</details>

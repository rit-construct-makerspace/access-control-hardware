## System-Wide:

* Write documentation for all new updates, switches, etc.

## Core
* Diode OR 5v and switch power to enable to MT9700, or else they will never enable when power is connected, or when an interface is connected
* Consider consolidating the interface screw terminals into the main board
  * Makes for a smaller, tighter integration
  * Reduces costs and assembly complexity
  * Faster response time to pressed and key changes
  * Ability to run more complex lighting animations
  * Frees up second DB9 to use as a debug port, future expansion (LCD, second switch, etc.)
* Remove EEPROM socket
  * Now that we have API keys and a WiFi password, we don't want that easily accessible
  * ESP32 has built-in encrypted "preferences.h"
  * These persist through firmware updates as well
  * Set up to take a JSON document to update parameters via serial (make program to configure?)
* Route UART0 to DB91, UART1 to DB92
  * Eliminates the need for UART mux circuitry, allows debug to be read when in use, etc.
  * Detect what port isn't the switch on startup, and make that the serial output
* Add a DIP switch to choose board mode
  * Consolidate the code bases for sign in and machine access
  * Read the switch on startup to determine the mode of the system
  * Simplfies code upkeep
* Make mechanical mounting easier, swap to M3 screws if possible
* Long-Term Future goals:
  * Add Ethernet to the box for faster connection
  * Swap to using our own deployment of an NFC reader on the board directly for ease of manufacturing and reliability

## USB Adapter
* RX and TX are flipped, need to correct
* Swap 232 RX and TX to interface with a Core

## Interface
* RS232 RX and TX are flipped
* Make UPDI test point larger for easier programming
* Considering retiring - see Core notes above

## AC Relay
* Make the system more robust and eliminate the need for wires
* Use Nema 5-15R receptacles meant for power strips that board mounts
* Switch to an IEC that is directly board mounted
* Board-mount a fuse, or better yet a resettable switch-type one

## New Switch: USB Interruptor
* Single USB in, USB out
* Switch fully interrupts not only power but also data
* Useful for systems without an OS to support hubs, self-powered devices
* Steals some power from USB

## New Component: Remote GPIO
* Allows machines in a room to send a second, lower-priority command wirelessly to a device
* Example Application: If any machine in the machine shop is powered on, turn on a warning light so deaf/HoH users know to be alert
* Example Application: Automate the opening/closing of blast gates in the wood shop depending on what machine is turned on

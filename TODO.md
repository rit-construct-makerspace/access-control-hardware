## System-Wide:

* Write documentation for all new updates, switches, etc.

## Core
* Keep track of pink eye bug, seemingly fixed
* Write code for ethernet, DIP switches on backend, and sign-in reader
* Design case for sign-in reader
* Fix USB wiring issue on full edition of the board
* Long-Term Future goals:
  * Swap to using our own deployment of an NFC reader on the board directly for ease of manufacturing and reliability
  * Write configuration software to eliminate the need for the serial terminal

## AC Relay
* Test board, case with new NEMA 5-15R from Qualtek
* Update case to have mounting for Dual-Lock

## New Switch: USB Interruptor
* Single USB in, USB out
* Switch fully interrupts not only power but also data
* Useful for systems without an OS to support hubs, self-powered devices
* Steals some power from USB

## New Component: Bluetooth Trigger
* Device that can be wirelessly linked to an ACS core, to act as a remote GPIO
* Developed and intended for automated blast gate control applications, but useful for other tasks like illuminated warning lights

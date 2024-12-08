## System-Wide:

* Write documentation for all new updates, switches, etc.

## Core
* Design case for sign-in reader
* Fix USB wiring issue on full edition of the board
* Software:
  * Develop sign-in reader code
  * Develop Ethernet code
  * Implement all DIP switch funcitonality
  * Overhaul how key overrides work; system stores a local list of approved users, key switch only works if one of their IDs are present. 
* Long-Term Future goals:
  * Swap to using our own deployment of an NFC reader on the board directly for ease of manufacturing and reliability
  * Write configuration software to eliminate the need for the serial terminal

## AC Relay
* All set!

## New Switch: USB Interruptor
* Single USB in, USB out
* Switch fully interrupts not only power but also data
* Useful for systems without an OS to support hubs, self-powered devices
* Steals some power from USB

## New Component: Status Board
* Simple hardware interface that allows shop managers to see the state of machines in their spaces with illuminated warnings. 

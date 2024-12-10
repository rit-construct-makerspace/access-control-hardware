## System-Wide:

* Write documentation for all new updates, switches, etc.

## Core
* Design case for sign-in reader
* Fix USB wiring issue on full edition of the board (only needed if keeping current USB hub design, probably not).
* Replace current USB implementation with a CP210x, allowing for custom name/ID. 
* Software:
  * Develop sign-in reader code
  * Develop Ethernet code
  * Implement all DIP switch funcitonality
  * Implement code to check for proper type of switch connected
  * Overhaul how key overrides work; system stores a local list of approved users, key switch only works if one of their IDs are present.
    * Every time an ID is approved, add them to the internal list of 1000 approved UIDs. If list is full, remove the oldest entry.
    * Every time an ID is not approved, remove it from the list.
* Long-Term Future goals:
  * Swap to using our own deployment of an NFC reader on the board directly for ease of manufacturing and reliability
  * Write configuration software to eliminate the need for the serial terminal

## New Switch: USB Interruptor
* Single USB in, USB out
* Switch fully interrupts not only power but also data
* Useful for systems without an OS to support hubs, self-powered devices
* Steals some power from USB

## New Component: Splitter
* Simple device that allows multiple downstream devices to connect to the same Core.

## New Component: Status Board
* Simple hardware interface that allows shop managers to see the state of machines in their spaces with illuminated warnings. 

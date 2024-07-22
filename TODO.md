## System-Wide:

* Write documentation for all new updates, switches, etc.

## Core
* Make mechanical mounting easier, swap to M3 screws if possible
* Long-Term Future goals:
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
  * TE 3-213598-2 likely choice
* Switch to an IEC that is directly board mounted
  * Adam Tech has a nice right-angle board-mmount and panel-mount option, need to find PN for that
* Board-mount a fuse, or better yet a resettable switch-type one found in power strips.

## New Switch: USB Interruptor
* Single USB in, USB out
* Switch fully interrupts not only power but also data
* Useful for systems without an OS to support hubs, self-powered devices
* Steals some power from USB

## New Component: Remote GPIO
* Allows machines in a room to send a second, lower-priority command wirelessly to a device
* Example Application: If any machine in the machine shop is powered on, turn on a warning light so deaf/HoH users know to be alert
* Example Application: Automate the opening/closing of blast gates in the wood shop depending on what machine is turned on

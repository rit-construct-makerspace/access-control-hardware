## System-Wide:

* Write documentation for all new updates, switches, etc.

## Core
* Design case for sign-in reader
* Fix USB wiring issue on full edition of the board (only needed if keeping current USB hub design, probably not).
* Remove DIP Switches
  * Not very convenient to open and edit, may be easier/better to just edit all in software.
* Wire speaker to ESP32 DAC pin to allow for Talkie library
* Replace S2 and AtTiny with S3 (Maybe ESP32-S3-WROOM-1-N16R8), dual-core for simpler programming and consolidated design.
  * Use the USB stack inside the ESP32, Espressif will give us a PID for this so it can have a custom name. 
* Software:
  * Develop sign-in reader code
  * Develop Ethernet code
  * Implement code to check for proper type of switch connected
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

## New Component: Logic Connector
* Allows 2 ACS Cores to control the same system in one of 3 logical combination modes;
  * OR: Either of the readers can activate the device or keep it online. Good for handing off machine between 2 people.
  * AND: Both IDs must be present for the system to unlock. Good for complex equipment and enforces a buddy system of trained users.
  * AND START OR: Both IDs must be present to activate the device, but only one is needed to keep it active. For machines that we want a staff to check 100% of the time before starting, but then students operate.

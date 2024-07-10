## System-Wide:

* Write documentation for all new updates, switches, etc.

## Core
* Diode OR 5v and switch power to enable to MT9700, or else they will never enable when power is connected, or when an interface is connected

## USB Adapter
* RX and TX are flipped, need to correct
* Add switch to swap 232 RX and TX, so you can communicate to either the Core or the Interface for debugging

## Interface
* RS232 RX and TX are flipped
* Make UPDI test point larger for easier programming

## New Component: Relay
* Simple device to act as an APSTA to extend the network
  * Now that the Core is only a STA and the Gateway is only an AP, need Relay to extend the network
* ESP32, indicator light, and EEPROM same as Core for getting SSID and password

## New Switch: USB Interruptor
* Single USB in, USB out
* Switch fully interrupts not only power but also data
* Useful for systems without an OS to support hubs, self-powered devices
* Steals some power from USB

## New Component: Switch Hub
* Allows for multiple switches to be used at the same time in a system
* 1 upsteream DB9 to core, 4 (?) downstream ones
* Power from each diode OR'ed together, consider idea diode controllers?
* Line driver on hub helps with fan-out on NO signal
* Address line grounded to indiate hub present
 * This is how I was going to indicate the User Interface, so maybe need alternative solution? 

## New Component: Monitor Panel
* Allows lab manager to monitor machine states, respond to help requests
* Acts as a gateway for Cores to connect to internet, database
* This may be replaced with a Python program instead. 

## New Component: Remote GPIO
* Allows machines in a room to send a second, lower-priority command wirelessly to a device
* Example Application: If any machine in the machine shop is powered on, turn on a warning light so deaf/HoH users know to be alert
* Example Application: Automate the opening/closing of blast gates in the wood shop depending on what machine is turned on

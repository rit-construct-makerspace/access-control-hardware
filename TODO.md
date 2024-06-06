## System-Wide:

* Design Concept Change: Power for the system must come from the switch, even if the switch cannot harvest the power itself
* Switch as many components to SMD as possible
* Add solder nuts to all boards.
* Add diode to output of each switch.
* Switch to cheaper 3rd party DS18B20 temperature sensors

* Write documentation for all new updates, switches, etc. 

## Core:

* Issues Found in Testing:
  * Power path control circuitry only works if there's 3.3v present, needs to be jump-started basically.
* Things found working in testing:
  * USB-UART bridge
  * ESP32 UART connection
  * Regulator
  * Auto flashing circuitry
  * debug LED
  * Buzzer and driver circuitry 
* Add level shifting, pulldown resistor to incoming interrupt pins
* Remove 4x M3 solder nuts on bottom (adhesive attachment likely in deployment). WIP
* Add 2x M2 tall solder nuts to left and right of card to constrain card when inserted, better assembe box. WIP
* Move R9 further from the center, so the "NFC Spacer" part has more PCB to adhere to.
* Better alternative to MicroSD card for config file. WIP
  * Maybe OTP EPROM in a DIP-8 slot?
  * More obscure, can be fully enclosed in the system, more reliable connection.
  * Microchip AT24C128 or similar in standard format.
  * Should make custom programmer/configuration software
* Changes to user interface;
  * Implement UART switch to select between programming UART and RS232
* Consider replacing SOT-353 74LVC1G17GW, currently the hardest-to-PNP component on the board. WIP
* Consider replacing Littel Fuses with fuses that are smaller, cheaper. SMD thermal or automotive blade maybe? WIP
* If data flow control for reprogramming works, remove programming buttons and replace them with test pads
  * Update: If removing USB in favor of DB9 serial, leave buttons but make them smaller.
  * Add labels to buttons
* Add TVS to DB-9 connector.
* Remove native USB connection, MUX, powerpath control WIP
* Add second DB9 to the back for either a second switch or for the new user interface WIP

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

## New Extra: Bypass Plug
* Simple device that is just a DB9 to a switch
* Allows you to use a device with a power switch connected, without actually having to be integrated with the rest of the system
* Useful for allowing machines to still work while deploying things

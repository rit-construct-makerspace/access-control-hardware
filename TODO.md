## System-Wide:

* Test all electronics Vx.x.1
* Design Concept Change: Power for the system must come from the switch, even if the switch cannot harvest the power itself
* Switch as many components to SMD as possible
* Add solder nuts to all boards.
* Add diode to output of each switch.

## Core:

* U11 P3 not connected, should be grounded.
* Remove 4x M3 solder nuts on bottom (adhesive attachment likely in deployment).
* Add 2x M2 tall solder nuts to left and right of card to constrain card when inserted, better assembe box.
* Move R9 further from the center, so the "NFC Spacer" part has more PCB to adhere to.
* Better alternative to MicroSD card for config file.
  * Maybe OTP EPROM in a DIP-8 slot?
  * More obscure, can be fully enclosed in the system, more reliable connection.
  * Microchip AT24C128 or similar in standard format.
  * Should make custom programmer/configuration software
* Changes to user interface;
  * Externally mounted user interface for more versatile positioning
  * 19mm circular button with integrated WS2812 (19-B-M-F1)
    * Comes with a cable harness for easy mounting
  * Original 3 position key switch re-added to handle lockout
  * External UI uses same DB9 connection, repurposes 3 unused pins.
  * Communicate over RS232 UART, with interrupt pin
  * Implement UART switch to select between programming UART and RS232
* Consider replacing SOT-353 74LVC1G17GW, currently the hardest-to-PNP component on the board.
* Consider replacing Littel Fuses with fuses that are smaller, cheaper. SMD thermal or automotive blade maybe? 
* If data flow control for reprogramming works, remove programming buttons and replace them with test pads
* Add TVS to DB-9 connector.
* Remove native USB connection, MUX, powerpath control
* Diode USB power, DB9 power to regulator to run ESP32 for programming
* Add second DB9 to the back for either a second switch or for the new user interface
* Add secondary wiring connection in parallel to DB9, that can be used for smaller integrations of the system in a single box.
  * JST or similar connector with the same number of pins connected

## USB Switch
* Replace USB-C connector with something easier to solder?

## Signal Relay
* Add power inlet with fuse, USB connector likely
* Consider swapping to relay rated to 250VAC, so we can interrupt mains contactors on 220V systems

## New Switch: Contactor Switch
* Variant of the AC switch, designed to integrate with machines that already have a contactor embedded in them.
* Screw terminals directly on board for live in, live out, neutral, earth.
* Able to switch a resistive load of a few amps at 220VAC
* Same power harvesting circuitry as AC switch

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

## New Component: User interface
* See "core" section for more details
* Communicates as i2c slave with interrupt pin
* Basic microcontroller like AtTiny to support everything on this side

## New Component: EEPROM Programmer
* USB-connected EEPROM programmer with a ZIF socket
* Allows easy setup of the configuration EEPROM

## New Component: Monitor Panel
* Allows lab manager to monitor machine states, respond to help requests
* Acts as a gateway for Cores to connect to internet, database

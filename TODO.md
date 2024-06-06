## System-Wide:

* Design Concept Change: Power for the system must come from the switch, even if the switch cannot harvest the power itself
* Switch as many components to SMD as possible
* Add solder nuts to all boards.
* Add diode to output of each switch.
* Switch to cheaper 3rd party DS18B20 temperature sensors

* Write documentation for all new updates, switches, etc. 
* Update changelog with major release

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

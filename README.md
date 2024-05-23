# Makerspace Access Control System

## Summary
There's a lot of equipment in a makerspace that can be dangerous to operate without proper training. To make sure only trained personnel are accessing equipment at the RIT SHED Makerspace, an access control system was developed. This system works off of student/faculty/staff NFC ID cards, and checks a central database (automatically updated by training programs) to ensure users are trained to use the machine they are trying to access. A number of different switching mechanisms are developed, to allow maximum flexibility in integration with a wide array of machines. 

### More information can be found on the [GitHub Wiki](https://github.com/rit-construct-makerspace/access-control-hardware/wiki).

## Current Release
There are no stable versions for release. The latest WIP is Version 2.1

## Current State
The access control system is currently being developed to version 2.1.X. 

## TODO
* Write firmware
* Test all electronics Vx.x.1
* Improvements to boards based on initial testing:
    * Core: U11 P3 not connected, should be grounded.
    * Core: Remove 4x M3 solder nuts on bottom (adhesive attachment likely in deployment).
    * Core: Add 2x M2 tall solder nuts to left and right of card to constrain card when inserted, better assembe box.
    * Core: Move R9 further from the center, so the "NFC Spacer" part has more PCB to adhere to.
    * Core: Consider switching button on the front? This one is overkill and has a lot more circuits tha needed.
    * Core: Better alternative to MicroSD card for config file. Maybe OTP EPROM in a DIP-8 slot? More obscure, can be fully enclosed in the system, more reliable connection. Microchip AT24C128 or similar in standard format.
    * Core: Consider swapping to RGB CBI LED? Takes up less space, more room for a button now.
    * Core: Consider mounting LEDs and button on daughterboard with cable? Allows more versatile enclosure designs, but at cost of larger enclosures. 

## Instructions & Further Documentation
[See the wiki on GitHub!](https://github.com/rit-construct-makerspace/access-control-hardware/wiki) 

## Photos & Media
Coming Soon!

## License
This project is licensed under the Creative Commons 4.0 Attribution-NonCommercial-ShareAlike. For more information, click [here](https://creativecommons.org/licenses/by-nc-sa/4.0/).

If you are interested in using this project under a different license (e.g. for commercial use), please contact me. 

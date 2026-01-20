# V2.X Switch Backwards Compatibility

ACS version 2.X Switches are backwards compatible with ACS version 3.X Cores and similar. A DB9 to HD15 adapter for converting PS2 video to VGA video can be used to adapt the interface. The following pins of the DB9 are connected in these adapters;

* DB9 (Pin) - HD15 (Pin)
* 1 (Access) - 1 (Access)
* 2 (OneWire) - 2 (OneWire)
* 3 (Ground) - 3 (Ground)
* 4 (Power) - 13 (+5V)
* 5 (Ground) - Not Connected
* 6 (Type) - 6 (+5V)
* 7 (Interrupt) - 7 (Interrupt)
* 8 (Unused) - Not Connected
* 9 (Unused) - 10, 11 (Ground)

This is only suggested with short cable runs, as there is already a >500mV voltage drop from the switch's diode.
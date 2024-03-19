# RIT Formula SAE Dashboard Firmware
Welcome! This is the home of all sources for the Dashboard, used to display useful car information to the driver.

## Hardware F30
* [Teensy 4.0](https://www.pjrc.com/store/teensy40.html)
* [Adafruit RA8875 TFT Driver Board](https://www.adafruit.com/product/1590)
* [AZ Displays ATM0430D12M TFT LCD Panel](https://www.azdisplays.com/digital-tft-panel/ATM0430D12M/)
* [Microchip Technology MCP2561H CAN Transciever](https://www.digikey.com/en/products/detail/microchip-technology/MCP2561-H-MF/4079962)

## Hardware F32
* [Teensy 4.0](https://www.pjrc.com/store/teensy40.html)
* [Adafruit RA8875 TFT Driver Board](https://www.adafruit.com/product/1590)
* [FOCUS LCDs E50GE-I-RW1100-N 5" IPS LCD](https://focuslcds.com/product/e50ge-i-rw1100-n/)
* [Microchip Technology MCP2561H CAN Transciever](https://www.digikey.com/en/products/detail/microchip-technology/MCP2561-H-MF/4079962)

## Installation
After cloning, be sure to run `git submodule update --init` from the project directory to download source dependencies.

[PlatformIO](https://platformio.org/) is used for building the Dashboard code.

## VS Code
It is recommended that you start by installing [VS Code]code.visualstudio.com(https://code.visualstudio.com/)

[PlatformIO IDE](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) then needs to be installed (search for "PlatformIO IDE" under the extensions tab on the left side bar)

## Building Everything/Running all tests
Once PlatformIO IDE is installed, a checkmark should appear on the bottom bar of VS Code that can be used build the code.
The right pointing arrow next to the checkmark can be used to upload to a Teensy if it is connected.

## Command-line steps
```bash
git clone https://github.com/RITRacingSoftware/F28Dashboard
cd F28Dashboard/
git submodule update --init
pio run				# To compile
pio run -t upload	# To compile and flash to Teensy
```

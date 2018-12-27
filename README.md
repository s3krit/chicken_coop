Chicken coop
============

This is a platformio project for controlling the chicken door at the
[Kuckucksmuele](https://wiki.ecohackerfarm.org/kuckucksmuehle:start) ecohackerfarm
project.

## Installing prerequisites

To build / flash the image onto the adurino you will need platformio CLI tools. One way of getting them is to download and run the get-platformio script. See https://docs.platformio.org/en/latest/installation.html for details.

You also need to check out the LowPower library, which is a build dependancy for this project. From the project directory run
```
  git clone https://github.com/rocketscream/Low-Power lib/LowPower
```

## Compiling the code

Once installed, plug the Arduino Nano into a computer, and run the following command:
```
  pio device list
```

Your arduino should show up in the list. You can then run the following command to build the project and flash the arduino with the resultant image:

```
  pio run --target upload --environment nano  --upload-port /dev/ttyUSB0
```
Replace the parameter after ```--upload-port``` with the device name you got from ```pio device list```.

## Debugging the code

To view the serial console debug messages that get generated while the program executes, run the following command:
```
  pio device monitor /dev/ttyUSB0
```
Replace the last parameter with the device name you got from ```pio device list```.

## Needed electronics

This project uses the following electronic components:
 * Arduino nano
 * L293D motor controller
 * 5V power supply
 * 12V power supply
 * Reversible 12V electric motor
 * 2 x push switches
 * 1 x light sensor
 * 3 x 2k resistors

## Viewing the schematics

The schematics are in the **schematics.fzz** file. A tool called Fritzing is needed to view them.

## Non-electronic hardware configuration

The chicken coop door hardware can be summed up as a giliotine - The door slides freely up and down in a channel, and is raised and lowered by a string that is attached to the top centre of it. There is a winch winding the string onto a reel, and a set of cogs to reduce the effort needed by the motor. At the bottom and at the top of intended movement of the door there are mechanical push switches that get closed by the door moving into them.


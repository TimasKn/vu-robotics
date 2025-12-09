## Problem
Measure light level of an area with two sensors and display their values in LCD diplay. Give a user an option, to change which sensors (or both) value to display. Everything should work asynchronously.

## Usage
- Press a button, to change which light level value (Average, Left sensor, Right sensor) is displayed;
- Turn the potentiometer to change the contrast of LCD display;

## EEPROM
Everything is saved in a form of a struct:
- Magic number (int);
- Option number (int);
- Average value of sensors (int);
- Right sensor value (int);
- Left sensor value (int);

## Timer
- Set delay using variable TIMER_DELAY. Delay can be any value from 1ms to 250ms;
- The delay will be converted to ticks, which will be used in timer CompareA register (OCR1A).

## Future improvements
- Add a possibility to 'freeze' device (stop reading sensor values) either with another input component or implementing several button presses;
- Add some sort of indicator, that device is frozen; 

## Components

| Component | Quantity |
| --------- | :--------: |
| Arduino Uno R3 | 1 |
| LCD display | 1 |
| Push Button | 1 |
| Photoresistor | 2 |
| Potentiometer 10kR | 1 |
| Resistor 220R | 1 |
| Resistor 1kR | 1 |
| Resistor 10kR | 2 |

## Wiring
![Wiring](https://github.com/user-attachments/assets/e5b9695b-f466-4d6b-84ec-6c1cc566c7c6)

## Demo


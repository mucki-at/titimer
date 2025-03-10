# Twilight Imperium Timer

A simple timing device to coerce players to make a decision with a set amount of time. Features:

* Display remaining time/status using RGB LED strip
* Simple 3 button user interface
* Project contains code (based on ATTiny85), BoM for reference hardware, and STL files for printing a case.

## User interface

The device has three buttons ("Choose", "Extend" and "Pause") as well as a row of color LEDs to indicate the current mode, as well as the remaining time left.
At ANY time the "Choose" button always sets/resets a 1 minute timer. This mode is intended to be used for when a player is choosing a system/component/strategy card to activate.
While the timer is running, the "Extend" button adds a further two minutes to the timer. This mode is intended to be used after a player has chosen a system/component/strategy to activate to give time to resolve the action in a timely manner.
While the timer is running, the "Pause" button pauses and unpauses the timer.
After the timer has run out, the "Pause" button can be used to reset the device to idle mode instead of using the "Choose" button to start a new timer.

The Buttons light up and/or change color to indicate the current mode of the device:

### Idle Mode

* The mode when nothing else is happening.
* Only the "Choose" button is illuminated and it is the only button which does anything in this mode.
* The LED strip is dark.
* * The buzzer is off.

### Choose! Mode

* When the "Choose" button is pressed, a 1 minute timer is started.
* The LED strip counts down the remaining time and changes color accordingly. 
* The Buzzer will alert with short tones at regular intervals.
* The "Choose" button is lit up green to indicate that this mode is in effect.
* The "Extend" and "Pause" buttons are lit up blue to indicate that it is valid to press it.

### Extend! Mode

* When the "Extend" button is pressed before the timer reaches zero, two minutes are added to the timer. This can be done only once per timer.
* The LED strip counts down the remaining time and changes color accordingly. 
* The Buzzer will alert with short tones at regular intervals.
* The "Choose" and "Extend" button is lit up yellow to indicate that this mode is in effect.
* The "Pause" button is lit up blue to indicate that it is valid to press it.

### Pause Mode

* When the "Pause" button is pressed before the timer reaches zero, the countdown is paused.
* The LED strip pulsates blue to indicate that pause is in effect
* The Buzzer makes a periodic sound to remind that the device is still active.
* The "Choose" and "Extend" buttons are dark to indicate that it is not valid to press it.
* The "Pause" button is pulsing or lit blue to indicate that it is valid to press it.

### Alert Mode

* When the timer runs out the device enters alert mode.
* The LED strip will flash red to indicate that time has run out.
* The Buzzer makes an aggressive alert sound.
* The "Choose" and "Pause" buttons are red to indicate that it is valid to press it.
* The "Extend" button is dark to indicate that it is not valid to press it.
* If no user interaction occurs within 20 seconds, the device will automatically go idel

### State Machine

The [state machine](design/statemachine.md) document has a more pecise documentation of the devices functionality.


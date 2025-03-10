# titimer state machine

Inputs:
* A (Button, Label = "Choose!")
* B (Button, Label = "Extend!")
* X (Button, Label = "Pause")

Outputs:
* A (RGB LED lights up button A)
* B (RGB LED lights up button B)
* X (RGB LED lights up button X)
* S (RGB LED strip, a linear sequence of LEDS)
* L (Buzzer/Beeper)

Variables:
* Time
* MaxTime

## State: IDLE (Starting State)

Output:
* A = Blue
* B, X, S, L = OFF

Input:
* A: Start Timer
  1. Set MaxTime = 60 seconds
  2. Set Time = 60 seconds
  3. Goto state: COUNTING
* B,X: Do nothing

## State: COUNTING

Output:
* A = Green
* B = Blue if MaxTime==60, else Yellow
* X = Blue
* S = Time/MaxTime shown as shrinking lights and green/yellow/red coloring
* L = OFF (optional alert at certain levels of Time/MaxTime

Input:
* A: Reset Timer
  1. Set MaxTime = 60 seconds
  2. Set Time = 60 seconds
* B: Extend Timer
  1. IF MaxTime==60 THEN
     1. Set MaxTime=180
     2. Set Time=Time+120
  2. ELSE Do Nothing
* X: Goto state: PAUSE
* ELSE:
  1. IF Time>0 THEN Decrease Time at a rate of one unit per second
  2. ELSE goto state: ALERT

## State: PAUSE

Output:
* A, B, X, S = Pulse/bink blue
* L = short beeps every second

Input:
* A, B: Do nothing
* X: Goto state: COUNTING

## State: ALERT

Output:
* A, B, X, S = Flash red
* L = angry beeping

Input:
* A: Reset Timer
  1. Set MaxTime = 60 seconds
  2. Set Time = 60 seconds
  3. Goto state: COUNTING
* B: Do nothing
* X: Goto state: IDLE
* ELSE: Goto state IDLE if no input for 20 seconds

# STM32 Practicum
Platform: SDK-1.1M based on STM32F427  
Includes: 
- Port expander PCA9538Pw
- Real-time clock MCP79411
- OLED-display WEO012864DL
- Ethernet
- Sound generator HC0903A
- Keybord (12 buttons)

## [Task 1. GPIO](https://github.com/bombanya/pvs/tree/lab1-vlad)
Develop "Combination lock". The code is a combination of long and short pressings of side button.  
If the combination is correct, green led is turning on for several seconds. Then it turns off and the combination should be entered again.  
Yellow led blinks for each correct pressing. After each wrong pressing red led blinks and then the combination resets.  
After 3 mistakes red led is on and input is blocked for several seconds.

## [Task 2. UART](https://github.com/bombanya/pvs/tree/lab2)
Modification of "Combination lock" from the previous task. Use UART instead of side button. UART processing should be implemented in 2 modes: with and without interrupts. Mode can be changed at any time.
Code for lock is a combination of 8 latin letters.  The combination can be changed by sending '+' charachter to device.

## [Task 3. Timers](https://github.com/bombanya/pvs/tree/lab3)
Develop "Rythm Game". Use 9 different sounds mapped on leds (brightness + color) to play melody. User should press buttons transmitted by UART to guess sound played. 
If guessed correctly, points will be added to user's score.  
Device should respond to this charachters:

- '1'-'9' - if melody is playing, check if correct button is pressed. Play mapped sound otherwise.
- '+' - Change game speed (impulse duration).
- 'a' - Change display mode: sound + led, led, sound
- 'Enter' - Start/stop melody.

## [Task 4. I2C and keyboard](https://github.com/bombanya/pvs/tree/lab4)
Modifaction of "Rythm Game" from previous task. Use keyboard buttons instead of UART.

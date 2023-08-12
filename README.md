# Traffic-Light-Controller

## Goal
 - Use FreeRTOS on the STM32 NUCLEO series to create an embedded system.
 - Read inputs from external sources and interrupt processes.
 - Utilize OS functions such as Tasks, Semaphores, Queues, Interrupts, Scheduling, etc.

## Description

Create a Traffic Light Controller (TLC) using FreeRTOS on embedded system using both on-board and external LEDs. Program the board to simulate the control of traffic lights at an intersection as well as a pedestrian signal. <br>

 - Green signal stays on for 4 seconds.
 - Yellow signal stays on for 1.5 seconds.
 - When the Red light signal of the first set turns active, the Green light of the second set becomes active after a delay of 1 second.
 - A button is used to activate the pedestrian signal.
   - If the button is pushed, the pedestrian signal turns on when the green light is turned on.
   - The pedestrian signal starts blinking 2 seconds later.
   - The pedestrian signal turns off as soon as the green light turns off.
   - If the button is not pushed, the pedestrian signal stays off even if the light turns green.
   - If the button is pushed when the green light is already on, pedestrian signal stays off.

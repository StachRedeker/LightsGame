# LightsGame
> A modern version of a classic (1972) game: 1D pong. But, most importantly: my first PCB!

As a second year Electrical Engineering student, I wanted to do a project that would show of various skills that I learned during my studies. One of the skills is PCB design, for which I followed an extracurricular course. I also wanted to use the seven segment displays that I have never worked with before. Because at the moment, those were collecting dust in my drawers.

My first thought was to make a [stack calculator](https://orkhanhuseyn.medium.com/what-are-stack-based-calculators-cf2dbe249264). Those stack calculators are interesting devices that are no longer sold, and... also very lame. Then, perhaps subliminally influenced by the latest [Mark Rober video](https://www.youtube.com/watch?v=Rsxao9ptdmI), the idea of making my own arcade game came up.

My own arcade game. Sounds awesome! The plan is to 

## Electric circuit design

### Schematic

![Electrical schematic](Pictures/Schematic_LightsGames.png)


### Breadboard realisation

![Messy breadboard realisation](Pictures/messyBreadboard2.jpeg)

## PCB design

<img src="Pictures/PCB_PCB_LightsGames.png" width="49%"/> <img src="Pictures/PCB_PCB_LightsGames_3D.png" width="49%"/> 

## Working version!

## Plans for improvements?
Currently, I have no plans for releasing a version 2. I am not planning on making this a commercially available product, scince LightsGame was solely designed as a fun weekend project. That being said, this project is fully open source. Feel free to make your own adaptations! If you like hardware design, extend the game to two dimensions. If you like software design, program your own gamemodes. And if you like PCB design, make a version with less mistakes than mine. Or heck, do it all!

The following improvements can be made:
- [ ] Assigning individual resistors to the LEDs in order to increase robustness.
- [ ] Using thicker PCB traces for power lines.
- [ ] Adding the yellow LEDs to the PCB (instead of just using red LEDs).
- [ ] Replacing the Arduino Nano by a programmable Atmega 328p SMD chip.

## Licence
[MIT](/LICENSE)

## Acknowledgements
https://docs.arduino.cc/tutorials/communication/guide-to-shift-out
https://www.youtube.com/watch?v=liZMwInWuTM


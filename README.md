
<!-- README.md -->


# Spatial Audio Headphones on STM32

ece153b W21 Final Project

Ethan Sifferman

---

Repository: https://github.com/E4tHam/stm32-spatial_audio/

Website: https://ethan.sifferman.dev/stm32-spatial_audio/


## Description

Apple has released headphones that simulate surround sound. By turning your head, the audio seems to change the location it comes from. I will do my best to replicate this feature. My final product will be used as follows:

1. Sync the connected HC-05 module to a computer.
2. Plug in headphones to the Discovery’s headphone jack.
3. Run a custom C++ script on the computer to send the wave file to the HC-05.
4. Place the Discovery Board with its accelerometer on your head.
5. Rotate your head to experience the 3D sound.


## Hardware Used

* STM32L476VGTx Discovery Board


## Libraries Used

* [manashmandal/Serial Port](https://github.com/manashmandal/SerialPort)
* [E4tHam/Wave-Parser](https://github.com/E4tHam/Wave-Parser)

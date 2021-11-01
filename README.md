
<!-- README.md -->

# Spatial Audio Headphones on STM32

ece153b W21 Final Project

Ethan Sifferman

---

Repository: <https://github.com/E4tHam/stm32-spatial_audio/>

Website: <https://ethan.sifferman.dev/stm32-spatial_audio/>

## Description

Apple has released headphones that simulate surround sound. By turning your head, the audio seems to change the location it comes from. I did my best to replicate this feature on an [STM32L476G Discovery Board](https://www.st.com/en/evaluation-tools/32l476gdiscovery.html). My final product can be used as follows:

1. Connect the Discovery Board to a computer.
2. Plug in headphones to the Discovery’s headphone jack.
3. Run a custom C++ script on the computer to send the wave file to the STM32.
4. Place the Discovery Board on your head.
5. Rotate your head to experience the 3D sound.

## Video

Video Explanation:

*Note that this project is unfinished and the results in the video are fabricated to show what the desired behavior is.*

[![stm32 spatial audio](https://img.youtube.com/vi/obdn38XznAw/0.jpg)](https://www.youtube.com/watch?v=obdn38XznAw)

## Hardware Used

* [STM32L476G Discovery Board](https://www.st.com/en/evaluation-tools/32l476gdiscovery.html)

## Libraries Used/Referenced

* [manashmandal/Serial Port](https://github.com/manashmandal/SerialPort)
* [E4tHam/Wave-Parser](https://github.com/E4tHam/Wave-Parser)
* [deater/vmw-meter](https://github.com/deater/vmw-meter/tree/master/stm32L476/chiptune_cs43l22)

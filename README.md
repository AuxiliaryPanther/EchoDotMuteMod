# EchoDotMuteMod
  
* INTRODUCTION

This program allows a Seeeduino Xiao (tiny Arduino-style board with a SAM D21 core) to add a hardware mic mute feature to the Amazon Echo Dot 2nd gen smart speaker. It is not controlled by the Dot's programming and causes the Dot to boot up in the mic muted state. This is separate and independent from the built-in software mic mute button. This is in response to security vulnerabilities popularized by Dolphin Attack research, which has expanded to speech-modulated lasers, too.

This program was created by Andrew Peters on 1/3/21 in the pursuit of learning how to control that which controls us. Freely use and build on this. Please let me know of any improvements or ideas you may have on the github discussion here or at andrewpeterst@gmail.com. I recently finished a video detailed this project as well: https://youtu.be/OSyf_FsDwfk
    
* SAFETY DISCLAIMER
  
This program was written for the Seeeduino Xiao microcontroller and the Amazon Echo Dot 2nd generation. I am not responsible for any damage or harm you may incur on yourself or your equipment, so do not try anything you do not understand. I am only  responsible for the permanent damage I did to my Dot's red DS0 LED while mindlessly prodding around with an ammeter. I failed to capture the magic smoke released from that.
   
* DESCRIPTION
  
The Echo Dot 2nd gen has seven microphones that share a common data line (using I2S communication standards). I endeavored to ground the mics, but with no exposed test points for the mics, the simplest way to disable them is to disable the shared data line. This is accomplished by grounding it using a DIO pin on the Xiao written to LOW. This is triggered by pressing the Dot's volume up and down buttons simultaneously, and 0.5 seconds later it will indicate the state as discussed further. I wanted to use the Dot's mic mute status red LEDs control (apply 3.3V to TM26) or temporarily disabling the audio chips to create a static (ground out TM11), but the SAM D21 chip does not have the power capability needed to support either of those options successfully, even with the modification in the next paragraph. Instead, the volume up and down buttons are triggered (TM34, TM33, resp.) which will also make the Dot play a tone. Four tones means the hardware mute is enabled, two tones means the mics are restored. The software mic mute feature of the Dot is not used in this modification.
   
* SAM D21 NOTE
  
The SAM D21 chip used in the Xiao has the following default current limitations: OUTPUT LOW 2.5mA, OUTPUT HIGH 2mA. This can be increased by modifying the output driver strength register bit from 0 to 1, increasing the limitation to OUTPUT LOW 10mA, OUTPUT HIGH 7mA, which is ideal for this project. A discussion about the hows and whys can be found at: https://github.com/arduino/ArduinoCore-samd/issues/158
   
* TEST POINTS ACKNOWLEDGEMENTS
  
Thanks to Sascha Schmidt for determining what some of the important test points do on the Echo Dot 2nd gen, found at: https://darkspirit510.de/2019/10/alexa-amazon-free.

If you want to start a similar project on the Echo Dot 3rd gen, some great test point documentation has been published on Brian Dorey's blog: https://www.briandorey.com/post/echo-dot-3rd-gen-digging-deeper.
    
glhf, and enjoy this gif I made that shows both sides of the LED/mic PCB:

![Alt Text](https://github.com/AuxiliaryPanther/EchoDotMuteMod/blob/main/EchoDot2MicBoard_BothSides_GIF.gif)

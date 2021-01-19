/************************** INTRODUCTION **************************
   This program was created by Andrew Peters on 1/3/21 in the 
    pursuit of learning how to control that which controls us. 
    Freely use and build on this. Please let me know of any 
    improvements or ideas you may have at:
     andrewpeterst@gmail.com
    
  *********************** SAFETY DISCLAIMER ***********************
  This program was written for the Seeeduino Xiao microcontroller 
   and the Amazon Echo Dot 2nd generation. I am not responsible for 
   any damage or harm you may incur on yourself or your equipment, 
   so do not try anything you do not understand. I am only 
   responsible for the permanent damage I did to my Dot's red DS0 
   LED while mindlessly prodding around with an ammeter. I failed 
   to capture the magic smoke released from that.
   
  ************************** DESCRIPTION **************************
  The Echo Dot 2nd gen has seven microphones that share a common 
   data line (using I2S communication standards). I endeavored to 
   ground the mics, but with no exposed test points for the mics,
   the simplest way to disable them is to disable the shared data 
   line. This is accomplished by grounding it using a DIO pin on 
   the Xiao written to LOW. This is triggered by pressing the Dot's
   volume up and down buttons simultaneously, and 0.5 seconds later
   it will indicate the state as discussed further.
  I wanted to use the Dot's mic mute status red LEDs control (apply 
   3.3V to TM26) or temporarily disabling the audio chips to create 
   a static (ground out TM11), but the SAM D21 chip does not have 
   the power capability needed to support either of those options 
   successfully, even with the modification in the next paragraph. 
   Instead, the volume up and down buttons are triggered (TM34, 
   TM33, resp.) which will also make the Dot play a tone. Four 
   tones means the hardware mute is enabled, two tones means the 
   mics are restored. The software mic mute feature of the Dot is 
   not used in this modification.
   
  ************************* SAM D21 NOTE **************************
  The SAM D21 chip used in the Xiao has the following default 
   current limitations: OUTPUT LOW 2.5mA, OUTPUT HIGH 2mA. This 
   can be increased by modifying the output driver strength 
   register bit from 0 to 1, increasing the limitation to OUTPUT 
   LOW 10mA, OUTPUT HIGH 7mA, which is ideal for this project. A 
   discussion about the hows and whys can be found at:
    https://github.com/arduino/ArduinoCore-samd/issues/158
   
  ***************** TEST POINTS ACKNOWLEDGEMENTS ******************
  Thanks to Sascha Schmidt for determining what some of the
   important test points do on the Echo Dot 2nd gen, found at:
    https://darkspirit510.de/2019/10/alexa-amazon-free/
   
  If you want to start a similar project on the Echo Dot 3rd gen, 
   some great test point documentation has been published on Brian 
   Dorey's blog:
    https://www.briandorey.com/post/echo-dot-3rd-gen-digging-deeper
    
  glhf */

#define Mute A10 // Dot TM10 (DOUT, grounding this hardware mutes the mic data stream)
#define Minus A9 // Dot TM33 (Grounded to trigger Volume Down button press)
#define Plus A7 // Dot TM34 (Grounded to trigger Volume Up button press)
                // Xiao VCC connects to Dot USB cable input test points
                // Xiao GND connects to Dot TM14 (but on the power board)
                
bool Muted = true; // hardware mute status

void setup() {
  // Initialize IO pins as inputs to place them in a high impedance state
  pinMode(Mute, INPUT); 
  pinMode(Minus, INPUT); // calling analogRead() auto changes pin to an analog input
  pinMode(Plus, INPUT);
  // It takes the Dot 20 seconds to enable buttons, 36 seconds for full functionality
  delay(30000); // give the Dot 30 seconds to initialize before muting
  MuteStream();
}

void loop() {
  // Change mute state if both Plus & Minus buttons are pressed
  if (analogRead(Minus)<300 && analogRead(Plus)<300){
    delay(30); // debounce both buttons at 30 ms
    if (analogRead(Minus)<300 && analogRead(Plus)<300){
      Muted = !Muted;    
      if (Muted == true) {MuteStream();}
      else{UnmuteStream();}
    }
  }
  delay(50);
}

void MuteStream(){
  // Mute the data stream by grounding it
  pinMode(Mute, OUTPUT);
  // Increase output driver strength of the Mute pin to support the 4mA current.
  PORT->Group[g_APinDescription[Mute].ulPort].PINCFG[g_APinDescription[Mute].ulPin].bit.DRVSTR = 1;
  // Indicate muting by cycling volume buttons twice
  delay(550);
  MakeTheBoop(Plus);
  MakeTheBoop(Minus); 
  MakeTheBoop(Plus);
  MakeTheBoop(Minus);  
}

void UnmuteStream(){
  // Unmute the data stream
  pinMode(Mute, INPUT);
  // Indicate muting by cycling volume buttons once
  delay(550);
  MakeTheBoop(Plus); 
  MakeTheBoop(Minus);
}

void MakeTheBoop(char Direc){
  // Presses the Plus or Minus button
  pinMode(Direc, OUTPUT); // doesn't seem to require any current
  delay(50);
  pinMode(Direc, INPUT);
  delay(500);
}

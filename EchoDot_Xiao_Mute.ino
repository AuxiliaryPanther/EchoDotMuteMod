/* This program was created by Aux Panther on 1/3/21. Visit 
   https://github.com/AuxiliaryPanther/EchoDotMuteMod for the most up to date code
   and information. glhf */

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

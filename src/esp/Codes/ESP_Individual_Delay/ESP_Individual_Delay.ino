#include <Arduino.h>
#include <avdweb_VirtualDelay.h>
#include <Streaming.h>

bool b;
int i;
VirtualDelay singleDelay; // default = millis

// Multiple delays
VirtualDelay delay1, delay2;

void setup(){ 
  Serial.begin(9600);
  Serial << "\ntestSequence";
}

void loop(){ 
//  singleDelay.start(10000);     // calls while running are ignored
//  if(singleDelay.elapsed()){
//    Serial.println("test");
//  }
//  Serial << endl << i++;        // since the cpu is not being blocked, printing goes fast

  // this sequence has a deadlock
  if(delay1.elapsed()){
    Serial.println("Stroom");
    Serial.println(millis());
    delay2.start(2000);
  }
  if(delay2.elapsed()){
    Serial.println("Gas");
    Serial.println(millis());
    delay1.start(2000);
  }
  // Breaks the deadlock, you can start with any delay object you want e.g. delay2.start(0);
  DO_ONCE(delay1.start(0));
}

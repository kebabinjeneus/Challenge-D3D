#define buttonGreen 12    // D6 on esp
#define buttonRed 13      // D7 on esp

int buttonStateGreen = 0;
int buttonStateRed = 0;

void setup(){
  Serial.begin(115200);
  pinMode(buttonGreen, INPUT);
  pinMode(buttonRed, INPUT);
}

void loop(){
  // Read the state of the pushbutton value
  buttonStateGreen = digitalRead(buttonGreen);
  buttonStateRed = digitalRead(buttonRed);

  // Check if the buttons are pressed, if it is, the buttonState is HIGH
  if(buttonStateGreen == HIGH){
    Serial.println("Green button pressed");
    delay(200);
  }
  if(buttonStateRed == HIGH){
    Serial.println("Red button pressed");
    delay(200);
  }
}

#define outputA 4
#define outputB 5


int counter = 0;
int rotations = 0;
int aState;
int aLastState;


void setup() {
  pinMode (outputA, INPUT);
  pinMode (outputB, INPUT);

  Serial.begin (9600);


  aLastState = digitalRead(outputA);
}


void loop() {
  aState = digitalRead(outputA);
  if (aState != aLastState) {
    if (digitalRead(outputB) != aState) {
      counter ++;
    } else {
      counter --;
    }
   /*
    if (counter > 0 && counter%30 == 0) {
    Serial.print("Position: ");
    Serial.println(counter/30);
    } else if (counter % 30 == 0) {
    Serial.print("Position: ");
    Serial.println(counter/-30);
    }
    */

    if (counter % 30 == 0) {
      rotations ++;
    Serial.print("Position: ");
    Serial.println(rotations);
    }
  }
  aLastState = aState;
}

# include <Stepper.h>
int STEPS_PER_REV = 200;
# define SPEED 20
Stepper motor(STEPS_PER_REV, 8,9,10,11);

int steps2rotate;



void setup() {
 Serial.begin(115200);
 motor.setSpeed(SPEED);
}

bool rotate(int steps){
  motor.step(steps);
}

void loop() {
   while (!Serial.available());
   
   steps2rotate = Serial.readString().toInt();
   rotate(steps2rotate);
   Serial.print(1);

}

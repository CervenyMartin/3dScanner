# include <Stepper.h>
int STEPS_PER_REV = 200;
# define SPEED 20
Stepper motor(STEPS_PER_REV, 8,9,10,11);

int angle2rotate;
int currentSteps = 0;


void setup() {
 Serial.begin(115200);
 motor.setSpeed(SPEED);
}

bool rotate(int angle){
  int target = int(angle/1.8);
  int steps2do = target-currentSteps;
  if (abs(steps2do)<STEPS_PER_REV/2){
    motor.step(steps2do);
  }
  else if(abs(steps2do+STEPS_PER_REV)< abs(steps2do-STEPS_PER_REV))
    motor.step(steps2do+STEPS_PER_REV);
  else
    motor.step(steps2do-STEPS_PER_REV);

  currentSteps = target;

}

void loop() {
   while (!Serial.available());
   
   angle2rotate = Serial.readString().toInt();
   rotate(angle2rotate);
   Serial.print(1);

}

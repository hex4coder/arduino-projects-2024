#include <Servo.h>


Servo srv;


int min = 0;
int max = 128;
int servoDelay = 3;

void setup() {
  srv.attach(11);
}

void loop() {
  for(int i = min; i < max; i++) {
    srv.write(i);
    delay(servoDelay);
  }

  for(int j = max; j > min; j--) {
    srv.write(j);
    delay(servoDelay);
  }

}

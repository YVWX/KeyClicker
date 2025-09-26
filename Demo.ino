/* References
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
 https://wiki.dfrobot.com.cn/_SKU_SEN0293_SEN0294_SEN0295_SEN0296_SEN0297_SEN0298_SEN0299__%E8%96%84%E8%86%9C%E5%8E%8B%E5%8A%9B%E4%BC%A0%E6%84%9F%E5%99%A8
*/

#include <Servo.h>

const int servo_num = 4;
Servo myservo[servo_num];
// twelve Servo objects can be created on most boards

#define sensorPin(x) (A0 + x)

int cycle_time = 720;

const int num_samples = 3;

int x = 0;
int threshold = num_samples;
float last_avg = 0, current_avg = 0;
int waiting_time = 1;

void setup() {
  Serial.begin(9600);
  myservo[0].attach(9);
  myservo[1].attach(10);
  myservo[2].attach(11);
  myservo[3].attach(12);
}

unsigned long press_delay_time = 0;

int op_code = 0;

void loop() {
  if (Serial.available())
  {
    int op_code = Serial.read();
    if (op_code == 'i')
    {
      Serial.println("cycle_time: " + String(cycle_time));
      Serial.println("threshold: " + String(threshold));
    }
    else if (op_code == 's')
    {
      int parameter = Serial.read();
      int value = Serial.parseInt();
      if (parameter == 'c')
      {
        cycle_time = value;
      }
      else if (parameter == 't')
      {
        threshold = value;
      }
    }
    else if (op_code == 'p')
    {
      int servo_index = Serial.parseInt();
      servo_index %= servo_num;

      myservo[servo_index].write(180);

      do
      {
        delay(waiting_time);
        current_avg = 0;
        for (int count = 0; count < num_samples; count++)
        {
          x = analogRead(sensorPin(servo_index));
          current_avg += x;
        }
      }
      while (current_avg <= threshold);

      myservo[servo_index].write(90);

      last_avg = 0;
      current_avg = 0;

      unsigned long press_start_time = millis();

      myservo[servo_index].write(180);

      do
      {
        last_avg = current_avg;
        delay(waiting_time);
        current_avg = 0;
        for (int count = 0; count < num_samples; count++)
        {
          x = analogRead(sensorPin(servo_index));
          current_avg += x;
        }
      }
      while (current_avg > last_avg);

      myservo[servo_index].write(90);

      unsigned long press_end_time = millis();

      press_delay_time = press_end_time - press_start_time;
      
      last_avg = 1024 * num_samples;
      current_avg = 1024 * num_samples;

      press_start_time = millis();

      myservo[servo_index].write(0);

      int press_release_time = max(2 * press_delay_time, cycle_time / 8);
      delay (press_release_time);

      myservo[servo_index].write(90);
    }
  }
  
  delay(10);
}

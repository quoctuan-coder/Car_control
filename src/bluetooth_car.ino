#include <Arduino.h>
#include <SoftwareSerial.h>

// Neu khong test thi chuyen thanh false
#define TESTING false

#define H_Type  HI216 // L298 

// Motor Car A
#define IN1_GPIO  7
//#define IN2_GPIO  8
#define ENA_SPEED 5

// Motor Car B
#define IN3_GPIO  9
//#define IN4_GPIO  10
#define ENB_SPEED 6

// Motor Cutting 
#define IN5_GPIO  12
//#define IN6_GPIO  13
#define ENC_SPEED 11

// Define speed 
#define MAX_SPEED_PWM      250
#define MIN_SPEED_PWM      0

SoftwareSerial my_serial(3, 2); // RX, TX

int save_speed = MAX_SPEED_PWM;
int speed_car = 0;
int speed_cutting = 0;


// Function to test
static void _test_hw_motor_A()
{
  digitalWrite(IN1_GPIO, HIGH);
 // digitalWrite(IN2_GPIO, LOW);
  for (int speed = 0; speed < 255; speed = speed + 10)
  {
    analogWrite(ENA_SPEED, speed);
    delay(1000);
  }
}

static void _test_hw_motor_B()
{
  digitalWrite(IN3_GPIO, HIGH);
  //digitalWrite(IN4_GPIO, LOW);
  for (int speed = 0; speed < 255; speed = speed + 10)
  {
    analogWrite(ENB_SPEED, speed);
    delay(1000);
  }
}

static void _test_hw_motor_Cutting()
{
  digitalWrite(IN5_GPIO, HIGH);
  //digitalWrite(IN6_GPIO, LOW);
  for (int speed = 0; speed < 255; speed = speed + 10)
  {
    analogWrite(ENC_SPEED, speed);
    delay(1000);
  }
}

// Main function
static void _control_stop_car()
{
   analogWrite(ENA_SPEED, 0);
   analogWrite(ENB_SPEED, 0);
  //  analogWrite(ENC_SPEED, 0);  Skip status motor
  
   //digitalWrite(IN1_GPIO, LOW);
   //digitalWrite(IN2_GPIO, LOW);
   //digitalWrite(IN3_GPIO, LOW);
   //digitalWrite(IN4_GPIO, LOW);
   //digitalWrite(IN5_GPIO, LOW);
   //digitalWrite(IN6_GPIO, LOW);
} 
void _control_cutting_set_speed(int speed_pwm)
{
  analogWrite(ENC_SPEED, speed_pwm);
}

void _control_cutting_on()
{
    // digitalWrite(IN4_GPIO, HIGH);
    digitalWrite(IN5_GPIO, LOW);
    _control_cutting_set_speed(speed_cutting);
}

void _control_cutting_off()
{
    //digitalWrite(IN4_GPIO, LOW);
    //digitalWrite(IN5_GPIO, LOW);
    _control_cutting_set_speed(0);
}

static void _control_car_set_speed(int speed_pwm)
{
   analogWrite(ENA_SPEED, speed_pwm);
   analogWrite(ENB_SPEED, speed_pwm);
}

static void _control_forward_car()
{ 
  digitalWrite(IN1_GPIO, HIGH);
  //digitalWrite(IN2_GPIO, LOW);

  digitalWrite(IN3_GPIO, HIGH);
  //digitalWrite(IN4_GPIO, LOW);

  _control_car_set_speed(speed_car);
}

static void _control_backward_car()
{
  digitalWrite(IN1_GPIO, LOW);
  //digitalWrite(IN2_GPIO, HIGH);

  digitalWrite(IN3_GPIO, LOW);
  //digitalWrite(IN4_GPIO, HIGH);
  
  _control_car_set_speed(speed_car);
}

static void _control_turn_right_car()
{
  digitalWrite(IN1_GPIO, HIGH);
  //digitalWrite(IN2_GPIO, LOW);

  digitalWrite(IN3_GPIO, LOW);
  //digitalWrite(IN4_GPIO, HIGH);

  _control_car_set_speed(speed_car);
}

static void _control_turn_left_car()
{
  digitalWrite(IN1_GPIO, LOW);
  //digitalWrite(IN2_GPIO, HIGH);

  digitalWrite(IN3_GPIO, HIGH);
  //digitalWrite(IN4_GPIO, LOW);

  _control_car_set_speed(speed_car);
}

static void _control_car_init()
{
    pinMode(IN1_GPIO, OUTPUT);
    //pinMode(IN2_GPIO, OUTPUT);
    pinMode(IN3_GPIO, OUTPUT);
    //pinMode(IN4_GPIO, OUTPUT);
    pinMode(IN5_GPIO, OUTPUT);
    //pinMode(IN6_GPIO, OUTPUT);

    pinMode(ENA_SPEED, OUTPUT);
    pinMode(ENB_SPEED, OUTPUT);
    pinMode(ENC_SPEED, OUTPUT);

    _control_stop_car();
}

static void _control_car_get_data_ble()
{
  if (my_serial.available() > 0) {
    char data = my_serial.read();
      switch (data) {
        case 'S':
          _control_stop_car();
          break;
        case 'F':
           _control_forward_car();
          Serial.println("Run forward");
          break;
        case 'B':
          _control_backward_car();
          Serial.println("Run backward");
          break;
        case 'L':
          _control_turn_left_car();
          Serial.println("Run left");
          break;
        case 'R':
          _control_turn_right_car();
          Serial.println("Run right");
          break;
        case 'X':
          _control_cutting_on();
          Serial.println("Cutting on");
          break;
        case 'x':
          _control_cutting_off();
          Serial.println("Cutting off");
          break;
        case 'V':
          speed_car = save_speed;
          Serial.println("Speed for car");
          break;
        case 'v':
          speed_cutting = save_speed;
          Serial.println("Speed for cutting");
          break;
        case '0':
          save_speed = MIN_SPEED_PWM; 
          break;
        case '1':
          save_speed = 25;
          break;
        case '2':
          save_speed = 50;
          break;
        case '3':
          save_speed = 75;
          break;
        case '4':
          save_speed = 100; 
          break;
        case '5':
          save_speed = 125; 
          break;
        case '6':
          save_speed = 150; 
          break;
        case '7':
          save_speed = 175; 
          break;
        case '8':
          save_speed = 200;
          break;
        case '9':
          save_speed = 225;
          break;
        case 'q':
          save_speed = MAX_SPEED_PWM;
          break;
      }      
  }
}

void setup() 
{
    Serial.begin(115200);
    my_serial.begin(9600);
    
    _control_car_init();
}

void loop() 
{
    #if TESTING
    // Call function to testing hardware
    _test_hw_motor_A();
    _test_hw_motor_B();
    _test_hw_motor_Cutting();
    #else
    _control_car_get_data_ble();
    #endif
}

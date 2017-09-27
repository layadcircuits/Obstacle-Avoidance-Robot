/***************************************************************************
 Example Code of an obstacle avoidance robot using the Saleng Mobile Robot Shield,
 an ultrasonic sensor and a 2-wheel (or 4-wheel) robot chassis.
 
 This software is free provided that this notice is not removed and proper attribution 
 is accorded to Layad Circuits and its Author(s).
 Layad Circuits invests resources in producing free software. By purchasing Layad Circuits'
 products or utilizing its services, you support the continuing development of free software
 for all.

 The Saleng Mobile Robot Shield is designed and made by Layad Circuits.
 By supporting our products, you support local innovation and share in the movement
 to spur local ingenuity. 
  
 Author(s): C.D.Malecdan for Layad Circuits Electronics Engineering
 Revision: 1.0 - 2017/09/27 - initial release
 Layad Circuits Electronics Engineering Supplies and Services
 B314 Lopez Bldg., Session Rd. cor. Assumption Rd., Baguio City, Philippines
 www.layadcircuits.com
 general: info@layadcircuits.com
 sales: sales@layadcircuits.com
 +63-916-442-8565
 ***************************************************************************/

// speed setting based on movement
// 255=full speed
#define SPEED_ON_TURNS 40
#define SPEED_FORWARD 64
#define SPEED_BACKWARD 40

#define AIN1 2 
#define BIN1 7
#define AIN2 4
#define BIN2 5
#define PWMA 3
#define PWMB 6
#define STBY 8
#define PIN_TRIG 12
#define PIN_ECHO A3

const unsigned int MAX_DISTANCE_CM = 100;
const unsigned int THRESHOLD_DISTANCE_CM = 25;
float duration;
float distance;

void speedSetting(byte val)
{
  analogWrite(PWMA,val);
  analogWrite(PWMB,val);
}

void forward()
{
  speedSetting(SPEED_FORWARD);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,HIGH);
}

void backward()
{
  speedSetting(SPEED_BACKWARD);
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,LOW);
}

void turnleft()
{
  speedSetting(SPEED_ON_TURNS);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,LOW);
}

void turnright()
{
  speedSetting(SPEED_ON_TURNS);
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,HIGH);
}

void motorstop()
{
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,LOW);
}

void shortbreak()
{
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,HIGH);
}

void usRoutine()
{
  static unsigned long t;
  if(millis() - t < 30) return;
  t=millis();
   digitalWrite(PIN_TRIG, LOW);
   delayMicroseconds(2);
   digitalWrite(PIN_TRIG, HIGH);
   delayMicroseconds(10);
   digitalWrite(PIN_TRIG, LOW);
   duration = pulseIn(PIN_ECHO, HIGH, 30000);
   distance = duration * 0.034/2;
   if((distance >= MAX_DISTANCE_CM) || (distance < 2))
    distance = MAX_DISTANCE_CM;
   Serial.print("Distance: ");
   Serial.println(distance);  
} 


void mainlogic(float d)
{
  static unsigned long t;
  if(millis() - t < 100) return;
  t=millis();

  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
    
  if(d <=THRESHOLD_DISTANCE_CM)
  {
    byte dir;
    shortbreak();
    delay(10);
    motorstop();
    dir = random(0,2);
    switch(dir)
    {
      case 0:
        backward();
      break;

      case 1:
        turnleft();
      break;

      case 2:
        turnright();
      break;
    }
  }
  else
  {
    forward();  
  }
}

void setup() {
  pinMode(PIN_TRIG,OUTPUT);
  pinMode(PIN_ECHO,INPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  pinMode(STBY,OUTPUT);
  digitalWrite(STBY,HIGH);//enable driver

  speedSetting(64); 
  
  delay(3000); // add 3s delay before rumbling
  Serial.begin(115200);
}

void loop() {
  usRoutine();
  mainlogic(distance);
} 

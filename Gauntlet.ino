#include "PointSystem.h"

PointSystem points(20.0, 20.0, 0.2);
byte center, up, forward;

int tick = 100;

void setup(){
  Serial.begin(9600);
  center = points.addLocation(PointSystem::IGNORE, PointSystem::IGNORE, 0);
  up = points.addLocation(PointSystem::IGNORE, 90, 1);
  forward = points.addLocation(0, 0, 1);
  pinMode(13, OUTPUT);
}
void loop(){
  byte keyframe = points.getKeyframe(980, 85, 0.9);//Currenly hardcoding to test points
  if(keyframe == 0) Serial.println("No keyframe");
  else if(keyframe == center) Serial.println("Center");
  else if(keyframe == up) Serial.println("Up");
  else if(keyframe == forward) Serial.println("Forward");
  else Serial.println(keyframe);

  if(tick == 50){
    digitalWrite(13, LOW);
  }else if(tick >= 100){
    tick = 0;
    digitalWrite(13, HIGH);
  }
  tick++;
  
  delay(10);
}

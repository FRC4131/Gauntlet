#include "MPU9250.h"
#include "quaternionFilters.h"
#include "Adafruit_9DOF.h"
#include "Adafruit_Sensor.h"
#include "PointSystem.h"

MPU9250 imu;
Adafruit_9DOF math;
PointSystem points(20.0, 20.0, 0.2);
byte center, up, forward, down;

int tick = 0;

void setup(){
  Serial.begin(9600);
  Wire.begin();
  initIMU();
  calibrateIMU();

  Serial.println("Set the yaw for forward.");
  for(int i=0; i<1500/5; i++){
    updateIMU();
    calculateSparkfun();
    delay(5);
  }
  double foreYaw = imu.yaw;
  Serial.print("Yaw recoreded as ");
  Serial.println(foreYaw, 2);
  
  center = points.addLocation(PointSystem::IGNORE, PointSystem::IGNORE, 0);
  up = points.addLocation(PointSystem::IGNORE, -90, 1);
  forward = points.addLocation(foreYaw, 0, 1);
  down = points.addLocation(PointSystem::IGNORE, 90, 1);
}
void loop(){
  updateIMU();
  if(++tick > 200){
    tick = 0;
    calculateSparkfun();
    Serial.print("yaw: "); Serial.println(imu.yaw, 2);
    Serial.print("pitch: "); Serial.println(imu.pitch, 2);
    Serial.print("roll: "); Serial.println(imu.roll, 2);
    Serial.print((float)imu.sumCount / imu.sum, 2); Serial.println("Hz");

    byte keyframe = points.getKeyframe(imu.yaw, imu.pitch, 1);//TODO calculate radius from accelerometer (double integrals, yay!)
    if(keyframe == 0) ;//Quick exit for the most common case
    else if(keyframe == center) Serial.println("Center");
    else if(keyframe == up) Serial.println("Up");
    else if(keyframe == forward) Serial.println("Forward");
    else if(keyframe == down) Serial.println("Down");
    
    imu.count = millis();
    imu.sumCount = 0;
    imu.sum = 0;
  }
}
void calibrateIMU(){
  Serial.println("Calibrating. Wave a figure-8!");
  delay(1000);
  int imu_min[] = {32767, 32767, 32767}, imu_max[3] = {-32768, -32768, -32768}, data[3], scale[3], samples = 128;
  for(int s=0; s<samples; s++){
    updateIMU();
    data[0] = imu.mx; data[1] = imu.my; data[2] = imu.mz;
    for(int c=0; c<3; ++c){
      if(data[c] < imu_min[c]) imu_min[c] = data[c];
      if(data[c] > imu_max[c]) imu_max[c] = data[c];
    }
    delay(135);//Data appears every 125ms
  }
  float avgScale = 0;
  for(int i=0; i<3; i++){
    imu.magbias[i] = ((imu_max[i] + imu_min[i])) / 2 * imu.mRes * imu.magCalibration[i];
    scale[i] = (imu_max[i] - imu_min[i]) / 2;
    avgScale += scale[i];
  }
  avgScale /= 3.0;
  for(int i=0; i<3; ++i){
    imu.magCalibration[i] *= avgScale / scale[i];
  }
  Serial.println("Calibration complete.");
}


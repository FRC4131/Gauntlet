void initIMU(){
  byte test = 0;
  
  byte imuID = imu.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  Serial.print("IMU WhoAmI: "); Serial.print(imuID, HEX); Serial.print(" v "); Serial.println(0x71, HEX);

  if(imuID == 0x71){//Valid address, everything's going smoothly
    Serial.println("Begin self-test");
    Serial.print("Acc  X: within "); Serial.print(imu.SelfTest[0], 1); Serial.println("%");
    Serial.print("Acc  Y: within "); Serial.print(imu.SelfTest[1], 1); Serial.println("%");
    Serial.print("Acc  Z: within "); Serial.print(imu.SelfTest[2], 1); Serial.println("%");
    Serial.print("Gyro X: within "); Serial.print(imu.SelfTest[3], 1); Serial.println("%");
    Serial.print("Gyro Y: within "); Serial.print(imu.SelfTest[4], 1); Serial.println("%");
    Serial.print("Gyro Z: within "); Serial.print(imu.SelfTest[5], 1); Serial.println("%");

    imu.calibrateMPU9250(imu.gyroBias, imu.accelBias);
    imu.initMPU9250();
    ++test;
  }
  byte magID = imu.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
  Serial.print("Mag WhoAmI: "); Serial.print(magID, HEX); Serial.print(" v "); Serial.println(0x48, HEX);
  if(magID == 0x48){
    imu.initAK8963(imu.magCalibration);
    ++test;
  }

  if(test < 2){
    Serial.print("Error connecting: imu="); Serial.print(imuID, HEX);
    Serial.print(", mag="); Serial.println(magID, HEX);
    while(1);
  }
}
void updateIMU(){
  if(imu.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01){//Data ready
    imu.readAccelData(imu.accelCount);
    imu.getAres();//Accelerometer resolution (please don't summon the Greek god of war in an IMU)
    //Convert to g's
    imu.ax = (float)imu.accelCount[0] * imu.aRes;
    imu.ay = (float)imu.accelCount[1] * imu.aRes;
    imu.az = (float)imu.accelCount[2] * imu.aRes;

    imu.readGyroData(imu.gyroCount);
    imu.getGres();//Gyro resolution
    imu.gx = (float)imu.gyroCount[0] * imu.gRes;
    imu.gy = (float)imu.gyroCount[1] * imu.gRes;
    imu.gz = (float)imu.gyroCount[2] * imu.gRes;

    imu.readMagData(imu.magCount);
    imu.getMres();//Magnetometer resolution

    //TODO calibrate in
//    imu.magbias[0] = +470.;
//    imu.magbias[1] = +120.;
//    imu.magbias[2] = +125.;
    
    imu.mx = (float)imu.magCount[0] * imu.mRes * imu.magCalibration[0] - imu.magbias[0];
    imu.my = (float)imu.magCount[1] * imu.mRes * imu.magCalibration[1] - imu.magbias[1];
    imu.mz = (float)imu.magCount[2] * imu.mRes * imu.magCalibration[2] - imu.magbias[2];
  }
  imu.updateTime();
  //TL;DR every sensor on the board has axes oriented every which way. If things seem out of order, that's why.
  MahonyQuaternionUpdate(imu.ax, imu.ay, imu.az, imu.gx * DEG_TO_RAD, imu.gy * DEG_TO_RAD, imu.gz * DEG_TO_RAD, imu.my, imu.mx, imu.mz, imu.deltat);
  imu.delt_t = millis() - imu.count;
}
void calculateSparkfun(){
  //Abandon all hope ye who enter here, for there be quaternions afoot.
  imu.yaw = atan2(2.0f * (*(getQ()+1) * *(getQ()+2) + *getQ() * *(getQ()+3)), *getQ() * *getQ() + *(getQ()+1) * *(getQ()+1) - *(getQ()+2) * *(getQ()+2) - *(getQ()+3) * *(getQ()+3));
  imu.pitch = -asin(2.0f * (*(getQ()+1) * *(getQ()+3) - *getQ() * *(getQ()+2)));
  imu.roll  = atan2(2.0f * (*getQ() * *(getQ()+1) + *(getQ()+2) * *(getQ()+3)), *getQ() * *getQ() - *(getQ()+1) * *(getQ()+1) - *(getQ()+2) * *(getQ()+2) + *(getQ()+3) * *(getQ()+3));
  imu.pitch *= RAD_TO_DEG;
  imu.yaw   *= RAD_TO_DEG;
  imu.yaw   -= 15.78;
  imu.roll  *= RAD_TO_DEG;
}
void calculateAdafruit(){
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_vec_t orientation;
  //Read data into accel_event (accel.getEvent(), Adafruit_LSM303_Accel_Unified::getEvent)
  accel_event.version = sizeof(sensors_event_t);
  accel_event.sensor_id = -1;
  accel_event.type = SENSOR_TYPE_ACCELEROMETER;
  accel_event.timestamp = millis();
  accel_event.acceleration.x = imu.ax;
  accel_event.acceleration.y = imu.ay;
  accel_event.acceleration.z = imu.az;
  
  if(math.accelGetOrientation(&accel_event, &orientation)){
    imu.roll = orientation.roll;
    imu.pitch = orientation.pitch;
  }else Serial.println("Error calculating roll and pitch");
  
  //Read data into mag_event (mag.getEvent(), Adafruit_LSM303_Mag_Unified::getEvent)
  mag_event.version = sizeof(sensors_event_t);
  mag_event.sensor_id = -1;
  mag_event.type = SENSOR_TYPE_MAGNETIC_FIELD;
  mag_event.timestamp = millis();
  mag_event.magnetic.x = imu.mx;
  mag_event.magnetic.y = imu.my;
  mag_event.magnetic.z = imu.mz;
  
  if(math.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation)){
    imu.yaw = orientation.heading;
  }else Serial.println("Error calculating heading");
}


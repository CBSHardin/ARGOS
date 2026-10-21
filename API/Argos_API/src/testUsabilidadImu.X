
floatVector3d pose;


void accelerometerCallback(floatVector3d acc)
{


}


void setup()
{


  imu.begin(ON_REQUEST);


  if(imu.doAccelerometerSelftest() == false)
    Serial.println("Error acelerometro");

  if(imu.doGyroscopeSelftest() == false)
    Serial.println("Error gyroscipio")

  //Para incluir callbaks:
  if(imu.setOnUpdateAccelerometerCallback(new delegate(&accelerometerCallback) ) == false)
    Serial.println("Fallo al registrar la callback");

}


void loop()
{


  computePose(imu.getLinealAcceleration(true), imu.getAngularSpeed(true), pose);


  Serial.println("Pose of the robot: " +String(pose));

  delay(100);


}

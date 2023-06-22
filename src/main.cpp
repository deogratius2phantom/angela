#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Classifier.h>
#include <Pipeline.h>
Adafruit_MPU6050 mpu;
int predictionCount=0,stepCount=0;
void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  //setupt motion detection
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

  Serial.println("");
  delay(100);
}

void loop() {
  
  //if(mpu.getMotionInterruptStatus()) {
    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    float features[] = {a.acceleration.x, a.acceleration.y, a.acceleration.z,g.gyro.x, g.gyro.y, g.gyro.z};
    if (!pipeline.transform(features))
      return;
    String prediction=forest.predictLabel(pipeline.X);
    if (prediction.equals("walking"))
    {
      predictionCount++;
      if(predictionCount>5)
      {
        predictionCount=0;
        stepCount++;
        Serial.print("Step detected: ");
        Serial.println(stepCount);

      }
      
    }
    // Serial.print("Predicted gesture: ");
    // Serial.print(forest.predictLabel(pipeline.X));
    // Serial.print(" (DSP: ");
    // Serial.print(pipeline.latencyInMicros());
    // Serial.print(" micros, Classifier: ");
    // Serial.print(forest.latencyInMicros());
    // Serial.println(" micros)");
  //}

  delay(10);
}
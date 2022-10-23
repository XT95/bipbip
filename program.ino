/* ---------------------------------------------------------------------------
Variometer

* Arduino Nano 3

* Buzzer : 
  D9 & D10 + 100ohm resistor

* BMP 390L Sensor
  GND to GND
  VDD to 3.3V
  SCL to A5
  SDA to A4

--------------------------------------------------------------------------- */
#include <toneAC.h>
#include <DFRobot_BMP3XX.h>


#define JINGLE
#define VOLUME 10
//#define PRINT_DEBUG_INFO

#define THRESHOLD_TIME 500
#define THRESHOLD_UP 0.2
#define THRESHOLD_DOWN -1.0

void playJingle();
void sensorInit();
void sensorPrintInfo();


float altitudeAverage;
int tapCount;

float altitudeLastAverage;
unsigned long lastTime;

DFRobot_BMP390L_I2C sensor(&Wire, sensor.eSDOVDD);


// ------------------------------------------------------------------- 
// Setup
// ------------------------------------------------------------------- 
void setup() {
  #ifdef PRINT_DEBUG_INFO
  Serial.begin(9600);
  #endif 
  
  // init jingle
  #ifdef JINGLE
  playJingle();
  #endif
  
  sensorInit();
 
  lastTime = millis();
  altitudeLastAverage = sensor.readAltitudeM();
  altitudeAverage = sensor.readAltitudeM();
  tapCount = 1;
}


// ------------------------------------------------------------------- 
// Loop
// ------------------------------------------------------------------- 
void loop() {
  float currentAltitude = sensor.readAltitudeM();
  unsigned long currentTime = millis();

  altitudeAverage += currentAltitude;
  tapCount++;

  if (currentTime - lastTime > THRESHOLD_TIME) {
    altitudeAverage /= tapCount;
    
    float delta = altitudeAverage - altitudeLastAverage;

    if (delta > THRESHOLD_UP) {
      toneAC(512 + delta*50, VOLUME, 1000/4, false);
    } else if (delta < THRESHOLD_DOWN) {
      toneAC(256 + delta*50, VOLUME, 1000/4, false);
    }

    altitudeLastAverage = altitudeAverage;
    altitudeAverage = currentAltitude;
    tapCount = 1;
    lastTime = currentTime;
  }



#ifdef PRINT_DEBUG_INFO
  sensorPrintInfo();
#endif


  delay(100);
}

// ------------------------------------------------------------------- 
// Jingle
// ------------------------------------------------------------------- 
void playJingle() {
  // Gameboy style!
  noToneAC();
  delay(10);
  toneAC_playNote(1025, 10);
  delay(70);
  int v = 10;
  while(v > 0){
    toneAC_playNote(2090, int(v) );
    delay(76);
    v--;
  }
  noToneAC();
}

// ------------------------------------------------------------------- 
// Sensor
// ------------------------------------------------------------------- 
void sensorInit() {
  int res;
  while( (res = sensor.begin()) != ERR_OK ) {
    #ifdef PRINT_DEBUG_INFO
    if(res == ERR_DATA_BUS) {
      Serial.println("Data bus error!!!");
    } else if(res == ERR_IC_VERSION) {
      Serial.println("Chip versions do not match!!!");
    }
    #endif
    delay(3000);
  }

  while( !sensor.setSamplingMode(sensor.eUltraPrecision) ) {
    #ifdef PRINT_DEBUG_INFO
    Serial.println("Set samping mode fail, retrying....");
    #endif
    delay(3000);
  }


#ifdef PRINT_DEBUG_INFO
  // Get the sampling period of the current measurement mode, unit: us
  float samplingPeriod = sensor.getSamplingPeriodUS();
  Serial.print("Sampling period :"); Serial.print(samplingPeriod); Serial.println("us.");

  // Get the sampling frequency of the current measurement mode, unit: Hz
  float sampingFrequencyHz = 1000000.f / samplingPeriod;
  Serial.print("samping frequency : "); Serial.print(sampingFrequencyHz); Serial.println(" Hz.");

  Serial.println();
#endif
}

void sensorPrintInfo() {
  float temperature = sensor.readTempC();
  Serial.print("Temperature : "); Serial.print(temperature); Serial.println(" C");

  float Pressure = sensor.readPressPa();
  Serial.print("Pressure : "); Serial.print(Pressure); Serial.println(" Pa");

  float altitude = sensor.readAltitudeM();
  Serial.print("Altitude : "); Serial.print(altitude); Serial.println(" m");

  Serial.println();
}
#define WIFI_NAME "ania24"
#define WIFI_PASSWORD "tuchowkrakow"
#define DEVICE_ID 2045
#define DEVICE_NAME "Weather Station"
#define TOKEN "~29_0Cd.MIakGx,x!"

#define WEBPAGE_DEVICE_ID 1001


#include <stdint.h>
#include "SparkFunBME280.h"

#include "Wire.h"
#include "SPI.h"



#include <ArduinoHttpClient.h>
#include <RemoteMe.h>
#include <ESP8266WiFi.h>

#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;
RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

BME280 mySensor;


void onUserSyncMessage(uint16_t senderDeviceId, uint16_t dataSize, uint8_t* data, uint16_t &returnDataSize, uint8_t *&returnData)
{
  uint16_t pos = 0;

  returnDataSize = 12;
  returnData = (uint8_t*)malloc(returnDataSize);
  

  RemoteMeMessagesUtils::putFloat(returnData,pos, (mySensor.readTempC()));
  RemoteMeMessagesUtils::putFloat(returnData, pos, (mySensor.readFloatPressure() ));
  RemoteMeMessagesUtils::putFloat(returnData, pos, (mySensor.readFloatHumidity() ));
  

}


void sort(double a[], int size) {
    for(int i=0; i<(size-1); i++) {
        for(int o=0; o<(size-(i+1)); o++) {
                if(a[o] > a[o+1]) {
                    double t = a[o];
                    a[o] = a[o+1];
                    a[o+1] = t;
                }
        }
    }
}



// the setup function runs once when you press reset or power the board
void setup() {
  
	Serial.begin(9600);
 while(!Serial){
  ;
  }

  
	
	//--------
  
	WiFiMulti.addAP(WIFI_NAME, WIFI_PASSWORD);
	while (WiFiMulti.run() != WL_CONNECTED) {
		delay(100);
	}


	  remoteMe.setUserSyncMessageListener(onUserSyncMessage);
    remoteMe.setupTwoWayCommunication();


	  remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);

  

    mySensor.settings.commInterface = I2C_MODE;
    mySensor.settings.I2CAddress = 0x76;
  
  
    //***Operation settings*****************************//
  
    //runMode can be:
    //  0, Sleep mode
    //  1 or 2, Forced mode
    //  3, Normal mode
    mySensor.settings.runMode = 3; //Forced mode
  
                     //tStandby can be:
                     //  0, 0.5ms
                     //  1, 62.5ms
                     //  2, 125ms
                     //  3, 250ms
                     //  4, 500ms
                     //  5, 1000ms
                     //  6, 10ms
                     //  7, 20ms
    mySensor.settings.tStandby = 0;
  
   
    mySensor.settings.filter = 4;
  
   
    mySensor.settings.tempOverSample = 5;
    mySensor.settings.pressOverSample = 5;
    mySensor.settings.humidOverSample = 5;
   
   
   mySensor.begin();
 

	
}

void loop(){
  static long lastRead=0;

if (lastRead+1000l*60*5<millis()){
  lastRead=millis();
  
  double temp[10];
  double pressure[10];
  double humm[10];
  for(int i=0;i<9;i++){
    temp[i]= mySensor.readTempC();
    pressure[i]= mySensor.readFloatPressure();
    humm[i]= mySensor.readFloatHumidity();
    delay(100);
  }

  sort(temp,10);
  sort(humm,10);
  sort(pressure,10);




    remoteMe.sendAddDataMessage(1, RemotemeStructures::_5M, 0, temp[5]);
    remoteMe.sendAddDataMessage(2, RemotemeStructures::_5M, 0, pressure[5]);
    remoteMe.sendAddDataMessage(3, RemotemeStructures::_5M, 0,humm[5]);


}
   remoteMe.loop();
    //digitalWrite(powerPIN, LOW);

    //remoteMe.disconnect();
  
    //ESP.deepSleep(36e8-(millis()-timeAtStart));

  
}




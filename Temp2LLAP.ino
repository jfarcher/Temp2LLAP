//////////////////////////////////////////////////////////////////////////
// LLAP message based on dallas temperature

// Based on LLAP Serial Library and examples from Ciseco
//////////////////////////////////////////////////////////////////////////

#include <LLAPSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;

#define DEVICEID "A1"	// this is the LLAP device ID

int i;
float f;


void setup() {
       	Serial.begin(115200);

	pinMode(8,OUTPUT);		// switch on the SRF radio
	digitalWrite(8,HIGH);
	delay(1000);		        // allow the radio to startup

//-------------enable SRF sleep mode 2---------------------------------------------- 
//http://openmicros.org/index.php/articles/88-ciseco-product-documentation/260-srf-configuration       
        pinMode(4,OUTPUT);           // hardwired XinoRF / RFu328 SRF sleep pin 
        digitalWrite(4,LOW);          // pull sleep pin high - sleep 2 disabled
	Serial.print("+++");            // enter AT command mode
        delay(1500);                   // delay 1.5s
        Serial.println("ATSM2");         // enable sleep mode 2 <0.5uA
        delay(2000);
        Serial.println("ATDN");          // exit AT command mode*/
        delay(2000);
//---------------------------------------------------------------------------------

        LLAP.init(DEVICEID);
        sensors.begin();
        if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
        sensors.setResolution(insideThermometer, 12);
	Serial.flush();

       
}   
void printTemperature(DeviceAddress deviceAddress)
{
        float tempC = sensors.getTempC(deviceAddress);
        LLAP.sendInt("TEMP",tempC);    
}  
void loop() {
  


  		sensors.requestTemperatures();
                printTemperature(insideThermometer);
//-------------------------------------------------------------------------------------
		
//-------Put ATmega328 and SRF Radio to sleep------------------------------------------
  delay(10);                    // allow radio to finish sending
  digitalWrite(4, HIGH);        // pull sleep pin high to enter SRF sleep 2
  LLAP.sleepForaWhile(30000);    // sleep ATmega328 for 5s (ms)
  digitalWrite(4, LOW);         // when ATmega328 wakes up, wake up SRF Radio
  delay(10);                    // allow radio to wake up
//-------------------------------------------------------------------------------------
	
}

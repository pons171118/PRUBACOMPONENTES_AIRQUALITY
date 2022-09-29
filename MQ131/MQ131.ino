#include <WiFi.h>
#include <MQUnifiedsensor.h>


const char* ssid = "##########";
const char* password = "######";

#define placa "ESP-32"
#define Voltage_Resolution 5
#define pin 34
#define type "MQ-131"
#define ADC_Bit_Resolution 2
#define RatioMQ131CleanAir 15  
MQUnifiedsensor MQ131(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

void setup() 
{
    delay(500);    
    Serial.begin(9600);   
    if(Serial) Serial.println("Serial is open");  
    WiFi.begin(ssid, password);    
    Serial.print("Connecting");   
    while (WiFi.status() != WL_CONNECTED) {     
        delay(500);     
        Serial.print(".");   
    }   
    Serial.println();    
    Serial.print("Connected, IP address: ");   
    Serial.println(WiFi.localIP()); 
      //Set math model to calculate the PPM concentration and the value of constants   
    MQ131.setRegressionMethod(1); //_PPM =  a*ratio^b   
    MQ131.setA(23.943); MQ131.setB(-1.11);
     
    MQ131.init();    
    Serial.print("Calibrating please wait.");   
    float calcR0 = 0;   
    for(int i = 1; i<=10; i ++)   {     
        MQ131.update(); // Update data, the arduino will be read the voltage on the analog pin     
        calcR0 += MQ131.calibrate(RatioMQ131CleanAir);    
        Serial.print(".");   
    }   
    MQ131.setR0(calcR0/10);   
    Serial.println("  done!.");      
    if(isinf(calcR0)) { Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}   
    if(calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}   
    /*****************************  MQ CAlibration **************************/                   
    MQ131.serialDebug(false); 
      
}
void loop() {
  MQ131.update(); // Update data, the arduino will be read the voltage on the analog pin   
    O3 = MQ131.readSensor();   
    Serial.print("Ozone: ");   
    Serial.println(O3);
    NOx = MQ131.readSensor(); // Sensor will read CO2 concentration using the model and a and b values setted before or in the setup   
    Serial.print("NO2: ");   
    Serial.println(NOx);   
    delay(5000); 
}

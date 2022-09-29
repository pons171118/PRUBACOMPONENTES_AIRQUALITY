#include <WiFi.h>
#include <MQUnifiedsensor.h>


const char* ssid = "##############";
const char* password = "##########";

#define placa "ESP-32"
#define Voltage_Resolution 3.3
#define pin 34
#define type "MQ-135"
#define ADC_Bit_Resolution 12
#define RatioMQ135CleanAir 3.6  
double CO2 = (0);  
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

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
    MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b   
    MQ135.setA(110.47); 
    MQ135.setB(-2.862); 
 
    MQ135.init();    
    Serial.print("Calibrating please wait.");   
    float calcR0 = 0;   
    for(int i = 1; i<=10; i ++)   {     
        MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin     
        calcR0 += MQ135.calibrate(RatioMQ135CleanAir);    
        Serial.print(".");   
    }   
    MQ135.setR0(calcR0/10);   
    Serial.println("  done!.");      
    if(isinf(calcR0)) { Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}   
    if(calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}   
    /*****************************  MQ CAlibration **************************/                   
    MQ135.serialDebug(false); 
      
}
void loop() {
  MQ135.update();   
    CO2 = MQ135.readSensor(); 
    Serial.println(CO2);  
    delay(5000); 
}

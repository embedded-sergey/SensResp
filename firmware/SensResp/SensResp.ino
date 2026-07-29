// Include following libraries to enable I2C protocol
#include <Wire.h>  // include the standard I2C library
#include <Ezo_i2c.h>  // include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include <sequencer4.h>  // imports a 2 function sequencer
#include <Ezo_i2c_util.h>  // brings in common print statements
#include <LiquidCrystal_I2C.h>  // I2C for LCD 20x4 display

// Temperature used for DO compensation when RTD reading is invalid (safe to edit by a user)
const float static_compensation_temperature = 20.0; // if no actual or valid value from RTD received
const float invalid_temperature = -273.15; // RTD readings below this value are considered invalid

//define I2C addresses for all the sensors and devices except SensResp
#define PumpResp_address 99  // define I2C address for PumpResp
#define PumpResp_size 5  // define the maximum size of 'Phase' string from PumpResp
int LED = 4;

String phase = "";

LiquidCrystal_I2C LCD(0x27, 20, 4); // Creates I2C LCD Object With (Address=0x27, Cols=20, Rows=4)

// AtlasScientific devices are defined using their own functions:
Ezo_board RTD = Ezo_board(100, "");  // create an RTD circuit object which address is 100
Ezo_board DO_1 = Ezo_board(101, "");
Ezo_board DO_2 = Ezo_board(102, "");
Ezo_board DO_3 = Ezo_board(103, "");
Ezo_board DO_4 = Ezo_board(104, "");

// Forward declarations of functions to use them in the sequencer before defining them
void triggerPumpRespAndRTD();
void sendTemperatureToDO();
void requestDOReadings();
void collectAllData();

// Calls the steps in sequence with time in between them
Sequencer4 Seq(&triggerPumpRespAndRTD, 50,
               &sendTemperatureToDO, 300,
               &requestDOReadings, 600,
               &collectAllData, 30);

void setup(){
  Serial.begin(9600);  // start the serial communication to PC with default baud rate
  Serial.println("CLEARRANGE,A,2,H,1000000"); // trick to get rid of AM-PM
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  
  Wire.begin();  // start the I2C without an address inside as SensResp is a master device
  
  LCD.init();
  LCD.backlight();
  LCD.noCursor();
  LCD.clear();
}


void loop() {
  Seq.run();  // run the sequencer to do the polling
}


void triggerPumpRespAndRTD() {
  // Write a start signal (i.e. zero) to the PumpResp
  Wire.beginTransmission(PumpResp_address);
  Wire.write(0);
  Wire.endTransmission();
  
  //send a read command. we use this command instead of RTD.send_cmd("R"); 
  //to let the library know to parse the reading
  RTD.send_read_cmd();
}


void sendTemperatureToDO() {
  // If the temperature reading has been received and it is valid
  if ((RTD.get_error() == Ezo_board::SUCCESS) && (RTD.get_last_received_reading() > invalid_temperature)) {  
    DO_1.send_cmd_with_num("T,", RTD.get_last_received_reading());
    DO_2.send_cmd_with_num("T,", RTD.get_last_received_reading());
    DO_3.send_cmd_with_num("T,", RTD.get_last_received_reading());
    DO_4.send_cmd_with_num("T,", RTD.get_last_received_reading());
  } else {                
    DO_1.send_cmd_with_num("T,", static_compensation_temperature);   
    DO_2.send_cmd_with_num("T,", static_compensation_temperature); 
    DO_3.send_cmd_with_num("T,", static_compensation_temperature); 
    DO_4.send_cmd_with_num("T,", static_compensation_temperature);  
  }
}


void requestDOReadings() {
  DO_1.send_read_cmd(); 
  DO_2.send_read_cmd();            
  DO_3.send_read_cmd();            
  DO_4.send_read_cmd();            
}


void collectAllData(){
  // Request data and read response from PumpResp
  Wire.requestFrom(PumpResp_address, PumpResp_size); 
  phase = ""; // add characters to a string (i.e. phase)
   while (Wire.available()) {
    char b = Wire.read();
    // Ignore common non-printable or trailing padding bytes
    if (b == '\0' || b == '\r' || b == '\n') {
      break; // Stop reading if the slave sent a terminator
    }
    phase += b;
  } 
  
  // Explicitly trim any unexpected accidental whitespace
  phase.trim();
  Serial.print(phase);
  // Get the reading from the RTD circuit
  receive_and_print_reading(RTD);
  // Get the reading from the DO circuits
  receive_and_print_reading(DO_1);
  receive_and_print_reading(DO_2);
  receive_and_print_reading(DO_3);
  receive_and_print_reading(DO_4);
  Serial.println();

  float a = RTD.get_last_received_reading();
  float b = DO_1.get_last_received_reading();
  float c = DO_2.get_last_received_reading();
  float d = DO_3.get_last_received_reading();
  float e = DO_4.get_last_received_reading();

  Serial.println((String) "DATA,DATE,TIME," + phase + (",") + a + (",") + b + (",") + c + (",")  + d + (",") + e);

  LCD_code();  // update LCD here
}


void LCD_code() {
    // Dissolved Oxygen readings
    LCD.setCursor(0, 0); LCD.print("DO1: " + String(DO_1.get_last_received_reading()) + "   ");
    LCD.setCursor(0, 1); LCD.print("DO2: " + String(DO_2.get_last_received_reading()) + "   ");
    LCD.setCursor(0, 2); LCD.print("DO3: " + String(DO_3.get_last_received_reading()) + "   ");
    LCD.setCursor(0, 3); LCD.print("DO4: " + String(DO_4.get_last_received_reading()) + "   ");

    // PumpResp phase: F - Flush, M - Measurement
    LCD.setCursor(13, 0); LCD.print("Phase:");
    LCD.setCursor(13, 1); LCD.print(phase + "  ");

    // Temperature readings
    LCD.setCursor(13, 2); LCD.print("Temp:");
    LCD.setCursor(13, 3); LCD.print(String(RTD.get_last_received_reading()) + "  ");
}


// Sources:
// 1. I2C mode instructionsfor communication between two Arduinos: 
//      https://dronebotworkshop.com/i2c-arduino-arduino/
// 2. Atlas Scientific I2C example for multiple sources:
//      https://github.com/Atlas-Scientific/Ezo_I2c_lib/blob/master/Examples/I2c_lib_examples/I2c_read_mulitple_circuits/I2c_read_mulitple_circuits.ino
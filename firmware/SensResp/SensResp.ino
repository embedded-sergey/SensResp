// Include Arduino Wire and Atlas Scientific libraries for I2C protocol
#include <Wire.h>  // include the standard I2C library
#include <Ezo_i2c.h>  // include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include <sequencer4.h>  // imports a 2 function sequencer 
#include <Ezo_i2c_util.h>  // brings in common print statements
#include <LiquidCrystal_I2C.h>

//define I2C addresses for all the sensors and devices except SensResp 
#define PumpResp_address 99  // define I2C address for PumpResp
#define PumpResp_size 5  // define the maximum size of 'Phase' string from PumpResp
int LED = 4;

String phase = "";

LiquidCrystal_I2C MyLCD(0x27, 20, 4); // Creates I2C LCD Object With (Address=0x27, Cols=20, Rows=4)
bool lcd_ready = false;

// AtlasScientific devices are defined using their own functions:
Ezo_board RTD = Ezo_board(100, "");  // create a Temperature circuit object, who's address is 91 and name is "PH"
Ezo_board DO_1 = Ezo_board(101, "");  // create an DO circuit object who's address is 101
Ezo_board DO_2 = Ezo_board(102, "");  // create an DO circuit object who's address is 102
Ezo_board DO_3 = Ezo_board(103, "");  // create an DO circuit object who's address is 103
Ezo_board DO_4 = Ezo_board(104, "");  // create an DO circuit object who's address is 104

// Forward declarations of functions to use them in the sequencer before defining them
void step1();  
void step2();
void step3();
void step4();

// Calls the steps in sequence with time in between them
Sequencer4 Seq(&step1, 50,
               &step2, 300,
               &step3, 600,
               &step4, 30);

void setup(){
  Serial.begin(9600);  // start the serial communication to PC with default baud rate
  Serial.println("CLEARRANGE,A,2,H,1000000"); // trick to get rid of AM-PM
  pinMode(LED, OUTPUT); 
  digitalWrite(LED, HIGH);
  
  Wire.begin();  // start the I2C without an address inside as SensResp is a master device
  
  MyLCD.init();
  MyLCD.backlight();
  MyLCD.noCursor();
  MyLCD.clear();
}

void loop() {
  Seq.run();  // run the sequencer to do the polling
    
if (lcd_ready) {
    lcd_ready = false;

    MyLCD.clear();

    MyLCD.setCursor(0, 0);
    MyLCD.print("Temp: ");
    MyLCD.print(RTD.get_last_received_reading());


    MyLCD.setCursor(0, 1);
    MyLCD.print("Temp: ");
    MyLCD.print(RTD.get_last_received_reading());

    MyLCD.setCursor(0, 2);
    MyLCD.print("DO1: ");
    MyLCD.print("----");

    MyLCD.setCursor(0, 3);
    MyLCD.print("DO2: ");
    MyLCD.print("----");
}
}

void step1() {
  // Write a start signal (i.e. zero) to the PumpResp
  Wire.beginTransmission(PumpResp_address);
  Wire.write(0);
  Wire.endTransmission();
  
  //DO_1.send_cmd("S, 31, ppt");
  // Send a command to read data from sensors  
  
  //send a read command. we use this command instead of RTD.send_cmd("R"); 
  //to let the library know to parse the reading
  RTD.send_read_cmd();
}

void step2() {
     
  // If the temperature reading has been received and it is valid
  if ((RTD.get_error() == Ezo_board::SUCCESS) && (RTD.get_last_received_reading() > -1000.0)) {  
     DO_1.send_cmd_with_num("T,", RTD.get_last_received_reading());
     DO_2.send_cmd_with_num("T,", RTD.get_last_received_reading());
     DO_3.send_cmd_with_num("T,", RTD.get_last_received_reading());
     DO_4.send_cmd_with_num("T,", RTD.get_last_received_reading());
  } else {                
    DO_1.send_cmd_with_num("T,", 10.0);   
    DO_2.send_cmd_with_num("T,", 10.0); 
    DO_3.send_cmd_with_num("T,", 10.0); 
    DO_4.send_cmd_with_num("T,", 10.0);  
  }
}

void step3() {
  // Send the read command to DO circuits
  DO_1.send_read_cmd(); 
  DO_2.send_read_cmd();            
  DO_3.send_read_cmd();            
  DO_4.send_read_cmd();            
}

void step4(){
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
  
  lcd_ready = true;

  Serial.print(phase);
  Serial.print("\t");

  // Get the reading from the RTD circuit
  receive_and_print_reading(RTD);  // get the reading from the RTD circuit and print it
  Serial.print("\t");
  // Get the reading from the DO circuits
  receive_and_print_reading(DO_1);
  Serial.print("\t");
  receive_and_print_reading(DO_2);
  Serial.print("\t");
  receive_and_print_reading(DO_3);
  Serial.print("\t");
  receive_and_print_reading(DO_4);
  Serial.println();

  float a = RTD.get_last_received_reading();
  float b = DO_1.get_last_received_reading();
  float c = DO_2.get_last_received_reading();
  float d = DO_3.get_last_received_reading();
  float e = DO_4.get_last_received_reading();

  Serial.println((String) "DATA,DATE,TIME," + phase + (",") + a + (",") + b + (",") + c + (",")  + d+ (",") + e);
}

// Sources:
// 1. I2C mode instructionsfor communication between two Arduinos: 
//      https://dronebotworkshop.com/i2c-arduino-arduino/
// 2. Atlas Scientific I2C example for multiple sources:
//      https://github.com/Atlas-Scientific/Ezo_I2c_lib/blob/master/Examples/I2c_lib_examples/I2c_read_mulitple_circuits/I2c_read_mulitple_circuits.ino
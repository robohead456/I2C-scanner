#include <Arduino.h>
#include <Wire.h>

// Serial print a byte as 1's and 0's
void printBits(byte myByte){
  for(byte mask = 0x80; mask; mask >>= 1) {
    if(mask & myByte)
      Serial.print('1');
    else
      Serial.print('0');
  }
  Serial.println();
}

// Read an I2C register
//   set output to 1 to print the contents of the register to serial
uint8_t readRegister(int add, int reg, bool output = 0) {
  Wire.beginTransmission(add);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(add,1);
  while(Wire.available() < 1) {}
  uint8_t data = Wire.read();
  Wire.endTransmission();
  if(output) {
    Serial.print("Register ");
    Serial.println(reg,HEX);
    printBits(data);
  }
  return data;
}
  
// Setup function
void setup() {
  Wire.begin();
  
  Serial.begin(9600);
  while (!Serial);
  Serial.println("\nI2C Scanner");
}

byte error, address;
int nDevices;

void loop() {

  Serial.println("Scanning...");
  
  nDevices = 0;

  // Loop through each possible address
  for(address = 1; address < 127; address++) {

    Wire.beginTransmission(address);
    error = Wire.endTransmission();
  
    // If the message was properly read
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
  
      nDevices++;

      readRegister(address, 0x03, 0);
      delay(10);
      readRegister(address, 0x15, 0);
      delay(10);
      readRegister(address, 0x16, 0);
      delay(10);
      readRegister(address, 0x17, 0);
      delay(10);
      readRegister(address, 0xFE, 0);

    }
    // If an error occurred
    else if (error==4) {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
    // If no device was found at that address (or other error)
    else {

    }
  }

  // Print number of devices found
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else {
    Serial.print(nDevices);
    Serial.println(" devices found");
    Serial.println("done\n");
  }
  
  delay(5000); // Repeat every 5 seconds
}

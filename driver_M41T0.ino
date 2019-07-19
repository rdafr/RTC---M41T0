// RTC M41T0: Driver
// Authors: Rubens Fernandes, Marcelo Barra, Sanuel Rego and Manoel Neto
// Date: 17/07/2019

/*--Libraries--*/
#include <Wire.h>
/*-------------*/

/*----Defines----*/
#define ADDR_I2C_M41T0 0x68
#define ADDR_REG_SEC 0x0
#define ADDR_REG_MIN 0x1
#define ADDR_REG_HRS 0X2
#define ADDR_REG_WDAY 0x3
#define ADDR_REG_MDAY 0x4
#define ADDR_REG_MTH 0x5
#define ADDR_REG_YEAR 0x6
#define ADDR_REG_CNTRL 0x7
/*---------------*/

void init_I2C(void)
{
  Wire.begin();//Initiates the I2C bus            
}

void init_serial(void)
{  
  Serial.begin(9600);//Initiates the serial port    
}

int bcdToDec(byte bcd)
{
  return( (bcd/16*10) + (bcd%16) );//converts BCD to Decimal
}

byte decToBcd(byte dec)
{
  return( (dec/10*16) + (dec%10) );//converts Decimal to BCD
}

void set_alt_read(int slave_address, int bytes)
{
  Wire.requestFrom(slave_address, bytes);//Realize the alternative read method from M41T0 module
} 

void request_reg_m41t0(int slave_address, int reg_address, int bytes_after)
{
  Wire.beginTransmission(slave_address);// Begin transmission with the M41T0 module
  Wire.write(reg_address);//Write in the chosen M41T0 register
  Wire.endTransmission();//Close the transmission with the M41T0 module
  Wire.requestFrom(slave_address, bytes_after);//Realize the alternative read method from M41T0 module
} 

void write_reg_m41t0(int slave_address, int reg_address, int value, int bytes_after)
{
  Wire.beginTransmission(slave_address);//Begin transmission with the M41T0 module
  Wire.write(reg_address);//Write in the chosen M41T0 register
  Wire.write(value);//Write the value in the chosen M41T0 register
  Wire.endTransmission();//Close the transmission with the M41T0 module
  Wire.requestFrom(slave_address, bytes_after);//Realize the alternative read method from M41T0 module
} 

void read_m41t0(void)
{
  while (Wire.available())//If there is still bytes been sended, do the code below
  {
    byte data = Wire.read();//Read the data and store in the data variable
    Serial.println(bcdToDec(data));//Print on the serial port the variable data converted to decimal       
  } 
}
  
String get_date()
{
  request_reg_m41t0(ADDR_I2C_M41T0, ADDR_REG_SEC, 7);//Calls the corresponding function
  unsigned int n[6];//declares a vector to store six variables
  int i= 0;//declares the variable i with zero as its start value
  String sec, mint, hr, wday, mday, mth, year;//Declares variables to store the values from the M41T0 register
  
  while (Wire.available())//If there is still bytes been sended, do the code below
  {
    n[i++] = Wire.read();//Read the data and store in the vector       
  }
  
  sec = String (bcdToDec(n[0]&0x7F));//Stores the firt value of the vector in the corresponding variable and removes the unecessary bits
  mint = String (bcdToDec(n[1]&0x7F));//Stores the second value of the vector in the corresponding variable and removes the unecessary bits       
  hr = String (bcdToDec(n[2]&0X3F));//Stores the third value of the vector in the corresponding variable and removes the unecessary bits
  wday = String (bcdToDec(n[3]&0X07));//Stores the fourth value of the vector in the corresponding variable and removes the unecessary bits 
  mday = String (bcdToDec(n[4]&0X3F));//Stores the fifth value of the vector in the corresponding variable and removes the unecessary bits 
  mth = String (bcdToDec(n[5]&0X1F));//Stores the sixth value of the vector in the corresponding variable and removes the unecessary bits 
  year = String (bcdToDec(n[6]));//Stores the seventh value of the vector in the corresponding variable and removes the unecessary bits 

  String date = String(year + "-" + mth + "-" + mday + " " + hr + ":" + mint + ":" + sec);//Create a string to insert the data formated 

  return date;//Return the string with the required data
  
}

void set_date( int year, int mth, int mday, int hr, int mint, int sec)
{
  write_reg_m41t0(ADDR_I2C_M41T0, ADDR_REG_SEC, decToBcd(sec), 0);//Calls the corresponding function with the set of values chosen
  write_reg_m41t0(ADDR_I2C_M41T0, ADDR_REG_MIN, decToBcd(mint), 0);//Calls the corresponding function with the set of values chosen
  write_reg_m41t0(ADDR_I2C_M41T0, ADDR_REG_HRS, decToBcd(hr), 0);//Calls the corresponding function with the set of values chosen
  write_reg_m41t0(ADDR_I2C_M41T0, ADDR_REG_MDAY, decToBcd(mday), 0);//Calls the corresponding function with the set of values chosen
  write_reg_m41t0(ADDR_I2C_M41T0, ADDR_REG_MTH, decToBcd(mth), 0);//Calls the corresponding function with the set of values chosen
  write_reg_m41t0(ADDR_I2C_M41T0, ADDR_REG_YEAR, decToBcd(year), 0);//Calls the corresponding function with the set of values chosen
}
void setup() 
{
  init_serial();//Calls the corresponding function
  init_I2C();//Calls the corresponding function
  set_date( 19, 7, 19, 9, 17, 40);// Calls the corresponding function
}

void loop() 
{ 
  String date;// Create a string variable date 
  date = get_date();//Calls the corresponding function and store the returned in the variable date
  Serial.println(date);// Print on the serial port the variable date
  delay(1000);//Wait for one second
}

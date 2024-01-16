const int flexPin = A0;
const float VCC = 3.3;
const float R_DIV = 70000.0;
const float flatResistance = 45000.0;
const float bendResistance = 125000.0;
String myInteger;
int i = 0;

void setup() {
  Serial.begin(115200); // Initialize serial communication and set baud rate to 115200
  pinMode(flexPin, INPUT);
}

void loop() 
{
  int ADCflex = analogRead(flexPin);
  float Vflex = 3.3 - ADCflex * VCC / 1023.0;
  float Rflex = R_DIV * (VCC / Vflex - 1.0);
  float angle = map(Rflex, flatResistance, bendResistance, 0, 90.0);
  
  String angleStr = String(ADCflex); // Convert the angle to string with one decimal place
  
  while(angleStr.length() < 10) // Ensure the string is 10 characters long
  {
    angleStr += " "; // Append space characters if the string is shorter than 10 characters
  }
  myInteger = Serial.readString();

  if(i%2==0)
  {
    Serial.print(myInteger);
  }
  else
  {
    Serial.print(angleStr); // Print the 10-character string
  }
  i++;
  while(Serial.available() <= 0)
  {
    
  }
  delay(500);
}
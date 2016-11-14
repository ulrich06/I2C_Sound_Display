#include <Wire.h>
#define MAX_Y 8
#define MAX_X 8
#define MAX_VALUE 1023
#define MIN_VALUE 0
#include <Rainbowduino.h>


struct Color{
  byte r;
  byte g;
  byte b;
};
typedef struct Color Color;
unsigned int currentX = 0;


void setup() {
  Rb.init();
  Serial.begin(9600); //For debug purposes
  Wire.begin(8);                // Join I2C with address #8
  Wire.onReceive(receiveEvent); // register event
}

unsigned int integerValue=0; 
char incomingByte;

void loop() {}

void draw(int signal){
  if (currentX % MAX_X == 0) Rb.blankDisplay();
  int cell;
  if (signal > MAX_VALUE){
    cell = MAX_Y - 1;
  } else if (signal < MIN_VALUE){
    cell = 0;
  } else {
    cell = signal / (MAX_VALUE / MAX_Y);
  }
  Serial.print("Signal: "); Serial.println(signal);
  Serial.print("Cell: "); Serial.println(cell);
  Serial.print("CurrentX: "); Serial.println(currentX);
  Serial.println("---");
  for (int i = 0; i <= cell; i++){
    Color color = defineColor(i);
    Rb.setPixelXY(currentX % MAX_X, i, color.r, color.g, color.b);
  }
  
  currentX++;
  if (currentX == MAX_X) currentX = 0; //Prevent overflow
}

Color defineColor(int cell){
  if (cell < 2) {
    return {0x00, 0x255, 0x00}; 
  } else if (cell >= 2 && cell < 5) {
    return {0x50, 0x230, 0x00}; 
  } else {
    return {0x255, 0x00, 0x00}; 
  } 
}


void receiveEvent(int nbBits) {
  if(nbBits == 2)
  {
    int receivedValue  = Wire.read() << 8;
    receivedValue |= Wire.read();
    draw(receivedValue);
    Serial.println(receivedValue);
  }
  else
  {
    Serial.print("Unexpected number of bytes received: ");
    Serial.println(nbBits);
  }
}
  

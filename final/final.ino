//Snake
#include <TrueRandom.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

//Initialization
#define TFT_CS  10  // Chip select line for TFT display
#define TFT_DC   9  // Data/command line for TFT
#define TFT_RST  8  // Reset line for TFT (or connect to +5V)
#define BUTTON_NONE 0
#define BUTTON_DOWN 1
#define BUTTON_RIGHT 2
#define BUTTON_SELECT 3
#define BUTTON_UP 4
#define BUTTON_LEFT 5

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

uint16_t pixelColor;
uint8_t buttonPressed, currentDirection;
boolean collision, appleOn, displayEnd, specialOn, allowSpecial, showTitle = true;
int head, timer, snakeSize, pixelLocationXAxis, pixelLocationYAxis, x[300], y[300],
    appleX, appleY, yMultiplier, selection = 100, difficulty, specialX, specialY, specialTime;
int score = 0;

//Pacman
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST     0

int melody[] = {
  NOTE_B4, 16, NOTE_B5, 16, NOTE_FS5, 16, NOTE_DS5, 16, //1
  NOTE_B5, 32, NOTE_FS5, -16, NOTE_DS5, 8, NOTE_C5, 16,
  NOTE_C6, 16, NOTE_G6, 16, NOTE_E6, 16, NOTE_C6, 32, NOTE_G6, -16, NOTE_E6, 8,

  NOTE_B4, 16,  NOTE_B5, 16,  NOTE_FS5, 16,   NOTE_DS5, 16,  NOTE_B5, 32,  //2
  NOTE_FS5, -16, NOTE_DS5, 8,  NOTE_DS5, 32, NOTE_E5, 32,  NOTE_F5, 32,
  NOTE_F5, 32,  NOTE_FS5, 32,  NOTE_G5, 32,  NOTE_G5, 32, NOTE_GS5, 32,  NOTE_A5, 16, NOTE_B5, 8
};

int tempo = 0;
int buzzer = 5;
int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int divider = 0, thisNote = 0;
//int propeller = 0;
bool active = false;

void setup () 
{ 
  //Snake
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(0x0000);
  
  if (showTitle && active == true)
  {
    showTitlScreen();
  }
  
  tft.fillScreen(0x0000);
  collision = false;
  appleOn = false;
  specialOn = false;
  allowSpecial = false;
  displayEnd = false;
  pixelLocationXAxis = tft.width() / 2 - 1;
  pixelLocationYAxis = tft.height() / 2 - 1;
  pixelColor = ST7735_WHITE;
  buttonPressed = BUTTON_RIGHT;
  currentDirection = BUTTON_RIGHT;
  specialTime = 0;
  head = 0;
  snakeSize = 5;
  timer = 0;
  updateScore(score);
  drawBoard();

  if (active == false)
  {
    tft.fillRect(0, 0, tft.width(), tft.height(), ST7735_BLACK);
  }
}


void loop () 
{
  if (active)
  {
    //Pacman
    divider = melody[thisNote + 1];
  
    /*
    if (divider > 0)
    {
      noteDuration = (wholenote) / divider;
    }
    else if (divider < 0)
    {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }
    */
    if (tempo != 1)
    {
      tempo += 1;
    }
    else
    {
      tempo = 0;
      changeNote();
    }
    
    //#Snake
    uint8_t b = readButton();
   
    if (b != BUTTON_NONE && b != BUTTON_SELECT)
    {
      buttonPressed = b;
    }
    
    if (!collision && active == true)
    {
      checkrecord();
      appleLogic();
      checkIfAppleGot();
      specialApple();
      checkIfSpecialGot();
      specialTimer();
      updateSnakePosition(buttonPressed);
      
      if (timer >= snakeSize - 1)
      {
        removeLastFromTail();
      }
      else
      {
        timer++;
      }
    }
    else
    {
      if (displayEnd == true)
      {
        displayEndingScreen();
      }
      showTitle = true;
      uint8_t buttonPressed = readButton();
      
      if (buttonPressed == BUTTON_SELECT && active == true)
      {
        setup();
      }
    }
  
    tone(buzzer, melody[thisNote]);
    delay(difficulty);
    noTone(buzzer);
  }
  else
  {
    /*
    propeller ++;
    Serial.println(propeller);
    delay(1000);

    if (propeller == 4)
    {
      active = true;
    }
    */
  }
  
  tone(buzzer, 0);
}

uint8_t readButton(void)
{
  int a = analogRead(3);

  if (a < 15) return BUTTON_LEFT;
  if (a < 45) return BUTTON_UP;
  if (a < 110) return BUTTON_DOWN;
  if (a < 190) return BUTTON_RIGHT;
  if (a < 380) return BUTTON_SELECT;
  else return BUTTON_NONE;
}

void changeNote()
{
  if (thisNote == 60)
  {
    thisNote = 0;
  }
  else
  {    
    thisNote += 2;
  }
}

//#Snake
void showTitlScreen()
{
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(4);
  tft.setTextColor(ST7735_GREEN);
  tft.setCursor(5, 30);
  tft.print("SNAKE");
  tft.setTextSize(1);
  tft.setTextColor(ST7735_GREEN);
  tft.setCursor(30, 150);
  tft.print("Team 9 @NCCU");
  titleStart();
}

void titleStart()
{
  uint8_t titleButton = readButton();
  tft.setCursor(55, 100);
  tft.fillRect(40, 100, 10, 8, ST7735_GREEN);
  tft.print("START");

  if (titleButton == BUTTON_SELECT)
  { 
    difficulty = 87;
    showTitle = false;
    return;
  }
  
  delay(100);
  titleStart();
}

/*
void titleSelection()
{
  uint8_t titleButton = readButton();

  if (titleButton == BUTTON_UP)
  {
    selection--;
  }
  if (titleButton == BUTTON_DOWN)
  {
    selection++;
  }

  if (selection % 3 == 0)
  { // easy mode
    tft.setCursor(40, 80);
    tft.fillRect(25, 80, 10, 8, ST7735_GREEN);
    tft.print("Easy");
    tft.setCursor(40, 100);
    tft.fillRect(25, 100, 10, 8, ST7735_BLACK);
    tft.print("Medium");
    tft.setCursor(40, 120);
    tft.fillRect(25, 120, 10, 8, ST7735_BLACK);
    tft.print("Hard");
    difficulty = 90;
  }
  else if (selection % 3 == 1)
  { // normal mode
    tft.setCursor(40, 80);
    tft.fillRect(25, 80, 10, 8, ST7735_BLACK);
    tft.print("Easy");
    tft.setCursor(40, 100);
    tft.fillRect(25, 100, 10, 8, ST7735_GREEN);
    tft.print("Medium");
    tft.setCursor(40, 120);
    tft.fillRect(25, 120, 10, 8, ST7735_BLACK);
    tft.print("Hard");
    difficulty = 65;
  }
  else
  { // hard mode
    tft.setCursor(40, 80);
    tft.fillRect(25, 80, 10, 8, ST7735_BLACK);
    tft.print("Easy");
    tft.setCursor(40, 100);
    tft.fillRect(25, 100, 10, 8, ST7735_BLACK);
    tft.print("Medium");
    tft.setCursor(40, 120);
    tft.fillRect(25, 120, 10, 8, ST7735_GREEN);
    tft.print("Hard");
    difficulty = 40;
  }

  if (titleButton == BUTTON_SELECT)
  {
    showTitle = false;
    return;
  }

  delay(100);
  titleSelection();
}
*/

void drawBoard() {
  tft.drawRect(0, 9, tft.width(), tft.height() - 9, ST7735_WHITE);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(3, 0);
  tft.print("RECORD");
}

void updateScore(int score) {
  tft.fillRect(38, 0, tft.width() - 75, 9, ST7735_BLACK);
  tft.setTextSize(1);
  tft.setCursor(45, 0);
  tft.setTextColor(ST7735_WHITE);
  tft.print(score);
}

void updateSnakePosition(uint8_t buttonPressed) {
  if (buttonPressed == BUTTON_UP)
  {
    if (currentDirection != BUTTON_DOWN)
    {
      pixelLocationYAxis -= 3;
      currentDirection = BUTTON_UP;
    }
    else
    {
      pixelLocationYAxis += 3;
    }
  }
  
  if (buttonPressed == BUTTON_DOWN)
  {
    if (currentDirection != BUTTON_UP)
    {
      pixelLocationYAxis += 3;
      currentDirection = BUTTON_DOWN;
    }
    else
    {
      pixelLocationYAxis -= 3;
    }
  }
  
  if (buttonPressed == BUTTON_LEFT)
  {
    if (currentDirection != BUTTON_RIGHT)
    {
      pixelLocationXAxis -= 3;
      currentDirection = BUTTON_LEFT;
    }
    else
    {
      pixelLocationXAxis += 3;
    }
  }
  
  if (buttonPressed == BUTTON_RIGHT)
  {
    if (currentDirection != BUTTON_LEFT)
    {
      pixelLocationXAxis += 3;
      currentDirection = BUTTON_RIGHT;
    }
    else
    {
      pixelLocationXAxis -= 3;
    }
  }

  if (pixelLocationYAxis < 10)
  {
    pixelLocationYAxis = 10;
  }
  
  if (pixelLocationYAxis > tft.height() - 3)
  {
    pixelLocationYAxis = tft.height() - 3;
  }
  
  if (pixelLocationXAxis < 1)
  {
    pixelLocationXAxis = 1;
  }
  
  if (pixelLocationXAxis > tft.width() - 3)
  {
    pixelLocationXAxis = tft.width() - 3;
  }

  drawSnake(pixelLocationXAxis, pixelLocationYAxis, pixelColor);
  x[head] = pixelLocationXAxis;
  y[head] = pixelLocationYAxis;
  collisionCheck();
  head++;
  
  if (head >= 300)
  {
    removeLastFromTail();
    resetArray();
    head = snakeSize + 1;
  }
}

void resetArray()
{
  for (int j = 1; j < snakeSize; j++)
  {
    x[snakeSize - j] = x[head - j];
    x[head - j] = 0;
    y[snakeSize - j] = y[head - j];
    y[head - j] = 0;
  }
  
  x[0] = 0;
  y[0] = 0;
}

void drawSnake(int pixelLocationXAxis, int pixelLocationYAxis, uint16_t pixelColor)
{
  tft.drawPixel(pixelLocationXAxis - 1, pixelLocationYAxis - 1, pixelColor);  //top left
  tft.drawPixel(pixelLocationXAxis, pixelLocationYAxis - 1, pixelColor);      //top
  tft.drawPixel(pixelLocationXAxis + 1, pixelLocationYAxis - 1, pixelColor);  //top right
  tft.drawPixel(pixelLocationXAxis - 1, pixelLocationYAxis, pixelColor);      //left
  tft.drawPixel(pixelLocationXAxis, pixelLocationYAxis, pixelColor);          //middle
  tft.drawPixel(pixelLocationXAxis + 1, pixelLocationYAxis, pixelColor);      //right
  tft.drawPixel(pixelLocationXAxis - 1, pixelLocationYAxis + 1, pixelColor);  //bottom left
  tft.drawPixel(pixelLocationXAxis, pixelLocationYAxis + 1, pixelColor);      //bottom
  tft.drawPixel(pixelLocationXAxis + 1, pixelLocationYAxis + 1, pixelColor);  //bottom right
}

void removeLastFromTail()
{
  drawSnake(x[head - snakeSize], y[head - snakeSize], ST7735_BLACK);
  x[head - snakeSize] = 0;
  y[head - snakeSize] = 0;
}

void collisionCheck()
{
  if (x[head] < 2 || x[head] > tft.width() - 2 || y[head] <= 11 || y[head] >= tft.height())
  {
    pixelColor = ST7735_RED;
    collision = true;
    drawSnake(pixelLocationXAxis, pixelLocationYAxis, ST7735_RED);
    displayEnd = true;
  }

  for (int z = 0; z < head; z++)
  {
    if ((x[head] == x[z]) && (y[head] == y[z]))
    {
      pixelColor = ST7735_RED;
      collision = true;
      drawSnake(pixelLocationXAxis, pixelLocationYAxis, ST7735_RED);
      displayEnd = true;
    }
  }
}

void appleLogic()
{
  if (!appleOn)
  {
    appleX = TrueRandom.random(3, tft.width() - 6);
    appleY = 10;
    yMultiplier = TrueRandom.random(1, 49);

    if (appleX % 3 == 1)
    {
      appleX--;
    }
    else if (appleX % 3 == 2)
    {
      appleX++;
    }
    
    appleY += (3 * yMultiplier);
    drawApple(appleX, appleY, ST7735_GREEN);
    appleOn = true;
  }
}

void checkrecord()
{
  if (score == 50)
  {
    Serial.print("stop");
    tft.fillRect(0, 0, tft.width(), tft.height(), ST7735_BLACK);
    exit(0);
  }
}

void checkIfAppleGot()
{
  if ((x[head - 1] == appleX))
  {
    if ((y[head - 1] == appleY))
    {
      appleOn = false;
      snakeSize += 2;
      score += 10;
      
      updateScore(score);
      allowSpecial = true;
    }
  }
  else
  {
    drawApple(appleX, appleY, ST7735_GREEN);
  }
}

void specialApple()
{
  if (!specialOn && (score % 100 == 0) && allowSpecial)
  {
    specialX = TrueRandom.random(2, tft.width() - 5);
    specialY = 10;
    yMultiplier = TrueRandom.random(1, 49);

    if (specialX % 3 == 1)
    {
      specialX--;
    }
    else if (specialX % 3 == 2)
    {
      specialX++;
    }
    
    specialY += (3 * yMultiplier);
    drawApple(specialX, specialY, ST7735_RED);
    specialTime = 0;
    specialOn = true;
    allowSpecial = false;
  }
}

void checkIfSpecialGot()
{
  if (specialOn)
  {
    if ((x[head - 1] == specialX))
    {
      if ((y[head - 1] == specialY))
      {
        specialSelection();
      }
    }
    else
    {
      drawApple(specialX, specialY, ST7735_RED);
    }
  }
}

void specialSelection()
{
  int removeSize = TrueRandom.random(1, 5);
  int addedScore = TrueRandom.random(1, 11) * 100;
  snakeSize -= removeSize;

  for (int i = 0; i < removeSize; i++)
  {
    drawSnake(x[head - snakeSize - i], y[head - snakeSize - i], ST7735_BLACK);
    x[head - snakeSize - i] = 0;
    y[head - snakeSize - i] = 0;
  }

  score += addedScore;
  updateScore(score);
  specialOn = false;
}

void specialTimer()
{
  specialTime++;
  if (specialTime > 150)
  {
    drawApple(specialX, specialY, ST7735_BLACK);
    specialOn = false;
    allowSpecial == false;
    specialTime = 0;
  }
}

void drawApple(int pixelLocationXAxis, int pixelLocationYAxis, uint16_t pixelColor)
{
  tft.drawPixel(pixelLocationXAxis, pixelLocationYAxis - 1, pixelColor); //top
  tft.drawPixel(pixelLocationXAxis - 1, pixelLocationYAxis, pixelColor); //left
  tft.drawPixel(pixelLocationXAxis, pixelLocationYAxis, pixelColor); //middle
  tft.drawPixel(pixelLocationXAxis + 1, pixelLocationYAxis, pixelColor); //right
  tft.drawPixel(pixelLocationXAxis, pixelLocationYAxis + 1, pixelColor); //bottom
}

void displayEndingScreen()
{
  tft.fillRect(25, 65, tft.width() - 50, 30, ST7735_BLUE);
  tft.setTextSize(3);
  tft.setCursor(35, 70);
  tft.setTextColor(ST7735_WHITE);
  tft.print(score);
  displayEnd = false;
}

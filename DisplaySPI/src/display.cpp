/*
 * pong.cpp
 *
 *  Created on: 23/12/2018
 *      Author: Pedro Teixeira
 */

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <string.h>

#define OLED_CLK   13
#define OLED_MOSI  11
#define OLED_CS    10
#define OLED_DC    9
#define OLED_RESET 8
#define BAR_HEIGHT 17
#define BAR_WIDTH 2
#define EDGE_LEFT 1
#define EDGE_TOP 1
#define EDGE_RIGHT 127
#define EDGE_BOT 63
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
#define DRAG 3
#define SIZE_OF_TIC_TAC_TOE 5
#define TIC_TAC_TOE_DELAY 5
#define MAX_PLAYS 5
#define LCD_HEIGHT 64
#define LCD_WIDTH 128
#define X1in A0
#define Y1in A1
#define X2in A6
#define Y2in A7
#define BT1	7
#define BT2	5
#define X1_INI 69
#define X2_INI 69
#define Y1_INI 10
#define Y2_INI 56

static const unsigned char  PROGMEM draw_image[] = {
		B11111111,B11111111,B11111111,B11111111,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00011011,B00000001,
		B10000000,B00000000,B00010110,B00000001,
		B10000000,B00000000,B00111100,B00000001,
		B10000000,B00000111,B00100100,B00000001,
		B10000000,B00000101,B10100100,B00000001,
		B10000000,B00001100,B11100100,B00000001,
		B10000000,B00111000,B00110100,B00000001,
		B10000000,B01100000,B00011100,B00000001,
		B10000000,B11000000,B00001100,B00000001,
		B10000000,B10000000,B00000110,B00000001,
		B10000001,B10000000,B00000011,B00000001,
		B10000011,B11111111,B11111111,B00000001,
		B10000001,B00000000,B00000010,B00000001,
		B10000001,B01111000,B00000010,B00000001,
		B10000001,B01001000,B00000010,B00000001,
		B10000001,B01001000,B00000010,B00000001,
		B10000001,B01111011,B11110010,B00000001,
		B10000001,B00000010,B00010010,B00000001,
		B10000001,B00000010,B00010010,B00000001,
		B10000001,B00000010,B00010010,B00000001,
		B10000001,B00000010,B00010010,B00000001,
		B10000001,B00000010,B00010010,B00000001,
		B10000001,B00000010,B00010010,B00000001,
		B10000001,B00000010,B01010010,B00000001,
		B10000001,B00000010,B00010010,B00000001,
		B10000001,B00000010,B00010010,B00000001,
		B10000001,B00000010,B00010010,B00000001,
		B11111111,B11111111,B11111111,B11111111
};
static const unsigned char  PROGMEM write_image[] = {
		B11111111,B11111111,B11111111,B11111111,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000111,B00000000,B00000001,
		B10000000,B00011101,B00000000,B00000001,
		B10000000,B00010000,B10000000,B00000001,
		B10000000,B00001000,B10000000,B00000001,
		B10000000,B00001000,B10000000,B00000001,
		B10000000,B00001000,B01000000,B00000001,
		B10000000,B00000100,B01000000,B00000001,
		B10000000,B00000100,B01000000,B00000001,
		B10000000,B00000100,B00100000,B00000001,
		B10000000,B00000010,B00100000,B00000001,
		B10000000,B00000010,B00010000,B00000001,
		B10000000,B00000001,B00010000,B00000001,
		B10000000,B00000001,B00010000,B00000001,
		B10000000,B00000001,B00001000,B00000001,
		B10000000,B00000000,B10001000,B00000001,
		B10000000,B00000000,B10001000,B00000001,
		B10000000,B00000000,B10000100,B00000001,
		B10000000,B00000000,B01000100,B00000001,
		B10000000,B00000000,B01100100,B00000001,
		B10000000,B00000000,B00100100,B00000001,
		B10000000,B00000000,B00111100,B00000001,
		B10000000,B00000000,B00011100,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B11111111,B11111111,B11111111,B11111111
};
static const unsigned char  PROGMEM tic_tac_toe_image[] =
{
		B11111111,B11111111,B11111111,B11111111,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00100000,B00000010,B00000001,
		B10000000,B00100000,B00000010,B00000001,
		B10000000,B00100000,B00000010,B00000001,
		B10000000,B00100000,B00000010,B00000001,
		B10000000,B00100000,B00000010,B00000001,
		B10001111,B11111111,B11111111,B11110001,
		B10000000,B00100000,B00000010,B00000001,
		B10000000,B00100000,B00000010,B00000001,
		B10000000,B00100100,B00010010,B00000001,
		B10000000,B00100010,B00100010,B00000001,
		B10000000,B00100001,B01000010,B00000001,
		B10000000,B00100000,B10000010,B00000001,
		B10000000,B00100001,B01000010,B00000001,
		B10000000,B00100010,B00100010,B00000001,
		B10000000,B00100100,B00010010,B00000001,
		B10000000,B00100000,B00000010,B00000001,
		B10000000,B00100000,B00000010,B00000001,
		B10001111,B11111111,B11111111,B11110001,
		B10000000,B00100000,B00000010,B00000001,
		B10000000,B00100000,B00000010,B00000001,
		B10000000,B00100000,B00000010,B00000001,
		B10000000,B00100000,B00000010,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B11111111,B11111111,B11111111,B11111111
};
static const unsigned char  PROGMEM pong_image[] = {
  B11111111, B11111111, B11111111, B11111111,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10011000, B00000001, B10000000, B00000001,
  B10011000, B00000011, B11000000, B00000001,
  B10011000, B00000011, B11000000, B00000001,
  B10011000, B00000001, B10000000, B00000001,
  B10011000, B00000000, B00000000, B00000001,
  B10011000, B00000000, B00000000, B00000001,
  B10011000, B00000000, B00000000, B00000001,
  B10011000, B00000000, B00000000, B00011001,
  B10011000, B00000000, B00000000, B00011001,
  B10011000, B00000000, B00000000, B00011001,
  B10011000, B00000000, B00000000, B00011001,
  B10000000, B00000000, B00000000, B00011001,
  B10000000, B00000000, B00000000, B00011001,
  B10000000, B00000000, B00000000, B00011001,
  B10000000, B00000000, B00000000, B00011001,
  B10000000, B00000000, B00000000, B00011001,
  B10000000, B00000000, B00000000, B00011001,
  B10000000, B00000000, B00000000, B00011001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B11111111, B11111111, B11111111, B11111111
};
static const unsigned char  PROGMEM read_image[] =
{
		B11111111,B11111111,B11111111,B11111111,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10001111,B10000000,B00000011,B11110001,
		B10001000,B11111000,B00111110,B00010001,
		B10001000,B00001111,B11100000,B00010001,
		B10001000,B00000001,B00000000,B00010001,
		B10001000,B00000001,B00000000,B00010001,
		B10001000,B00000001,B00000000,B00010001,
		B10001001,B00000001,B00001111,B11010001,
		B10001001,B11111001,B00000000,B00010001,
		B10001000,B00000001,B00000000,B00010001,
		B10001000,B00000001,B00001100,B01010001,
		B10001001,B11111001,B00000111,B11010001,
		B10001000,B00000001,B00000000,B00010001,
		B10001000,B00000001,B00000000,B00010001,
		B10001000,B00000000,B10000000,B00010001,
		B10001000,B00000000,B10000000,B00010001,
		B10001000,B00001111,B11100000,B00010001,
		B10001000,B11111000,B00111111,B00010001,
		B10001111,B10000000,B00000001,B11110001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B10000000,B00000000,B00000000,B00000001,
		B11111111,B11111111,B11111111,B11111111
};

/////////////////////////////////////////////////////////////
char str1[141]= "teste 1\r\n";
char str2[141]=	"teste 2\r\n";

Adafruit_SSD1306 display(OLED_MOSI,OLED_CLK , OLED_DC, OLED_RESET, OLED_CS);

int state = 0;
bool RE = false;
////////////////////////////////////////////////////////////
bool is_RGT1_pressed()
{
	int X1;
	X1 = analogRead (X1in);
	if(X1<=1023&&X1>700) return 1;
	else return 0;
}
bool is_LFT1_pressed()
{
	int X1;
	X1 = analogRead (X1in);
	if(X1<300&&X1>=0) return 1;
	else return 0;
}
bool is_DWN1_pressed()
{
	int Y1;
	Y1 = analogRead (Y1in);
	if(Y1<300&&Y1>=0) return 1;
	else return 0;
}
bool is_UP1_pressed()
{
	int Y1;
	Y1 = analogRead (Y1in);
	if(Y1<=1023&&Y1>700) return 1;
	else return 0;
}
/////////////////////////////////////////////////////////
bool is_RGT2_pressed()
{
	int X2;
	X2 = analogRead (X2in);
	if(X2<=1023&&X2>540) return 1;
	else return 0;
}
bool is_LFT2_pressed()
{
	int X2;
	X2 = analogRead (X2in);
	if(X2<480&&X2>=0) return 1;
	else return 0;
}
bool is_DWN2_pressed()
{
	int Y2;
	Y2 = analogRead (Y2in);
	if(Y2<480&&Y2>=0) return 1;
	else return 0;
}
bool is_UP2_pressed()
{
	int Y2;
	Y2 = analogRead (Y2in);
	if(Y2<=1023&&Y2>540) return 1;
	else return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////77
  void indicator(int x, int y, int r)                               // cursor usado no demo e Draw
      {

			display.fillRect(x-r-1, y-r-1, 2*r+3, 2*r+3, 2);
			display.fillRect(x-r, y-r, 2*r+1, 2*r+1, 2);
			display.drawPixel(x-r-1,y-r-1,2);
			display.drawPixel(x-r-1,y+r+1,2);
			display.drawPixel(x+r+1,y-r-1,2);
			display.drawPixel(x+r+1,y+r+1,2);

      }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////7777
  void indicator_x(int x, int y, int size,int colour)                               // cursor usado no demo e Draw
        {

  			display.drawLine(x-size-1, y-size-1, x+size+1,y+size+1,colour);  //1st diagonal
  			display.drawLine(x-size-1, y-size, x+size,y+size+1,colour);     //down contorn
  			display.drawLine(x-size, y-size-1, x+size+1,y+size,colour);		//upper contorn
  			display.drawLine(x-size-1, y-size-1, x+size+1,y+size+1,colour); //inverted 1st diagonal
  		/////////////////////////////////////////////////////////////////////////////////////
  			display.drawLine(x-size-1, y+size+1, x+size+1,y-size-1,colour);//2nd Diagonal
  			display.drawLine(x-size-1, y+size, x+size,y-size-1,colour);     //down contorn
			display.drawLine(x-size, y+size+1, x+size+1,y-size,colour);		//upper contorn
			display.drawLine(x-size-1, y+size+1, x+size+1,y-size-1,colour); //inverted 2nd diagonal


        }
  ///////////////////////////////////////////////////////////////////////////////////////////////////7777777
  void indicator_o(int x, int y, int r,int colour)
                            // cursor usado no demo e Draw
          {
	  	  	  display.drawCircle(x, y, r+1,colour);
	  	  	  display.drawCircle(x, y, r,colour);
          }
  ///////////////////////////////////////////////////////////////////////////////
  void tic_tac_toe(int r)

  { ///////////////////////////R - RADIUS
	  int  dtime=TIC_TAC_TOE_DELAY, x1,x2,y2,y1,nplays1=0,nplays2=0;
	  bool REaux2=false, REaux1=false, win=false, draw=false;
	  bool plays1[3][3]={false};
	  bool plays2[3][3]={false};

	  display.clearDisplay();

	  x1=X1_INI;x2=X2_INI;y1=Y1_INI;y2=Y2_INI;
	  indicator_o(x1,y1,r,1);
	  indicator_x(x2,y2,r,1);
	  display.display();
	  while(digitalRead(BT1)||digitalRead(BT2))
	  {
		  if(nplays1==nplays2||win||draw)
		  {
				  if(is_LFT1_pressed() && x1-1>0) //LEFT 1
					{
					   delay(dtime);
					   indicator_o(x1,y1,r,2);
					   indicator_o(x1-1,y1,r,2);
					   x1--;
					 }
					 if(is_RGT1_pressed() && x1+1<127) //RIGHT 1
					 {
					   delay(dtime);
					   indicator_o(x1,y1,r,2);
					   indicator_o(x1+1,y1,r,2);
					   x1++;
					 }

					 if(is_UP1_pressed() && y1-1>0) //UP 1
					 {
					   delay(dtime);
					   indicator_o(x1,y1,r,2);
					   indicator_o(x1,y1-1,r,2);
					   y1--;
					 }
					 if(is_DWN1_pressed() && y1+1<63) //DOWN 1
					 {
					   delay(dtime);
					   indicator_o(x1,y1,r,2);
					   indicator_o(x1,y1+1,r,2);
					   y1++;
					 }
		    }
		         /////////////////////////////////////////////////////////////////
		  	 if(nplays1>nplays2||win||draw)
		  	 {
					 if(is_LFT2_pressed() && x2-1>0) //LEFT 2
					 {
					   delay(dtime);
					   indicator_x(x2,y2,r,2);
					   indicator_x(x2-1,y2,r,2);
					   x2--;
					 }
					 if(is_RGT2_pressed() && x2+1<127) //RIGHT 2
					 {
					   delay(dtime);
					   indicator_x(x2,y2,r,2);
					   indicator_x(x2+1,y2,r,2);
					   x2++;
					 }

					 if(is_UP2_pressed() && y2-1>0) //UP 2
					 {
					   delay(dtime);
					   indicator_x(x2,y2,r,2);
					   indicator_x(x2,y2-1,r,2);
					   y2--;
					 }
					 if(is_DWN2_pressed() && y2+1<63) //DOWN 2
					 {
					   delay(dtime);
					   indicator_x(x2,y2,r,2);
					   indicator_x(x2,y2+1,r,2);
					   y2++;
					 }
		  	 }
		  	 /////////////////////////////////////////////////////////////////////////////////////////////////
					if(digitalRead(BT1) && !is_DWN1_pressed()&& !is_UP1_pressed() && !is_RGT1_pressed()&& !is_LFT1_pressed())//check RE1
						REaux1=true;
					if(digitalRead(BT2) && !is_DWN1_pressed()&& !is_UP1_pressed() && !is_RGT1_pressed()&& !is_LFT1_pressed())
						REaux2=true;   //check RE2
					if(!digitalRead(BT1)&&x1>74&&y1<30&&REaux1)			//rematch
					{
						tic_tac_toe(SIZE_OF_TIC_TAC_TOE);
						return;
						REaux1=false;
					}
					if(!digitalRead(BT2)&&x2>74&&y2<30&&REaux2)
					{

						tic_tac_toe(SIZE_OF_TIC_TAC_TOE);
						return;
						REaux1=false;
					}
					if(!digitalRead(BT1)&&x1>74&&y1>34&&REaux1)
					{
						RE=false;                                     //leave
						state = 0;
						return;
					}
					if(!digitalRead(BT2)&&x2>74&&y2>34&&REaux2)
					{
						RE=false;
						state = 0;
						return;
					}
					if(!digitalRead(BT1) && !is_DWN1_pressed()&& !is_UP1_pressed() && !is_RGT1_pressed()&& !is_LFT1_pressed()&&REaux1)
					{
						REaux1=false;

						///check hitbox
						if(x1>0&&x1<22)
						{
							if(y1>0&&y1<22&&!plays2[0][0]&&!plays1[0][0])
							{
								plays1[0][0]=true;
								nplays1++;
							}
							else if(y1>22&&y1<44&&!plays2[0][1]&&!plays1[0][1])
							{
								plays1[0][1]=true;
								nplays1++;
							}
							else if(y1>44&&y1<64&&!plays2[0][2]&&!plays1[0][2])
							{
								plays1[0][2]=true;
								nplays1++;
							}

						}
						else if(x1>22&&x1<44)
							{
								if(y1>0&&y1<22&&!plays2[1][0]&&!plays1[1][0])
								{
									plays1[1][0]=true;
									nplays1++;
								}
								else if(y1>22&&y1<44&&!plays2[1][1]&&!plays1[1][1])
								{
									plays1[1][1]=true;
									nplays1++;
								}
								else if(y1>44&&y1<64&&!plays2[1][2]&&!plays1[1][2])
								{
									plays1[1][2]=true;
									nplays1++;
								}

							}
						else if(x1>44&&x1<64)
							{
								if(y1>0&&y1<22&&!plays2[2][0]&&!plays1[2][0])
								{
									plays1[2][0]=true;
									nplays1++;
								}
								else if(y1>22&&y1<44&&!plays2[2][1]&&!plays1[2][1])
								{
									plays1[2][1]=true;
									nplays1++;
								}
								else if(y1>44&&y1<64&&!plays2[2][2]&&!plays1[2][2])
								{
									plays1[2][2]=true;
									nplays1++;
								}

							}

							x1 = X1_INI;
							y1= Y1_INI;  //reset position
					}


					if(!digitalRead(BT2) && !is_DWN1_pressed()&& !is_UP1_pressed() && !is_RGT1_pressed()&& !is_LFT1_pressed()&&REaux2)
					{
						REaux2=false;

						///check hitbox
						if(x2>0&&x2<22)
						{
							if(y2>0 && y2<22 &&!plays1[0][0]&&!plays2[0][0])
							{
								plays2[0][0]=true;
								nplays2++;
							}
							else if(y2>22&&y2<44&&!plays1[0][1]&&!plays2[0][1])
							{
								plays2[0][1]=true;
								nplays2++;
							}
							else if(y2>44&&y2<64&&!plays1[0][2]&&!plays2[0][2])
							{
								plays2[0][2]=true;
								nplays2++;
							}

						}
						else if(x2>22&&x2<44)
							{
								if(y2>0&&y2<22&&!plays1[1][0]&&!plays2[1][0])
								{
									plays2[1][0]=true;
									nplays2++;
								}
								else if(y2>22&&y2<44&&!plays1[1][1]&&!plays2[1][1])
								{
									plays2[1][1]=true;
									nplays2++;
								}
								else if(y2>44&&y2<64&&!plays1[1][2]&&!plays2[1][2])
								{
									plays2[1][2]=true;
									nplays2++;
								}

							}
						else if(x2>44&&x2<64)
							{
								if(y2>0&&y2<22&&!plays1[2][0]&&!plays2[2][0])
								{
									plays2[2][0]=true;
									nplays2++;
								}
								else if(y2>22&&y2<44&&!plays1[2][1]&&!plays2[2][1])
								{
									plays2[2][1]=true;
									nplays2++;
								}
								else if(y2>44&&y2<64&&!plays1[2][2]&&!plays2[2][2])
								{
									plays2[2][2]=true;
									nplays2++;
								}

							}

						x2 = X2_INI; //reset position
						y2= Y2_INI;
					}
					indicator_o(X1_INI,Y1_INI,r,1);
					indicator_x(X2_INI,Y2_INI,r,1);

					if( (plays1[0][0]&&plays1[1][0]&&plays1[2][0])||
						(plays1[0][1]&&plays1[1][1]&&plays1[2][1])||	//HORIZONTAL WIN
						(plays1[0][2]&&plays1[1][2]&&plays1[2][2])||
						(plays1[0][0]&&plays1[0][1]&&plays1[0][2])||
						(plays1[1][0]&&plays1[1][1]&&plays1[1][2])||	//VERTICAL WIN
						(plays1[2][0]&&plays1[2][1]&&plays1[2][2])||
						(plays1[0][0]&&plays1[1][1]&&plays1[2][2])|| 	//DIAGONAL WIN
						(plays1[2][0]&&plays1[1][1]&&plays1[0][2]))
					{
						win=true;
						display.setCursor(0,26);
						display.setTextColor(BLACK,WHITE);
						display.print("  Player 1 ");
						display.setCursor(0,34);
						display.print("    WIN    ");
					}

					if( (plays2[0][0]&&plays2[1][0]&&plays2[2][0])||
						(plays2[0][1]&&plays2[1][1]&&plays2[2][1])||	//HORIZONTAL WIN
						(plays2[0][2]&&plays2[1][2]&&plays2[2][2])||
						(plays2[0][0]&&plays2[0][1]&&plays2[0][2])||
						(plays2[1][0]&&plays2[1][1]&&plays2[1][2])||	//VERTICAL WIN
						(plays2[2][0]&&plays2[2][1]&&plays2[2][2])||
						(plays2[0][0]&&plays2[1][1]&&plays2[2][2])|| 	//DIAGONAL WIN
						(plays2[2][0]&&plays2[1][1]&&plays2[0][2]))
					{
						win=true;
						display.setCursor(0,26);
						display.setTextColor(BLACK,WHITE);
						display.print("  Player 2 ");
						display.setCursor(0,34);
						display.print("    WIN    ");
					}
					if(nplays1+nplays2==9)
					{
						draw=true;
						display.setCursor(0,30);
						display.setTextColor(BLACK,WHITE);
						display.print("    DRAW  ");
					}

					display.fillRect(0,22,display.height(),2,1);
					display.fillRect(0,44,display.height(),2,1);
					display.fillRect(44,0,2,display.height(),1);
					display.fillRect(22,0,2,display.height(),1);
					display.drawRect(78,4,46,24,1);
					display.drawRect(78,38,46,24,1);
					display.setCursor(80,16);
					display.setTextColor(WHITE);
					display.print("Restart");
					display.setCursor(80,48);
					display.print(" Leave");
					display.display();
			}
  }
////////////////////////////////////////////////////////////////////////////////////////////////
  void write(int message)
  {

  		int x=0, y=8, letters=0, lines=0, i=0;
  		char ch;
  		char str[141];
	    display.clearDisplay();
	    display.setTextSize(1);
	    display.setTextColor(BLACK,WHITE);
        display.setCursor(0,0);
	    display.print(" >>Message number");
	    display.print(message);
	    display.println("<< ");
	    Serial.println("-Write Mode-");
	    Serial.print(">>Message number");
	    Serial.print(message);
	    Serial.println("<<");
	    display.setTextColor(WHITE);
	    display.display();
	while(!(is_UP1_pressed()||is_UP2_pressed()))
	{
  		if(lines<8)
  		{

  			if (Serial.available())
  			{
  				ch = Serial.read();

  				if (ch >= ' '|| ch == 13)  //13 enter
  				{
  					if(ch==127)
  					{
  						if(letters>0)
  						{
  							letters--;
  							x-=6;
  							Serial.print(ch);
  							display.setCursor(x,y);
  							display.fillRect(x,y,6,8,0);
  							str[i--]='\0';
  						}
  						if(letters==0&&lines>0)
  						{
  							letters=20;
  							y-=8;
  							x= 20*6;
  							Serial.print(ch);
  							display.setCursor(x,y);
  							display.fillRect(x,y,6,8,0);
  							str[i--]='\0';

  						}
  					}
  					else
  					{
  						if(letters==21)
  						{
  							letters=0;
  							lines++;
  							y+=8;
  							x=0;
  							display.setCursor(x,y);
  						}
  						if( ch==13 )
  						{
  							letters=0;
  							lines++;
  							y+=8;
  							x=0;
  							str[i]='\r';
  							i++;
  							str[i]='\n';
  							i++;
  							display.setCursor(x,y);
  							Serial.print("\r\n");
  						}
  						else
  						{
  							display.setCursor(x,y);
  							Serial.print(ch);
  							display.print(ch);
  							str[i]=ch;
  							x+=6;
  							letters++;
  							i++;
  						}
  					}
  				}
  				display.display();
  			}
  		}
	if(lines==8)
		break;
	}
	while(1)
	{
		while(digitalRead(BT1)&&digitalRead(BT2))
		{
			str[i]='\r';
			str[i+1]='\n';
			str[i+2]='\0';
			display.clearDisplay();
			display.setCursor(0,10);
			display.setTextSize(1);
			display.setTextColor(BLACK,WHITE);
			display.print("  >>Save text ?<<   ");
			display.setCursor(0,30);
			display.setTextColor(WHITE);
			display.print(">Joystick 1 - Yes");
			display.setCursor(0,40);
			display.print(">Joystick 2 - No ");
			display.display();

		while(!digitalRead(BT1))
			{
				switch (message)
				{
				case 1:
					strcpy(str1,str);
					break;
				case 2:
					strcpy(str2,str);
					break;
				}
				Serial.print("\r\n");
				state=0;
				RE=false;
				return;
			}
		while(!digitalRead(BT2))
			{
				Serial.print("\r\n");
				state=0;
				RE=false;
				return;

			}
		}
  	}
  }
  //////////////////////////////////////////////////////////////////////////////////////////
  void read(int mem)
  {

	char  str[141];
	switch (mem)
	{

		case 1:
			strcpy(str,str1);
			break;
		case 2:
			strcpy(str,str2);
			break;
	}
  	    display.clearDisplay();
  	    display.setTextSize(1);
  	    display.setTextColor(BLACK,WHITE);
        display.setCursor(0,0);
        display.print(" >>Message number");
		display.print(mem);
		display.println("<< ");
		Serial.println("-Read Mode-");
		Serial.print(">>Message number");
		Serial.print(mem);
		Serial.println("<<");
  	    display.display();
  	    display.setCursor(0,8);
  	    display.setTextColor(WHITE);
  	    if(strlen(str)==0)
  	    {
  	    	strcpy(str,"(Empty message)\r\n");
  	    }
  	    display.print(str);
  		Serial.print(str);
  		display.display();
  		delay(1000);
		while(!(is_UP1_pressed()||is_UP2_pressed()))
		{
			 display.fillRect(0,0, display.width(), 8, 0);
			 display.setCursor(0,0);
			 display.setTextColor(BLACK,WHITE);
			 display.print(" >>Up to  go back<<  ");
			 display.setTextColor(WHITE);
			 display.display();
		}
		if((is_UP1_pressed()||is_UP2_pressed()))
		{
			while(digitalRead(BT1)&&digitalRead(BT2))
			{
			display.clearDisplay();
			display.setCursor(0,10);
			display.setTextSize(1);
			display.setTextColor(BLACK,WHITE);
			display.print("  >>Keep/Delete<<   ");
			display.setCursor(0,30);
			display.setTextColor(WHITE);
			display.print("> Joystick 1 - keep");
			display.setCursor(0,40);
			display.print("> Joystick 2 - delete ");
			display.display();
			}
			while(!digitalRead(BT1))
				{
					state=0;
					RE=false;
					return;
				}
			while(!digitalRead(BT2))
			{
				state=0;
				RE=false;
				switch (mem)
				{
					case 1:
						str1[0]= '\0';
						break;
					case 2:
						str2[0]= '\0';
						break;
				}
				Serial.print("\r\n");
				return;
			}
    	}
    }
 ///////////////////////////////////////////////////////////////////////////////////////////////
  void draw()
      {
        int xind = 64, yind = 32, dtime=10;  // r- radius of the cursor
        bool REaux=false;
        bool REaux2=false;
        int r =0;
        display.clearDisplay();
        indicator(xind,yind,r);

        while((digitalRead(BT1)||digitalRead(BT2)))
        {
        	 //Serial.println(dtime);
			if(!is_UP2_pressed()&&!is_DWN2_pressed()&&!is_RGT2_pressed()&&!is_LFT2_pressed())
			{
				REaux=true;
			}
			if(is_UP2_pressed() && dtime>0 && REaux)
			{
				dtime-=5;
				REaux=false;													//select velocity
			}
			if(is_DWN2_pressed() && dtime<100 && REaux)
			{
				dtime+=5;
				REaux=false;
			}
			if(is_LFT2_pressed() && r>0 && REaux)
			{
				display.fillRect(xind-r-1, yind-r-1, 2*r+3, 2*r+3, 0);             //clean earlier indicator
				r--;
				REaux=false;													// select radius
				indicator(xind,yind,r);
			}
			if(is_RGT2_pressed() && r<30 && REaux)
			{
				display.fillRect(xind-r-1, yind-r-1, 2*r+3, 2*r+3, 0);
				r++;
				REaux=false;
				indicator(xind,yind,r);
			}
        /////////////////////////////////////////////////////////////////
				if(is_LFT1_pressed() && xind-1>0) //LEFT
				{
				  delay(dtime);
				  indicator(xind,yind,r);
				  indicator(xind-1,yind,r);
				  xind--;
				  if(!digitalRead(BT2))
					  display.fillRect(xind-r+1, yind-r, 2*r+1, 2*r+1, 0);
				  if(!digitalRead(BT1))
					  display.fillRect(xind-r+1, yind-r, 2*r+1, 2*r+1, 1);
				}

				if(is_RGT1_pressed() && xind+1<127) //RIGHT
				{
				  delay(dtime);
				  indicator(xind,yind,r);
				  indicator(xind+1,yind,r);
				  xind++;
				  if( !digitalRead(BT2))
					  display.fillRect(xind-r-1, yind-r, 2*r+1, 2*r+1, 0);
				  if( !digitalRead(BT1))
					  display.fillRect(xind-r-1, yind-r, 2*r+1, 2*r+1, 1);
				}

				if(is_UP1_pressed() && yind-1>0) //UP
				{
				  delay(dtime);
				  indicator(xind,yind,r);
				  indicator(xind,yind-1,r);
				  yind--;
				  if( !digitalRead(BT2))
					  display.fillRect(xind-r, yind-r+1, 2*r+1, 2*r+1, 0);
				  if( !digitalRead(BT1))
					  display.fillRect(xind-r, yind-r+1, 2*r+1, 2*r+1, 1);
				}
				if(is_DWN1_pressed() && yind+1<63) //DOWN
				{
				  delay(dtime);
				  indicator(xind,yind,r);
				  indicator(xind,yind+1,r);
				  yind++;
				  if(!digitalRead(BT2))
					  display.fillRect(xind-r, yind-r-1, 2*r+1, 2*r+1, 0);
				  if(!digitalRead(BT1))
					  display.fillRect(xind-r, yind-r-1, 2*r+1, 2*r+1, 1);
				}

				if( !digitalRead(BT1)&&!digitalRead(BT2))
					{
						RE = false;
						state= 0;
						return;
					}
				if(digitalRead(BT2) && !is_DWN1_pressed()&& !is_UP1_pressed() && !is_RGT1_pressed()&& !is_LFT1_pressed())
					REaux2=true;
				if(!digitalRead(BT2) && !is_DWN1_pressed()&& !is_UP1_pressed() && !is_RGT1_pressed()&& !is_LFT1_pressed())
				{																													//write while stopped
					REaux2=false;
					display.fillRect(xind-r, yind-r, 2*r+1, 2*r+1, 1);
				}
				if(digitalRead(BT1) && !is_DWN1_pressed()&& !is_UP1_pressed() && !is_RGT1_pressed()&& !is_LFT1_pressed())
					REaux2=true;
				if(!digitalRead(BT1) && !is_DWN1_pressed()&& !is_UP1_pressed() && !is_RGT1_pressed()&& !is_LFT1_pressed())
				{																											//delete while stopped
					REaux2=false;
					display.fillRect(xind-r, yind-r, 2*r+1, 2*r+1, 0);
				}

				display.display();

       }
       while(1)
        {
    	   while(digitalRead(BT1))
    	   {
    		   draw();
    		   state= 0;
    		   return;
    	   }
    	  while(digitalRead(BT2))
    	  {
			 RE = false;
			 state= 0;
			 return;
    	  }
        }
      }
 //////////////////////////////////////////////////////////////////////////////////////////////
  void pong_1_player()
      {
      	int xl, xr, yl, yr, xball, yball, vx = 1, vy = 1, point = 0;
      	int hit1=0, hit2=0;
        display.clearDisplay();
        display.drawRect(0,0,128,64,1);  			//frame
        xl = EDGE_LEFT+1;	 		    	// draw almost centered rackets  (4)
        yl = 30 - BAR_HEIGHT/2;						 //(xl,yl) top left corner ()
        xr = EDGE_RIGHT-BAR_WIDTH-1;					//(xr,yl) top left corner
        yr = 30 - BAR_HEIGHT/2;
      	display.drawRect(xl,yl,BAR_WIDTH,BAR_HEIGHT,1);
      	display.drawRect(xr,yr,BAR_WIDTH,BAR_HEIGHT,1);                            // ---> vx positive \/\/ vy positive

      	//Draw Ball centered
      	// (xball,yball) top left corner
        yball = ((LCD_HEIGHT/2)-1);
        xball = ((LCD_WIDTH/2)-1);
      	display.setCursor(30,20);
      	display.setTextColor(WHITE);
      	display.setTextSize(1);
      	display.print("Starting in");
      	display.display();
    	delay(1000);
      	display.setCursor(30,28);
      	display.print("   - 3 -     ");
      	display.display();
      	delay(1000);
    	display.fillRect(30, 28, display.width()-60, 8 , 0);
    	display.setCursor(34,28);
    	display.print("  - 2 -     ");
    	display.display();
        delay(1000);
        display.fillRect(30, 28, display.width()-60, 8 , 0);
		display.setCursor(34,28);
		display.print("  - 1 -    ");
		display.display();
		delay(1000);
		display.fillRect(30, 20, display.width()-60, 8 , 0);
		display.fillRect(30, 28, display.width()-60, 8 , 0);
        while(!point)
        {

			if(is_UP1_pressed() && yl > EDGE_TOP)
			{
					display.drawRect(xl,yl-1+BAR_HEIGHT,2,1,0);
					display.drawRect(xl,yl-1,2,1,1);			//move bar up
					yl--;
			}

			if(is_DWN1_pressed() && yl < EDGE_BOT-BAR_HEIGHT)
			{
					display.drawRect(xl,yl,2,1,0);
					display.drawRect(xl,yl+BAR_HEIGHT,2,1,1);	//move bar down
					yl++;
			}

			if(( (vx > 0) && (yr > EDGE_TOP)) && (yball < yr+BAR_HEIGHT/2))
			{
			  display.drawRect(xr,yr-1+BAR_HEIGHT,2,1,0);
			  display.drawRect(xr,yr-1,2,1,1);					//automove bar up
			  yr--;
			}
			if(((vx > 0) && (yr < EDGE_BOT-BAR_HEIGHT)) && (yball > yr+BAR_HEIGHT/2))
			{
			  display.drawRect(xr,yr,2,1,0);
			  display.drawRect(xr,yr+BAR_HEIGHT,2,1,1);	       //automove bar up
			  yr++;
			}


				//colision detection
			if((xball+vx<= xl+2)&& (yball >= yl-1 && yball < yl+BAR_HEIGHT))
			{
				if(yball==(yl+(BAR_HEIGHT/2)))
				{
					vy=-vy;
					vx=(-vx)+1;
				}
				else
				{
				vx=-vx;
				vy= (yball-(yl+(BAR_HEIGHT/2)))/DRAG;
				}
			}
			if((xball+vx>= xr-2) && (yball >= yr-1 && yball < yr+BAR_HEIGHT))
			{
				vx=-vx;
				vy= -vy;

			}
			if(yball+vy < EDGE_TOP+1 || yball+vy > EDGE_BOT-2 )
			{
				vy = -vy;
				//Serial.print(vy);
			}


			//Ball movement
			display.drawRect(xball,yball,2,2,0);
			xball+=vx;
			yball+=vy;
			display.drawRect(xball,yball,2,2,1);


			  display.display();

			  if(xball+vx <= EDGE_LEFT)
				point = -1;
			  if(xball+vx >= EDGE_RIGHT-1)
				point = 1;
			  if(!digitalRead(BT1)&&!digitalRead(BT2))
			  {
				  break;
			  }
		}

        	if(!point)
        	{
			  state = 0;    //emergency exit
			  RE = false;
			  return;
			}
        	for (int i = 0; i<4;i++)
        	{
        		display.fillRect(30, 11, display.width()-60, 15 ,0);
				display.setTextSize(1);
				display.setTextColor(WHITE);
				display.setCursor(34,15);
				display.print("GAME  OVER");
				display.display();
				delay(200);
				display.fillRect(30, 11, display.width()-60, 15 , 1);
				display.setTextSize(1);
				display.setTextColor(BLACK);
				display.setCursor(34,15);
				display.print("GAME  OVER");
				display.display();
				delay(200);

        	}
        	display.setTextColor(WHITE);
			display.setCursor(7,35);

			while(point == 1)
				{
				  display.print("  Winner:Player 1");
				  display.setCursor(0,45);
				  display.print("  Joy 1 to leave");
				  display.setCursor(0,55);
				  display.print("  Joy 2 to rematch");
				  display.display();
				  while(1)
				  {
					  if(!digitalRead(BT1))
					{
					  state = 0;
					  RE = false;
					  return;
					}
					  if(!digitalRead(BT2))
					  {
						  pong_1_player();
					  }
				  }
			    }
			while(point == -1 )
				{
				  display.print("  Winner:Pic_32  ");
				  display.setCursor(0,45);
				  display.print("  Joy 2 to leave");
				  display.setCursor(0,55);
				  display.print("  Joy 1 to rematch");
				  display.display();
				  while(1)
				  {
					  while(!digitalRead(BT2))
					  {
						  state = 0;
						  RE = false;
						  return;
					  }
					  while(!digitalRead(BT1))
						  {
							  pong_1_player();
						  }
				  }
				}

      }
  /////////////////////////////////////////////////////////////////////////////////////////
  void pong_2_player()
        {
			int xl, xr, yl, yr, xball, yball, vx = 1, vy = 1, point = 0;
			int hit1=0, hit2=0;
			display.clearDisplay();
			display.drawRect(0,0,128,64,1);  			//frame
			xl = EDGE_LEFT+1;	 		    	// draw almost centered rackets  (4)
			yl = 30 - BAR_HEIGHT/2;						 //(xl,yl) top left corner ()
			xr = EDGE_RIGHT-BAR_WIDTH-1;					//(xr,yl) top left corner
			yr = 30 - BAR_HEIGHT/2;
			display.drawRect(xl,yl,BAR_WIDTH,BAR_HEIGHT,1);
			display.drawRect(xr,yr,BAR_WIDTH,BAR_HEIGHT,1);

			//Draw Ball centered
			// (xball,yball) top left corner
			yball = ((LCD_HEIGHT/2)-1);
			xball = ((LCD_WIDTH/2)-1);
			display.setCursor(30,20);
			display.setTextColor(WHITE);
			display.setTextSize(1);
			display.print("Starting in");
			display.display();
			delay(1000);
			display.setCursor(30,28);
			display.print("   - 3 -     ");
			display.display();
			delay(1000);
			display.fillRect(30, 28, display.width()-60, 8 , 0);
			display.setCursor(34,28);
			display.print("  - 2 -     ");
			display.display();
			delay(1000);
			display.fillRect(30, 28, display.width()-60, 8 , 0);
			display.setCursor(34,28);
			display.print("  - 1 -    ");
			display.display();
			delay(1000);
			display.fillRect(30, 20, display.width()-60, 8 , 0);
			display.fillRect(30, 28, display.width()-60, 8 , 0);
			while(!point)
			{


				if(is_UP1_pressed() && yl > EDGE_TOP)
				{
						display.drawRect(xl,yl-1+BAR_HEIGHT,2,1,0);
						display.drawRect(xl,yl-1,2,1,1);			//move bar up 1
						yl--;
				}

				if(is_DWN1_pressed() && yl < EDGE_BOT-BAR_HEIGHT)
				{
						display.drawRect(xl,yl,2,1,0);
						display.drawRect(xl,yl+BAR_HEIGHT,2,1,1);	//move bar down1
						yl++;
				}

				if(is_UP2_pressed() && yr > EDGE_TOP)
				{
						display.drawRect(xr,yr-1+BAR_HEIGHT,2,1,0);
						display.drawRect(xr,yr-1,2,1,1);			//move bar up 2
						yr--;
				}

				if(is_DWN2_pressed() && yr < EDGE_BOT-BAR_HEIGHT)
				{
						display.drawRect(xr,yr,2,1,0);
						display.drawRect(xr,yr+BAR_HEIGHT,2,1,1);	//move bar down 2
						yr++;
				}


				//colision detection
			if((xball+vx <= xl+2)&& (yball >= yl-1 && yball < yl+BAR_HEIGHT))
			{
				if(yball==(yl+(BAR_HEIGHT/2)))
				{
					vy=-vy;
					vx=(-vx)+1;
				}
				else
				{
				vx=-vx;
				vy= (yball-(yl+(BAR_HEIGHT/2)))/DRAG;
				}
			}
			if((xball+vx >= xr-2) && (yball >= yr-1 && yball < yr+BAR_HEIGHT))
			{
				if(yball==(yl+(BAR_HEIGHT/2)))
				{
					vy=-vy;
					vx=(-vx)+1;
				}
				else
				{
				vx=-vx;
				vy= (yball-(yl+(BAR_HEIGHT/2)))/DRAG;
				}

			}
			if(yball+vy < EDGE_TOP+1 || yball+vy > EDGE_BOT-2 )
			{
				vy = -vy;
			}


			//Ball movement
			display.drawRect(xball,yball,2,2,0);
			xball+=vx;
			yball+=vy;
			display.drawRect(xball,yball,2,2,1);

				  display.display();

				  if(xball == EDGE_LEFT)
					point = -1;
				  if(xball == EDGE_RIGHT-1)
					point = 1;

				  if(!digitalRead(BT1)&&!digitalRead(BT2))
					  break;


	    }
			while(!point)
			{
			  state = 0;    //emergency exit
			  RE = false;
			  return;
			}

			for (int i = 0; i<4;i++)
			{
				display.fillRect(30, 11, display.width()-60, 15 ,0);
				display.setTextSize(1);
				display.setTextColor(WHITE);
				display.setCursor(34,15);
				display.print("GAME  OVER");
				display.display();
				delay(200);
				display.fillRect(30, 11, display.width()-60, 15 , 1);
				display.setTextSize(1);
				display.setTextColor(BLACK);
				display.setCursor(34,15);
				display.print("GAME  OVER");
				display.display();
				delay(200);

			}
			display.setTextColor(WHITE);
			display.setCursor(7,35);
  			while(point == 1)
				{
				  display.print("  Winner:Player 1");
				  display.setCursor(0,45);
				  display.print("  Joy 1 to leave");
				  display.setCursor(0,55);
				  display.print("  Joy 2 to rematch");
				  display.display();
				  while(1)
				  {
					  if(!digitalRead(BT1))
					{

					  state = 0;
					  RE = false;
					  return;
					}
					  if(!digitalRead(BT2))
					  {
						  pong_2_player();
					  }
				  }
				}
			while(point == -1 )
				{
				  display.print("  Winner:Player 2");
				  display.setCursor(0,45);
				  display.print("  Joy 2 to leave");
				  display.setCursor(0,55);
				  display.print("  Joy 1 to rematch");
				  display.display();
				  while(1)
				  {
					  while(!digitalRead(BT2))
					  {
						  state = 0;
						  RE = false;
						  return;
					  }
					  while(!digitalRead(BT1))
						  {
							  pong_2_player();
						  }
				  }
				}

	  }
//////////////////////////////////////////////////////////////////////////////////////////////
 void dimmer()
  {
	 bool REaux;
  	 if(!is_UP2_pressed()&&!is_DWN2_pressed())
  	       		  REaux=true;
  	       	if(is_UP2_pressed() && REaux)
  	       	{
  	       		display.dim(0);
  	       		REaux=false;
  	       	}
  	       	if(is_DWN2_pressed() && REaux)
  	       	{
  	       	  		display.dim(1);
  	       	  		REaux=false;
  	       	}
  	       	return;
  }
 ///////////////////////////////////////////////////////////////////////////////////////////
 void inverter()
 {
 	 bool REaux;
   	 if(!is_UP2_pressed()&&!is_DWN2_pressed())
   	       		  REaux=true;
   	       	if(is_LFT2_pressed() && REaux)
   	       	{
   	       		display.invertDisplay(0);
   	       		REaux=false;
   	       	}
   	       	if(is_RGT2_pressed() && REaux)
   	       	{
   	       	  		display.invertDisplay(1);
   	       	  		REaux=false;
   	       	}
   	       	return;
   }

/////////////////////////////////////////////////////////////////////////////////////////////
  void setup()
  {
  Serial.begin(9600);
  pinMode (BT1, INPUT);
  pinMode (BT2, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC);          // inicialização do display.
  display.display();
  delay(3000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(45,15);
  display.println("Autores");
  display.drawRect(40, 13 ,50, 12, 1);
  display.setCursor(20,30);
  display.println("- Daniela Faria");
  display.setCursor(20,40);
  display.println("- Pedro Teixeira");
  display.display();
  delay(3000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,25);
  display.println("Computadores 18/19");
  display.display();
  delay(3000);
}

void loop(){
	bool REaux;
  switch(state) {               // menus

     case 0 :
        display.clearDisplay();          // caso default
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(15,0);
        display.println(">>MENU<<");
        display.setTextSize(1);
        display.print("\n");
        display.print(">>");
        display.setTextColor(BLACK, WHITE);
        display.print("Read     ");
        display.setTextColor(WHITE);
        display.println("<<");
        display.println("  Write    ");
        display.println("  Draw     ");
        display.println("  Pong     ");
        display.println("  TicTacToe");
        display.display();
        state = 1;
        break;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     case 1 :
        display.setCursor(0,24);
        display.setTextColor(WHITE);
        display.print(">>");
        display.setTextColor(BLACK, WHITE);
        display.print("Read     ");
        display.setTextColor(WHITE);
        display.print("<<");
        display.fillRect(80, 24, 32, 32, 0);
        display.drawBitmap(80, 24,  read_image, 32, 32, 1);
        display.display();



        if(digitalRead(BT1)&&!is_UP1_pressed()&&!is_DWN1_pressed())
           RE=true;


       if(!digitalRead(BT1) && RE )
       {
			display.clearDisplay();
			display.setTextSize(2);
			display.setTextColor(WHITE);
			display.setCursor(0,0);
			display.print(" >>READ<<");
			display.setTextSize(1);
			display.setCursor(0,24);
			display.print(">>");
			display.setTextColor(BLACK, WHITE);
			display.println("Message 1      ");
			display.setTextColor(WHITE);
			display.println("     Message 2 ");
			display.print("     Back");


			display.display();
			RE = false;
			state = 11;
			break;
	  }
       if(is_UP1_pressed() && RE)
       {
         state = 5;
         display.fillRect(0, 24, display.width(), 8, 0);
         display.setCursor(0,24);
         display.setTextColor(WHITE);
         display.print("  Read     ");
         RE = false;
         break;
       }

       if(is_DWN1_pressed()&& RE){
         state = 2;
         display.fillRect(0, 24, display.width(), 16, 0);
         display.setCursor(0,24);
         display.setTextColor(WHITE);
         display.print("  Read     ");
         RE = false;
         break;
       }
       dimmer();
       inverter();

       break;
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
     case 2 :
             display.setCursor(0,32);
             display.setTextColor(WHITE);
             display.print(">>");
             display.setTextColor(BLACK, WHITE);
             display.print("Write    ");
             display.setTextColor(WHITE);
             display.print("<<");
             display.fillRect(80, 24, 32, 32, 0);
             display.drawBitmap(80, 24,  write_image, 32, 32, 1);
             display.display();


             if(digitalRead(BT1)&&!is_UP1_pressed()&&!is_DWN1_pressed())
                RE=true;

              if(!digitalRead(BT1) && RE ){
            	display.clearDisplay();
				display.setTextSize(2);
				display.setTextColor(WHITE);
				display.setCursor(0,0);
				display.print(" >>WRITE<<");
				display.setTextSize(1);
				display.setCursor(0,24);
				display.print(">>");
				display.setTextColor(BLACK, WHITE);
				display.println("Message 1      ");
				display.setTextColor(WHITE);
				display.println("     Message 2 ");
				display.print("     Back");
                state = 21;
                RE = false;
                break;
              }

           if(is_UP1_pressed()&&RE ){
             state = 1; // caso demmo
             display.fillRect(0, 24, display.width(), 16, 0);
             display.setCursor(0,32);
             display.setTextColor(WHITE);
             display.print("  Write    ");
             RE = false;
             break;
           }

           if(is_DWN1_pressed()&& RE){
             state = 3;
             display.fillRect(0, 32, display.width(), 16, 0);
             display.setCursor(0,32);
             display.setTextColor(WHITE);
             display.print("  Write    ");
             RE = false;
             break;
           }
           if(!is_UP2_pressed()&&!is_DWN2_pressed())
           		  REaux=true;
           	if(is_UP2_pressed() && REaux)
           	{
           		display.dim(0);
           		REaux=false;
           		break;
           	}
           	if(is_DWN2_pressed() && REaux)
           	{
           	  		display.dim(1);
           	  		REaux=false;
           	  		break;
           	}
           break;
///////////////////////////////////////////////////////////////////////////////////////////7777
     case 3 :
             display.setCursor(0,40);
             display.setTextColor(WHITE);
             display.print(">>");
             display.setTextColor(BLACK, WHITE);
             display.print("Draw     ");
             display.setTextColor(WHITE);
             display.print("<<");
             display.fillRect(80, 24, 32, 32, 0);
             display.drawBitmap(80, 24,  draw_image, 32, 32, 1);
             display.display();

            if(digitalRead(BT1)&&!is_UP1_pressed()&&!is_DWN1_pressed())
            	 RE=true;


           if(is_UP1_pressed() && RE )
           {
             state = 2;
             display.fillRect(0,32,display.width(), 16, 0);
             display.setCursor(0,40);
             display.setTextColor(WHITE);
             display.print("  Draw     ");
             RE = false;
             break;
           }

           if(is_DWN1_pressed() && RE)
           {
             state = 4;
             display.fillRect(0, 40, display.width(), 16, 0);
             display.setCursor(0,40);
             display.setTextColor(WHITE);
             display.print("  Draw     ");
             RE = false;
             break;
           }
           dimmer();
           inverter();

           	if(!digitalRead(BT1) && RE )
			{
			  draw();
			  break;
			}
           break;
///////////////////////////////////////////////////////////////////////////////////////////777
         case 4 :
            display.setCursor(0,48);
            display.setTextColor(WHITE);
            display.print(">>");
            display.setTextColor(BLACK, WHITE);
            display.print("Pong     ");
            display.setTextColor(WHITE);
            display.print("<<");
            display.fillRect(80, 24, 32, 32, 0);
            display.drawBitmap(80, 24,  pong_image, 32, 32, 1);
            display.display();

            if(digitalRead(BT1)&&!is_UP1_pressed()&&!is_DWN1_pressed())
                    	 RE=true;

            if(!digitalRead(BT1) && RE )
            {
            	display.clearDisplay();
				display.setTextSize(2);
				display.setTextColor(WHITE);
				display.setCursor(0,0);
				display.print(" >>PONG <<");
				display.setTextSize(1);
				display.setCursor(0,24);
				display.print(">>");
				display.setTextColor(BLACK, WHITE);
				display.println("1 Player       ");
				display.setTextColor(WHITE);
				display.println("     2 Players ");
				display.print("     Back");

				display.display();
				RE = false;
				state = 41;
				break;
			  }

           if(is_UP1_pressed() && RE )
           {
             state = 3;
             display.fillRect(0, 48, display.width(), 16, 0);
             display.setCursor(0,48);
             display.setTextColor(WHITE);
             display.println("  Pong     ");
             display.print("  TicTacToe");
             RE = false;
             break;
           }

           if(is_DWN1_pressed() && RE)
           {
             state = 5;
             display.fillRect(0, 48, display.width(), 8, 0);
             display.setCursor(0,48);
             display.setTextColor(WHITE);
             display.print("  Pong     ");
             RE = false;
             break;
           }
           dimmer();
           inverter();
           break;
       //////////////////////////////////////////////////////////////////////////////////////////////////////
         case 5 :

			display.setCursor(0,56);
			display.setTextColor(WHITE);
			display.print(">>");
			display.setTextColor(BLACK, WHITE);
			display.print("TicTacToe");
			display.setTextColor(WHITE);
			display.print("<<");
			display.fillRect(80, 24, 32, 32, 0);
			display.drawBitmap(80, 24,  tic_tac_toe_image, 32, 32, 1);
			display.display();

			if(digitalRead(BT1)&&!is_UP1_pressed()&&!is_DWN1_pressed())
			 RE=true;

		  if(is_UP1_pressed() && RE )
		  {
			state = 4;
			display.fillRect(0, 56, display.width(), 16, 0);
			display.setCursor(0,56);
			display.setTextColor(WHITE);
			display.print("  TicTacToe");
			RE = false;
			break;
		  }

		  if(is_DWN1_pressed() && RE)
		  {
			state = 1;
			display.fillRect(0, 56, display.width(), 16, 0);
			display.setCursor(0,56);
			display.setTextColor(WHITE);
			display.print("  TicTacToe");
			RE = false;
			break;
		  }
		  dimmer();
		  inverter();
			if(!digitalRead(BT1) && RE )
			{
			 tic_tac_toe(SIZE_OF_TIC_TAC_TOE);
			 break;
			 }
		  break;
      //////////////////////////////////////////////////////////////////////////////////////////////////////
         case 11 :

			  display.setCursor(0,24);
			  display.setTextColor(WHITE);
			  display.print(">>");
			  display.setTextColor(BLACK, WHITE);
			  display.print("   Message 1    ");
			  display.setTextColor(WHITE);
			  display.print("<<");
			  display.display();

			  if(digitalRead(BT1)&&!is_UP1_pressed()&&!is_DWN1_pressed())
			   RE=true;

			 if(!digitalRead(BT1) && RE )
			 {
				read(1);

				break;
			   }

			if(is_DWN1_pressed() && RE)
			{
			  state = 12;
			  display.fillRect(0, 24, display.width(), 16, 0);
			  display.setCursor(0,24);
			  display.setTextColor(WHITE);
			  display.println("     Message 1       ");
			  RE = false;
			  break;
			}
		   if(is_UP1_pressed() && RE)
		   {
				state = 13;
				display.fillRect(0, 24, display.width(), 16, 0);
				display.setCursor(0,24);
				display.setTextColor(WHITE);
				display.println("     Message 1       ");
				display.setCursor(0,32);
				display.println("     Message 2       ");
				RE = false;
				break;
			}
		   dimmer();
		   inverter();
			break;

		  case 12 :

			display.setCursor(0,32);
			display.setTextColor(WHITE);
			display.print(">>");
			display.setTextColor(BLACK, WHITE);
			display.print("   Message 2    ");
			display.setTextColor(WHITE);
			display.print("<<");
			display.display();


		   if(digitalRead(BT1)&&!is_UP1_pressed()&&!is_DWN1_pressed())
			RE=true;

		   if(!digitalRead(BT1) && RE )
		   {
			read(2);

			break;
		   }

			if(is_UP1_pressed() && RE)
			{
			  state = 11;
			  display.fillRect(0, 32, display.width(), 16, 0);
			  display.setCursor(0,32);
			  display.setTextColor(WHITE);
			  display.print("     Message 2   ");
			  display.setCursor(0,40);
			  display.print("     Back ");
			  RE = false;
			  break;
			}
		   if(is_DWN1_pressed() && RE)
		   {
		   state = 13;
		   display.fillRect(0, 32, display.width(), 16, 0);
		   display.setCursor(0,32);
		   display.setTextColor(WHITE);
		   display.print("     Message 2   ");
		   RE = false;
		   break;
		 }
		   dimmer();
		   inverter();
			break;

		  case 13 :


			display.setCursor(0,40);
			display.setTextColor(WHITE);
			display.print(">>");
			display.setTextColor(BLACK, WHITE);
			display.print("   Back         ");
			display.setTextColor(WHITE);
			display.print("<<");
			display.display();


		   if(digitalRead(BT1)&&!is_UP1_pressed()&&!is_DWN1_pressed())
			 RE=true;
		   if(!digitalRead(BT1) && RE )
		   {
			  state=0;
			  RE = false;
			  break;
			}

			if(is_UP1_pressed() && RE){
			  state = 12;
			  display.fillRect(0, 40, display.width(), 16, 0);
			  display.setCursor(0,40);
			  display.setTextColor(WHITE);
			  display.print("     Back      ");
			  RE = false;
			  break;
			}
		   if(is_DWN1_pressed() && RE){
		   state = 11;
		   display.fillRect(0, 40, display.width(), 16, 0);
		   display.setCursor(0,40);
		   display.setTextColor(WHITE);
		   display.print("     Back      ");
		   RE = false;
		   break;
		 }
		   dimmer();
		   inverter();
		   break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		  case 41 :

				  display.setCursor(0,24);
				  display.setTextColor(WHITE);
				  display.print(">>");
				  display.setTextColor(BLACK, WHITE);
				  display.print("   1 Player     ");
				  display.setTextColor(WHITE);
				  display.print("<<");
				  display.display();

                 if(digitalRead(BT1)&&!is_UP1_pressed()&&!is_DWN1_pressed())
                  RE=true;

               if(is_DWN1_pressed() && RE)
               {
					state = 42;
					display.fillRect(0, 24, display.width(), 16, 0);
					display.setCursor(0,24);
					display.setTextColor(WHITE);
					display.println("     1 Player        ");
					RE = false;
					break;
               }
              if(is_UP1_pressed() && RE)
              {
                   state = 43;
                   display.fillRect(0, 24, display.width(), 16, 0);
					display.setCursor(0,24);
					display.setTextColor(WHITE);
					display.println("     1 Player        ");
					display.setCursor(0,32);
					display.println("     2 Players       ");
					RE = false;
					break;
              }
              if(!digitalRead(BT1) && RE )
				 {
				   pong_1_player();
					break;
				   }
              dimmer();
              inverter();
               break;

		  case 42 :

					display.setCursor(0,32);
					display.setTextColor(WHITE);
					display.print(">>");
					display.setTextColor(BLACK, WHITE);
					display.print("   2 Players    ");
					display.setTextColor(WHITE);
					display.print("<<");
					display.display();


              if(digitalRead(BT1)&&!is_UP1_pressed()&&!is_DWN1_pressed())
               RE=true;

              if(!digitalRead(BT1) && RE )
              {
            	  pong_2_player();
            	  RE=false;
             	break;
              }

               if(is_UP1_pressed() && RE)
               {
                 state = 41;
                 display.fillRect(0, 32, display.width(), 16, 0);
                 display.setCursor(0,32);
                 display.setTextColor(WHITE);
                 display.setTextColor(WHITE);
				  display.print("     2 Players   ");
				  display.setCursor(0,40);
				  display.print("     Back ");
				  RE = false;
				  break;
               }
              if(is_DWN1_pressed() && RE)
              {
			   state = 43;
			   display.fillRect(0, 32, display.width(), 16, 0);
			   display.setCursor(0,32);
			   display.setTextColor(WHITE);
			   display.print("     2 Players   ");
			   RE = false;
			   break;
			 }
              dimmer();
              inverter();
               break;

		  case 43 :


				display.setCursor(0,40);
				display.setTextColor(WHITE);
				display.print(">>");
				display.setTextColor(BLACK, WHITE);
				display.print("   Back         ");
				display.setTextColor(WHITE);
				display.print("<<");
				display.display();


			   if(digitalRead(BT1)&&!is_UP1_pressed()&&!is_DWN1_pressed())
				 RE=true;
			   if(!digitalRead(BT1) && RE )
			   {
				  state=0;
				  RE = false;
				  break;
			   	}

				if(is_UP1_pressed() && RE){
				  state = 42;
				  display.fillRect(0, 40, display.width(), 16, 0);
				  display.setCursor(0,40);
				  display.setTextColor(WHITE);
				  display.print("     Back ");
				  RE = false;
				  break;
				}
			   if(is_DWN1_pressed() && RE){
			   state = 41;
			   display.fillRect(0, 40, display.width(), 16, 0);
			   display.setCursor(0,40);
			   display.setTextColor(WHITE);
			   display.print("     Back   ");
			   RE = false;
			   break;
			 }
			   dimmer();
			   inverter();
			   break;
/////////////////////////////////////////////////////////////////////////////////////////////////////
			   case 21 :
				 //Serial.println("mess 1");
			  display.setCursor(0,24);
			  display.setTextColor(WHITE);
			  display.print(">>");
			  display.setTextColor(BLACK, WHITE);
			  display.print("   Message 1    ");
			  display.setTextColor(WHITE);
			  display.print("<<");
			  display.display();

			  if(digitalRead(BT1)&&!is_UP1_pressed()&&!is_DWN1_pressed())
			   RE=true;

			if(is_DWN1_pressed() && RE)
			{
			  state = 22;
			  display.fillRect(0, 24, display.width(), 16, 0);
			  display.setCursor(0,24);
			  display.setTextColor(WHITE);							//um jogador branco no preto
			  display.println("     Message 1       ");
			  RE = false;
			  break;
			}
		   if(is_UP1_pressed() && RE)
		   {
				state = 23;
				display.fillRect(0, 24, display.width(), 16, 0);
				display.setCursor(0,24);
				display.setTextColor(WHITE);							//um jogador branco no preto
				display.println("     Message 1       ");
				display.setCursor(0,32);
				display.println("     Message 2       ");
				RE = false;
				break;
			}
		   if(!digitalRead(BT1) && RE )
			 {
				write(1);
				break;
			   }
		   dimmer();
		   inverter();
			break;

		  case 22 :

			display.setCursor(0,32);
			display.setTextColor(WHITE);
			display.print(">>");
			display.setTextColor(BLACK, WHITE);
			display.print("   Message 2    ");        //dois jogadores branco no preto
			display.setTextColor(WHITE);
			display.print("<<");
			display.display();


		   if(digitalRead(BT1)&&!is_UP1_pressed()&&!is_DWN1_pressed())
			RE=true;

		  	if(is_UP1_pressed() && RE)
			{
			  state = 21;
			  display.fillRect(0, 32, display.width(), 16, 0);
			  display.setCursor(0,32);
			  display.setTextColor(WHITE);
			  display.print("     Message 2   ");
			  display.setCursor(0,40);
			  display.print("     Back ");

			  RE = false;
			  break;
			}
		   if(is_DWN1_pressed() && RE)
		   {
		   state = 23;
		   display.fillRect(0, 32, display.width(), 16, 0);
		   display.setCursor(0,32);
		   display.setTextColor(WHITE);
		   display.print("     Message 2   ");
		   RE = false;
		   break;
		 }
		   if(!digitalRead(BT1) && RE )
			   {
				write(2);
				break;
			   }
		   dimmer();
		   inverter();
			break;

		  case 23 :


			display.setCursor(0,40);
			display.setTextColor(WHITE);
			display.print(">>");
			display.setTextColor(BLACK, WHITE); //voltar preto no branco
			display.print("   Back         ");
			display.setTextColor(WHITE);
			display.print("<<");
			display.display();


		   if(digitalRead(BT1)&&!is_UP1_pressed()&&!is_DWN1_pressed())
			 RE=true;
		   if(!digitalRead(BT1) && RE )
		   {
			  state=0;
			  RE = false;
			  break;
			}

			if(is_UP1_pressed() && RE){
			  state = 22;
			  display.fillRect(0, 40, display.width(), 16, 0);
			  display.setCursor(0,40);
			  display.setTextColor(WHITE);
			  display.print("     Back      ");
			  RE = false;
			  break;
			}
		   if(is_DWN1_pressed() && RE){
		   state = 21;
		   display.fillRect(0, 40, display.width(), 16, 0);
		   display.setCursor(0,40);
		   display.setTextColor(WHITE);
		   display.print("     Back      ");
		   RE = false;
		   break;
		 }
		   dimmer();
		   inverter();
		   break;
  	  }
  }

//*******************************************************************
// This program Interfaces with TFT LCD
//
// Add the folder to additional Includes in the project 
//
// For Win32 Simulator
//
// gPrint.cpp Implements some print overrrieds
// gSim_TFT.cpp,.h Implements some TFT overrrieds
//
//*******************************************************************

#define NEED_REMOTE
#define NEED_ENCODER
#define NEED_KEYS

#define NEED_CROSSOVER

// On Board Encoder
#define ENCODER_PIN_A		7
#define ENCODER_PIN_B		5
#define ENCODER_PIN_SW		6

#define IR_RECV_PIN			2

#ifndef _WIN32

	#include <avr/pgmspace.h>

	#ifdef NEED_CROSSOVER
		/*
			https://github.com/liman324/TDA7419
		*/
		#include "src/TDA7419/TDA7419.h"

		/*
			https://github.com/adafruit/Adafruit-GFX-Library
		*/
		#include <Adafruit_GFX.h>
		#include <Adafruit_ILI9341.h>

		#include <Wire.h>
	#endif

	#ifdef NEED_REMOTE
		/*
			http://github.com/shirriff/Arduino-IRremote
		*/
		#include <IRremote.h>
	#endif

#else
	// Needed for Win32 Simulator.
	int g_XOFF,g_YOFF,g_MAXX,g_MAXY,PixW,PixH;
#endif

#ifdef NEED_ENCODER
	/*
		https://github.com/mathertel/RotaryEncoder
	*/
	#ifndef WIN32
		#include <RotaryEncoder.h>
	#endif

	// Setup a RoraryEncoder for pins A2 and A3:
	RotaryEncoder encoder(ENCODER_PIN_A, ENCODER_PIN_B);
	int DebounceDir;
#endif

#ifdef NEED_KEYS

	byte KEY_PINS[]={ENCODER_PIN_SW,A0,A1,A2,A3};/* 1 2 3 4 */
	
	#define DEBOUNCE_COUNT 100
	#include "gKeyReader.h"

	#ifdef NEED_ANALOG_KEY_BUTTONS
		#error "NEED_KEYS and NEED_ANALOG_KEY_BUTTONS cannot be defined together"
	#endif

#endif



#define PRINT_F(X) Serial.print( F((X)) ) 
#define PRINTLN_F(X) Serial.println( F((X)) ) 
void DisplayBootLogo();

#ifdef NEED_CROSSOVER
	TDA7419 tda;
#endif

#ifdef NEED_REMOTE

IRrecv irrecv(IR_RECV_PIN);
decode_results results;


void dump(decode_results *results) 
{
/*
  int count = results->rawlen;

  PRINT_F("[REMOTE ");

  if (results->decode_type == UNKNOWN) {
    //Serial.print("Unknown encoding: ");
	  PRINT_F("UNK ");
  } 
  else if (results->decode_type == NEC) {
    //Serial.print("Decoded NEC: ");
    PRINT_F("NEC ");
  } 
  else if (results->decode_type == SONY) {
    //Serial.print("Decoded SONY: ");
	  PRINT_F("SON ");
  } 
  else if (results->decode_type == RC5) {
    //Serial.print("Decoded RC5: ");
	PRINT_F("RC5 ");
  } 
  else if (results->decode_type == RC6) {
    //Serial.print("Decoded RC6: ");
	PRINT_F("RC6 ");
  }
  else if (results->decode_type == PANASONIC) 
  {	
    PRINT_F("Decoded PANASONIC - Address: ");
    Serial.print(results->panasonicAddress,HEX);
    PRINT_F(" Value: ");
  }
  else if (results->decode_type == JVC) 
  {
     //Serial.print("Decoded JVC: ");
	  PRINT_F("JVC ");
  }


  Serial.print(results->value, HEX);
  //PRINT_F("]"));

  /*
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");
  *~/

  /*
  for (int i = 0; i < count; i++) {
    if ((i % 2) == 1) {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    } 
    else {
      Serial.print(-(int)results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  *~/
  Serial.println("");
*/
}

#endif

#define MAX_GBUF_SIZE	200

char SerialData[MAX_GBUF_SIZE+1];
byte SerialDataPos=0;

#define VOL_MIN		0
#define VOL_MAX		79

#define IN_MIN		1
#define IN_MAX		3

#define MUTE_MIN	0
#define MUTE_MAX	1

#define CONTROLS_PERPAGE	5

#define SEL_PROGRESS_BAR	1
#define SEL_OPTIONS			2
#define SEL_ON_OFF			3
#define SEL_VOLUME_BAR		4
#define SEL_TONE_BAR		5
#define SEL_ATTUNATION_BAR	6

#define SLABEL_LEN	3
#define LABEL_LEN	16
#define CTRL_ROW_LEN	21

#define CTRL_VOL	0
#define CTRL_BASS	1
#define CTRL_MID	2
#define CTRL_TREBLE	3
#define CTRL_LOD	4
#define CTRL_LTF	5
#define CTRL_RTF	6
#define CTRL_LTB	7
#define CTRL_RTB	8
#define CTRL_SUBV	9
#define CTRL_BCF	10
#define CTRL_BQ		11
#define CTRL_MCF	12
#define CTRL_MQ		13
#define CTRL_TCF	14
#define CTRL_LODCF	15
#define CTRL_LHIBST	16
#define CTRL_SUBCOF	17
#define CTRL_SMOTH  18
#define CTRL_G1		19

#define CONTROL_COUNT		20

// CONTROLS IN OTHER PLACES
#define CTRL_INPUT			20
#define CTRL_MUTE			21

#define TOTAL_CONTROL_COUNT	22

//TDA7814 needs +16 for < 1db
int VolumeLookUp[30]={  
79+16,//0
60+16,//1
50+16,//2
40+16,//3
30+16,//4
25+16,//5
20+16,//6
17+16,//7
14+16,//8
12+16,//9
10+16,//10
9+16, //11
8+16, //12
7+16, //13
6+16, //14
5+16, //15
4+16, //16
3+16, //17
2+16, //18
1+16, //19
0+16, //20
1,    //21
2,    //22
4,    //23
8,    //24
10,   //25
11,   //26
12,   //27
14,   //28
15};  //29

//byte Input=1;
//int  Mute=1;// OFF
int PControlData[CONTROL_COUNT+5];
int ControlData[CONTROL_COUNT+5]=
{
	 20,//CTRL_VOL

	 15,//CTRL_BASS
	 15,//CTRL_MID
	 15,//CTRL_TREBLE
	 8,//CTRL_LOD

	20,//CTRL_LTF
	20,//CTRL_RTF
	20,//CTRL_LTB
	20,//CTRL_RTB
	20,//CTRL_SUBV

	 0,//CTRL_BCF	
	 0,//CTRL_BQ	
	 0,//CTRL_MCF	
	 0,//CTRL_MQ	
	 0,//CTRL_TCF	
	 0,//CTRL_LODCF	
	 0,//CTRL_LHIBST
	 0,//CTRL_SUBCOF
	 0,//CTRL_SMOTH	
	15,//CTRL_G1	

	 1,//CTRL_INPUT
	 1,//CTRL_MUTE
};

/*
          1         2
012345678901234567890123456789
*/
const char Controls[] PROGMEM =
{
"\
VOL|Volume          4\
BAS|Bass            5\
MID|Mid             5\
TRE|Treble          5\
LoD|Loudness        6\
LtF|Left Frnt Vol   4\
RtF|Right Frnt Vol  4\
LtB|Left Back Vol   4\
RtB|Right Back Vol  4\
SBv|Sub Vol         4\
BcF|Bass C.Freq     2\
Bq |Bass Q          2\
McF|Mid C.Freq      2\
Mq |Mid Q           2\
TcF|Treb C.Freq     2\
LoF|Loud C.Freq     2\
LoH|LoudHi Boost    3\
SbF|Sub Cutoff      2\
SmF|Smooth Filter   3\
GN1|Gain 1          1\
"
};


//****************************************************************************************
#ifdef NEED_SERIAL_DUMP

void DumpTranslatedVol(byte idx)
{
	int v=VolumeLookUp[ControlData[idx]];
	if (v>15) {v-=16;v*=-1;}
	Serial.print(v);
	PRINT_F("db ");
	Serial.println(ControlData[idx]); 
}

void DumpTranslatedTone(byte idx)
{
	Serial.print(ControlData[idx]-15);
	PRINT_F("db ");
	Serial.println(ControlData[idx]); 
}

void DumpAudio()
{
	PRINT_F("Input=");Serial.println(ControlData[CTRL_INPUT]);
	PRINT_F("Mute=");Serial.print(ControlData[CTRL_MUTE]);
	if(ControlData[CTRL_MUTE]== 1 ) PRINTLN_F(" OFF")  ;
	if(ControlData[CTRL_MUTE]== 0 ) PRINTLN_F(" ON")  ;

PRINT_F("CTRL_VOL=");DumpTranslatedVol(CTRL_VOL);
PRINT_F("CTRL_LTF=");DumpTranslatedVol(CTRL_LTF);
PRINT_F("CTRL_RTF=");DumpTranslatedVol(CTRL_RTF);
PRINT_F("CTRL_LTB=");DumpTranslatedVol(CTRL_LTB);
PRINT_F("CTRL_RTB=");DumpTranslatedVol(CTRL_RTB);
PRINT_F("CTRL_SUBV=");DumpTranslatedVol(CTRL_SUBV);

PRINT_F("CTRL_BASS=");DumpTranslatedTone(CTRL_BASS);
PRINT_F("CTRL_MID=");DumpTranslatedTone(CTRL_MID);
PRINT_F("CTRL_TREBLE=");DumpTranslatedTone(CTRL_TREBLE);
PRINT_F("CTRL_BASS=");DumpTranslatedTone(CTRL_BASS);

PRINT_F("CTRL_G1=");Serial.println(ControlData[CTRL_G1]);
PRINT_F("CTRL_LOD=");Serial.println(ControlData[CTRL_LOD]);
	
	PRINT_F("CTRL_LODCF=");Serial.print(ControlData[CTRL_LODCF]);
	if(ControlData[CTRL_LODCF]== 0 ) PRINTLN_F(" FLAT")  ;
	if(ControlData[CTRL_LODCF]== 1 ) PRINTLN_F(" 400Hz") ;
	if(ControlData[CTRL_LODCF]== 2 ) PRINTLN_F(" 800Hz") ;
	if(ControlData[CTRL_LODCF]== 3 ) PRINTLN_F(" 2400Hz");

	PRINT_F("CTRL_TCF=");Serial.print(ControlData[CTRL_TCF]);
	if(ControlData[CTRL_TCF]== 0 ) PRINTLN_F(" 10KHz") ;
	if(ControlData[CTRL_TCF]== 1 ) PRINTLN_F(" 12.5kHz");
	if(ControlData[CTRL_TCF]== 2 ) PRINTLN_F(" 15kHz") ;
	if(ControlData[CTRL_TCF]== 3 ) PRINTLN_F(" 17.5kHz");

	PRINT_F("CTRL_MCF=");Serial.print(CTRL_MCF);
	if(ControlData[CTRL_MCF]== 0 ) PRINTLN_F(" 0.5KHz") ;
	if(ControlData[CTRL_MCF]== 1 ) PRINTLN_F(" 1kHz") ;
	if(ControlData[CTRL_MCF]== 2 ) PRINTLN_F(" 1.5kHz") ;
	if(ControlData[CTRL_MCF]== 3 ) PRINTLN_F(" 2.5kHz") ;

	PRINT_F("CTRL_BCF=");Serial.print(CTRL_BCF);
	if(ControlData[CTRL_BCF]== 0 ) PRINTLN_F(" 60Hz") ;
	if(ControlData[CTRL_BCF]== 1 ) PRINTLN_F(" 80Hz") ;
	if(ControlData[CTRL_BCF]== 2 ) PRINTLN_F(" 100Hz") ;
	if(ControlData[CTRL_BCF]== 3 ) PRINTLN_F(" 200Hz") ;

	PRINT_F("CTRL_SUBCOF=");Serial.print(CTRL_SUBCOF);
   if(ControlData[CTRL_SUBCOF]== 0 ) PRINTLN_F(" FLAT")  ;
   if(ControlData[CTRL_SUBCOF]== 1 ) PRINTLN_F(" 80Hz")  ;
   if(ControlData[CTRL_SUBCOF]== 2 ) PRINTLN_F(" 120Hz") ;
   if(ControlData[CTRL_SUBCOF]== 3 ) PRINTLN_F(" 160Hz") ;

   PRINT_F("CTRL_MQ=");Serial.print(CTRL_MQ);
   if(ControlData[CTRL_MQ]== 0 ) PRINTLN_F(" 0.5")  ;
   if(ControlData[CTRL_MQ]== 1 ) PRINTLN_F(" 0.75") ;
   if(ControlData[CTRL_MQ]== 2 ) PRINTLN_F(" 1.0")  ;
   if(ControlData[CTRL_MQ]== 3 ) PRINTLN_F(" 1.25") ;

   PRINT_F("CTRL_BQ=");Serial.print(CTRL_BQ);
   if(ControlData[CTRL_BQ]== 0 ) PRINTLN_F(" 1.0") ;
   if(ControlData[CTRL_BQ]== 1 ) PRINTLN_F(" 1.25") ;
   if(ControlData[CTRL_BQ]== 2 ) PRINTLN_F(" 1.5") ;
   if(ControlData[CTRL_BQ]== 3 ) PRINTLN_F(" 2.0") ;

   PRINT_F("CTRL_LHIBST=");Serial.print(CTRL_LHIBST);
   if(ControlData[CTRL_LHIBST]== 0 ) PRINTLN_F(" ON") ;
   if(ControlData[CTRL_LHIBST]== 1 ) PRINTLN_F(" OFF") ;

   PRINT_F("CTRL_SMOTH=");Serial.print(CTRL_SMOTH);
   if(ControlData[CTRL_SMOTH]== 0 ) PRINTLN_F(" ON") ;
   if(ControlData[CTRL_SMOTH]== 1 ) PRINTLN_F(" OFF") ;

}

#endif

#define ENCODER_CW				'A'
#define ENCODER_CCW				'B'
#define ENCODER_OK				'C'

#define IRKEY_MUTE				'G'
#define IRKEY_MENU				'H'
#define IRKEY_UP				'I'
#define IRKEY_DN				'J'
#define IRKEY_LEFT				'K'
#define IRKEY_RIGHT				'L'
#define IRKEY_OK				'M'

void HandleRemoteCommands(byte ch);

void ReadSerialData()
{
	char ch=Serial.read();



	if (ch=='{') SerialDataPos=0;

	if (ch=='}')
	{
		byte ok=0;

		switch(SerialData[1])
		{

#ifdef NEED_SERIAL_DUMP
		// Set the Values.{Z}
		case 'Z': {DumpAudio();ok=1;}break;
#endif
		}

		if (ok) 
		{
			
			PRINTLN_F("OK");
		}
	}
	


	SerialData[SerialDataPos]=ch;
	if (SerialDataPos==MAX_GBUF_SIZE) SerialDataPos=0;
	
	SerialDataPos++;
	SerialData[SerialDataPos]='\0';
}

void UpdateControlData()
{
	memcpy(PControlData,ControlData,sizeof(ControlData));
}

byte IsChanged(byte p1,byte p2=200,byte p3=200,byte p4=200)
{
	byte ret=0;
	if (ControlData[p1]!=PControlData[p1]) ret=1;
	if ((p2!=200)&&(ControlData[p2]!=PControlData[p2])) ret=1;
	if ((p3!=200)&&(ControlData[p3]!=PControlData[p3])) ret=1;
	if ((p4!=200)&&(ControlData[p3]!=PControlData[p3])) ret=1;

	return  ret;
}

byte EEPromOK=0;
void LoadSettings()
{
	int i,address=16,data;
	// Signature
	data=eeprom_read_word((uint16_t *) 0);
	if (data!=0xBABE) return;
	EEPromOK=1;
	for(i=0;i<TOTAL_CONTROL_COUNT;i++)
	{
		ControlData[i]=eeprom_read_word((uint16_t *) address);
		PControlData[i]=ControlData[i];
		address+=2;
	}

	for(i=0;i<TOTAL_CONTROL_COUNT;i++)
	{
		Serial.print(ControlData[i]);Serial.print(' ');
	}
	Serial.println();

}

void SaveSettings()
{
	int i,address=16;

	for(i=0;i<TOTAL_CONTROL_COUNT;i++)
	{
		if ((PControlData[i]!=ControlData[i])||// Write only changed values.
			(EEPromOK==0)// Write all values for the first time.
			)
		{
			eeprom_write_word((uint16_t *) address,ControlData[i]);
			Serial.print(i);Serial.print(' ');
		}
		PControlData[i]=ControlData[i];
		address+=2;
	}
	Serial.println(F("Saved"));
	for(i=0;i<TOTAL_CONTROL_COUNT;i++)
	{
		Serial.print(ControlData[i]);Serial.print(' ');
	}
	Serial.println();

	if (EEPromOK==0) eeprom_write_word((uint16_t *) 0,0xBABE);	
	EEPromOK=1;
}

byte IsDirty=0;
unsigned long stime,oldtime;
void SendCommands()
{
	
	if (IsChanged(CTRL_INPUT,CTRL_G1))
	{
		tda.setInput(ControlData[CTRL_INPUT],ControlData[CTRL_G1], 0 ) ;
		tda.setInput2(5 , 0, 0 );// Muted
		IsDirty=1;
	}

	if (IsChanged(CTRL_LOD,CTRL_LODCF,CTRL_LHIBST))
	{
		tda.setAtt_loudness( 15-ControlData[CTRL_LOD], 
							ControlData[CTRL_LODCF], 
							ControlData[CTRL_LHIBST], 0 ) ;
		IsDirty=1;
	}

	if (IsChanged(CTRL_MUTE))
	{
		tda.setSoft(ControlData[CTRL_MUTE] , 0 , 0 , 0 , 0 ) ;
		IsDirty=1;
	}


	if (IsChanged(CTRL_VOL)) {tda.setVolumeNew(TDA7419_VOLUME,VolumeLookUp[ControlData[CTRL_VOL]]);IsDirty=1;}
	if (IsChanged(CTRL_LTF)) {tda.setVolumeNew(TDA7419_ATT_LF,VolumeLookUp[ControlData[CTRL_LTF]]);IsDirty=1;}
	if (IsChanged(CTRL_RTF)) {tda.setVolumeNew(TDA7419_ATT_RF,VolumeLookUp[ControlData[CTRL_RTF]]);IsDirty=1;}
	if (IsChanged(CTRL_LTB)) {tda.setVolumeNew(TDA7419_ATT_LT,VolumeLookUp[ControlData[CTRL_LTB]]);IsDirty=1;}
	if (IsChanged(CTRL_RTB)) {tda.setVolumeNew(TDA7419_ATT_RT,VolumeLookUp[ControlData[CTRL_RTB]]);IsDirty=1;}
	if (IsChanged(CTRL_SUBV)) {tda.setVolumeNew(TDA7419_ATT_SUB,VolumeLookUp[ControlData[CTRL_SUBV]]);IsDirty=1;}

	if (IsChanged(CTRL_TREBLE,CTRL_TCF)) 
	{
		tda.setFilter_Treble(ControlData[CTRL_TREBLE]-15, ControlData[CTRL_TCF], 0 ) ;
		IsDirty=1;
	}

	if (IsChanged(CTRL_MID,CTRL_MQ)) 
	{
		tda.setFilter_Middle(ControlData[CTRL_MID]-15, ControlData[CTRL_MQ], 0 ) ;
		IsDirty=1;
	}

	if (IsChanged(CTRL_BASS,CTRL_BQ)) 
	{
		tda.setFilter_Bass(ControlData[CTRL_BASS]-15, ControlData[CTRL_BQ], 0 ) ;
		IsDirty=1;
	}

	if (IsChanged(CTRL_SUBCOF,CTRL_MCF,CTRL_BCF,CTRL_SMOTH)) 
	{
		tda.setSub_M_B(ControlData[CTRL_SUBCOF], ControlData[CTRL_MCF], 
					ControlData[CTRL_BCF], 0,ControlData[CTRL_SMOTH]);
		IsDirty=1;
	}
	
	//setMix_Gain_Eff(int mix_l, int mix_r, int mix_en, int sub_en, int gain_eff){}; 
	//(0=on/1=off, on/off, on/off, on/off, 0...10 = 0...22 dB)
	if (IsChanged(CTRL_INPUT))
	{
		tda.setMix_Gain_Eff(1 , 1 , 1 , 0 , 0 ) ; // no mix, sub on, hpf gain =0
		tda.setSpektor(0 , 0 , 0 , 0 , 0 , 0 , 0 ) ;
		IsDirty=1;
	}

	// Wait for 2 Seconds from the Last update.
	if (IsDirty) oldtime=stime;
	
}


#ifdef WIN32

	#include "Fonts/FreeSans9pt7b.h"
	gSim_TFT tft = gSim_TFT(320, 240, 0, 0, 0,0, 0, 0);
	
#else
	
	#include <FreeSans9pt7b.h>

	#define TFT_CLK 13
	#define TFT_MISO 12
	#define TFT_MOSI 11

	#define TFT_DC 8
	#define TFT_RST 9
	#define TFT_CS 10

	//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
	Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC , TFT_RST);
#endif


char SLabel[10];
char SValue[10];
char Label[18];
char PrevLabel[18]="";
char options[5][6];
byte CType,CurCType;
int cursel=0,scrly=0,curfocussed=200;
int ocurcontrol=200,oscrly=200;

void LoadLabel(byte sel)
{
	byte curcontrol=cursel+scrly;
	byte i;
	int16_t p=sel*CTRL_ROW_LEN;// Load from Program Mem.

	for(i=0;i<SLABEL_LEN;i++,p++)
	{
		SLabel[i]=pgm_read_byte_near(Controls + p);
	}
	SLabel[i]='\0';
	p++;
	for(i=0;i<LABEL_LEN;i++,p++)
	{
		Label[i]=pgm_read_byte_near(Controls + p);
	}
	Label[i]='\0';
	CType=pgm_read_byte_near(Controls + p)-'0';

	if (curcontrol==sel) CurCType=CType;
	// This saves RAM 
	switch(sel)
	{
		case CTRL_BCF:	 strcpy_P(options[0],(const char *) F("60Hz"));
						 strcpy_P(options[1],(const char *) F("80Hz"));
						 strcpy_P(options[2],(const char *) F("100Hz"));
						 strcpy_P(options[3],(const char *) F("200Hz"));
						 break;
		case CTRL_MCF:	 strcpy_P(options[0],(const char *) F("500Hz"));
						 strcpy_P(options[1],(const char *) F("1K"));
						 strcpy_P(options[2],(const char *) F("1.5K"));
						 strcpy_P(options[3],(const char *) F("2.5K"));
						 break;

		case CTRL_TCF:	 strcpy_P(options[0],(const char *) F("10K"));
						 strcpy_P(options[1],(const char *) F("12.5K"));
						 strcpy_P(options[2],(const char *) F("15K"));
						 strcpy_P(options[3],(const char *) F("17.5K"));
						 break;

		case CTRL_SUBCOF:strcpy_P(options[0],(const char *) F("FLAT"));
						 strcpy_P(options[1],(const char *) F("80Hz"));
						 strcpy_P(options[2],(const char *) F("120Hz"));
						 strcpy_P(options[3],(const char *) F("160Hz"));
						 break;

		case CTRL_LODCF: strcpy_P(options[0],(const char *) F("FLAT"));
						 strcpy_P(options[1],(const char *) F("400Hz"));
						 strcpy_P(options[2],(const char *) F("800Hz"));
						 strcpy_P(options[3],(const char *) F("2.4K"));
						 break;
		
		case CTRL_MQ:
		case CTRL_BQ:	 strcpy_P(options[0],(const char *) F("LOW"));
						 strcpy_P(options[1],(const char *) F("MED"));
						 strcpy_P(options[2],(const char *) F("HI"));
						 strcpy_P(options[3],(const char *) F("V.HI"));
						 break;
	}
}

#define HEADER_HEIGHT	26
#define CONTROL_HEIGHT	26
#define FONT_HEIGHT_ADJ	8
#define INPUT_BTN_W		46
#define CTRL_ROW_HEIGHT	36

void DrawFooter();
void ShowFullLabel();

void DrawHeader()
{
  int w,h;
  byte curcontrol=cursel+scrly;

  w=tft.width();
  h=tft.height();
  
  tft.fillRect(0,0,w,HEADER_HEIGHT,ControlData[CTRL_MUTE]?ILI9341_BLUE:ILI9341_RED);
  tft.drawRect(0,0,w,HEADER_HEIGHT,ILI9341_YELLOW);

  tft.setTextColor(ILI9341_YELLOW); 
  tft.setCursor(10, HEADER_HEIGHT-FONT_HEIGHT_ADJ+1);
  tft.print(ControlData[CTRL_MUTE]?F("RASA"):F("RASA Mute"));

  DrawFooter();
}

void DrawFooter()
{
  int w=tft.width();
  int h=tft.height();

  tft.setTextColor(ILI9341_YELLOW);
  tft.fillRect(0,h-HEADER_HEIGHT,w,HEADER_HEIGHT,ControlData[CTRL_MUTE]?ILI9341_BLUE:ILI9341_RED);
  tft.drawRect(0,h-HEADER_HEIGHT,w,HEADER_HEIGHT,ILI9341_YELLOW);

  ShowFullLabel();
}

void DrawSelInput()
{
  int x=150,y=3/*,w=46*/,Input=ControlData[CTRL_INPUT];
  tft.fillRect((x-4),y,INPUT_BTN_W*3,HEADER_HEIGHT-6,ILI9341_BLUE);
  tft.fillRect((x-4)+((ControlData[CTRL_INPUT]-1)*INPUT_BTN_W),y,INPUT_BTN_W,HEADER_HEIGHT-6,ILI9341_GREEN);
  y+=16; // For Text 
  tft.setTextColor((Input==1)?ILI9341_BLUE:ILI9341_YELLOW);tft.setCursor(x,  y);tft.print(F("USB"));x+=INPUT_BTN_W;
  tft.setTextColor((Input==2)?ILI9341_BLUE:ILI9341_YELLOW);tft.setCursor(x+8,y);tft.print(F("BT"));x+=INPUT_BTN_W;
  tft.setTextColor((Input==3)?ILI9341_BLUE:ILI9341_YELLOW);tft.setCursor(x,  y);tft.print(F("AUX"));x+=INPUT_BTN_W;
  //tft.setTextColor((Input==4)?ILI9341_BLUE:ILI9341_YELLOW);tft.setCursor(x,  y);tft.print("CD");x+=INPUT_BTN_W;
}

int GetCMax(byte CType)
{
	int cmax=15;// SEL_ATTUNATION_BAR , SEL_PROGRESS_BAR
	if (CType==SEL_VOLUME_BAR) cmax=29;
	else if (CType==SEL_OPTIONS) cmax=3;
	else if (CType==SEL_ON_OFF) cmax=1;
	else if (CType==SEL_TONE_BAR) cmax=30;

	return cmax;
}

void ShowFullLabel()
{
	// Hide the Prev Label
	if (ControlData[CTRL_MUTE]==1)
		tft.setTextColor(ILI9341_BLUE);
	else
		tft.setTextColor(ILI9341_RED);
	// Hide the Prev Label
   	tft.setCursor(FONT_HEIGHT_ADJ,tft.height()-FONT_HEIGHT_ADJ);tft.print(PrevLabel);

	// Show the New Label
	tft.setTextColor(ILI9341_YELLOW);
	tft.setCursor(FONT_HEIGHT_ADJ,tft.height()-FONT_HEIGHT_ADJ);tft.print(Label);

	// Back up the current Label
	strcpy(PrevLabel,Label);
}

void DrawControls(byte key)
{
  int i,j,lab=0,bw,xx,cmax,x,y,w,h;
  byte curcontrol=cursel+scrly;

  x=4;  y=26+10;

  h=tft.height();w=tft.width()-60;

  for(i=scrly;i<(scrly+CONTROLS_PERPAGE);i++)
  {
	  LoadLabel(i);

	  if (i>scrly) y+=CTRL_ROW_HEIGHT;
	  if (ocurcontrol!=200)
	  {
		  if (i==ocurcontrol)
		  {
			  // Paint.
		  }
		  else if (i==curcontrol)
		  {
			  ShowFullLabel();
		  }
		  else
		  {
			continue;
		  }
	  }
	  else
	  {
		  // When Page Changes.
		  if (i==curcontrol) ShowFullLabel();
	  }

	  Serial.print(F("Paiting Row "));
	  Serial.println(i);
	  
	  
	  if (curcontrol==i)
	  {
		  if (curfocussed==i)
		  {
		    tft.fillRect(x,y-2,48,CONTROL_HEIGHT+4,ILI9341_YELLOW);//BG
			tft.drawRect(x,y-2,w+2+50,CONTROL_HEIGHT+4,ILI9341_YELLOW);//BG
		  }
		  else
		  {
			  tft.fillRect(x,y-2,48,CONTROL_HEIGHT+4,ILI9341_GREEN);//BG
			  tft.drawRect(x,y-2,w+2+50,CONTROL_HEIGHT+4,ILI9341_BLACK);//BG
		  }

		  //if (ocurcontrol!=curcontrol) 
			//  DrawFooter();


//****************************************************************

	  tft.setTextColor(ILI9341_BLUE);
	  tft.setCursor(w, h-8);tft.print(SValue);
	  if (CType==SEL_VOLUME_BAR)
	  {
		int v=VolumeLookUp[ControlData[i]];
		if (v>15) {v-=16;v*=-1;}
		sprintf(SValue,"%ddb",v);
		tft.setTextColor(ILI9341_YELLOW);
		tft.setCursor(w, h-8);tft.print(SValue);
	  }
	  else if (CType==SEL_TONE_BAR)
	  {
		sprintf(SValue,"%ddb",ControlData[i]-15);
		tft.setTextColor(ILI9341_YELLOW);
		tft.setCursor(w, h-8);tft.print(SValue);
	  }
	  else if (CType==SEL_ATTUNATION_BAR)
	  {
		sprintf(SValue,"%ddb",ControlData[i]-15);
		tft.setTextColor(ILI9341_YELLOW);
		tft.setCursor(w, h-8);tft.print(SValue);
	  }
	  else if (CType==SEL_PROGRESS_BAR)
	  {
		sprintf(SValue,"%ddb",ControlData[i]);
		tft.setTextColor(ILI9341_YELLOW);
		tft.setCursor(w, h-8);tft.print(SValue);
	  }
	  

//****************************************************************


		  tft.setTextColor(ILI9341_BLACK);
	  }
	  else
	  {
		  tft.drawRect(x,y-2,w+2+50,CONTROL_HEIGHT+4,ILI9341_BLACK);//BG
		  tft.fillRect(x,y-2,48,CONTROL_HEIGHT+4,ILI9341_BLACK);//BG
		  tft.setTextColor(ILI9341_YELLOW);
	  }
	  tft.setCursor(x+6, y+16+2);tft.print(SLabel);



	  // We dont have to paint the Controls when Switching UP DN
	  if (((key==IRKEY_UP)||(key==IRKEY_DN))&&(ocurcontrol!=200)) continue ;

	  //********************
	  tft.fillRect(x+50,y,w,CONTROL_HEIGHT,ILI9341_BLUE);//BG
	  cmax=GetCMax(CType);
	  switch(CType)
	  {
	  case SEL_TONE_BAR:// + / -
				{
					bw=((ControlData[i]-15)*(w-10))/cmax;
					if (bw<0)
						tft.fillRect(x+50+(w/2),y+4,bw,CONTROL_HEIGHT-8,ILI9341_MAGENTA);//VAL
					else
						tft.fillRect(x+50+(w/2),y+4,bw,CONTROL_HEIGHT-8,ILI9341_YELLOW);//VAL
					tft.drawFastVLine(x+50+(w/2),y,CONTROL_HEIGHT,ILI9341_YELLOW);//BG
				}
				break;
	  case SEL_VOLUME_BAR:
		  {
				byte blue=0;
				byte red=0;
				if (ControlData[i]>20)
				{
					blue=20;
					red=ControlData[i]-20;
				}
				else
				{
					blue=ControlData[i];
				}

				bw=(blue*(w-10))/cmax;
				tft.fillRect(x+50+4,y+4,bw,CONTROL_HEIGHT-8,ILI9341_YELLOW);//VAL
				x+=bw;
				bw=(red*(w-10))/cmax;
				tft.fillRect(x+50+4,y+4,bw,CONTROL_HEIGHT-8,ILI9341_MAGENTA);//VAL
				x=4;
				break;
		  }
	  case SEL_ATTUNATION_BAR:
	  case SEL_PROGRESS_BAR:
		  {
				bw=(ControlData[i]*(w-10))/cmax;
				tft.fillRect(x+50+4,y+4,bw,CONTROL_HEIGHT-8,ILI9341_YELLOW);//VAL
				break;
		  }
	  case SEL_OPTIONS:
		  {
			  xx=x+2+50;
			  for(j=0;j<=cmax;j++)
			  {
				if (ControlData[i]==j) 
				{
					tft.fillRect(xx,y,64,CONTROL_HEIGHT,ILI9341_YELLOW);//BG
					tft.setTextColor(ILI9341_BLUE);
				}
				else
				{
					tft.setTextColor(ILI9341_YELLOW);
				}
				tft.setCursor(xx+6, y+16+2);
				tft.print(options[j]);
				xx+=64;
			  }
			  xx=x+2+50;
			  tft.setCursor(xx+6, y+16+2);
			  break;
		  }
	  case SEL_ON_OFF:
		  {
			  xx=x+2+50;
			  for(j=0;j<=cmax;j++)
			  {
				if (ControlData[i]==j) 
				{
					tft.fillRect(xx,y,64,CONTROL_HEIGHT,ILI9341_YELLOW);//BG
					tft.setTextColor(ILI9341_BLUE);
				}
				else
				{
					tft.setTextColor(ILI9341_YELLOW);
				}
				tft.setCursor(xx+6, y+16+2);
				if (j==0) 
					tft.print(F("ON"));
				else
					tft.print(F("OFF"));
				xx+=64;
			  }
			  break;
		  }
	  }
  }

}


void DrawAll()
{
	byte curcontrol=cursel+scrly;
	tft.fillScreen(ILI9341_BLACK);
	LoadLabel(curcontrol);
	DrawHeader();
	DrawSelInput();
	DrawControls(0);
}


void HandleRemoteCommands(byte ch)
{
	byte ok=0,cmax;
	byte curcontrol=cursel+scrly;
	ocurcontrol=curcontrol;

	cmax=GetCMax(CurCType);

	if (curfocussed==curcontrol)
	{
		if (ch==ENCODER_CCW) ch=IRKEY_LEFT;
		if (ch==ENCODER_CW) ch=IRKEY_RIGHT;
	}
	else
	{
		if (ch==ENCODER_CCW) ch=IRKEY_UP;
		if (ch==ENCODER_CW) ch=IRKEY_DN;
	}

	switch(ch) {
		case ENCODER_OK:
		case IRKEY_OK:
				if (curfocussed==200)
					curfocussed=curcontrol;
				else
					curfocussed=200;
				ok=1;
				break;
		case IRKEY_MENU:
			ControlData[CTRL_INPUT]++;if (ControlData[CTRL_INPUT]>IN_MAX) ControlData[CTRL_INPUT]=IN_MIN;
			DrawSelInput();
			ok=2;
			break;
#ifdef WIN32
		case IRKEY_MM:
#endif
		case IRKEY_MUTE:
			ControlData[CTRL_MUTE]++;if (ControlData[CTRL_MUTE]>MUTE_MAX) ControlData[CTRL_MUTE]=MUTE_MIN;
			DrawHeader();
			DrawSelInput();
			ok=2;
			break;

		case IRKEY_DN:
			curfocussed=200;//RESET FOCUSSED
			if (cursel==(CONTROLS_PERPAGE-1))
			{
				if (curcontrol==(CONTROL_COUNT-1)) break;
				//cursel=(CONTROLS_PERPAGE-1);
				//if ((cursel+scrly)<(CONTROL_COUNT-1)) scrly++;

				cursel=0;scrly+=CONTROLS_PERPAGE;
				if ((cursel+scrly+CONTROLS_PERPAGE)>=CONTROL_COUNT) 
				{
					scrly=CONTROL_COUNT-CONTROLS_PERPAGE;
				}

			}
			else
			{
				cursel++;
			}

			ok=1;
			break;
		case IRKEY_UP:
			curfocussed=200;//RESET FOCUSSED
			if (cursel==0) 
			{
				//cursel=0;
				//if (scrly>0) scrly--;
				cursel=CONTROLS_PERPAGE-1;
				scrly-=CONTROLS_PERPAGE;
				if (scrly<0) {scrly=0;cursel=0;}
			}
			else
			{
				cursel--;
			}

			ok=1;
			break;

		case IRKEY_LEFT:
			if (ControlData[curcontrol]>0) ControlData[curcontrol]--;ok=1;break;
		case IRKEY_RIGHT:
			if (ControlData[curcontrol]<cmax) ControlData[curcontrol]++;ok=1;break;
			
		}

	Serial.println(CurCType);	
	Serial.println(cmax);
	Serial.println(ControlData[curcontrol]);

	if (ok) 
	{
		SendCommands();
		if (ok==2)
		{
			return;// already painted.
		}

		if (oscrly!=scrly)
		{
			oscrly=scrly;
			ocurcontrol=200;
			DrawControls(ch);
		}
		else
		{
			DrawControls(ch);
		}

		
	}
}


void setup() {
	Serial.begin(9600);

	//int x,y,w,CONTROL_HEIGHT;

#ifdef NEED_KEYS
	SetupSwitches();
	PRINTLN_F("Keys Ready");
#endif

//***********************************************************************************

	if (digitalRead(ENCODER_PIN_SW)==0)
	{
		PRINTLN_F("DEFAULT VALUES");
	}
	else
	{
		PRINTLN_F("Loading Saved");
		LoadSettings();
	}

#ifdef NEED_CROSSOVER
  memset(PControlData,0xff,sizeof(PControlData));
  SendCommands();

  // To prevent the first change from writing all values
  memcpy(PControlData,ControlData,sizeof(PControlData));

  PRINTLN_F("Crossover Ready");
  IsDirty=0;
#endif

//***********************************************************************************
	
#ifndef WIN32
	pinMode(TFT_CS, OUTPUT);
	pinMode(TFT_DC, OUTPUT);

	tft.begin(10000000L);
	tft.setRotation(3);
#endif
	tft.setFont(&FreeSans9pt7b);
	DrawAll();  

//***********************************************************************************
  
  PRINTLN_F("RASA DSP Ready...");

#ifdef NEED_REMOTE
  irrecv.enableIRIn(); // Start the receiver  
  PRINTLN_F("IR Remote Ready");
#endif


}
byte OldKey=0;

void loop() {
	stime = millis();
	if (oldtime==0) oldtime=stime;
	if ((stime-oldtime)>2000)
	{
		oldtime=stime;
		if (IsDirty)	
		{
			IsDirty=0;
			SaveSettings();
		}
	}

#ifdef WIN32
	{
		int ch;
		if (MFC_kbhit()) 
		{
			ch=MFC_getch();
			HandleRemoteCommands(ch);
		}
	}
#endif

	
	for(;;) 
	{
		if (Serial.available()) 
			ReadSerialData();
		else
			break;
	}
#ifdef NEED_KEYS

	byte key=ReadKey();
	if (OldKey!=key)
	{
		OldKey=key;
		if (key>0)
		{
			Serial.println(key);
			if (key==1) HandleRemoteCommands(IRKEY_OK);
		}
	}
  
#endif

#ifdef NEED_ENCODER

  static int pos = 0;
  encoder.tick();

  int newPos = encoder.getPosition();
  if (pos != newPos) {

    //Serial.println(newPos);

	//if (pos>newPos) Serial.print(" CW ");
	//if (pos<newPos) Serial.print(" CCW ");

	if (pos>newPos) DebounceDir++;
	if (pos<newPos) DebounceDir--;

	if (DebounceDir==1) 
	{	
		PRINTLN_F("[ENCODER CW]");DebounceDir=0;
		HandleRemoteCommands(ENCODER_CW);
	}
	if (DebounceDir==-1) 
	{
		PRINTLN_F("[ENCODER CCW]");DebounceDir=0;
		HandleRemoteCommands(ENCODER_CCW);
	}

    pos = newPos;
  } // if

#endif

	

#ifdef NEED_REMOTE
  if (irrecv.decode(&results)) 
  {
	  // This is where you can call the Keys based on your custom Remote.
	  switch(results.value)
	  {
		/*
		case 0x609E6897:HandleRemoteCommands(IRKEY_UP);break;
		case 0x609E0AF5:HandleRemoteCommands(IRKEY_DN);break;
		case 0x609E2AD5:HandleRemoteCommands(IRKEY_LEFT);break;
		case 0x609EA857:HandleRemoteCommands(IRKEY_RIGHT);break;
		case 0x609E48B7:HandleRemoteCommands(IRKEY_OK);break;
		case 0x609E00FF:HandleRemoteCommands(IRKEY_MENU);break;
		case 0x609E38C7:HandleRemoteCommands(IRKEY_MUTE);break;
		*/

		case 0x9EB92:HandleRemoteCommands(IRKEY_UP);break;
		case 0x5EB92:HandleRemoteCommands(IRKEY_DN);break;
		case 0xDEB92:HandleRemoteCommands(IRKEY_LEFT);break;
		case 0x3EB92:HandleRemoteCommands(IRKEY_RIGHT);break;
		case 0xD0B92:HandleRemoteCommands(IRKEY_OK);break;
		case 0xD8B92:HandleRemoteCommands(IRKEY_MENU);break;
		case 0x26B92:HandleRemoteCommands(IRKEY_MUTE);break;

	  }

    Serial.println(results.value, HEX);
    dump(&results);
    irrecv.resume(); // Receive the next value
  }
#endif
}


char hex2asc(byte ch)
{
	if ((ch>=0)&&(ch<=9)) return (ch+'0');
	if ((ch>=10)&&(ch<=15)) return (ch+'A'-10);
	return '0';
}


byte asc2hex(char ch)
{
	if ((ch>='0')&&(ch<='9')) return (ch-'0');
	if ((ch>='A')&&(ch<='F')) return (ch-'A'+10);
	if ((ch>='a')&&(ch<='f')) return (ch-'a'+10);
	return 0;
}

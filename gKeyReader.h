
byte SwitchCount[10];
#ifndef DEBOUNCE_COUNT
	#define DEBOUNCE_COUNT 5 /* 100 ms in main loop*/
#endif
//****************************************************************************************
// Key Read from A0 - A6
//****************************************************************************************

void SetupSwitches()
{
	byte idx=0;
	for(idx=0;idx<sizeof(KEY_PINS);idx++)
	{
		pinMode(KEY_PINS[idx], INPUT);	//
		//Input Pins
		#ifdef KEYS_HI_IS_ON
			digitalWrite(KEY_PINS[idx], LOW);	//
		#else
			digitalWrite(KEY_PINS[idx], HIGH);	//
		#endif
	}
}

byte ReadKey()
{
/*

#ifdef WIN32

	if (_kbhit())
	{
		byte b=_getch();

		if (b==13)	return 'M';
		if (b==224)
		{
			b=_getch();
			if (b==72) return 'I';
			if (b==80) return 'J';
			if (b==75) return 'K';
			if (b==77) return 'L';

		}

		if ((b>='1')&&(b<='9')) return b-'0';

		return b;
	}

	return 0;

#endif
*/

	byte idx=0;
	for(idx=0;idx<sizeof(KEY_PINS);idx++)
	{
	#ifdef KEYS_HI_IS_ON
		if (digitalRead(KEY_PINS[idx])==HIGH) 
	#else
		if (digitalRead(KEY_PINS[idx])==LOW) 
	#endif
		{
			if (SwitchCount[idx]==DEBOUNCE_COUNT)
			{
				return idx+1;
			}
			else
			{
				SwitchCount[idx]++;
			}
		}
		else 
		{
			SwitchCount[idx]=0;
		}
	}
	
	return 0;
}


byte ReadKeyDirect()
{
	byte idx=0;
	for(idx=0;idx<sizeof(KEY_PINS);idx++)
	{
	#ifdef KEYS_HI_IS_ON
		if (digitalRead(KEY_PINS[idx])==HIGH) 
	#else
		if (digitalRead(KEY_PINS[idx])==LOW) 
	#endif
		{
			return idx+1;
		}
	}
	return 0;
}

#include <Arduino.h>
#include <Wire.h>
#include "TDA7419.h"

TDA7419::TDA7419(){
	Wire.begin();
}

void TDA7419::setInput(int input, int gain, int auto_z){
  switch (input) {
    case 0: input = 0b00000000;break;
	case 1: input = 0b00000001;break;
	case 2: input = 0b00000010;break;
	case 3: input = 0b00000011;break;
	case 4: input = 0b00000100;break;
	case 5: input = 0b00000101;break;
	}
  switch (gain) {
        case  0: gain = 0b00000000;break;
        case  1: gain = 0b00001000;break;
        case  2: gain = 0b00010000;break;
        case  3: gain = 0b00011000;break;
        case  4: gain = 0b00100000;break;
        case  5: gain = 0b00101000;break;
        case  6: gain = 0b00110000;break;
        case  7: gain = 0b00111000;break;
        case  8: gain = 0b01000000;break;
        case  9: gain = 0b01001000;break;
        case 10: gain = 0b01010000;break;
        case 11: gain = 0b01011000;break;
        case 12: gain = 0b01100000;break;
        case 13: gain = 0b01101000;break;
        case 14: gain = 0b01110000;break;
        case 15: gain = 0b01111000;break;
	} 
	switch (auto_z) {
        case 0: auto_z = 0b00000000;break;
	case 1: auto_z = 0b10000000;break;
	}
	int selector = input + gain + auto_z;
  writeWire(TDA7419_INPUT, selector);
}


void TDA7419::setAtt_loudness(int loud_att, int center_f, int h_boost, int loud_step){
  switch (loud_att) {
        case  0: loud_att = 0b00000000;break;
        case  1: loud_att = 0b00000001;break;
        case  2: loud_att = 0b00000010;break;
        case  3: loud_att = 0b00000011;break;
        case  4: loud_att = 0b00000100;break;
        case  5: loud_att = 0b00000101;break;
        case  6: loud_att = 0b00000110;break;
        case  7: loud_att = 0b00000111;break;
        case  8: loud_att = 0b00001000;break;
        case  9: loud_att = 0b00001001;break;
        case 10: loud_att = 0b00001010;break;
        case 11: loud_att = 0b00001011;break;
        case 12: loud_att = 0b00001100;break;
        case 13: loud_att = 0b00001101;break;
        case 14: loud_att = 0b00001110;break;
        case 15: loud_att = 0b00001111;break;
	} 
  switch (center_f) {
        case  0: center_f = 0b00000000;break;
        case  1: center_f = 0b00010000;break;
        case  2: center_f = 0b00100000;break;
        case  3: center_f = 0b00110000;break;
	} 
  switch (h_boost) {
        case  0: h_boost = 0b00000000;break;
        case  1: h_boost = 0b01000000;break;
	} 
  switch (loud_step) {
        case  0: loud_step = 0b00000000;break;
        case  1: loud_step = 0b10000000;break;
        }	
        int main_loudness = loud_att + center_f + h_boost + loud_step;
  writeWire(TDA7419_LOUDNESS, main_loudness);
}


void TDA7419::setSoft(int s_mute, int pin, int s_m_time, int s_time, int clock){
  switch (s_mute) {
        case  0:  s_mute = 0b00000000;break;
        case  1:  s_mute = 0b00000001;break;
        }  
  switch (pin) {
        case  0:  pin = 0b00000000;break;
        case  1:  pin = 0b00000010;break;
        } 
  switch (s_m_time) {
        case  0:  s_m_time = 0b00000000;break;
        case  1:  s_m_time = 0b00000100;break;
        case  2:  s_m_time = 0b00001000;break;
        }  
  switch (s_time) {
        case  0:  s_time = 0b00000000;break;
        case  1:  s_time = 0b00010000;break;
        case  2:  s_time = 0b00100000;break;
        case  3:  s_time = 0b00110000;break;
        case  4:  s_time = 0b01000000;break;
        case  5:  s_time = 0b01010000;break;
        case  6:  s_time = 0b01100000;break;
        case  7:  s_time = 0b01110000;break;
        }  
  switch (clock) {
        case  0:  clock = 0b00000000;break;
        case  1:  clock = 0b10000000;break;
        } 
        int soft_mute_gen = s_mute + pin + s_m_time + s_time + clock;
        writeWire(TDA7419_SOFT, soft_mute_gen);
}
  
void TDA7419::setVolumeNew(int reg,int val)
{
//case   0b10000000;break; // Soft
	val&=0b01111111; // Safety
	writeWire(reg, val);
}

void TDA7419::setVolume(int volume, int soft0){
   if(volume >= 0){} // 0 ... +15 dB
  if(volume < 0){volume = abs(volume) + 16;} //-79 ... 0 dB
  switch (soft0) {
  case  0:  soft0 = 0b00000000;break;
  case  1:  soft0 = 0b10000000;break;
  }
  writeWire(TDA7419_VOLUME, volume + soft0);
}

void TDA7419::setAtt_LF(int left_f, int soft1){
//  left_f = 95 - left_f;
   if(left_f >= 0){} // 0 ... +15 dB
  if(left_f < 0){left_f = abs(left_f) + 16;} //-79 ... 0 dB

  switch (soft1) {
  case  0:  soft1 = 0b00000000;break;
  case  1:  soft1 = 0b10000000;break;
  }
  writeWire(TDA7419_ATT_LF, left_f + soft1);
}

void TDA7419::setAtt_RF(int right_f, int soft2){
//  right_f = 95 - right_f;
   if(right_f >= 0){} // 0 ... +15 dB
  if(right_f < 0){right_f = abs(right_f) + 16;} //-79 ... 0 dB

  switch (soft2) {
  case  0:  soft2 = 0b00000000;break;
  case  1:  soft2 = 0b10000000;break;
  }
  writeWire(TDA7419_ATT_RF, right_f + soft2);
}

void TDA7419::setAtt_LT(int left_t, int soft3){
//  left_t = 95 - left_t;
   if(left_t >= 0){} // 0 ... +15 dB
  if(left_t < 0){left_t = abs(left_t) + 16;} //-79 ... 0 dB

  switch (soft3) {
  case  0:  soft3 = 0b00000000;break;
  case  1:  soft3 = 0b10000000;break;
  }
  writeWire(TDA7419_ATT_LT, left_t + soft3);
}

void TDA7419::setAtt_RT(int right_t, int soft4){
//  right_t = 95 - right_t;
   if(right_t >= 0){} // 0 ... +15 dB
  if(right_t < 0){right_t = abs(right_t) + 16;} //-79 ... 0 dB

  switch (soft4) {
  case  0:  soft4 = 0b00000000;break;
  case  1:  soft4 = 0b10000000;break;
  }
  writeWire(TDA7419_ATT_RT, right_t + soft4);
}

void TDA7419::setAtt_SUB(int subwoofer, int soft5){
//  subwoofer = 95 - subwoofer;
   if(subwoofer >= 0){} // 0 ... +15 dB
  if(subwoofer < 0){subwoofer = abs(subwoofer) + 16;} //-79 ... 0 dB

  switch (soft5) {
  case  0:  soft5 = 0b00000000;break;
  case  1:  soft5 = 0b10000000;break;
  }
  writeWire(TDA7419_ATT_SUB, subwoofer + soft5);
}

void TDA7419::setAtt_Mix(int mixing, int soft6){
//  mixing = 95 - mixing;
   if(mixing >= 0){} // 0 ... +15 dB
  if(mixing < 0){mixing = abs(mixing) + 16;} //-79 ... 0 dB

  switch (soft6) {
  case  0:  soft6 = 0b00000000;break;
  case  1:  soft6 = 0b10000000;break;
  }
  writeWire(TDA7419_MIX_LEV, mixing + soft6);
}


void TDA7419::setFilter_Treble(int treble_g, int treble_c, int treble_ref) {
  switch (treble_g) {
        case  -1: treble_g  = 0b00000001;break;
        case  -2: treble_g  = 0b00000010;break;
        case  -3: treble_g  = 0b00000011;break;
        case  -4: treble_g  = 0b00000100;break;
        case  -5: treble_g  = 0b00000101;break;
        case  -6: treble_g  = 0b00000110;break;
        case  -7: treble_g  = 0b00000111;break;
        case  -8: treble_g  = 0b00001000;break;
        case  -9: treble_g  = 0b00001001;break;
        case  -10: treble_g = 0b00001010;break;
        case  -11: treble_g = 0b00001011;break;
        case  -12: treble_g = 0b00001100;break;
        case  -13: treble_g = 0b00001101;break;
        case  -14: treble_g = 0b00001110;break;
        case  -15: treble_g = 0b00001111;break;
        
        case  0: treble_g   = 0b00000000;break;
        case  1: treble_g   = 0b00010001;break;
        case  2: treble_g   = 0b00010010;break;
        case  3: treble_g   = 0b00010011;break;
        case  4: treble_g   = 0b00010100;break;
        case  5: treble_g   = 0b00010101;break;
        case  6: treble_g   = 0b00010110;break;
        case  7: treble_g   = 0b00010111;break;
        case  8: treble_g   = 0b00011000;break;
        case  9: treble_g   = 0b00011001;break;
        case  10: treble_g  = 0b00011010;break;
        case  11: treble_g  = 0b00011011;break;
        case  12: treble_g  = 0b00011100;break;
        case  13: treble_g  = 0b00011101;break;
        case  14: treble_g  = 0b00011110;break;
        case  15: treble_g  = 0b00011111;break;     
	} 
  switch (treble_c) {
        case  0:  treble_c = 0b00000000;break;
        case  1:  treble_c = 0b00100000;break;
        case  2:  treble_c = 0b01000000;break;
        case  3:  treble_c = 0b01100000;break;
        }
  switch (treble_ref) {
        case  0:  treble_ref = 0b00000000;break;
        case  1:  treble_ref = 0b10000000;break;   
        }
        int treble_sum = treble_g + treble_c + treble_ref;   
  writeWire(TDA7419_TREBLE, treble_sum);
}


void TDA7419::setFilter_Middle(int middle_g, int middle_q, int middle_s) {
  switch (middle_g) {
        case  -1: middle_g = 0b00000001;break;
        case  -2: middle_g = 0b00000010;break;
        case  -3: middle_g = 0b00000011;break;
        case  -4: middle_g = 0b00000100;break;
        case  -5: middle_g = 0b00000101;break;
        case  -6: middle_g = 0b00000110;break;
        case  -7: middle_g = 0b00000111;break;
        case  -8: middle_g = 0b00001000;break;
        case  -9: middle_g = 0b00001001;break;
        case  -10: middle_g = 0b00001010;break;
        case  -11: middle_g = 0b00001011;break;
        case  -12: middle_g = 0b00001100;break;
        case  -13: middle_g = 0b00001101;break;
        case  -14: middle_g = 0b00001110;break;
        case  -15: middle_g = 0b00001111;break;
  
        case  0: middle_g = 0b00000000;break;
        case  1: middle_g = 0b00010001;break;
        case  2: middle_g = 0b00010010;break;
        case  3: middle_g = 0b00010011;break;
        case  4: middle_g = 0b00010100;break;
        case  5: middle_g = 0b00010101;break;
        case  6: middle_g = 0b00010110;break;
        case  7: middle_g = 0b00010111;break;
        case  8: middle_g = 0b00011000;break;
        case  9: middle_g = 0b00011001;break;
        case  10: middle_g = 0b00011010;break;
        case  11: middle_g = 0b00011011;break;
        case  12: middle_g  = 0b00011100;break;
        case  13: middle_g  = 0b00011101;break;
        case  14: middle_g  = 0b00011110;break;
        case  15: middle_g  = 0b00011111;break;  
	} 
	switch (middle_q) {
        case  0: middle_q = 0b00000000;break;
        case  1: middle_q = 0b00100000;break;
        case  2: middle_q = 0b01000000;break;
        case  3: middle_q = 0b01100000;break;
        }
        switch (middle_s) {
        case  0: middle_s = 0b00000000;break;
        case  1: middle_s = 0b10000000;break;
        }
        int middle_sum = middle_g + middle_s + middle_s;
  writeWire(TDA7419_MIDDLE , middle_sum);
}

void TDA7419::setFilter_Bass(int bass_g, int bass_q, int bass_s) {
        switch (bass_g) {
        case  -1: bass_g = 0b00000001;break;
        case  -2: bass_g = 0b00000010;break;
        case  -3: bass_g = 0b00000011;break;
        case  -4: bass_g = 0b00000100;break;
        case  -5: bass_g = 0b00000101;break;
        case  -6: bass_g = 0b00000110;break;
        case  -7: bass_g = 0b00000111;break;
        case  -8: bass_g = 0b00001000;break;
        case  -9: bass_g = 0b00001001;break;
        case  -10: bass_g = 0b00001010;break;
        case  -11: bass_g = 0b00001011;break;
        case  -12: bass_g = 0b00001100;break;
        case  -13: bass_g = 0b00001101;break;
        case  -14: bass_g = 0b00001110;break;
        case  -15: bass_g = 0b00001111;break;
  
        case  0: bass_g = 0b00000000;break;
        case  1: bass_g = 0b00010001;break;
        case  2: bass_g = 0b00010010;break;
        case  3: bass_g = 0b00010011;break;
        case  4: bass_g = 0b00010100;break;
        case  5: bass_g = 0b00010101;break;
        case  6: bass_g = 0b00010110;break;
        case  7: bass_g = 0b00010111;break;
        case  8: bass_g = 0b00011000;break;
        case  9: bass_g = 0b00011001;break;
        case  10: bass_g = 0b00011010;break;
        case  11: bass_g = 0b00011011;break;
        case  12: bass_g = 0b00011100;break;
        case  13: bass_g = 0b00011101;break;
        case  14: bass_g = 0b00011110;break;
        case  15: bass_g = 0b00011111;break;     
	} 
	switch (bass_q) {
        case  0: bass_q = 0b00000000;break;
        case  1: bass_q = 0b00100000;break;
        case  2: bass_q = 0b01000000;break;
        case  3: bass_q = 0b01100000;break;
        }
        switch (bass_s) {
        case  0: bass_s = 0b00000000;break;
        case  1: bass_s = 0b10000000;break;
        }
         int bass_sum = bass_g + bass_s + bass_s;
  writeWire(TDA7419_BASS , bass_sum);
        }
        
        
void TDA7419::setInput2(int input2, int gain2, int auto_z2){
  switch (input2) {
        case 0: input2 = 0b00000000;break;
	case 1: input2 = 0b00000001;break;
	case 2: input2 = 0b00000010;break;
	case 3: input2 = 0b00000011;break;
	case 4: input2 = 0b00000100;break;
	case 5: input2 = 0b00000101;break;
	}
  switch (gain2) {
        case  0: gain2 = 0b00000000;break;
        case  1: gain2 = 0b00001000;break;
        case  2: gain2 = 0b00010000;break;
        case  3: gain2 = 0b00011000;break;
        case  4: gain2 = 0b00100000;break;
        case  5: gain2 = 0b00101000;break;
        case  6: gain2 = 0b00110000;break;
        case  7: gain2 = 0b00111000;break;
        case  8: gain2 = 0b01000000;break;
        case  9: gain2 = 0b01001000;break;
        case 10: gain2 = 0b01010000;break;
        case 11: gain2 = 0b01011000;break;
        case 12: gain2 = 0b01100000;break;
        case 13: gain2 = 0b01101000;break;
        case 14: gain2 = 0b01110000;break;
        case 15: gain2 = 0b01111000;break;
	} 
	switch (auto_z2) {
        case 0: auto_z2 = 0b00000000;break;
	case 1: auto_z2 = 0b10000000;break;
	}
	int selector2 = input2 + gain2 + auto_z2;
  writeWire(TDA7419_INPUT2, selector2);
}


void TDA7419::setSub_M_B(int sub, int mid, int bas, int bas_d, int s_filter){
  switch (sub) {
        case  0: sub = 0b00000000;break;//flat
        case  1: sub = 0b00000001;break;//80 Hz
        case  2: sub = 0b00000010;break;//120 Hz
        case  3: sub = 0b00000011;break;//160 Hz
        }
   switch (mid) {    
        case  0: mid = 0b00000000;break;//500 Hz
        case  1: mid = 0b00000100;break;//1000 Hz
        case  2: mid = 0b00001000;break;//1500 Hz
        case  3: mid = 0b00001100;break;//2500 Hz
        }
   switch (bas) {    
        case  0: bas = 0b00000000;break;//60 Hz
        case  1: bas = 0b00010000;break;//80 Hz
        case  2: bas = 0b00100000;break;//100 Hz
        case  3: bas = 0b01100000;break;//200 Hz   
	} 
   switch (bas_d) {
        case  0: bas_d = 0b00000000;break;//bass dc on
        case  1: bas_d = 0b01000000;break;//bass dc off
        }
   switch (s_filter) {
        case  0: bas_d = 0b00000000;break;//bass dc on
        case  1: bas_d = 0b01000000;break;//bass dc off
        }     
	int smb = sub + mid + bas + bas_d + s_filter;
  writeWire(TDA7419_SUB_M_B, smb);
}

void TDA7419::setMix_Gain_Eff(int mix_l, int mix_r, int mix_en, int sub_en, int gain_eff){
        switch (mix_l) {    
        case  0: mix_l = 0b00000000;break;
        case  1: mix_l = 0b00000001;break;
        }
        switch (mix_r) {    
        case  0: mix_r = 0b00000000;break;
        case  1: mix_r = 0b00000010;break;
        }
        switch (mix_en) {    
        case  0: mix_en = 0b00000000;break;
        case  1: mix_en = 0b00000100;break;
        }
        switch (sub_en) {    
        case  0: sub_en = 0b00000000;break;
        case  1: sub_en = 0b00001000;break;
        }
        switch (gain_eff) {    
        case  0: gain_eff = 0b11000000;break;
        case  1: gain_eff = 0b00000000;break;
        case  2: gain_eff = 0b00010000;break;
        case  3: gain_eff = 0b00100000;break;
        case  4: gain_eff = 0b00110000;break;
        case  5: gain_eff = 0b01000000;break;
        case  6: gain_eff = 0b01010000;break;
        case  7: gain_eff = 0b01100000;break;
        case  8: gain_eff = 0b01110000;break;
        case  9: gain_eff = 0b10000000;break;
        case  10: gain_eff = 0b10010000;break;
        }
        int mix_sum = mix_l + mix_r + mix_en + sub_en + gain_eff;
  writeWire(TDA7419_MIX_G_EFF, mix_sum);
        }

void TDA7419::setSpektor(int spek_q, int spek_res, int spek_an_s, int spek_an_r, int spek_reset, int spek_cl, int spek_mode){
        switch (spek_q) {    
        case  0: spek_q = 0b00000000;break;
        case  1: spek_q = 0b00000001;break;
        }
        switch (spek_res) {    
        case  0: spek_res = 0b00000000;break;
        case  1: spek_res = 0b00000010;break;
        }
        switch (spek_an_s) {    
        case  0: spek_an_s = 0b00000000;break;
        case  1: spek_an_s = 0b00000100;break;
        }
        switch (spek_an_r) {    
        case  0: spek_an_r = 0b00000000;break;
        case  1: spek_an_r = 0b00001000;break;
        }
        switch (spek_reset) {    
        case  0: spek_reset = 0b00000000;break;
        case  1: spek_reset = 0b00010000;break;
        }
        switch (spek_cl) {    
        case  0: spek_cl = 0b00000000;break;
        case  1: spek_cl = 0b00100000;break;
        }
        switch (spek_mode) {    
        case  0: spek_mode = 0b00000000;break;
        case  1: spek_mode = 0b01000000;break;
        case  2: spek_mode = 0b10000000;break;
        case  3: spek_mode = 0b11000000;break;
        }
        int spek_sum = spek_q + spek_res + spek_an_s + spek_an_r + spek_reset + spek_cl + spek_mode;
  writeWire(TDA7419_SPECTRUM, spek_sum);
}

void TDA7419::writeWire(char a, char b){
  Wire.beginTransmission(TDA7419_address);
  Wire.write (a);
  Wire.write (b);
  Wire.endTransmission();
}

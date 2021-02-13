// Алксандр Лиман
// liman324@yandex.ru
// Пример использования - https://rcl-radio.ru/?p=57658

#ifndef TDA7419_H
#define TDA7419_H
// address 7 bit
#define TDA7419_address 0x44 // HEX 88 > 0b10001000 > 7 bit = 0b1000100 = 0x44

//Sub addresses
#define TDA7419_INPUT        0b01000000 // Main Source Selector
#define TDA7419_LOUDNESS     0b01000001 // Main Loudness
#define TDA7419_SOFT         0b01000010 // Soft Mute / Clock Generator
#define TDA7419_VOLUME       0b01000011 // Volume
#define TDA7419_TREBLE       0b01000100 // Treble
#define TDA7419_MIDDLE       0b01000101 // Middle
#define TDA7419_BASS         0b01000110 // Bass
#define TDA7419_INPUT2       0b01000111 // Second Source Selector
#define TDA7419_SUB_M_B      0b01001000 // Subwoofer / Middle / Bass
#define TDA7419_MIX_G_EFF    0b01001001 // Mixing / Gain / Effect
#define TDA7419_ATT_LF       0b01001010 // Speaker Attenuator Left Front
#define TDA7419_ATT_RF       0b01001011 // Speaker Attenuator Right Front
#define TDA7419_ATT_LT       0b01001100 // Speaker Attenuator Left Rear
#define TDA7419_ATT_RT       0b01001101 // Speaker Attenuator Right Rear
#define TDA7419_MIX_LEV      0b01001110 // Mixing Level Control
#define TDA7419_ATT_SUB      0b01001111 // Subwoofer Attenuator
#define TDA7419_SPECTRUM     0b01010000 // Spectrum Analyzer / Clock Source / AC Mode

#include <Arduino.h>
class TDA7419
{
  public:
    TDA7419();
     /// Main Source Selector ///             
        void setInput(int input, int gain, int auto_z); // int (Source Selector, Input Gain, Auto Zero)  
        //Source Selector:      0...5 (QD/SE: QD, SE1, SE2, SE3, QD/SE: SE, mute)
        //Input Gain:           0...15 (0...+15 dB)
        //Auto Zero:            0...1 (on, off)
        
     /// Main Loudness /// 
	void setAtt_loudness(int loud_att, int center_f, int h_boost, int loud_step);  // (Attenuation, Center Frequency, High Boost, Loudness Soft Step)
        //Attenuation:          0...15 (0...-15 dB)
        //Center Frequency:     0...3 (Flat, 400 Hz, 800 Hz, 2400 Hz)
        //High Boost:           0...1 (on, off)
        //Loudness Soft Step:   0...1 (on, off)
     
     ///  Soft Mute / Clock Generator ///
        void setSoft(int s_mute, int pin, int s_m_time, int s_time, int clock); // (Soft Mute, Pin Influence for Mute, Soft Mute Time, Soft Step Time, Clock Fast Mode)
        //Soft Mute:              0...1 (on, off)
        //Pin Influence for Mute: 0...1 (Pin and IIC, IIC)
        //Soft Mute Time:         0...2 (0.48ms, 0.96ms, 123ms)
        //Soft Step Time:         0...7 (0.160ms, 0.321ms, 0.642ms, 1.28ms, 2.26ms, 5.12ms, 10.24ms, 20.48ms)
        //Clock Fast Mode:        0...1 (on, off)
     
     /// Volume (Gain/Attenuation, Soft Step) ///
		void setVolumeNew(int reg,int val);
        void setVolume(int volume, int soft0);     // Volume  
        void setAtt_LF(int left_f, int soft1);     // Volume   LF
        void setAtt_RF(int right_f, int soft2);    // Volume   RF
        void setAtt_LT(int left_t, int soft3);     // Volume   LT
        void setAtt_RT(int right_t, int soft4);    // Volume   RT
        void setAtt_SUB(int subwoofer, int soft5); // Volume   SUBWOOFER
        void setAtt_Mix(int mixing, int soft6);    // Volume   MIXING
        //Gain/Attenuation:       0...80 (80...0) 0 - mute
        //Soft Step:              0...1 (on, off)              
        
        
        
     /// Filter ///
        void setFilter_Treble(int treble_g, int treble_c, int treble_ref);      
            //  (-15...15, 0...3, 0...1) (-15 dB....15 dB, 10kHz 12.5kHz 15 kHz 17.5 kHz , Vref = 4V Vref = 3.3V )
            //(Gain/Attenuation, Treble Center Frequency, Reference Output Select) 
       
        void setFilter_Middle(int middle_g, int middle_q, int middle_s );
            // (-15...15, 0...3, 0...1) (-15 dB....15 dB, 0.5 0.75 1 1.25, on off)      
            // (Gain/Attenuation, Middle Q Factor, Middle Soft Step)
            
        void setFilter_Bass(int bass_g, int bass_q, int bass_s);           
        //Gain/Attenuation: -15...15 db (int -15...15)
        //Bass Q Factor: 1.0 1.25 1.5 2.0 (int 0...3)
        // Bass Soft Step: on/off (int 0...1)
      
      ///  Second Source Selector ///             
        void setInput2(int input2, int gain2, int auto_z2); // int (Source Selector, Input Gain, Auto Zero)  
        //Source Selector:      0...5 (QD/SE: QD, SE1, SE2, SE3, QD/SE: SE, mute)
        //Input Gain:           0...15 (0...+15 dB)
        //Auto Zero:            0...1 (on, off)
      
      /// Subwoofer /Middle / Bass ///
        void setSub_M_B(int sub, int mid, int bas, int bas_d, int s_filter); // (0...3, 0...3, 0...1, 0...1)
        //  (int sub) Subwoofer Cut-off Frequency:  flat, 80Hz, 120Hz, 160Hz
        //  (int mid) Middle Center Frequency:  500Hz, 1000Hz, 1500Hz, 2500Hz
        //  (int bas) Bass Center Frequency:  60Hz, 80Hz, 100Hz, 200Hz
        
        //  (int bas_d)  Bass DC Mode: on, off
        //  (int s_filter)  Smoothing Filter:  on, off (bypass)
        
        void setMix_Gain_Eff(int mix_l, int mix_r, int mix_en, int sub_en, int gain_eff); //(on/off, on/off, on/off, on/off, 0...10 = 0...22 dB)
        
        void setSpektor(int spek_q, int spek_res, int spek_an_s, int spek_an_r, int spek_reset, int spek_cl, int spek_mode);
        // on/off, on/off, on/off, on/off, on/off, on/off, 0...3

  private:
	void writeWire(char a, char b);
};
	
#endif //TDA7419_H

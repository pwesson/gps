#ifndef uBlox_NEO_M9N_h
  #define uBlox_NEO_M9N_h

  #include "Arduino.h"

  class uBlox_NEO_M9N{
	public:
		uBlox_NEO_M9N();
		void setSerial(HardwareSerial *serialPort);
		void listen();
            void read(char);
            void messageGGA(char*);
		void messageRMC(char*);
            bool CheckSum(char*);
		int Hex2Dec(char);
            void parseString(char*);
		void SelectSentences();
		void AllSentences();
		void SelectGGAonly();
		float DegreeToDecimal(float, byte);
               
  		HardwareSerial *gpsSerial;
		float gpstime;
		float gpsdate;
		float latitude;
            float longitude;
            float altitude;
		float gpsknots;
            float gpstrack;

		char latNS, lonEW;
		char gpsstatus;
		int fixquality;
		int numsatelites;
                
	private:
		volatile int ptr = 0;
		volatile bool flag = true;
		volatile char redbuffer[120];
		volatile char blubuffer[120];
};

#endif
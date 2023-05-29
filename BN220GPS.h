#ifndef BN220GPS_h
  #define BN220GPS_h
  
  #include <Arduino.h>

  class BN220GPS{
	public:
		BN220GPS();
		void setSerial(HardwareSerial *serialPort);
		void listen();
            void read(char);
            void messageGGA(char*);
		void messageRMC(char*);
            bool CheckSum(char*);
		void AllSentences();
		void SelectSentences();
 		void SelectGGAonly();
		void SelectMode(int);
		int Hex2Dec(char);
            void parseString(char*);
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
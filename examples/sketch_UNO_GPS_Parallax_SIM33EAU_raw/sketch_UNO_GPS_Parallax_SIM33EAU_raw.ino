// Arduino UNO and the Parallax SIM33EAU
// Copyright (C) 2021 https://www.roboticboat.uk
// 6c16d072-a03d-4c8c-93f3-6a2e1a6b1d11
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
// These Terms shall be governed and construed in accordance with the laws of 
// England and Wales, without regard to its conflict of law provisions.


//The UNO needs the software emulator of the serial port
#include <SoftwareSerial.h>

// Global variables
String inputMessage = "";        // A string to hold incoming data
boolean IsMessageReady = false;  // Whether the string is complete

// gpsSerial(receive from GPS,transmit to the GPS module)
SoftwareSerial gpsSerial(2,3);

void setup()
{
  // Keep the User informed
  Serial.begin(9600);
  Serial.println("Initializing GPS");
  
  //Receive from the GPS device (the NMEA sentences) - Green wire
  pinMode(2, INPUT);   

  //Transmit to the GPS device - Yellow wire
  pinMode(3, OUTPUT);  

  // Connect to the GPS module
  gpsSerial.begin(9600);

  delay(1000);

  AllSentences();
  
  // Reserve 200 bytes for the SoftwareSerial string
  inputMessage.reserve(200);
}

void loop()
{
  while (gpsSerial.available() && IsMessageReady == false) 
  {
     // Read the new byte:
     char nextChar = (char)gpsSerial.read();
     
     // Append to the inputSerial1 string
     inputMessage += nextChar;
     
     // If a newline, then set flag so that the main loop will process the string
     if (nextChar == '\n') {
       IsMessageReady = true;
     }
   }
   
    // Does SoftwareSeria1 have a new message?
   if (IsMessageReady) 
   {
     // Print new message on a new line. 
     // The last character of the SoftwareSerial is a new line
     Serial.print(inputMessage);
     
     // clear the string:
     inputMessage = "";
     IsMessageReady = false;
   }
}

void AllSentences()
{
  // Turn-off Static mode
  // PMTK_API_SET_STATIC_NAV_THD
  // Command in the MT3337 Platform NMEA Message Specification_V1.00
  gpsSerial.println("$PMTK386,0*23");
  delay(1000);

  // Select output sentences
  // PMTK_API_SET_MNEA_OUTPUT
  // Supported NMEA Sentences: 
  //    0 NMEA_SEN_GLL,  // GPGLL interval - Geographic Latitude longitude  
  //    1 NMEA_SEN_RMC,  // GPRMC interval - Recomended Minimum Specific  
  //    2 NMEA_SEN_VTG,  // GPVTG interval - Course Over Ground Speed  
  //    3 NMEA_SEN_GGA,  // GPGGA interval - GPS Fix Data  
  //    4 NMEA_SEN_GSA,  // GPGSA interval - GNSS Satellites Active   
  //    5 NMEA_SEN_GSV,  // GPGSV interval - GNSS Satellites in View  
  //    6 NMEA_SEN_GRS,  // GPGRS interval – GNSS Range Residuals 
  //    7 NMEA_SEN_GST,  // GPGST interval – GNSS Pseudorange Errors Statistics 
  //   17 NMEA_SEN_ZDA,  // GPZDA interval – Time & Date  
  //   18 NMEA_SEN_MCHN, // PMTKCHN interval – GNSS channel status 
  //   19 NMEA_SEN_DTM,  // GPDTM interval – Datum reference

  // To work out the checksum I used the spreadsheet below
  // https://www.roboticboat.uk/Excel/NMEAchecksum.xlsx
  
  gpsSerial.println("$PMTK314,1,1,1,1,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0*30");
  delay(1000);
}

// Arduino UNO and the Adafruit Ultimate GPS v3
// Copyright (C) 2021 https://www.roboticboat.uk
// 2d44f27a-e9db-41e7-b9cf-45f3c6757932
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
  
  // Reserve 200 bytes for the SoftwareSerial string
  inputMessage.reserve(200);

  SelectAll();
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

void SelectAll()
{
  // 0 NMEA_GLL output interval - Geographic Position - Latitude longitude
  // 1 NMEA_RMC output interval - Recommended Minimum Specific GNSS Sentence
  // 2 NMEA_VTG output interval - Course Over Ground and Ground Speed
  // 3 NMEA_GGA output interval - GPS Fix Data
  // 4 NMEA_GSA output interval - GNSS DOPS and Active Satellites
  // 5 NMEA_GSV output interval - GNSS Satellites in View
  // 6 NMEA_GRS output interval - GNSS Range Residuals
  // 7 NMEA_GST output interval - GNSS Pseudorange Errors Statistics

  gpsSerial.println("$PMTK314,1,1,1,1,1,5,1,1,1,1,1,1,0,1,1,1,1*2C");

  delay(100);

}

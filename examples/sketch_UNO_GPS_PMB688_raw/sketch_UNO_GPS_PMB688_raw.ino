// Arduino UNO and the PMB 688
// Copyright (C) 2021 https://www.roboticboat.uk
// b7bdb054-bb52-4afd-8b10-8bc2488cfe46
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
  gpsSerial.begin(4800);
  
  // Set the GPS in walk mode (turn off static navigation)
  SetGPSWalkMode();

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


void NMEA2Binary()
{
  // Swtich to Binary communication mode with the GPS Module
  gpsSerial.println("$PSRF100,0,4800,8,1,0*0F"); //Sets it to SiRF Binary Mode

  // Wait for the GPS to think
  delay(2000);  
}

void Binary2NMEA()
{
  // Start Sequence
  gpsSerial.write(0xA0);       
  gpsSerial.write(0xA2);
  gpsSerial.write((byte)0x00); // Payload length 
  gpsSerial.write(0x02);       // dec2hex(2 bytes)=0x02

  gpsSerial.write(0x87);       // Message ID 135
  gpsSerial.write(0x02);       // NMEA = 2

  // Message Checksum
  gpsSerial.write((byte)0x00); 
  gpsSerial.write(0x89);       // 0x8F+0x02 (the payload)

  // End Sequence
  gpsSerial.write(0xB0);       
  gpsSerial.write(0xB3);
}

void SetGPSWalkMode()
{
  //The GPS is by default set up as Static Navigation
  //which means the lat/lon will not change unless moving say 50 meters
  //The script below turns the GPS to pedestrian/walk mode but need to go via 
  //SiRF binary mode to set the parameters. The GPS will of course must have
  //the SiRF chipset.
   
  // Switch from NMEA to SiRF binary - REQUIRES THE SiRF chip!
  // $PSRF100 - Message ID
  // 0 = SiRF binary (1=NMEA)
  // Band = 4800
  // DataBits = 8
  // StopBits = 1
  // Parity = 0
  // Checksum *0C

  NMEA2Binary();

  // Static Navigation (Message ID 143) ------------------
  // Allows the user to enable or disable static navigation to the receiver
  // Static model is meant to be used in cars and thus can be turned off
  gpsSerial.write(0xA0); // Start Sequence
  gpsSerial.write(0xA2);
  gpsSerial.write((byte)0x00); // Payload length 
  gpsSerial.write(0x02); // dec2hex(2 bytes)=0x02

  gpsSerial.write(0x8F); //  1 Message ID = 143
  gpsSerial.write((byte)0x00); //  2 Disable = 0 (Enable = 1)

  gpsSerial.write((byte)0x00); // Message Checksum
  gpsSerial.write(0x8F); // 0x8F+0x00 (the payload)
  
  gpsSerial.write(0xB0); // End Sequence
  gpsSerial.write(0xB3);
   
  delay(1000);

  Binary2NMEA();
}

void SelectSentences()
{  

  NMEA2Binary();

  // Switch to NMEA Protocol (Message ID 129) ------------
  gpsSerial.write((byte)0xA0); // Start Sequence
  gpsSerial.write((byte)0xA2);
  gpsSerial.write((byte)0x00); // Payload length
  gpsSerial.write((byte)0x18); // dec2hex(24 bytes)=0x18
   
  delay(100);
   
  gpsSerial.write((byte)0x81); // byte  1 Message ID = 129
  gpsSerial.write((byte)0x02); // byte  2 Mode
  gpsSerial.write((byte)0x01); // byte  3 GGA message (ON) 1 second
  gpsSerial.write((byte)0x01); // byte  4 Checksum
  gpsSerial.write((byte)0x00); // byte  5 GLL message (OFF) 0 second
  gpsSerial.write((byte)0x01); // byte  6 Checksum
  gpsSerial.write((byte)0x00); // byte  7 GSA message (OFF) 0 second
  gpsSerial.write((byte)0x01); // byte  8 Checksum
  gpsSerial.write((byte)0x00); // byte  9 GSV message (OFF) 0 second
  gpsSerial.write((byte)0x01); // byte 10 Checksum
  gpsSerial.write((byte)0x01); // byte 11 RMC message (ON) 1 second
  gpsSerial.write((byte)0x01); // byte 12 Checksum

  delay(100);

  gpsSerial.write((byte)0x00); // byte 13 VTG message (OFF) 0 second  
  gpsSerial.write((byte)0x01); // byte 14 Checksum
  gpsSerial.write((byte)0x00); // byte 15 MSS message (OFF) 0 second
  gpsSerial.write((byte)0x01); // byte 16 Checksum
  gpsSerial.write((byte)0x00); // byte 17 EPE message
  gpsSerial.write((byte)0x01); // byte 18 Checksum
  gpsSerial.write((byte)0x00); // byte 19 ZDA message
  gpsSerial.write((byte)0x01); // byte 20 Checksum
  gpsSerial.write((byte)0x00); // byte 21 Unused field
  gpsSerial.write((byte)0x01); // byte 22 Unused field
  gpsSerial.write((byte)0x12); // byte 23 Bit rate dec2hex(4800)=12C0
  gpsSerial.write((byte)0xC0); // byte 24

  delay(100);

  gpsSerial.write((byte)0x01); // Message Checksum
  gpsSerial.write((byte)0x61); // 0x81+0x02+0x01+0x01+0x00+ .... + 0x12+0xC0
  gpsSerial.write((byte)0xB0); // End Sequence
  gpsSerial.write((byte)0xB3);

  //Returns in NMEA format, so no need to request Binary2NMEA
}

void AllSentences()
{  

  NMEA2Binary();

  // Switch to NMEA Protocol (Message ID 129) ------------
  gpsSerial.write((byte)0xA0); // Start Sequence
  gpsSerial.write((byte)0xA2);
  gpsSerial.write((byte)0x00); // Payload length
  gpsSerial.write((byte)0x18); // dec2hex(24 bytes)=0x18
   
  delay(100);
   
  gpsSerial.write((byte)0x81); // byte  1 Message ID = 129
  gpsSerial.write((byte)0x02); // byte  2 Mode
  gpsSerial.write((byte)0x01); // byte  3 GGA message (ON) 1 second
  gpsSerial.write((byte)0x01); // byte  4 Checksum
  gpsSerial.write((byte)0x01); // byte  5 GLL message (OFF) 0 second
  gpsSerial.write((byte)0x01); // byte  6 Checksum
  gpsSerial.write((byte)0x01); // byte  7 GSA message (OFF) 0 second
  gpsSerial.write((byte)0x01); // byte  8 Checksum
  gpsSerial.write((byte)0x05); // byte  9 GSV message (OFF) 0 second
  gpsSerial.write((byte)0x01); // byte 10 Checksum
  gpsSerial.write((byte)0x01); // byte 11 RMC message (ON) 1 second
  gpsSerial.write((byte)0x01); // byte 12 Checksum

  delay(100);

  gpsSerial.write((byte)0x00); // byte 13 VTG message (OFF) 0 second  
  gpsSerial.write((byte)0x01); // byte 14 Checksum
  gpsSerial.write((byte)0x00); // byte 15 MSS message (OFF) 0 second
  gpsSerial.write((byte)0x01); // byte 16 Checksum
  gpsSerial.write((byte)0x00); // byte 17 EPE message
  gpsSerial.write((byte)0x01); // byte 18 Checksum
  gpsSerial.write((byte)0x00); // byte 19 ZDA message
  gpsSerial.write((byte)0x01); // byte 20 Checksum
  gpsSerial.write((byte)0x00); // byte 21 Unused field
  gpsSerial.write((byte)0x01); // byte 22 Unused field
  gpsSerial.write((byte)0x12); // byte 23 Bit rate dec2hex(4800)=12C0
  gpsSerial.write((byte)0xC0); // byte 24

  delay(100);

  gpsSerial.write((byte)0x01); // Message Checksum
  gpsSerial.write((byte)0x68); // 0x81+0x02+0x01+0x01+0x00+ .... + 0x12+0xC0
  gpsSerial.write((byte)0xB0); // End Sequence
  gpsSerial.write((byte)0xB3);

  //Returns in NMEA format, so no need to request Binary2NMEA
}

#include <Wire.h>
#include <SPI.h>
#include <string.h>
#include <Adafruit_PN532.h>
#include <Adafruit_CC3000.h>
#include "Adafruit_FONA.h"

//RGB ints

int redPin = 38;
int greenPin = 39;
int bluePin = 40;

//Fona pins
#define FONA_RX 51
#define FONA_TX 50
#define FONA_RST 52

//rfid pins
#define PN532_SCK  (22)
#define PN532_MOSI (24)
#define PN532_SS   (26)
#define PN532_MISO (28)

#define PN532_IRQ   (2)
#define PN532_RESET (6)
//wifi  pins
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIVIDER);
// WiFi network (change with your settings !)
#define WLAN_SSID       "YouCantSeeMe"        // cannot be longer than 32 characters!
#define WLAN_PASS       "cancelbacknext"
#define WLAN_SECURITY   WLAN_SEC_WPA2 // This can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2

// Local server IP, port, and repository (change with your settings !)
uint32_t ip = cc3000.IP2U32(192,168,1,102);//your computers ip address
int port = 8080;//webserver port 
String repository = "/beta1/php/";//the folder on webserver where the rfid.php file is located
//RFID
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

//Fona
char replybuffer[255];
#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

void setup(void) {
  
  Serial.begin(115200);
  
   // wifi module
  Serial.println(F("\nInitialising the WiFi Module..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Unable to initialise, Check your wiring?"));
    while(1);
  }
  Serial.println("Connecting to Wifi...");
  // Connect to  WiFi network
  cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  Serial.println("Connected to WiFi network!");
    
  // Check DHCP
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100);
  }
  
  // See if the FONA is responding
 fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while(1);
  }
  Serial.println(F("FONA is OK"));

  // Print SIM card IMEI number.
  char imei[15] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("SIM card IMEI: "); Serial.println(imei);
  }
  
  Serial.println("FONA Ready");
  
  //RFID module
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion(); 
  nfc.SAMConfig();
  
  Serial.println("Establishing Connection to server");
  String request = "GET "+ repository + " HTTP/1.0";
  Serial.println("Connected");
  
    //rgb pin output
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}



void loop(void) {
char sendto[]="09166023110";
char message[]="Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";

        setColor(0, 0, 255);
        
        uint8_t success;
	uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; 
	uint8_t uidLength;

	Serial.println("Tap Your Tag");

	success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

	uint32_t cardidentifier = 0;

	if (success) {
		cardidentifier = uid[3];
		cardidentifier <<= 8; cardidentifier |= uid[2];
		cardidentifier <<= 8; cardidentifier |= uid[1];
		cardidentifier <<= 8; cardidentifier |= uid[0];
////////////////Tag #1/////////////////////////////////////
		if (cardidentifier == 3426662194) 
                        { 
			Serial.print("card identified card # ");
                        Serial.println(cardidentifier);
                        setColor(0, 255, 255); 
                        String request = "GET "+ repository + "id1.php?" + " HTTP/1.0";
                        send_request(request); 
                            if (!fona.sendSMS(sendto, message)) {
                                  Serial.println(F("Failed"));
                                  setColor(80, 0, 80);
                                } else {
                                  Serial.println(F("Sent!"));
                                  setColor(255, 0, 0);
                                }              
                        }
/////////////////////////////////////////////////////

                else
                {
                  Serial.println("not recorded");
                  Serial.println(cardidentifier);
                  setColor(255, 0, 0);
                }
	}


delay(1000);
}


// Function to send a TCP request and get the result as a string
void send_request (String request) {
     
    // Connect    
    Serial.println("Starting connection to server...");
    Adafruit_CC3000_Client client = cc3000.connectTCP(ip, port);
    
    // Send request
    if (client.connected()) {
      client.println(request);      
      client.println(F(""));
      Serial.println("Connected & Data sent");
    } 
    else {
      Serial.println(F("Connection failed"));    
    }

    while (client.connected()) {
      while (client.available()) {

      // Read answer
      char c = client.read();
      }
    }
    Serial.println("Closing connection");
    Serial.println("");
    client.close();
    
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

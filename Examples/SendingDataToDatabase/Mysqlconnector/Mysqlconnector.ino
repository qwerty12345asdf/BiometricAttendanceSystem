#include "MySQL_Connection.h"
#include "MySQL_Cursor.h" //To execute the query
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
 
const char* ssid="sebi";//wifi credentials
const char* pass="9172321736";
//variable
int i=0;
IPAddress server_addr(192,168,56,1);//ip of the mysql server.  
char user[]="seb";  //MySQL username
char password[]="hello"; //MySQL password
WiFiClient client;
MySQL_Connection conn((Client *)&client);
MySQL_Cursor* cursor;
//Sample query
char INSERT_SQL[]=  "INSERT INTO nodemcu.person(p_name,p_age) VALUES('Ramu','45')";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  //Begin Wifi section
  WiFi.begin("Sebastian","OSInit()");
  Serial.print("Connecting");
  while(WiFi.status()!= WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to IP Address:");
  Serial.println(WiFi.localIP());
  //End WiFi Section
  Serial.print("Connecting to SQL...  ");
  if (conn.connect(server_addr, 3306, user,password))
    Serial.println("OK.");
  else
    Serial.println("FAILED.");
  // create MySQL cursor object
  cursor = new MySQL_Cursor(&conn);
}

void loop() {
  if(i<3){
  if (conn.connected())
  cursor->execute(INSERT_SQL);
  delay(5000);
  i++;
  }
}

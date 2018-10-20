#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"

// set up software serial pins for Arduino's w/ Atmega328P's
// FPS (TX) is connected to pin 4 (Arduino's Software RX)
// FPS (RX) is connected through a converter to pin 5 (Arduino's Software TX)
FPS_GT511C3 fps(4, 5); // (Arduino SS_RX = pin 4, Arduino SS_TX = pin 5)
void Enroll();
unsigned short int attInd;
short unsigned int attendance[500];
void setup() {

  Serial.begin(115200); //set up Arduino's hardware serial UART_RX
  attInd=0;
  //ESP.intr_lock(15); // all intruptt off
  ESP.wdt_disable();
  
}

void loop() {
 
  
  ESP.wdtFeed();
  fps.SetLED(false);
  Serial.println("What do you want to do?");
  int flag;
  char a,b,x,y,z,w;
  flag=0;
  while(Serial.available()==0);
  a=Serial.read();
  Serial.println(a);
  int d=0;
  
  if(a=='1'){
    flag=1;
    while(flag){
      Serial.println("enter fingerprint to delete");
      fps.SetLED(true);
      while(fps.IsPressFinger()==0);
      fps.CaptureFinger(false);
    int id = fps.Identify1_N();
    Serial.println(id);
  fps.DeleteID(id);
  Serial.println("1.To delete another \n2.To exit");
  while(Serial.available()==0);
  if(Serial.available()>0)
  b=Serial.read();
  if(b=='2')
  flag--;
  }
  }
  
  else if(a=='2'){
    while(flag==0){
      fps.SetLED(true);
      Enroll();
      Serial.println("1.Add another");
      Serial.println("2.End Enrollment");
      lol:
      while(Serial.available()==0);
      if(Serial.available()>0)
      b=Serial.read();                   
      if(b=='1')
      continue;
      else if(b=='2')
      flag=1;
      else{
        Serial.println("Choose valid option");
      goto lol;
      }
      }
    }
    
  else if(a=='3'){
    fps.SetLED(true);
    while(flag==0){
      if (fps.IsPressFinger())
      {
        fps.CaptureFinger(false);
        int id = fps.Identify1_N();
        if (id <200) //<- change id value depending model you are using
        {
          if(id<2)
          {
            flag++;
          }
       else
          {   
          Serial.print("Verified ID:");
          Serial.println(id);
          attendance[attInd]=id;
          attInd++;
          }
      }
      else
      {//if unable to recognize
      Serial.println("Finger not found");
      }
      }
    b=Serial.read();
    if(b=='x')
    flag++;
    }  
  }

  else if(a=='4'){
    for(int i=0;i<attInd;i++)
    Serial.println(attendance[i]);
    
    }
  else if(a=='5'){
    fps.DeleteAll();
    }
 
     
}
void Enroll()
{
  // Enroll test

  // find open enroll id
  int enrollid = 0;
  bool usedid = true;
  while (usedid == true)
  {
    usedid = fps.CheckEnrolled(enrollid);
    if (usedid==true) enrollid++;
  }
  fps.EnrollStart(enrollid);

  // enroll
  Serial.print("Press finger to Enroll #");
  Serial.println(enrollid);
  while(fps.IsPressFinger() == false) delay(100);
  bool bret = fps.CaptureFinger(true);
  int iret = 0;
  if (bret != false)
  {
    Serial.println("Remove finger");
    fps.Enroll1(); 
    while(fps.IsPressFinger() == true) delay(100);
    Serial.println("Press same finger again");
    while(fps.IsPressFinger() == false) delay(100);
    bret = fps.CaptureFinger(true);
    if (bret != false)
    {
      Serial.println("Remove finger");
      fps.Enroll2();
      while(fps.IsPressFinger() == true) delay(100);
      Serial.println("Press same finger yet again");
      while(fps.IsPressFinger() == false) delay(100);
      bret = fps.CaptureFinger(true);
      if (bret != false)
      {
        Serial.println("Remove finger");
        iret = fps.Enroll3();
        if (iret == 0)
        {
          Serial.println("Enrolling Successful");
        }
        else
        {
          Serial.print("Enrolling Failed with error code:");
          Serial.println(iret);
        }
      }
      else Serial.println("Failed to capture third finger");
    }
    else Serial.println("Failed to capture second finger");
  }
  else Serial.println("Failed to capture first finger");
}




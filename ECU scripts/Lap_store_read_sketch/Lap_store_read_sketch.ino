#include <uEEPROMLib.h>
#include <time.h>

uEEPROMLib eeprom(0x57);

int laptotal;
int laptotal_erased;
int laptime[100];
int lapcounter;
int i;
int menu_choice;

void setup() {
  // put your setup code here, to run once:
  eeprom.eeprom_read(0, &laptotal_erased);
  Serial.begin(9600);
  while (!Serial){
    //wait for serial to start
  }
  Serial.println ("Press 'r' to read data, 'e' to erase data and 'u' to undo erase");
  Serial.println ("");
}

void loop(){
  int menu_choice = Serial.read();
  switch (menu_choice){
    case 'r':
    Read_Stored_Laps();
    break;

    case 'e':
    Erase_EEPROM();
    break;

    case 'u':
    Undo_Erase();
    break;
  }
}

void Read_Stored_Laps() {
  eeprom.eeprom_read(0, &laptotal);
  Serial.print ("Total laps: ");
  Serial.println (laptotal);
  Serial.print ("");

  for(i = 1; i < laptotal+1; i++)
  {
  eeprom.eeprom_read((i*2), &laptime[i]);
  Serial.print("Lap:");
  Serial.print(i);
  Serial.print(" - ");
  Serial.print (laptime[i]);
  Serial.println("s");
  }

  Serial.println ("Thats all");
  Serial.println ("");
  delay(1000);
  return(0);
}
void Erase_EEPROM(){
  eeprom.eeprom_write(0, 0);
  Serial.println ("EEPROM Reset");
  Serial.println ("");
  return(0);
}
void Undo_Erase(){
  eeprom.eeprom_write(0, laptotal_erased);
  eeprom.eeprom_read(0, &laptotal);
  Serial.println ("EEPROM Restored");
  Serial.println ("");
  return(0);
}
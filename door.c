//Comenzamos agregando algunas variables que vamos a utilizar por comodidad
int RFIDResetPin = 2;
int doorPin = 5;
char tagString[14] = "";
char myTag[14] = "434056786236";
int reading = 0;
int index = 0;
int ldPin = 4;
int tempo = 300;
//Configuramos los pins que vamos a utilizar, ponemos en HIGH el pin de reset, sin esto, el lector no funciona y setteamos el puerto de serie para
//poder leer los tags
void setup(){
  pinMode(5, OUTPUT);
  Serial.begin(9600);
  pinMode(RFIDResetPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  digitalWrite(RFIDResetPin, HIGH);
}
 
//comenzamos a buscar por tarjetas
void loop(){
  readCard();
}
 
void readCard() {
  while(Serial.available()){ //Cuando hay datos para leer
    char readByte = Serial.read(); //leer byte uno por uno
    if(readByte == 2) { //Si nuestro byte es igual a 2, marca el comienzo del id de una tag que hayamos leido
      reading = 1;
      index = 0;
    }
    if(readByte == 3) {  //Si es 3, determina que la lectura finalizó
      reading = 2;
    }
    if(reading == 1 && readByte != 2 && readByte != 10 && readByte != 13 && readByte != 3){
      tagString[index] = readByte; //Leemos cada byte y lo metemos en nuestro buffer
      index++;
    }
  }
  if(reading == 2) {  //Si la lectura finalizó, la comparo con mi código y abro la puerta
    if (compareTag(myTag) == true) {
       openDoor();
    }
    reading = 0; //Vuelvo a settear reading a 0 para que pueda leer nuevas tarjetas
    Serial.flush(); //Borro el buffer del puerto de serie para evitar problemas
  }
}
 
//Funcion no necesaria, la dejo por si alguien quiere utilizarla
void resetReader(){
  ///////////////////////////////////
  //Reset the RFID reader to read again.
  ///////////////////////////////////
  digitalWrite(RFIDResetPin, LOW);
  digitalWrite(RFIDResetPin, HIGH);
  delay(250);
}
 
//Compara 2 tags
boolean compareTag(char two[]){
  if(strlen(tagString) == 0) return false; //empty
 
  for(int i = 0; i < 12; i++){
    if(tagString[i] != two[i]) return false;
  }
 
  return true; //no mismatches
}
//Abre la puerta
void openDoor(){
    digitalWrite(doorPin, HIGH);
    delay(2500);
    digitalWrite(doorPin, LOW);
}
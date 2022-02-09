
/*
  Titre      : Controle de servo moteur avec capteur de rotation
  Auteur     : Yvan Tankeu
  Date       : 09/02/2022
  Description: Controler la rotation d'un servo moteur avec 01 capteurs de rotation
  Version    : 0.0.1
*/

#include <Arduino.h>
#include <Servo.h>
#include "WIFIConnector_MKR1000.h"
#include "MQTTConnector.h"

const int POTENTIOMETRE = 1; // Connecter le capteur de rotation à la broche analogue 1
const int ServoPin = 3;   // Connecter Servo moteur à la broche Digital qui est pwm 3

const int MIN_RESOLUTION = 0; // Val min qu'on peut lire/écricre sur broche analogue 2
const int MAX_ANALOG_RESOLUTION = 1023;    // Val  max lu sur broche analogue 2
const int MAX_SERVO_RESOLUTION = 90;  // Val max du mapping de rotation

int valeurPotentiometre = 0; // valeur analogue issue du potentiometre
int tensionPotentio = 0; // valeur de tension issue du mapping
int angleServo = 0;

Servo servo;  // creation de l'objet Servo

void setup() {
  
  wifiConnect(); //Branchement au réseau WIFI
  MQTTConnect(); //Branchement au broker MQTT

  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  servo.attach(ServoPin);  // attache le servo sur la broche 3 à l'objet servo
}

void loop() {

  // read the analog in value:
  valeurPotentiometre =  analogRead(POTENTIOMETRE);
   Serial.print("valeur potentiometre = ");
  Serial.println(valeurPotentiometre);

  /** grace à la fonction map on pourra ré-étalonner la valeur entre 0 et 1023
          sur une fourchette entre 0 et 90
      * @brief mapper un nombre d’une plage de 1 à 1024 à une plage de 0 à 90,
      *
      * @param valeurPotentiometre variable dont on souhaite convertir la plage.
      * @param MIN_RESOLUTION valeur minimale d’origine de la plage de la variable.
      * @param MAX_ANALOG_RESOLUTION valeur maximale d’origine de la plage de la variable.
      * @param MIN_RESOLUTION maximale de la nouvelle plage de la variable.
      */
  // map it to the range of the analog out:
  tensionPotentio = map(valeurPotentiometre, MIN_RESOLUTION, MAX_ANALOG_RESOLUTION, MIN_RESOLUTION, MAX_SERVO_RESOLUTION);
  servo.write(tensionPotentio);
  delay(3000);

  angleServo = servo.read();
 
  // print the results to the serial monitor:
  Serial.print("valeur tension lu par le capteur de rotation ");
  Serial.println(tensionPotentio);
  Serial.print("dégré servo = ");
  Serial.println(angleServo);

  if (tensionPotentio != tensionPotentio){

    appendPayload("rotation", tensionPotentio); //Ajout de la donnée de roation au message MQTT
    sendPayload();                                  //Envoie du message via le protocole MQTT

    appendPayload("servo position", angleServo);
    sendPayload();

  }
  //delay(2000);
}
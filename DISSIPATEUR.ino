//----------------------------------- DISSIPATEUR DE TENSION - HYSTERESIS ----------------

//---------------------------------- Derniere VERSION - 13/06/22 --------------------------

//------------------------------------ IMPORTATION DE LIBRAIRIE ----------------------------
#include <SimpleTimer.h> // timer permettant l'appel de fonction 
SimpleTimer timer;
// ----------------------------------- DEFINITION DES BROCHES -----------------------------
const int _IN1 = 0; // Mesure de la tension passant par une sonde differentielle
const int _IN2 = 1; // Mesure de reference passant par une sonde differentielle
const int _IN3 = 2; 
const int _IN4 = 3;
const int _IN5 = 4;
const int _IN6 = 5;
const int _NUM_OUT1 = 3; // PIN DE SORTIES POUR LES VISUALISER A L OSCILLOSCOPE
const int _NUM_OUT2 = 5; // PIN DE SORTIES POUR LES VISUALISER A L OSCILLOSCOPE
const int _NUM_OUT3 = 6; // PIN DE SORTIES POUR LES VISUALISER A L OSCILLOSCOPE
const int _AN_OUT4 = 9;
const int _AN_OUT5 = 10;
const int _AN_OUT6 = 11;
const int _AN_OUT7 = 13;

// ------------------------------------- INITIALISATION --------------------------------------

void setup()
 {
   // reglage de la frequence des signaux PWM
   TCCR0B = TCCR0B & 0b11111000 | 0x03; // pin 3 et 11 a  980Hz (base de temps de base)
   TCCR1B = TCCR1B & 0b11111000 | 0x02; // pin 9 et 10 a  4kHz 
   TCCR3B = TCCR3B & 0b11111000 | 0x02; // pin 5 a  4kHz
   TCCR4B = TCCR4B & 0b11111000 | 0x02; // pin 6 et 13 a  15kHz 
   
// ------------------------ CONFIGURATION BROCHES EN ENTREE ET EN SORTIE -----------------------   

   pinMode(_IN1,INPUT); // afin d'avoir 0 pour une tension nulle
   pinMode(_IN2,INPUT); // afin d'avoir 0 pour une tension nulle
   pinMode(_NUM_OUT1,OUTPUT);  //
   pinMode(_NUM_OUT2,OUTPUT); 
   pinMode(_NUM_OUT3,OUTPUT); 
   pinMode(_AN_OUT4,OUTPUT); 
   pinMode(_AN_OUT5,OUTPUT);   
   pinMode(_AN_OUT6,OUTPUT); 
   pinMode(_AN_OUT7,OUTPUT); 
   timer.setInterval(0.5,protection); // la fonction BF_i sera appelee
 }
// --------------------------------------- BOUCLE ------------------------------------------------

void loop()
 {
  timer.run(); //on viens run la fonction - la fonction timer est activee
 }
 
// ---------------------------------- PROGRAMME PRINCIPALE APPELE --------------------------------------

void protection()
 {
  //Definition des variables
  
  static int etat_150;
  static int etat_200;
  static int etat_250;
  float v_mes, v_ref, v_result; // Variables flotantes de récupération de valeurs et de convertion en volts
  
// -------------------------------- MESURE ET CALCULE DE LA TENSION --------------------------------------
  
  v_mes = analogRead(_IN1); // lecture de la tension mesuree sur la pin _IN1
  v_ref = analogRead(_IN2); // lecture de la reference sur la pin _IN2
  
  // Produit en croix de la valeur analogique vers la tension 
  v_result = 100*(((v_mes-v_ref)*5)/1024); // v_mes est en Volts (avec une sonde d'isolation x100)
  // pour 1024 on a 5V donc pour v_result on fait le produit en croix puis, le produit par "100"
  // due a la sonde
   
// ------------------------------------------ TESTS DES ETATS  --------------------------------------
  if(v_result >= 250) // PAS (delta) DE 10 ENTRE LES DEUX ETATS POUR 250
    etat_250 = 255; // ENVOIE état haut ==> LevelShifter
  else
  if(v_result <= 240) // PAS (delta) DE 10 ENTRE LES DEUX ETATS POUR 250
    etat_250 = 0; // RESTE A 0
  
  if(v_result >= 200)
    etat_200 = 255; // ENVOIE état haut ==> LevelShifter
  else
  if(v_result <= 190)
    etat_200 = 0; // RESTE A 0

  if(v_result >= 150)
    etat_150 = 255;// ENVOIE état haut ==> LevelShifter
  else
  if(v_result <= 140)
    etat_150 = 0; // RESTE A 0
// ---------------------------- AFFICHAGE DANS LE MONITEUR SERIE --------------------------------------
   //renvoi vers le PC de la valeur lue  
  Serial.print("v_resultat = "); 
  Serial.println(v_result); // CONNEXION PORT COM - TELEVERSEMENT CODE - OUVERTURE DU MONITEUR SERIE (Ctrl + Maj + M)
  Serial.print(" V");
// ------------ ECRITURE SUR BORCHES DE SORTIES SI ETAT 255 => 15V ou ETAT 0 => 0 ---------------------
   
  analogWrite(_NUM_OUT1, etat_150); // Analyse de l etat par le connecteur BNC 150
  analogWrite(_NUM_OUT2, etat_200); // Analyse de l etat par le connecteur BNC 200
  analogWrite(_NUM_OUT3, etat_250); // Analyse de l etat par le connecteur BNC 250
 }

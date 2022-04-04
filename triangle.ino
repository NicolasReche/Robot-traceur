#include <FlexiTimer2.h>

volatile long int Nb_Tick=0;
volatile long int Nb_Tickgauche=0;
long int consigne_pos=0;// nombre de tour entier
long int consigne_posgauche=0;
int byte_read;
volatile int erreur=0;
volatile int erreurgauche=0;

const int maxPwm=150;// Vitesse max souhaitée de rotation de la roue

const float kp=0.7;
const float kpgauche=0.75;

byte form=1;

volatile int somerreurgauche=0;


volatile int pwm=0;
volatile int pwmgauche=0;

void setup() {

Serial.begin(115200);

//Moteur Droite
pinMode(13,OUTPUT);// INA1
pinMode(12,OUTPUT);// INA2
pinMode(6,OUTPUT);// PWMA Attention pin 11 ne marche pas car conflit avec FlexiTimer2
pinMode(3,INPUT);// INT1
//Moteur Gauche
pinMode(8,OUTPUT);// INA1
pinMode(9,OUTPUT);// INA2
pinMode(10,OUTPUT);// PWMA Attention pin 11 ne marche pas car conflit avec FlexiTimer2
pinMode(2,INPUT);// INT1

attachInterrupt(digitalPinToInterrupt(3), codeur, CHANGE);//On lit 16 ticks/tours du codeur.
attachInterrupt(digitalPinToInterrupt(2), codeurgauche, CHANGE);//On lit 16 ticks/tours du codeur.

FlexiTimer2::set(10, asserv); // Lance la fonction asserv toutes les 10ms
FlexiTimer2::start();
}


void loop() 
{

int sens=1;
unsigned long currentTime=millis();

//Triangle
if(currentTime<1500){
  consigne_pos=1883;
  consigne_posgauche=1883;
  }
  if(currentTime>1500&&currentTime<3000){
 
  consigne_posgauche=1883+3139;
  }
   if(currentTime>3000&&currentTime<4500){
  consigne_pos=2*1883;
  consigne_posgauche=5022+1883;
  }
   if(currentTime>4500&&currentTime<6000){
  consigne_posgauche=6905+3139;
  }
   if(currentTime>6000&&currentTime<7500){
  consigne_pos=3*1883;
  consigne_posgauche=10044+1883;
  }
  if(currentTime>7500&&currentTime<9000){
  consigne_posgauche=12050+3139;
  }
}

void codeur()
{
  if(pwm>=0) 
  {
  Nb_Tick++;
  }
  else
  {
  Nb_Tick--;
  }
}
void codeurgauche()
{
  if(pwmgauche>=0) 
  {
  Nb_Tickgauche++;
  }
  else
  {
  Nb_Tickgauche--;
  }
}

void asserv()
{
erreur=consigne_pos-Nb_Tick;
erreurgauche=consigne_posgauche-Nb_Tickgauche;
volatile int corr_p = erreur*kp;
volatile int corr_pgauche = erreurgauche*kpgauche;
if(corr_p > maxPwm)
{
  corr_p=maxPwm;
}
if(corr_p < -maxPwm)
{
  corr_p = -maxPwm;
}
if(corr_pgauche > maxPwm)
{
  corr_pgauche=maxPwm;
}
if(corr_pgauche < -maxPwm)
{
  corr_pgauche = -maxPwm;
}

pwm=corr_p;
pwmgauche=corr_pgauche;

if(pwm > maxPwm)
  {
  pwm= maxPwm;
  }
if(pwm <-maxPwm)
  {
  pwm=-maxPwm;
  }
  if(pwmgauche > maxPwm)
  {
  pwmgauche= maxPwm;
  }
if(pwmgauche <-maxPwm)
  {
  pwmgauche=-maxPwm;
  }

  

if(pwm>=0)
{
droit()  ;
}else{
arriere();
  }
  
if(pwmgauche>=0)
{
droitg()  ;
}else{
arriereg();
 }
  
Serial.print(" ");
Serial.print(consigne_pos);
Serial.print(" ");
Serial.println(Nb_Tick);

}

void droit(){
  analogWrite(6,pwm); 
  digitalWrite(13,LOW);
  digitalWrite(12,HIGH);

  }

 void arriere(){
    analogWrite(6,-pwm); 
  digitalWrite(13,HIGH);
  digitalWrite(12,LOW);

   
  }

  void droitg(){
 
  analogWrite(10,pwmgauche); 
  digitalWrite(9,LOW);
  digitalWrite(8,HIGH);
  }
  
  void arriereg(){
   analogWrite(10,-pwmgauche); 
  digitalWrite(9,HIGH);
  digitalWrite(8,LOW);
 
  }
  

  
  

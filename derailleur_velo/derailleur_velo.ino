
const int RPWM = 5; // on initialise le pin pour la vitesse du moteur vers la droite
const int LPWM = 6; // on initialise le pin pour la vitesse du moteur vers la gauche
const int  R_EN = 8; // on initialise le pin pour le sens du moteur vers la droite
const int L_EN = 7; // on initialise le pin pour le sens du moteur vers la gauche
const int entree_impulsion = 2;  //on initialise le pin de mesure des impulsions
int etat_capteur_actuel = 0;  //on mesurera l'état du capteur
int etat_capteur_precedent = 0;  //pour garder en mémoire l'ancien état du capteur
float vitesse = 0; // initialisation de la vitesse à 0 tr/min
unsigned long chrono = 0; // initialisation du chrono à zéro
unsigned int changement_d_etat = 0; // initialisation du compteur de changement d'état à zéro
unsigned int nombre_de_tours = 0; // initialisation du compteurs de nombre de tours à zéro
int N = 1; // initialisation du numéro du pignon


void setup()   
{
  Serial.begin(9600); //initialiser la communication serie

  pinMode (RPWM,OUTPUT);  // vitesse du moteur vers la droite initialisée en sortie
  pinMode (LPWM,OUTPUT); // vitesse du moteur vers la gauche initialisée en sortie
  pinMode ( R_EN,OUTPUT); // sens du moteur vers la droite initialisé en sortie
  pinMode ( L_EN,OUTPUT); // sens du moteur vers la gauche initialisé en sortie
  analogWrite(RPWM,LOW); // vitesse du moteur vers la droite initialisée en sortie analogique
  analogWrite(LPWM,LOW); // vitesse du moteur vers la gauche initialisée en sortie analogique
  digitalWrite(R_EN,LOW); // sens du moteur vers la droite initialisé en sortie digitale
  digitalWrite(L_EN,LOW); // sens du moteur vers la gauche initialisé en sortie digitale

  pinMode(entree_impulsion, INPUT); // Broche entree_impulsion en entrée
}

void loop()
{
  nombre_de_tours = 0; // initialisation du compteurs de nombre de tours à zéro
  changement_d_etat = 0; // initialisation du compteur de changement d'état à zéro
  vitesse = 0; // initialisation de la vitesse à 0 tr/min
  chrono = millis(); //on affecte millis() au chrono
  while (millis() - chrono < 2000) //mesurer pendant 2 secondes
    {
      etat_capteur_actuel = digitalRead(entree_impulsion); // lecture de l'état du capteur
      if (etat_capteur_actuel != etat_capteur_precedent) // si le capteur change d'état
        {
         changement_d_etat++; // on incrémente le compteur de changement d'état
          etat_capteur_precedent = etat_capteur_actuel; // on affecte etat_capteur_actuel à etat_capteur_precedent
        }
      if (changement_d_etat == 20) //si le compteur de changement d'état atteint 20 changments d'état
        {
         nombre_de_tours= nombre_de_tours+1; // on incrémente le compteur de nombre de tours
         changement_d_etat = 0; // on remet à zéro le compteur de changement d'état
        }
      delay(4); // pause de 4 millisecondes pour que le capteur ne lise pas en permanence 
    }
    vitesse = nombre_de_tours*30 + changement_d_etat*1,5; // calcul de la vitesse en  tours par minute : vitesse = nombre_de_tours*60/2 + changement_d_etat/20 * 60/2
    Serial.println(vitesse); //  afficher sur l'écran de l'ordinateur la vitesse de pédalage 
  
  if (vitesse<=95 && vitesse>=75) // si la vitesse est comprise entre 75 tr/min et 95 tr/min
    {
       // il n'y a aucune action
    }

  if (vitesse<75 && vitesse>10 && N>1) // si la vitesse est comprise entre 10 tr/min et 75 tr/min et que le pignon sur lequel est la chaîne n'est pas celui avec le diamètre le plus grand
    {
      digitalWrite( L_EN,HIGH); // le moteur peut tourner vers la gauche
      digitalWrite( R_EN,HIGH); // le moteur peut tourner vers la droite
      analogWrite(RPWM, 0); // vitesse vers la gauche minimale
      analogWrite(LPWM, 50); // on augmente la vitesse vers la gauche
      delay(50); // faire tourner le moteur à cette vitesse vers la gauche pendant 0,05 seconde
      analogWrite(LPWM, 150); // on augmente la vitesse vers la gauche
      delay(50); // faire tourner le moteur à cette vitesse vers la gauche pendant 0,05 seconde
      analogWrite(LPWM, 255); //  on augmente la vitesse vers la droite jusqu'à la vitesse maximale
      delay(500); // faire tourner le moteur vers la gauche à sa vitesse maximale pendant 0,5 seconde
     
      analogWrite(LPWM, 150); // on diminue la vitesse vers la gauche
      delay(100); // faire tourner le moteur à cette vitesse pendant 0,1 seconde
      analogWrite(LPWM, 50); // on diminue la vitesse vers la gauche
      delay(100); // // faire tourner le moteur à cette vitesse vers la gauche pendant 0,1 seconde
      analogWrite(LPWM,0); // on met à zéro la vitesse du moteur vers la gauche
      N = N-1; // indiquer que le pignon sur lequel est la chaîne est celui avec un diamètre un peu plus grand
      delay(1000); // pause de 1 seconde
    }
    
  if (vitesse<180 && vitesse>95 && N<5) // si la vitesse est comprise entre 95 tr/min et 180 tr/min et que le pignon sur lequel est la chaîne n'est pas celui avec le diamètre le plus petit
    {
      digitalWrite( L_EN,HIGH); // le moteur peut tourner vers la gauche
      digitalWrite( R_EN,HIGH); // le moteur peut tourner vers la droite
      analogWrite(LPWM,0); // vitesse vers la gauche minimale
      analogWrite(RPWM, 50); // on augmente la vitesse vers la droite
      delay(50); // faire tourner le moteur à cette vitesse vers la droite pendant 0,05 seconde
      analogWrite(RPWM, 150); // on augmente la vitesse vers la droite
      delay(50); // faire tourner le moteur à cette vitesse vers la droite pendant 0,05 seconde
      analogWrite(RPWM, 255); //  on augmente la vitesse vers la droite jusqu'à la vitesse maximale
      delay(500); // faire tourner le moteur vers la droite à sa vitesse maximale pendant 0,5 seconde
     
      analogWrite(RPWM,150); // on diminue la vitesse vers la droite
      delay(100); // faire tourner le moteur à cette vitesse vers la droite pendant 0,1 seconde
      analogWrite(RPWM, 50); // on diminue la vitesse vers la droite
      delay(100); // faire tourner le moteur à cette vitesse  vers la droite pendant 0,1 seconde
      analogWrite(RPWM,0); // on met à zéro la vitesse du moteur vers la droite
      N = N+1; // indiquer que le pignon sur lequel est la chaîne est celui avec un diamètre un peu plus petit
      delay(1000); // pause de 1 seconde
    }
    
  Serial.println(N); //  afficher sur l'écran de l'ordinateur le numéro du pignon sur lequel est placé la chaîne 
}
 


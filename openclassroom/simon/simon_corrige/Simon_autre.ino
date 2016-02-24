
//*******************************************************************************************************
//                                                                                                      
//                                        PROGRAMME DU JEU SIMON
//                                                                                  
//*******************************************************************************************************

//variables

int mode = 0;                                      //mode dans le programme
int taille = 0;                                    // taille de la séquence en cours
int posTest = 0;                                   // position de la réponse
int sequence[50];                                  //tableau de la séquence (50 max)
int duree = 250;                                   //temps pour l'allumage des LED

//constantes

const int pinLed[4] = {5,4,3,2};                   // tableau pour stocker les pins des LED
const int pinSon = 8;                              //constante pour le pin du buzzer
const int freqSon[4] = {261, 329, 392, 523};       //tableau de constantes des fréquences de sons
//À faire => constantes pour la lecture du CAN0
float r1=10000.0;                                  // valeur pour R1
float r2=220.0;                                    // valeur pour R2
float p=5.0;                                       // pourcentage d'erreur
float ue=5000.0;                                   // tension d'entrée en mV
const int nbBoutons=4;                             // nombre de boutons
int valeurs[nbBoutons];                            // tableau de stockage des valeurs basses


///*******************************************************************************************************
//                                                                                                      
//                                 sequence d'initialisation                                                  
//*******************************************************************************************************

void setup() 
{
  randomSeed(analogRead(A1));                      // initialisation des nombres aléatoires
  for (int i = 0; i < 3; i++)
    {
      pinMode(pinLed[i], OUTPUT);                  // mise des pins des led en mode OUTPUT
      digitalWrite(pinLed[i], LOW);                // mise à 0 des commandes led
    }
  pinMode(pinSon, OUTPUT);                         //mise du pin du HP en mode OUTPUT
  for (int t=0;t<nbBoutons;t++)                    // tableau des valeurs pour la lecture des boutons
    {
      float pr2=r2*p/100;                          //valeur du pourcentage
      float usmin=(r1/(r1+t*(r2+pr2)))*ue;         // valeur sortie min
      valeurs[t]=map (int(usmin),0,5000,0,1023);   //ajout d'une valeur
    }
}

//*******************************************************************************************************
//                                                                                                      
//                                  boucle principale                                                   
//*******************************************************************************************************

void loop()                                        //les actions sont réalisées en fonction du mode en cours
{
  switch (mode)
    {                                              //on regarde le mode
      case 0:                                      //mode de démarrage
        start();                                   // appel de la fonction de démarrage
        taille = 0;                                //on met la séquence à 0
        mode = 1;                                  // on peut passer au mode suivant
        break;
      case 1:                                      //mode de tirage d'une nouvelle séquence
        augmenteSequence();                        //appel de la fonction pour allonger la séquence
        mode = 2;                                  // on peut passer au mode suivant
        break;
      case 2:                                      // mode d'écoute de la séquence
        joueSequence();                            // on joue la séquence en cours
        posTest = 0;                               //on initialise la position de réponse
        mode = 3;                                  // on peut passer au mode suivant
        break;
      case 3:                                      // mode de réponse du joueur
        int b = testBouton();                      // on appelle la fonction de lecture des boutons
        if (b != -1)                               // si bouton appuyé
          {                                        
            compare(b);                            // appel de la fonction de validation
          }
        break;
  }
}

//*******************************************************************************************************
//                                                                                                      
//                  fonction de son et lumière pour le démarrage d'une nouvelle séquence                                                    
//*******************************************************************************************************

void start()
{
for (int n=3;n>-1;n--)                             // on parcourt les 4 led
  {
    sonLum(n,300);                                 // envoie de la séquence led et son pendant 300ms
  } 
delay(500); 
}

//*******************************************************************************************************
//                                                                                                      
//                     fonction de son et lumière pour valider une suite de réponses                                                   
//*******************************************************************************************************

void valide()
{
for(int t=0;t<4;t++)                               // on boucle 4 fois
  {
    for (int n=0;n<4;n++)                          // on parcourt les 4 led 
      {
        sonLum(n,10);                              // envoie de la séquence led et son pendant 10ms
      }
  }
delay(200);    
}

//*******************************************************************************************************
//                                                                                                      
//                   fonction de son et lumière pour invalider une suite de réponses                                                   
//*******************************************************************************************************

void nonValide()
{
for (int t=0;t<3;t++)                              //  on boucle 3 fois
  {
    for (int n=1;n<4;n=n+2)                        // on parcourt les led pairs (2 et 4)
      {
        sonLum(n,100);                             // envoie de la séquence led et son pendant 100ms
      }
    for (int n=0;n<4;n=n+2)                        // on parcourt les led impairs (1 et 3)
      {
        sonLum(n,100);                             // envoie de la séquence led et son pendant 100ms
      }
  }
delay(200);  
}  

//*******************************************************************************************************
//                                                                                                      
//              fonction qui allume une LED (l) en jouant le son associé pendant une durée (d)                                         
//*******************************************************************************************************
                                                   
void sonLum(int l, int d)
{
  digitalWrite(pinLed[l], HIGH);                   // commande niveau haut de la led l
  tone(pinSon, freqSon[l], d);                     // envoie de la frequence freqSon[l] pendant d ms 
  delay(d);                                        // delai d ms
  digitalWrite(pinLed[l], LOW);                    // commande niveau bas de la led l
  delay(d);                                        // delai d ms
}

//*******************************************************************************************************
//                                                                                                      
//                              fonction de tirage aléatoire d'une séquence                                                   
//*******************************************************************************************************

void augmenteSequence()
{
  sequence[taille] = random(16) / 4;               // permet un tirage mieux réparti
  if (taille<50) taille++;                         // si moins 50 coups on incrémente le nombre de coups 
}

//*******************************************************************************************************
//                                                                                                      
//                              fonction qui joue la séquence en cours                                                   
//*******************************************************************************************************

void joueSequence()
{
for (int n=0;n<taille;n++)                         // on parcourt les coups tirés au hazard
  {
    sonLum(sequence[n],300);                       // envoie de la séquence led et son pendant 300ms
  }
delay(500);
}

//*******************************************************************************************************
//                                                                                                      
//                 fonction qui teste l'état des boutons et retourne le numéro de bouton
//*******************************************************************************************************

int testBouton()
{
int t;
int val=analogRead(A0);                            // lecture du CAN 0
if (val)                                           // si différent de 0
  { 
    for (t=0;t<nbBoutons;t++)
      {                                            // parcours des valeurs
        if (val>=valeurs[t])
          {                                        //test si supérieur à valeur concernée affichage
            sonLum(t,300);
            break;                                 //sortie de boucle
          }
      }
  return t;                                        // retourne le numero du bouton
  }
return -1;                                         // retourne -1 si pas de bouton appuye
}

//*******************************************************************************************************
//                                                                                                      
//                       fonction qui compare la réponse avec la séquence en cours                                             
//*******************************************************************************************************

void compare (int b)
{
if ( b==sequence[posTest])                         //  compare le bouton appuye et la valeur dans la sequence 
  {
    if (posTest<taille-1)                          // est ce la fin de sequence
      {
        posTest=posTest+1;                         // augmente le nombre de coups 
        mode=3;                                    // test du bouton     
      }
    else
      {
        valide();                                  // le resultat est juste sequence de validation
        mode=1;                                    // nouvelle sequence 
      }  
  }
else
  {
    nonValide();                                    // erreur du joueur sequence d'erreur
    taille=0;                                       // recommence la partie à un coup
    mode=1;                                         // debut de la nouvelle partie
  }  
}

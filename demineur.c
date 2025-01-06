#include <stdio.h>

#define MINES 20
#define GRILLE 10

int trouver_identifiant_depuis_position(int x, int y){
  //Renvoie l'identifiant correspondant à la case (x,y) du plateau
  //Identifiants comptés de gauche à droite puis de haut en bas
  return x + y*GRILLE;
}

int trouver_position_x_depuis_identifiant(int identifiant){
  return identifiant%(GRILLE);
}

int trouver_position_y_depuis_identifiant(int identifiant){
  return identifiant/(GRILLE);
}

void initialiser(char** plateau){
    int i = 0;
    while(i < MINES){
        mine = rand() % GRILLE*GRILLE;
        x = trouver_position_x_depuis_identifiant(mine);
        y = trouver_position_y_depuis_identifiant(mine);
        if(plateau[x][y] != "M"){
            plateau[x][y] = "M";
            i++;
        }
    }
}

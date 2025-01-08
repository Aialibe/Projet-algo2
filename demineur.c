#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <bool.h>

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



//plateau mines et les chiffres
//visible drapeau, pas vu la case, case vue

char** allouer_matrice(int taille){
    char** mat = malloc(sizeof(char*)*taille);
    for(int i=0;i<GRILLE;i++){
        mat[i]=malloc(sizeof(char)*taille);
    }
    return mat;
}

void liberer_matrice(char** mat,int taille){
    for(int i=0;i<taille;i++){
        free(mat[i]);
    }
    free(mat);
}


void initialiser_plateau(char** plateau){
    int i = 0;
    for(int x = 0; x < GRILLE; x++){
        for(int y = 0; y < GRILLE; y++){
            plateau[x][y] = '0'
        }
    }
    while(i < MINES){
        int mine = rand() % GRILLE*GRILLE;
        int x = trouver_position_x_depuis_identifiant(mine);
        int y = trouver_position_y_depuis_identifiant(mine);
        if(plateau[x][y] != 'M'){
            plateau[x][y] = 'M';
            for(int i = -1; i <= 1; i++){
                for(int j = -1; j <= 1; j++){
                    //Parcours de toutes les cases adjacentes à la mine qui vient d'être posée
                    if(x + i >= 0 && x + i < GRILLE && y + j >= 0 && y + j < GRILLE){
                        //On vérifie que la case adjacente courante ne dépasse pas du plateau
                        if(plateau[x + i][y + j] != 'M'){
                            //Cas où la case adjacente courante n'est pas une mine, et est donc un numéro qui doit être incrémenté en raison de la mine ayant été ajouté en (x, y)
                            plateau[x + i][y + j]++;
                            //Incrémenter un caractère correspondant à un chiffre compris entre 0 et 8 de 1 correspond à incrémenter son code ASCII de 1 donc pas besoin de passer par des entiers
                        }
                    }
                }
            }
            i++;
        }
    }
}

void initialiser_visible(char** visible){
  //initialise la matrice visible par le joueur, au depart, rien n'est dcouvert donc on met des # partout
  for(int i=0;i<GRILLE;i++){
    for(int j=0;j<GRILLE; j++){
      visible[i][j]='#';
    }
  }
}



void afficher_matrice_utilisateur(char** visible, char** plateau){
    printf("  ");
    for(int k=0; k<GRILLE;k++){
        printf("%d ",k);
    }
    printf("\n");
    for(int i=0;i<GRILLE;i++){
        printf("%d ",i);
        for(int j=0; j<GRILLE;j++){
            if(visible[i][j]=='D' || visible[i][j]=='#'){//si on a mis un drapeau ou la case n'est pas devoilee
                printf("%c ",visible[i][j]);
            }
            else{//la case est dévoilée
                printf("%c ",plateau[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void afficher_matrice(char** mat, int taille){
    for(int i=0;i<taille;i++){
        for(int j=0;j<taille;j++){
            printf("%c ",mat[i][j]);
        }
      printf("\n");
    }
  printf("\n");
}


void fonction_principale(){
    bool* perdu = false;
    bool* gagne = false;
    int compteur_de_temps;
    int* compteur_mines=MINES;
    
//init les matrices


    while(!perdu && !gagne){
        jouer(perdu, gagne, compteur_mines);
        afficher_matrice
    }
    if(perdu){
        printf("BOUH PERDU HAHA LOSER \n");
        
    }
    else{
        printf("TU AS GAGNE TOUS LES MARISSONS\n");
    }

}



int main(){
    char** plateau = allouer_matrice(GRILLE);
    initialiser_plateau(plateau);
    char** visible

    return 0;
}

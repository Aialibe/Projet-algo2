#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

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


void initialiser(char** plateau){
    int i = 0;
    while(i < MINES){
        int mine = rand() % GRILLE*GRILLE;
        int x = trouver_position_x_depuis_identifiant(mine);
        int y = trouver_position_y_depuis_identifiant(mine);
        if(plateau[x][y] != 'M'){
            plateau[x][y] = 'M';
            i++;
        }
    }
}

void afficher_matrice_utilisateur(char** visible, char** plateau){
    for(int i=0;i<GRILLE;i++){
        for(intj=0; j<GRILLE;j++){
            if(visible[i][j]=='D' || visible[i][j]=='#'){//si on a mis un drapeau ou la case n'est pas devoilee
                printf("%c",visible[i][j]);
            }
            else{//la case est dévoilée
                printf("%c",plateau[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

int main(){
    return 0;
}

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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

bool est_dans_plateau(int x, int y){
    //Renvoie true si (x, y) est dans le plateau, false sinon
    if(x >= 0 && x < GRILLE && y >= 0 && y < GRILLE){
        return true;
    }
    else{
        return false;
    }
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
                    if(est_dans_plateau(x,y)){
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
            if(visible[i][j]=='D' || visible[i][j]=='#' || visible[i][j]=='X'){//si on a mis un drapeau ou la case n'est pas devoilee
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


void decouvrir_chiffes_adjacents(char** plateau, char** visible, int x, int y);
void decouvrir_chiffes_adjacents(char** plateau, char** visible, int x, int y){
    //x c'est la ligne qui va de haut en bas et y la colonne qui va de gauche a droite
    //Découvre toutes les cases qui doivent être découvertes lorsque la case (x, y) est cassée

    if(visible[x][y]!='D' && plateau[x][y]!='M'){
        visible[x][y]=plateau[x][y];//on affiche la case, ca devrait etre un chiffre
        if(plateau[x][y] == '0'){
            for(int i=-1;i<=1;i++){
                //on regarde toutes les cases autours si c'est un 0
                for(int j=-1;j<=1;j++){
                    if(/*i!=0 && j!=0 && */plateau[x][y]=='#'){//je n'appelle pas sur une case déja devoilée
                        decouvrir_chiffes_adjacents(plateau, visible, x+i, y+j);
                    }
                }
            }
        }
    }
}



int nombre_drapeaux_adjacents(char** plateau, char** visible, bool* bien_places, int x, int y){
    *bien_places = true;
    int nombre_drapeaux = 0
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(est_dans_plateau(x + i, y + j)){
                if(visible[x + i][y + j] == 'D'){
                    nombre_drapeaux ++;
                    if(plateau[x][y] != 'M'){
                        *bien_places = false;
                    }
                }
            }
        }
    }
    return nombre_drapeaux;
}

void jouer(char** plateau, char** visible, bool* perdu, bool* gagne, int* compteur_mines){
    int action = 0;
    printf("Voulez-vous casser une case (1), poser ou casser un drapeau (2) ?");
    while(action != 1 && action != 2){
        scanf("%d", &action);
    }
    int x;
    printf("Entrez une ligne : ");
    scanf("%d", &x);
    int y;
    printf("Entrez une colonne : ");
    scanf("%d", &y);
    if(action == 1){
        //Le joueur casse une case
        if(visible[x][y] == 'D'){
            printf("Action impossible");
            jouer(plateau, visible, perdu, gagne, compteur_mines);
        }
        if(plateau[x][y] == 'M'){
            //Le jouer essaye de casser une mine : défaite
            *compteur_mines -= 1;
            *perdu = true;
            plateau[x][y] == 'X';       //Marquage resérvé pour la mine qui a fait perdre le jouer sous reserve d'existence
        }
        else{
            bool bien_places;
            int nombre_drapeaux = nombre_drapeaux_adjacents(plateau, visible, &bien_places, x, y);
            if(visible[x][y] >= '1' && visible[x][y] <= '9' && nombre_drapeaux == atoi(visible[x][y]) && !bien_places){
                *perdu = true;
            }
            else{
                //Le joueur découvre un chiffre
                decouvrir_chiffes_adjacents(plateau, visible, x, y);
            }
        }
    }
    else{
        //Le joueur pose ou casse un drapeau
        if(visible[x][y] == 'D'){
            //La case visée était un drapeau donc on le casse
            visible[x][y] = '#';
        }
        else if(visible[x][y] == '#'){
            //La case visée n'est pas un drapeau et n'est pas découverte, elle devient un drapeau
            visible[x][y] = 'D';
        }
        else{
            //Le joueur essaye de poser un drapeau sur une case déjà découverte
            printf("Action impossible");
            jouer(plateau, visible, perdu, gagne, compteur_mines);
        }
    }
}



void fonction_principale(){
    bool* perdu = false;
    bool* gagne = false;
    int* compteur_mines=MINES;
    long clk_tck = CLOCKS_PER_SEC;
    clock_t t_depart, t_apres_tour;
    int temps_ecoule_depuis_debut=0;//en secondes
 
    //init les matrices
    char** plateau = allouer_matrice(GRILLE);
    char** visible = allouer_matrice(GRILLE);
    initialiser_visible(visible);
    initialiser_plateau(plateau);

    t_depart = clock(); //le temps au debut de la partie, ce qui nous interesse c'est la difference de temps
    while(!perdu && !gagne){
        
        jouer(perdu, gagne, compteur_mines);
        t_apres_tour=clock();
        temps_ecoule_depuis_debut=t_apres_tour-t_depart;
        printf("Temps écoulé jusqu'ici : %d secondes \n\n",temps_ecoule_depuis_debut);
        afficher_matrice_utilisateur(visible, plateau);

    }
    if(perdu){
        printf("BOUH PERDU HAHA LOSER \n");
        
    }
    else{
        printf("TU AS GAGNE TOUS LES MARISSONS\n");
    }

}








int main(){
    fonction_principale();

    return 0;
}


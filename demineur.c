#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "ARBRE.h"

#define MINES 5
#define GRILLE 10
#define GRIS 189
#define EPAIS 3


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
    for(int i=0;i<taille;i++){
        mat[i]=malloc(sizeof(char)*taille);
    }
    return mat;
}

char** copier_matrice(char** mat, int taille){
    char** m = allouer_matrice(taille);
    for(int i = 0; i < taille; i++){
        for(int j = 0; j < taille; j++){
            m[i][j] = mat[i][j];
        }
    }
    return m;
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
            plateau[x][y] = '0';
        }
    }
    while(i < MINES){
        int mine = rand() % (GRILLE*GRILLE);
        printf("%d \n",mine);
        int x = trouver_position_x_depuis_identifiant(mine);
        int y = trouver_position_y_depuis_identifiant(mine);
        if(plateau[x][y] != 'M'){
            plateau[x][y] = 'M';
            for(int i = -1; i <= 1; i++){
                for(int j = -1; j <= 1; j++){
                    //Parcours de toutes les cases adjacentes à la mine qui vient d'être posée
                    if(est_dans_plateau(x + i, y + j)){
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
    printf("     ");
    for(int k=0; k<GRILLE;k++){
        printf("%2d ",k);
    }
    printf("\n \n");
    for(int i=0;i<GRILLE;i++){
        printf("%2d   ",i);
        for(int j=0; j<GRILLE;j++){
            if(visible[i][j]=='D' || visible[i][j]=='#' || visible[i][j]=='X'){//si on a mis un drapeau ou la case n'est pas devoilee
                printf("%c ",visible[i][j]);
            }
            else{//la case est dévoilée
                printf("%2c ",plateau[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void afficher_matrice(char** mat, int taille){
  printf("     ");
  for(int k=0; k<taille;k++){
      printf("%2d ",k);
  }
  printf("\n\n");
  for(int i=0;i<taille;i++){
      printf("%2d   ",i);
      for(int j=0;j<taille;j++){
          printf("%2c ",mat[i][j]);
      }
    printf("\n");
  }
  printf("\n");
}



//les super jolies fonctions d'affichage

void quelle_case(int x, int y,int* a){
    //renvoie les co de la ou on a clique sur la grille
    
    a[0]=y/40;//dans sdl, x va de gauche a droite et y de haut en bas
    a[1]=x/40;//dans notre truc, x va de haut en bas et y de gauche a droite
    //return a;
}

void afficher_zero(SDL_Renderer *renderer,int x, int y){
    //pour afficher dans la bonne case 
    
    x=x*40;y=y*40;
    //on creer un rectangle qu'on va afficher ensuite
    SDL_Rect rect = { x, y, 40, 40 };
    //on change la couleur du pinceau
    SDL_SetRenderDrawColor(renderer, GRIS, GRIS, GRIS, 255);//gris
    //on dessine le rectangle defini au dessus
    SDL_RenderFillRect(renderer, &rect);
    //on met a jour la fenetre pour afficher le rectangle dessine
    SDL_RenderPresent(renderer);
}

void afficher_un(SDL_Renderer *renderer,int x, int y){
    x=x*40;y=y*40;
    SDL_Rect rect = { x, y, 40, 40 };
    SDL_SetRenderDrawColor(renderer, GRIS, GRIS, GRIS, 255);//gris
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);//bleu
    SDL_Rect rect1 = { x+30, y+5, -EPAIS, 30 };
    SDL_RenderFillRect(renderer, &rect1);
    
    SDL_RenderPresent(renderer);
}

void afficher_deux(SDL_Renderer *renderer,int x, int y){
    //pour afficher dans la bonne case
    x=x*40;y=y*40;
    //on veut mettre le fond de la case ne gris clair
    SDL_Rect rect = { x, y, 40, 40 };
    SDL_SetRenderDrawColor(renderer, GRIS, GRIS, GRIS, 255);//gris
    SDL_RenderFillRect(renderer, &rect);
    //on va maintenant afficher les lignes qui font un 2 en vert
    SDL_Rect rect1 = { x+8, y+5, 22, EPAIS };
    SDL_Rect rect2 = { x+8, y+20, 22, EPAIS };
    SDL_Rect rect3 = { x+30, y+5, EPAIS, 15+EPAIS };
    SDL_Rect rect4 = { x+8, y+35, 22+EPAIS, -EPAIS };
    SDL_Rect rect5 = { x+8, y+20, EPAIS, 15 };

    SDL_SetRenderDrawColor(renderer, 0, 123, 0, 255);//vert
    SDL_RenderFillRect(renderer, &rect1);
    SDL_RenderFillRect(renderer, &rect2);
    SDL_RenderFillRect(renderer, &rect3);
    SDL_RenderFillRect(renderer, &rect4);
    SDL_RenderFillRect(renderer, &rect5);
    


    //on met a jour la fenetre
    SDL_RenderPresent(renderer);
}

void afficher_trois(SDL_Renderer *renderer,int x, int y){
    x=x*40;y=y*40;
    SDL_Rect rect = { x, y, 40, 40 };
    SDL_SetRenderDrawColor(renderer, GRIS, GRIS, GRIS, 255);//gris
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 12, 12, 255);//rouge
    SDL_Rect rect1 = { x+8, y+5, 22, EPAIS };
    SDL_Rect rect2 = { x+8, y+20, 22, EPAIS };
    SDL_Rect rect3 = { x+30, y+5, EPAIS, 30 };
    SDL_Rect rect4 = { x+8, y+35, 22, -EPAIS };
    SDL_RenderFillRect(renderer, &rect1);
    SDL_RenderFillRect(renderer, &rect2);
    SDL_RenderFillRect(renderer, &rect3);
    SDL_RenderFillRect(renderer, &rect4);


    SDL_RenderPresent(renderer);
}

void afficher_quatre(SDL_Renderer *renderer,int x, int y){
    x=x*40;y=y*40;
    SDL_Rect rect = { x, y, 40, 40 };
    SDL_SetRenderDrawColor(renderer, GRIS, GRIS, GRIS, 255);//gris
    SDL_RenderFillRect(renderer, &rect);
    SDL_Rect rect1 = { x+8, y+5, EPAIS, 15 };
    SDL_Rect rect2 = { x+8, y+20, 22, EPAIS };
    SDL_Rect rect3 = { x+30, y+5, -EPAIS, 30 };

    SDL_SetRenderDrawColor(renderer, 8, 8, 128, 255);//bleu foncé
    //SDL_RenderDrawLine(renderer,x+8,y+5,x+8,y+20);
    SDL_RenderFillRect(renderer, &rect1);
    SDL_RenderFillRect(renderer, &rect2);
    SDL_RenderFillRect(renderer, &rect3);
    SDL_RenderPresent(renderer);
}

void afficher_cinq(SDL_Renderer *renderer,int x, int y){
    x=x*40;y=y*40;
    SDL_Rect rect = { x, y, 40, 40 };
    SDL_SetRenderDrawColor(renderer, GRIS, GRIS, GRIS, 255);//gris
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 123, 0, 0, 255);//rouge foncé
    SDL_Rect rect1 = { x+8, y+5, 22, EPAIS };
    SDL_Rect rect2 = { x+8, y+20, 22, EPAIS };
    SDL_Rect rect3 = { x+30, y+20, EPAIS, 15 };
    SDL_Rect rect4 = { x+8, y+35, 22+EPAIS, -EPAIS };
    SDL_Rect rect5 = { x+8, y+5, EPAIS, 15 };
    SDL_RenderFillRect(renderer, &rect1);
    SDL_RenderFillRect(renderer, &rect2);
    SDL_RenderFillRect(renderer, &rect3);
    SDL_RenderFillRect(renderer, &rect4);
    SDL_RenderFillRect(renderer, &rect5);
    SDL_RenderPresent(renderer);
}

void afficher_six(SDL_Renderer *renderer,int x, int y){
    x=x*40;y=y*40;
    SDL_Rect rect = { x, y, 40, 40 };
    SDL_SetRenderDrawColor(renderer, GRIS, GRIS, GRIS, 255);//gris
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 123, 123, 255);//cyan
    SDL_Rect rect1 = { x+8, y+5, 22, EPAIS };
    SDL_Rect rect2 = { x+8, y+20, 22, EPAIS };
    SDL_Rect rect3 = { x+30, y+20, EPAIS, 15 };
    SDL_Rect rect4 = { x+8, y+35, 22+EPAIS, -EPAIS };
    SDL_Rect rect5 = { x+8, y+5, EPAIS, 30 };
    SDL_RenderFillRect(renderer, &rect1);
    SDL_RenderFillRect(renderer, &rect2);
    SDL_RenderFillRect(renderer, &rect3);
    SDL_RenderFillRect(renderer, &rect4);
    SDL_RenderFillRect(renderer, &rect5);
    SDL_RenderPresent(renderer);
}

void afficher_sept(SDL_Renderer *renderer,int x, int y){
    x=x*40;y=y*40;
    SDL_Rect rect = { x, y, 40, 40 };
    SDL_SetRenderDrawColor(renderer, GRIS, GRIS, GRIS, 255);//gris
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 22, 200, 40, 255);//vert ?
    SDL_Rect rect1 = { x+8, y+5, 22, EPAIS };
    SDL_Rect rect2 = { x+30, y+5, EPAIS, 30 };
    SDL_RenderFillRect(renderer, &rect1);
    SDL_RenderFillRect(renderer, &rect2);

    SDL_RenderPresent(renderer);
}

void afficher_huit(SDL_Renderer *renderer,int x, int y){
    x=x*40;y=y*40;
    SDL_Rect rect = { x, y, 40, 40 };
    SDL_SetRenderDrawColor(renderer, GRIS, GRIS, GRIS, 255);//gris
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 230, 90, 230, 255);//rose ?
    SDL_Rect rect1 = { x+8, y+5, 22, EPAIS };
    SDL_Rect rect2 = { x+8, y+20, 22, EPAIS };
    SDL_Rect rect3 = { x+30, y+5, EPAIS, 30 };
    SDL_Rect rect4 = { x+8, y+35, 22+EPAIS, -EPAIS };
    SDL_Rect rect5 = { x+8, y+5, EPAIS, 30 };
    SDL_RenderFillRect(renderer, &rect1);
    SDL_RenderFillRect(renderer, &rect2);
    SDL_RenderFillRect(renderer, &rect3);
    SDL_RenderFillRect(renderer, &rect4);
    SDL_RenderFillRect(renderer, &rect5); 
    SDL_RenderPresent(renderer);
}

void afficher_drapeau(SDL_Renderer *renderer,int x, int y){
    x=x*40;y=y*40;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);//rouge
    SDL_RenderDrawLine(renderer,x+20,y+5,x+8,y+11);
    SDL_RenderDrawLine(renderer,x+20,y+20,x+8,y+11);
    SDL_RenderDrawLine(renderer,x+20,y+5,x+20,y+20);
    SDL_RenderPresent(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//noir
    SDL_RenderDrawLine(renderer,x+20,y+20,x+8,y+35);
    SDL_RenderDrawLine(renderer,x+20,y+20,x+30,y+35);
    SDL_RenderDrawLine(renderer,x+8,y+35,x+30,y+35);
    SDL_RenderPresent(renderer);
}

void afficher_case_base(SDL_Renderer *renderer,int x, int y){
    x=x*40;y=y*40;
    SDL_Rect rect = { x, y, 39, 39 };
    SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);//gris fonce
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
}


void afficher_bombe(SDL_Renderer *renderer,int x, int y){
    x=x*40;y=y*40;
    SDL_Rect rect = { x, y, 39, 39 };
    SDL_SetRenderDrawColor(renderer, GRIS, GRIS, GRIS, 255);//gris
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);//rouge ?
    SDL_RenderDrawLine(renderer,x,y,x+40,y+40);
    SDL_RenderDrawLine(renderer,x+40,y,x,y+40);
    SDL_RenderPresent(renderer);
}




void init_jeu(SDL_Renderer *renderer){
    // Couleur de fond (gris)
    SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);//gris foncé de base
    SDL_RenderClear(renderer);

    // Couleur des carrés non remplis pour faire les lignes (noir)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    
    // Affichage de plusieurs carrés (les lignes)
    for (int i = 0; i < GRILLE; i++) {
        for(int j=0;j<GRILLE;j++){
            SDL_Rect rect = { i*40, 40*j, 40, 40 };
            SDL_RenderDrawRect(renderer,&rect);
        }
        
    }
    // Mise à jour de l'écran
    SDL_RenderPresent(renderer);
}


void mettre_a_jour_affichage(SDL_Renderer *renderer, char** visible, char** plateau){
    for(int i=0;i<GRILLE;i++){
        for(int j=0; j<GRILLE;j++){
            if(visible[i][j]=='D' || visible[i][j]=='#' || visible[i][j]=='X'){//si on a mis un drapeau ou la case n'est pas devoilee
                switch (visible[i][j]){
                    case 'D': afficher_drapeau(renderer,j,i);
                    break;
                    case 'X': afficher_bombe(renderer,j,i);
                    break;
                    case '#': afficher_case_base(renderer,j,i);
                    break;
                    default : afficher_case_base(renderer,j,i);
                    break;//ca fait rien, je pourrais autant mettre NULL
                }
            }
            else{//la case est dévoilée
                switch (plateau[i][j]){
                    case '0': afficher_zero(renderer,j,i);
                    break;
                    case '1': afficher_un(renderer,j,i);
                    break;
                    case '2': afficher_deux(renderer,j,i);
                    break;
                    case '3': afficher_trois(renderer,j,i);
                    break;
                    case '4': afficher_quatre(renderer,j,i);
                    break;
                    case '5': afficher_cinq(renderer,j,i);
                    break;
                    case '6': afficher_six(renderer,j,i);
                    break;
                    case '7': afficher_sept(renderer,j,i);
                    break;
                    case '8': afficher_huit(renderer,j,i);
                    break;
                    default :afficher_case_base(renderer,j,i);break;
                }
            }
        }
        printf("\n");
    }
}


void affichage_fin(SDL_Renderer *renderer, char** visible, char** plateau){
    for(int i=0;i<GRILLE;i++){
        for(int j=0; j<GRILLE;j++){
            if(visible[i][j]=='D' ){
                afficher_drapeau(renderer,j,i);
            }
            else{//la case est dévoilée
                switch (plateau[i][j]){
                    case '0': afficher_zero(renderer,j,i);
                    break;
                    case '1': afficher_un(renderer,j,i);
                    break;
                    case '2': afficher_deux(renderer,j,i);
                    break;
                    case '3': afficher_trois(renderer,j,i);
                    break;
                    case '4': afficher_quatre(renderer,j,i);
                    break;
                    case '5': afficher_cinq(renderer,j,i);
                    break;
                    case '6': afficher_six(renderer,j,i);
                    break;
                    case '7': afficher_sept(renderer,j,i);
                    break;
                    case '8': afficher_huit(renderer,j,i);
                    break;
                    case 'M': afficher_bombe(renderer,j,i);
                    break;
                    default :afficher_case_base(renderer,j,i);break;
                }
            }
        }
        printf("\n");
    }
}








void decouvrir_chiffes_adjacents(char** plateau, char** visible, int x, int y);
void decouvrir_chiffes_adjacents(char** plateau, char** visible, int x, int y){
    //x c'est la ligne qui va de haut en bas et y la colonne qui va de gauche a droite
    //Découvre toutes les cases qui doivent être découvertes lorsque la case (x, y) est cassée

    if(visible[x][y]!='D' && plateau[x][y]!='M'){
        visible[x][y]=plateau[x][y];//on affiche la case, ca devrait etre un chiffre
        if(plateau[x][y] == '0'){
            for(int i=-1;i<=1;i++){
                for(int j=-1;j<=1;j++){
                    //on regarde toutes les cases autours si c'est un 0
                    if(est_dans_plateau(x+i, y+j)){
                        if(visible[x+i][y+j]=='#'){
                            //je n'appelle pas sur une case déja devoilée
                            decouvrir_chiffes_adjacents(plateau, visible, x+i, y+j);
                        }
                    }
                }
            }
        }
    }
}



int compter_drapeaux_adjacents(char** plateau, char** visible, bool* bien_places, int x, int y){
    *bien_places = true;
    int nombre_drapeaux = 0;
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(est_dans_plateau(x + i, y + j)){
                if(visible[x + i][y + j] == 'D'){
                    nombre_drapeaux ++;
                    if(plateau[x + i][y + j] != 'M'){
                        *bien_places = false;
                    }
                }
            }
        }
    }
    return nombre_drapeaux;
}

bool est_ce_quon_a_gagne(char** visible, int taille, int compteur_mines){
    int compteur_de_drapeau=0;
    for(int i=0;i<taille;i++){
        for(int j=0;j<taille;j++){
            if(visible[i][j]=='#'){
                return false;
            }
            else if(visible[i][j]=='D'){
                compteur_de_drapeau++;
            }
        }
    }
    return (compteur_de_drapeau==compteur_mines);
}





void prochaine_etape(ARBRE a, int etape, int nb_etapes, int case_centrale, int TAILLE_ELT){
    if(etape < nb_etapes){
        if(etape == case_centrale){
            prochaine_etape(a, etape + 1, nb_etapes, case_centrale, TAILLE_ELT);
        }
        else{
            int i = etape/TAILLE_ELT;
            int j = etape%TAILLE_ELT;
            if(a->val[i][j] == '#'){
                ELT mat_mine = copier_matrice(a->val, TAILLE_ELT);
                mat_mine[i][j] = 'D';
                ELT mat_pas_mine = copier_matrice(a->val, TAILLE_ELT);
                ARBRE a_mine = ARBRE_creer(mat_mine, NULL, NULL);
                ARBRE a_pas_mine = ARBRE_creer(mat_pas_mine, NULL, NULL);
                a->g = a_mine;
                a->d = a_pas_mine;
                prochaine_etape(a->g, etape + 1, nb_etapes, case_centrale, TAILLE_ELT);
                prochaine_etape(a->d, etape + 1, nb_etapes, case_centrale, TAILLE_ELT);
            }
            else{
                prochaine_etape(a, etape + 1, nb_etapes, case_centrale, TAILLE_ELT);
            }
        }
    }
}

void creer_arbre_decision(ARBRE a, int TAILLE_ELT){
    int MILIEU = TAILLE_ELT/2;
    int nb_etapes = TAILLE_ELT*TAILLE_ELT;
    int case_centrale = (MILIEU+1)*(MILIEU+1) + MILIEU*MILIEU - 1;
    printf("case_centrale = %d\n", case_centrale);
    prochaine_etape(a, 0, nb_etapes, case_centrale, TAILLE_ELT);
}

bool est_possible_situation(ELT m, int TAILLE_ELT){
    //Renvoie true si la situation décrite pas m est bien définie et false sinon
   
    bool res = true;
    for(int x = 0; x < TAILLE_ELT; x++){
        for(int y = 0; y < TAILLE_ELT; y++){
            int nombre_voisins_pas_dans_matrice = 0;
            if(m[x][y] >= '0' && m[x][y] <= '8'){
                int nombre_drapeaux_adjacents = 0;
                for(int i = -1; i <= 1; i++){
                    for(int j = -1; j <= 1; j++){
                        if(x + i >= 0 && x + i < TAILLE_ELT && y + j >= 0 && y + j < TAILLE_ELT){
                            //Cas où la case (x+i, y+j) est dans la matrice m
                            if(m[x+i][y+j] == 'D'){
                                nombre_drapeaux_adjacents++;
                            }
                        }
                        else{
                            nombre_voisins_pas_dans_matrice++;
                        }
                    }
                }
                if(nombre_voisins_pas_dans_matrice == 0){
                    if(nombre_drapeaux_adjacents != (m[x][y] - 48)){
                        //48 est le code ascii de 0 donc m[x][y] - 48 correspond au chiffre associé au caractère m[x][y]
                        res = false;
                    }
                }
                else{
                    if(nombre_drapeaux_adjacents > (m[x][y] - 48) || nombre_drapeaux_adjacents + nombre_voisins_pas_dans_matrice < (m[x][y] - 48)){
                        res = false;
                    }
                }
            }
        }
    }
    return res;
}

void compter_feuilles_possibles(ARBRE a, int* nb_feuilles, int* nb_feuilles_possibles, int TAILLE_ELT){
    //Met à jour le nombre de feuilles et le nombre de feuilles correspondant à un état possible du plateau dans a
    if(a->g != NULL){
        //a est un arbre binaire strict donc vérification de l'existence d'un des 2 fils suffit
        //Cas où a n'est pas une feuille
        compter_feuilles_possibles(a->g, nb_feuilles, nb_feuilles_possibles, TAILLE_ELT);
        compter_feuilles_possibles(a->d, nb_feuilles, nb_feuilles_possibles, TAILLE_ELT);
    }
    else{
        //Cas où a est une feuille
        *nb_feuilles = *nb_feuilles + 1;
        if(est_possible_situation(a->val, TAILLE_ELT)){
            *nb_feuilles_possibles = *nb_feuilles_possibles + 1;
        }
    }
}

void ia(char** visible, int x_depart, int y_depart){
    //Renvoie le pourcentage de chances que la case (x_depart, y_depart) soit une mine
    //On suppose que (x_depart, y_depart) n'est pas découverte
    int nombre_cases_non_decouvertes = 0;
    int i = 1;
    while(i < GRILLE){
        for(int j = -i; j <= i; j++){
            if(est_dans_plateau(x_depart-i, y_depart+j)){
                if(visible[x_depart-i][y_depart+j] == '#'){
                    nombre_cases_non_decouvertes++;
                }
            }
        }
        for(int j = -i + 1; j <= i; j++){
            if(est_dans_plateau(x_depart+j, y_depart-i)){
                if(visible[x_depart+j][y_depart-i] == '#'){
                    nombre_cases_non_decouvertes++;
                }
            }
            if(est_dans_plateau(x_depart+j, y_depart+i)){
                if(visible[x_depart+j][y_depart+i] == '#'){
                    nombre_cases_non_decouvertes++;
                }
            }
        }
        for(int j = -i + 1; j < i; j++){
            if(est_dans_plateau(x_depart+i,y_depart+ j)){
                if(visible[x_depart+i][y_depart+j] == '#'){
                    nombre_cases_non_decouvertes++;
                }
            }
        }
        if(nombre_cases_non_decouvertes < 15){
            i++;
        }
        else{
            break;
        }
    }
    if(i%2 == 0){
        i-=1;       //TAILLE_ELT doit être impair
    }
    int TAILLE_ELT = i;
    int MILIEU = TAILLE_ELT/2;

    char** alentours = allouer_matrice(TAILLE_ELT);  //Cases adjacentes à (x_depart, y_depart)
    for(int x = 0; x < TAILLE_ELT; x++){
        for(int y = 0; y < TAILLE_ELT; y++){
            if(est_dans_plateau(x_depart + x - MILIEU, y_depart + y - MILIEU)){
                alentours[x][y] = visible[x_depart + x - MILIEU][y_depart + y - MILIEU];
            }
            else{
                alentours[x][y] = '-';
            }
        }
    }
    //Création des arbres de décision :
    ELT mat_mine = copier_matrice(alentours, TAILLE_ELT);
    mat_mine[MILIEU][MILIEU] = 'D';
    ARBRE a_mine = ARBRE_creer(mat_mine, NULL, NULL);
    creer_arbre_decision(a_mine, TAILLE_ELT);

    ELT mat_pas_mine = copier_matrice(alentours, TAILLE_ELT);
    ARBRE a_pas_mine = ARBRE_creer(mat_pas_mine, NULL, NULL);
    creer_arbre_decision(a_pas_mine, TAILLE_ELT);
  
    

    printf("a_mine :\n");
    ARBRE_afficher_feuilles(a_mine, TAILLE_ELT);
    printf("a_pas_mine\n");
    ARBRE_afficher_feuilles(a_pas_mine, TAILLE_ELT);

    int nb_feuilles_a_mine = 0;
    int nb_feuilles_possibles_a_mine = 0;
    compter_feuilles_possibles(a_mine, &nb_feuilles_a_mine, &nb_feuilles_possibles_a_mine, TAILLE_ELT);

    int nb_feuilles_a_pas_mine = 0;
    int nb_feuilles_possibles_a_pas_mine = 0;
    compter_feuilles_possibles(a_pas_mine, &nb_feuilles_a_pas_mine, &nb_feuilles_possibles_a_pas_mine, TAILLE_ELT);

    float ratio_possible_mine = (float)nb_feuilles_possibles_a_mine/(float)nb_feuilles_a_mine;
    float ratio_possible_pas_mine = (float)nb_feuilles_possibles_a_pas_mine/(float)nb_feuilles_a_pas_mine;

    ARBRE_liberer(a_mine);
    ARBRE_liberer(a_pas_mine);
    liberer_matrice(alentours, TAILLE_ELT);
    liberer_matrice(mat_pas_mine, TAILLE_ELT);
    liberer_matrice(mat_mine,TAILLE_ELT);
  
    bool securise;
    bool certitude;
    if(ratio_possible_mine == 0){
        securise = true;
        certitude = true;
    }
    else if(ratio_possible_pas_mine == 0){
        securise = false;
        certitude = true;
    }
    else if(ratio_possible_mine == 1 && ratio_possible_pas_mine == 1){
        securise = false;
        certitude = false;
    }
    else if(ratio_possible_mine == 1){
        securise = true;
        certitude = true;
    }
    else if(ratio_possible_pas_mine == 1){
        securise = false;
        certitude = true;
    }
    else{
        certitude = false;
        if(ratio_possible_pas_mine > ratio_possible_mine){
            securise = true;
        }
        else{
            securise = false;
        }
    }
    if(certitude){
        printf("C'est à 100 pour cent ");
    }
    else{
        printf("C'est probablement ");
    }
    if(securise){
        printf("securisé\n");
    }
    else{
        printf("pas securisé");
    }
}





void jouer(char** plateau, char** visible, bool* perdu, bool* gagne, int* compteur_mines, int x, int y, int act){
    
    if(!(*gagne)){
      int action = act;
      
      if(action == 1){
          //Le joueur casse une case
          printf("cas 1 \n");
          if(visible[x][y] == 'D'){
              printf("cas 1.D \n");
              printf("Action impossible\n");
              //jouer(plateau, visible, perdu, gagne, compteur_mines,x,y);
          }
          else if(plateau[x][y] == 'M'){
              printf("cas 1.M \n");
              //Le jouer essaye de casser une mine : défaite
              *compteur_mines -= 1;
              *perdu = true;
              plateau[x][y] = 'X';       //Marquage resérvé pour la mine qui a fait perdre le jouer sous reserve d'existence
          }
          else{
              printf("cas 1.autre \n");
              bool bien_places = false;
              int nombre_drapeaux_adjacents = compter_drapeaux_adjacents(plateau, visible, &bien_places, x, y);
              printf("x = %d | y = %d\n", x, y);
              if(visible[x][y] >= '1' && visible[x][y] <= '9' && nombre_drapeaux_adjacents == visible[x][y] - 48){
                  //48 est le code ascii de 0 donc visible[x][y] - 48 correspond au chiffre associé au caractère visible[x][y]
                  if(bien_places){
                      printf("cas 1.autre.particulier.decouvre \n");
                      for(int i = -1; i <= 1; i++){
                          for(int j = -1; j <= 1; j++){
                              if(est_dans_plateau(x+i,y+j)){
                                  if(visible[x+i][y+j] != 'D'){
                                      visible[x+i][y+j] = plateau[x+i][y+j];
                                  }
                              }
                          }
                      }
                  }
                  else{
                      printf("cas 1.autre.particulier.perdu \n");
                      *perdu = true;
                  }
                  
              }
              else{
                  printf("cas 1.autre.decouvr \n");
                  //Le joueur découvre un chiffre
                  decouvrir_chiffes_adjacents(plateau, visible, x, y);
              }
          }
      }
      else if(action==2){
          printf("cas 2 \n");
          //Le joueur pose ou casse un drapeau
          if(visible[x][y] == 'D'){
              printf("cas 2.D \n");
              //La case visée était un drapeau donc on le casse
              printf("avant \n");
              visible[x][y] = '#';
              printf("apres\n");
          }
          else if(visible[x][y] == '#'){
              printf("cas 2.# \n");
              //La case visée n'est pas un drapeau et n'est pas découverte, elle devient un drapeau
              visible[x][y] = 'D';
          }
          else{
              //Le joueur essaye de poser un drapeau sur une case déjà découverte
              printf("cas 2.imposs \n");
              printf("Action impossible\n");
              //jouer(plateau, visible, perdu, gagne, compteur_mines,x,y,action);
              /*Quznd on fait une action impossible (casser la ou ya un drapeau), et qu'on, dans le meme tour, 
              eneleve le drapeua on obtient un 0 la ou yavait le drapeau*/
          }
      }
      else{//action = 3, clique molette donc ia
        printf("action = 3\n");
        ia(visible,x,y);
      }
    }
}


void fonction_principale(){
    srand (time(NULL));
    bool perdu = false;
    bool gagne = false;
    int compteur_mines= MINES;
    //long clk_tck = CLOCKS_PER_SEC;
    clock_t t_depart, t_apres_tour;
    int temps_ecoule_depuis_debut=0;//en secondes
 
    //init les matrices
    char** plateau = allouer_matrice(GRILLE);
    char** visible = allouer_matrice(GRILLE);
    initialiser_visible(visible);
    initialiser_plateau(plateau);

//partie affichage graphique
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        //return 1;
    }
    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow("Affichage de carrés", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GRILLE*40, GRILLE*40, SDL_WINDOW_SHOWN);
    SDL_SetWindowTitle(window,"Demineur");
    // Création du renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    init_jeu(renderer);
//fin partie affichage graphique


    afficher_matrice_utilisateur(visible, plateau);
    afficher_matrice(plateau,GRILLE);
    t_depart = clock(); //le temps au debut de la partie, ce qui nous interesse c'est la difference de temps
    
    Uint32 buttons;
    SDL_Event events; 
    SDL_bool run = SDL_TRUE;
    //tan que qu'on ferme pas la fenetre on joue
    while(!perdu && !gagne && run){
        while (SDL_PollEvent(&events)) {//si ya on fait un truc en particulier (ici click souris interessant)
            switch(events.type){
                case SDL_WINDOWEVENT://si on clique sur la croix de la fentre
                    if (events.window.event == SDL_WINDOWEVENT_CLOSE)
                        run = SDL_FALSE;//on ferme la fenetre et fin du jeu
                    break;
                case SDL_MOUSEBUTTONDOWN: // Click de souris 
                    int x; int y; int action=0;
                    int* kase=malloc(sizeof(int)*2);
                    //int* kase;
                    buttons =SDL_GetMouseState(&x, &y);
                    
                    quelle_case(x,y,kase);//je recupère la case ou on a clique
                    SDL_Log("+clic en %d %d",kase[0],kase[1]);
                    action=0;
                    if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)){
                        action=1;//clique gauche
                        printf("gauche");
                    }
                    else if(buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)){
                        printf("droite");
                        action=2;
                    }
                    else if(buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)){
                        printf("clique molette");
                        action=3;
                    }
                    
                    jouer(plateau, visible, &perdu, &gagne, &compteur_mines,kase[0],kase[1],action);
                    free(kase);
                    t_apres_tour=clock();
                    temps_ecoule_depuis_debut=(t_apres_tour-t_depart) /CLOCKS_PER_SEC;
                    printf("Temps écoulé jusqu'ici : %d secondes \n\n",temps_ecoule_depuis_debut);
                    mettre_a_jour_affichage(renderer,visible, plateau);
                    afficher_matrice_utilisateur(visible,plateau);
                    //SDL_RenderPresent(renderer);
                        //SDL_RenderClear(renderer);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                
                // Affichage de plusieurs carrés noirs pour remettre les lignes entre les cases
                    for (int i = 0; i < GRILLE; i++) {
                        for(int j=0;j<GRILLE;j++){
                            SDL_Rect rect = { i*40, 40*j, 40, 40 };
                            SDL_RenderDrawRect(renderer,&rect);
                        }
                        
                    }
                    SDL_RenderPresent(renderer);
                                break;
                                
                }
                gagne=est_ce_quon_a_gagne(visible, GRILLE, compteur_mines);
        }
        
    
    }
    

    
    if(perdu){
        printf("BOUH PERDU HAHA LOSER \n");
        affichage_fin(renderer,visible,plateau);
        SDL_Delay(3000);
        
    }
    else if(gagne){
        printf("TU AS GAGNE TOUS LES MARISSONS\n");
        affichage_fin(renderer,visible,plateau);
        SDL_Delay(3000);
       
    }
    
    // Nettoyage
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    liberer_matrice(plateau, GRILLE);
    liberer_matrice(visible, GRILLE);

}





int main(){
    fonction_principale();

    return 0;
}

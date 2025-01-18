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
    printf("    ");
    for(int k=0; k<GRILLE;k++){
        printf("%d ",k);
    }
    printf("\n \n");
    for(int i=0;i<GRILLE;i++){
        printf("%d   ",i);
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
  printf("    ");
  for(int k=0; k<taille;k++){
      printf("%d ",k);
  }
  printf("\n\n");
  for(int i=0;i<taille;i++){
      printf("%d   ",i);
      for(int j=0;j<taille;j++){
          printf("%c ",mat[i][j]);
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

void dessiner_zero(SDL_Renderer *renderer,int x, int y){
    //pour dessiner dans la bonne case 
    
    x=x*40;y=y*40;
    //on creer un rectangle qu'on va dessiner ensuite
    SDL_Rect rect = { x, y, 40, 40 };
    //on change la couleur du pinceau
    SDL_SetRenderDrawColor(renderer, GRIS, GRIS, GRIS, 255);//gris
    //on dessine le rectangle defini au dessus
    SDL_RenderFillRect(renderer, &rect);
    //on met a jour la fenetre pour dessiner le rectangle dessine
    SDL_RenderPresent(renderer);
}

void dessiner_un(SDL_Renderer *renderer,int x, int y){
    x=x*40;y=y*40;
    SDL_Rect rect = { x, y, 40, 40 };
    SDL_SetRenderDrawColor(renderer, GRIS, GRIS, GRIS, 255);//gris
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);//bleu
    SDL_Rect rect1 = { x+30, y+5, -EPAIS, 30 };
    SDL_RenderFillRect(renderer, &rect1);
    
    SDL_RenderPresent(renderer);
}

void dessiner_deux(SDL_Renderer *renderer,int x, int y){
    //pour dessiner dans la bonne case
    x=x*40;y=y*40;
    //on veut mettre le fond de la case ne gris clair
    SDL_Rect rect = { x, y, 40, 40 };
    SDL_SetRenderDrawColor(renderer, GRIS, GRIS, GRIS, 255);//gris
    SDL_RenderFillRect(renderer, &rect);
    //on va maintenant dessiner les lignes qui font un 2 en vert
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

void dessiner_trois(SDL_Renderer *renderer,int x, int y){
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

void dessiner_quatre(SDL_Renderer *renderer,int x, int y){
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

void dessiner_cinq(SDL_Renderer *renderer,int x, int y){
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

void dessiner_six(SDL_Renderer *renderer,int x, int y){
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

void dessiner_sept(SDL_Renderer *renderer,int x, int y){
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

void dessiner_huit(SDL_Renderer *renderer,int x, int y){
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

void dessiner_drapeau(SDL_Renderer *renderer,int x, int y){
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

void dessiner_case_base(SDL_Renderer *renderer,int x, int y){
    x=x*40;y=y*40;
    SDL_Rect rect = { x, y, 39, 39 };
    SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);//gris fonce
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
}


void dessiner_bombe(SDL_Renderer *renderer,int x, int y){
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
                    case 'D': dessiner_drapeau(renderer,j,i);
                    break;
                    case 'X': dessiner_bombe(renderer,j,i);
                    break;
                    case '#': dessiner_case_base(renderer,j,i);
                    break;
                    default : dessiner_case_base(renderer,j,i);
                    break;//ca fait rien, je pourrais autant mettre NULL
                }
            }
            else{//la case est dévoilée
                switch (plateau[i][j]){
                    case '0': dessiner_zero(renderer,j,i);
                    break;
                    case '1': dessiner_un(renderer,j,i);
                    break;
                    case '2': dessiner_deux(renderer,j,i);
                    break;
                    case '3': dessiner_trois(renderer,j,i);
                    break;
                    case '4': dessiner_quatre(renderer,j,i);
                    break;
                    case '5': dessiner_cinq(renderer,j,i);
                    break;
                    case '6': dessiner_six(renderer,j,i);
                    break;
                    case '7': dessiner_sept(renderer,j,i);
                    break;
                    case '8': dessiner_huit(renderer,j,i);
                    break;
                    default :dessiner_case_base(renderer,j,i);break;
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
                dessiner_drapeau(renderer,j,i);
            }
            else{//la case est dévoilée
                switch (plateau[i][j]){
                    case '0': dessiner_zero(renderer,j,i);
                    break;
                    case '1': dessiner_un(renderer,j,i);
                    break;
                    case '2': dessiner_deux(renderer,j,i);
                    break;
                    case '3': dessiner_trois(renderer,j,i);
                    break;
                    case '4': dessiner_quatre(renderer,j,i);
                    break;
                    case '5': dessiner_cinq(renderer,j,i);
                    break;
                    case '6': dessiner_six(renderer,j,i);
                    break;
                    case '7': dessiner_sept(renderer,j,i);
                    break;
                    case '8': dessiner_huit(renderer,j,i);
                    break;
                    case 'M': dessiner_bombe(renderer,j,i);
                    break;
                    default :dessiner_case_base(renderer,j,i);break;
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



void jouer(char** plateau, char** visible, bool* perdu, bool* gagne, int* compteur_mines, int x, int y, int act){
    *gagne=est_ce_quon_a_gagne(visible, GRILLE, *compteur_mines);
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
      else{
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
    }
}


void fonction_principale(){
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


    dessiner_matrice_utilisateur(visible, plateau);
    dessiner_matrice(plateau,GRILLE);
    t_depart = clock(); //le temps au debut de la partie, ce qui nous interesse c'est la difference de temps

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
                    int x, y;
                    //int* kase;
                    Uint32 buttons =SDL_GetMouseState(&x, &y);
                    int* kase=malloc(sizeof(int)*2);
                    quelle_case(x,y,kase);//je recupère la case ou on a clique
                    SDL_Log("+clic en %d %d",kase[0],kase[1]);
                    int action=0;
                    if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)){
                        action=1;//clique gauche
                        printf("gauche");
                    }
                    else if(buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)){
                        printf("droite");
                        action=2;
                    }
                    
                    jouer(plateau, visible, &perdu, &gagne, &compteur_mines,kase[0],kase[1],action);
                    free(kase);
                    t_apres_tour=clock();
                    temps_ecoule_depuis_debut=(t_apres_tour-t_depart) /CLOCKS_PER_SEC;
                    printf("Temps écoulé jusqu'ici : %d secondes \n\n",temps_ecoule_depuis_debut);
                    mettre_a_jour_affichage(renderer,visible, plateau);
                    dessiner_matrice_utilisateur(visible,plateau);
                    //SDL_RenderPresent(renderer);
                    break;
                }
        }
        
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

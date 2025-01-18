#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "ARBRE.h"

ARBRE ARBRE_creer(ELT elt, ARBRE ag, ARBRE ad) {
  ARBRE a = malloc(sizeof(struct Noeud));
  if(a!=NULL) {
    a->val=elt;
    a->g=ag; 
    a->d=ad;
  }
  return a;
}

void ARBRE_lire(ARBRE a, ELT * pelt, bool * perr) {
  if(a==NULL) {
    *perr=true;
  } else {
    *perr=false;
    *pelt=a->val;
  }
}

void ARBRE_ss_ag(ARBRE a, ARBRE * pssag, bool * perr) {
  if(a==NULL) {
    *perr=true;
  } else {
    *perr=false;
    *pssag=a->g;
  }
}

void ARBRE_ss_ad(ARBRE a, ARBRE * pssad, bool * perr) {
  if(a==NULL) {
    *perr=true;
  } else {
    *perr=false;
    *pssad=a->d;
  }
}

void ARBRE_aff_valeur(ARBRE a, ELT elt, bool * perr) {
  if(a==NULL) {
    *perr=true;
  } else {
    *perr=false;
    a->val=elt;
  }
} 
void ARBRE_aff_ss_ag(ARBRE a, ARBRE ag, bool * perr) {
  if(a==NULL) {
    *perr=true;
  } else {
    *perr=false;
    a->g=ag;
  }
} 
void ARBRE_aff_ss_ad(ARBRE a, ARBRE ad, bool * perr) {
  if(a==NULL) {
    *perr=true;
  } else {
    *perr=false;
    a->d=ad;
  }
} 

ARBRE ARBRE_rechercher(ARBRE a, ELT elt, int TAILLE_ELT) {
  ARBRE at;
  /*cas de l'arbre vide */
  if(a==NULL) {
    return NULL;
  } else {
    /* si on a trouve */
    if(ELT_eg(a->val,elt,TAILLE_ELT)) {
      return a;
    } else {
      /* sinon on cherche a gauche */
      at = ARBRE_rechercher(a->g,elt, TAILLE_ELT);
      if(at!=NULL) {
	return at;
      } else {
	/* si pas trouve a gauche, on cherche a droite */
	return ARBRE_rechercher(a->d,elt, TAILLE_ELT);
      }
    }
  }
}

void ARBRE_afficher(ARBRE a, int TAILLE_ELT) {
  if(a==NULL) { /* Base : affichage de l'arbre vide */
    printf("NULL");
  } else { /* Recurrence */
    /* on affiche l'etiquette */ 
    printf("<");
    ELT_afficher(a->val, TAILLE_ELT);
    printf(",");
    /* on affiche a gauche */
    ARBRE_afficher(a->g, TAILLE_ELT);
    printf(",");
    /* on affiche a droite */
    ARBRE_afficher(a->d, TAILLE_ELT);
    printf(">");
  }
}

void ARBRE_afficher_feuilles(ARBRE a, int TAILLE_ELT) {
  if(a != NULL){
    //Cas d'arrêt : arbre vide
    if(a->g == NULL && a->d == NULL){
      //Cas où a est une feuille : on affiche
      ELT_afficher(a->val, TAILLE_ELT);
      printf("---------------------------\n");
    }
    else{
      //Cas où a n'est pas une feuille : récurrence
      ARBRE_afficher_feuilles(a->g, TAILLE_ELT);
      ARBRE_afficher_feuilles(a->d, TAILLE_ELT);
    }
  }
}

void ARBRE_liberer(ARBRE a){
    if(a != NULL){
        ARBRE_liberer(a->d);
        ARBRE_liberer(a->g);
        free(a);
    }
}

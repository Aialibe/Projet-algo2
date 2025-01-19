#include <stdio.h>
#include <stdlib.h>
#include "ELT.h"

bool ELT_eg(ELT e1, ELT e2, int TAILLE_ELT) {
  bool res = true;
  for(int x = 0; x < TAILLE_ELT; x++){
    for(int y = 0; y < TAILLE_ELT; y++){
      if(e1[x][y] != e2[x][y]){
        res = false;
      }
    }
  }
  return res;
}

void ELT_afficher(ELT e, int TAILLE_ELT) {
  printf("\n    ");
  for(int k=0; k<TAILLE_ELT;k++){
      printf("%d ",k);
  }
  printf("\n\n");
  for(int i=0;i<TAILLE_ELT;i++){
      printf("%d   ",i);
      for(int j=0;j<TAILLE_ELT;j++){
          printf("%c ",e[i][j]);
      }
    printf("\n");
  }
  printf("\n");
}

void ELT_liberer(ELT e, int TAILLE_ELT){
  for(int i = 0; i < TAILLE_ELT; i++){
    free(e[i]);
  }
  free(e);
}

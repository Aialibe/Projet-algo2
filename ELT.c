#include <stdio.h>
#include "ELT.h"

bool ELT_eg(ELT e1, ELT e2) {
  bool res = true;
  for(int x = 0; x < 3; x++){
    for(int y = 0; y < 3; y++){
      if(e1[x][y] != e2[x][y]){
        res = false;
      }
    }
  }
  return res;
}

void ELT_afficher(ELT e) {
  printf("    ");
  for(int k=0; k<3;k++){
      printf("%d ",k);
  }
  printf("\n\n");
  for(int i=0;i<3;i++){
      printf("%d   ",i);
      for(int j=0;j<3;j++){
          printf("%c ",e[i][j]);
      }
    printf("\n");
  }
  printf("\n");
}
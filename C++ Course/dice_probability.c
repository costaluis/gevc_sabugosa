#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIDES 6
#define R_SIDE (rand() % SIDES + 1)

int main(){
    int trials;
    int n_dice = 2;
    int d1, d2;
    int * outcomes = calloc(13, sizeof(int));
    srand(clock());
    printf("\nEnter number of trials: ");
    scanf("%d", &trials);
    for(int j=0; j< trials; j++){
        outcomes[(d1 = R_SIDE) + (d2 = R_SIDE)]++;
    }
    printf("probability\n");
    for(int j=2; j<n_dice * SIDES + 1; j++){
        printf("j= %d | p= %lf\n",j,(double)outcomes[j]/trials*100);
    }
    free(outcomes);
}


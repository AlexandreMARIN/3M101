
#include "road_traffic.h"
#include <stdlib.h>
#include <stdio.h>


road * road_generation(int places, unsigned long int *n){

    /* number of possibilities : factorial(length)/factorial(length-places) */

    int i, j;
    road * ptr;
    int comb[places];//combinations

    //particular cases
    if(places==0){
        *n = 1lu;
        return (road*)calloc(1, sizeof(road));
    }else{
        if(places==BITSIZE){
            *n = 1lu;
            ptr = (road*)calloc(1, sizeof(road));
            *ptr = ~(*ptr);
            return ptr;
        }
    }


    *n = BITSIZE;//computes all the possibilities : C^{places}_{length}
    if(places*2<BITSIZE){//length must be different from 0
        for(i=1;i<places;i++){
            *n *= BITSIZE-i;
        }
        for(i=2;i<=places;i++){
            *n /= i;
        }
    }else{
        for(i=1;i<BITSIZE-places;i++){
            *n *= BITSIZE-i;
        }
        for(i=2;i<=BITSIZE-places;i++){
            *n /= i;
        }
    }

    ptr = (road*)calloc((*n), sizeof(road));//integers are set to zero
    if(!ptr){
        fprintf(stderr, "road_generation : not enough memory\n");
        return NULL;
    }

    /* elements of the table ptr are initialized here */
    for(i=0;i<places;i++){
        comb[i] = i;
    }

    for(i=0;i<*n;i++){//i : index of the current road
        //this block sets some bits to 1
        for(j=0;j<places;j++){//j : index of the current bit of the current road
            ptr[i] |= (1u<<((BITSIZE-1)-(comb[j]%BITSIZE)));
        }

        if(comb[places-1]==BITSIZE-1){//this block computes the next combination
            j = places-1;
            while(j>=1 && comb[j]==comb[j-1]+1){
                j--;
            }
            if(j>0){
                j--;
            }
            comb[j]++;
            for(j++;j<places;j++){
                comb[j] = comb[j-1]+1;
            }
        }else{
            comb[places-1]++;
        }
    }

    return ptr;
}

void road_show(road r){

    int k;
    printf("\n\n\t");
    for(k=BITSIZE-1;k>=0;k--){
        if((r&(1u<<k))==0u){
            printf("0");
        }else{
            printf("1");
        }
    }

    printf("\n");

}

void road_function(road r, road *res){

    *res = ((~r)&((r>>1)|(r<<(BITSIZE-1))))|(r&((r<<1)|(r>>(BITSIZE-1))));

}

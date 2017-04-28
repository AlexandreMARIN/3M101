
#include "road_traffic.hpp"
#include <cstdlib>
#include <cstdio>


/* operators overloading */

int road::length = 6;

int road::len(){
    return length;
}

void road::set_length(int l){
    length = l;
}

road road::operator= (const road &op){
    this->i = op.i;
    return *this;
}

road road::operator= (const int &op){
    this->i = op;
    return *this;
}

road road::operator<< (const int &op) const{
    road res;
    res.i = this->i;
    res.i <<= op;
    return res;
}

road road::operator>> (const int &op) const{
    road res;
    res.i = this->i;
    res.i <<= (32-length);//some bits are unused at the left of the integer.
    res.i >>= 32-length+op;//so they must be set to 0 : it apes the operator >> for classic integers by adding zeros at the left.
    return res;
}

road::operator uint32_t() const{
    uint32_t i = this->i;
    i <<= (32-length);
    i >>= (32-length);//it is possible that some bits at the left of the integer are set to 1.
    return i;//the shift is necessary.
}

road operator~ (const road &op){
    road res;
    res.i = ~(op.i);
    return res;
}

road operator& (const road &op1, const road &op2){
    road res;
    res.i = op1.i & op2.i;
    return res;
}

road operator| (const road &op1, const road &op2){
    road res;
    res.i = op1.i | op2.i;
    return res;
}

road operator|= (road &op1, const unsigned int &op2){
    op1.i |= op2;
    return op1;
}

bool operator== (const road &op1, const road &op2){
    return (op1.i<< (32-road::length)) == (op2.i<< (32-road::length));//for the same reasons above (only the first length bits at the right are considered)
}

int road::key(int c, int cars){
    int p = 0, n = length;
    uint32_t u = 1u<<(length-1);

    while(c!=1){
        if(u & i){
            c *= cars;
            c /= n;
            n--;
            cars--;
        }else{
            p += (c*cars)/n;
            c *= (n-cars);
            c /= n;
            n--;
        }
        u >>= 1;
    }

    return p;
}

/* other functions */

road * road_generation(int places, uint64_t *n){

    /* number of possibilities : factorial(length)/factorial(length-places) */

    int i, j;
    road * ptr;
    int comb[places];//combinations

    //particular cases
    if(places==0){
        *n = 1lu;
        return (road*)calloc(1, sizeof(road));
    }else{
        if(places==road::len()){
            *n = 1lu;
            ptr = (road*)calloc(1, sizeof(road));
            *ptr = ~(*ptr);
            return ptr;
        }
    }


    *n = road::len();//computes all the possibilities : C^{places}_{length}
    if(places*2<road::len()){//length must be different from 0
        for(i=1;i<places;i++){
            *n *= road::len() - i;
        }
        for(i=2;i<=places;i++){
            *n /= i;
        }
    }else{
        for(i=1;i<road::len() - places;i++){
            *n *= road::len() - i;
        }
        for(i=2;i<=road::len() - places;i++){
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

    for(i=0;i<(int)*n;i++){//i : index of the current road
        //this block sets some bits to 1
        for(j=0;j<places;j++){//j : index of the current bit of the current road
            ptr[i] |= (1u<<((road::len() - 1)- comb[j]));//the location of a bit is count from left here
        }

        if(comb[places-1]==road::len()-1){//this block computes the next combination
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
    for(k=road::len() - 1;k>=0;k--){
        if((r&(1u<<k))==0u){
            printf("0");
        }else{
            printf("1");
        }
    }

    printf("\n");

}

void road_function(road r, road *res){

    *res = ((~r)&((r>>1)|(r<<(road::len()-1))))|(r&((r<<1)|(r>>(road::len()-1))));

}

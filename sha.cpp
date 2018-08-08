#include<stdio.h>
#include<string.h>
#include<secp256k1.h>
#include<iostream>
#include<string>
#include<cstring>

#include "crypto/utils.h"



void printCode(unsigned char* out,int outsize){
  printf("digest result:\n");
  int i;
  for(i=0;i<outsize;i++){
    printf("%02x\n",out[i]);
  }
  printf("\n");
}


int main(){
  
}

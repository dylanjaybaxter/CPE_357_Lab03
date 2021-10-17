/*
main.c
Project: CPE 357 Lab03
Author: Dylan dybaxter
Description: This file contains the main functionality of a program that
reads in a file, builds a huffman tree and ouputs a list of the encoded values
*/
#include<stdio.h>
#include<unistd.h>
#include<ctype.h>
#include<getopt.h>
#include"huffman.h"

int main(int argc, char* const argv[]){
    /*If options included, exit*/
    int opt;
    while((opt = getopt(argc, argv,":n:")) != -1){
        perror("usage: htable filename");
        exit(EXIT_FAILURE);
    }
    /* For file*/
    int* freqs;
    /*Generate Character Histogram*/
    freqs = countFreq(argv[optind]);

    /*Build Huffman Tree*/
    huff_Node* tree = createHTree(freqs);

    /*Generate Codes*/
    codeIndex* codeInd = genCodes(tree);

    /*Print Codes*/
    unsigned int i;
    int j;
    unsigned int code;
    for(i=0;i<256;i++){
        if(codeInd->lens[i] != 0 ){
            printf("0x%02x:  ",i);
            code = codeInd->codes[i];
            for(j=0;j<codeInd->lens[i];j++){
                printf("%d", code%2);
                code = code >> 1;
            }
            printf("\n");
        }
    }

    /*Free Memory*/
    free(freqs);
    freeTree(tree);
    freeIndex(codeInd);
    return 0;
}

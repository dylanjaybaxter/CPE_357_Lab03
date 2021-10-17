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
#include<math.h>
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
    char buff[16] = "";
    for(i=0;i<256;i++){
        code = codeInd->codes[i];
        if(codeInd->lens[i] != 0 ){
            /* Fill Buffer with bits in correct order*/
            for(j=0;j<codeInd->lens[i];j++){
                buff[codeInd->lens[i]-j-1] = ((code%2)+48);
                code = code>>1;
            }
            /*Print bits*/
            printf("0x%02x: ", i);
            for(j=0;j<codeInd->lens[i];j++){
                printf("%c", buff[j]);
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

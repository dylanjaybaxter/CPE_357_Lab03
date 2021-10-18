/*
huffman.h
Project: CPE 357 Lab03
Author: Dylan dybaxter
Description: This file defines functions and structures contained in
huffman.c
*/
#include <stdlib.h>
#include <string.h>
#include<stdio.h>
#include<unistd.h>
#include<getopt.h>

typedef struct huff_Node huff_Node;

/*Huffman Node*/
typedef struct huff_Node{
    int freq;
    int c;
    huff_Node *next;
    huff_Node *prev;
    huff_Node *left;
    huff_Node *right;
}huff_Node;

/*Node List*/
typedef struct{
    int items;
    huff_Node* head;
    huff_Node* tail;
}Node_List;

typedef struct{
    int* codes;
    int* lens;
}codeIndex;

/*-------------------Initializers-------------------*/
Node_List* init_Node_List();

huff_Node* init_Huff_Node(int freq, int c);

codeIndex* init_CodeIndex();

/*-------------------Deconstructors-------------------*/
void freeTree(huff_Node* tree);

void freeIndex(codeIndex* index);

void freeList(Node_List* list);

/*-------------------Ordered Lists------------------*/
/*Creates a huffman tree frm list of freqs*/
Node_List* createNodeList(int* freqs);

/*Compares 2 nodes frequencies: outputs 1 if 1>2, -1 if 1<2 and 0 if equal*/
int compareNodes(huff_Node* node1, huff_Node* node2);

/*Insert node to list in order*/
void insertNode(Node_List* list, huff_Node* node);

/*Pops the minimum value from the stack as node*/
huff_Node* popMin(Node_List* list);


/*-------------------Huffman Functions-------------------*/
/*Counts Frequency of characters from 0 to 255*/
int* countFreq(char *filename);

/*Creates huffman codes from tree*/
huff_Node* createHTree(int* freqs);

/*Generate Codes Recursively from tree*/
void genCodesRecur(huff_Node* tree, codeIndex* index, int code, int len);

/* Helps internalize Recursion*/
codeIndex* genCodes(huff_Node* tree);

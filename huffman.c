/*
huffman.c
Project: CPE 357 Lab03
Author: Dylan dybaxter
Description: This file contains functions and structures necessary for
huffman encoding
*/
#include<unistd.h>
#include<stdio.h>
#include"huffman.h"


/*------------------------- List Functions---------------------------*/
int compareNodes(huff_Node* node1, huff_Node* node2){
    /* Compare Frequencies*/
    if(node1->freq > node2->freq){
        return 1;
    }
    else if(node1->freq < node2->freq){
        return -1;
    }
    else{
        /* Compare Characters*/
        if(node1->c > node2->c){
            return 1;
        }
        else if(node1->c < node2->c){
            return -1;
        }
        else{
            return 0;
        }
    }
}

Node_List* init_Node_List(){
    Node_List* list = (Node_List*)malloc(sizeof(Node_List));
    list->items = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void freeList(Node_List* list){
    /*if((list->head != NULL) && (list->tail != NULL)){
        while(list->head != list->tail){
            free(list->tail);
            list->tail = (huff_Node*)list->tail->next;
        }
        free(list->tail);
    }*/
    free(list);
}

huff_Node* init_Huff_Node(int freq, int c){
    huff_Node* newNode = (huff_Node*)malloc(sizeof(huff_Node));
    newNode->freq = freq;
    newNode->c = c;
    newNode->prev = NULL;
    newNode->next = NULL;
    newNode->right = NULL;
    newNode->left = NULL;
    return newNode;
}

void insertNode(Node_List* list, huff_Node* node){
    /* If list is empty*/
    if(list->head==NULL && list->tail==NULL){
        list->head = node;
        list->tail = node;
        list->items = 1;
    }
    else{
        /*If list is not empty*/
        huff_Node* currNode = list->tail;
        huff_Node* prevNode = NULL;
        while(1){
            /*If current node is greater than node
            Insert at the back of current node*/
            if(compareNodes(currNode, node) >= 0){
                /*If not 1st item*/
                if(currNode->prev != NULL){
                    /*link previous node to inserted*/
                    prevNode = (huff_Node*)currNode->prev;
                    prevNode->next = node;
                    node->prev = prevNode;
                }
                else{
                    /*inserted must be new tail*/
                    list->tail = node;
                }
                /*link inserted behind current node*/
                currNode->prev = node;
                node->next = currNode;

                /*If end of list*/
                list->items = list->items + 1;
                break;
            }
            else if(compareNodes(currNode, node) < 0){
                /*If node is greater than current, move to next*/
                if(currNode->next == NULL){
                    currNode->next = node;
                    node->prev = currNode;
                    list->head = node;
                    list->items = list->items + 1;
                    break;
                }
                currNode = (huff_Node*)currNode->next;
            }
            else{
                /*if equal*/
                /*link inserted behind current node*/


                break;
            }
        }
    }
}

huff_Node* popTail(Node_List* list){
    /*If list is not empty */
    if(list->items > 0){
        /*Set up new tail and temp pointer to return val*/
        huff_Node* oldTail = list->tail;
        list->tail = (huff_Node*)list->tail->next;
        list->tail->prev = NULL;

        /*Decrement item count */
        list->items--;

        /*Return old tail*/
        return oldTail;
    }
    else{
        return NULL;
    }
}

/*------------------------- Huffman Functions---------------------------*/
/*Counts Frequency of characters from 0 to 255 and outputs an ordered list*/
int* countFreq(char *filename){
    FILE *file = NULL;
    /* Open File */
    if(!(file = fopen(filename,"r"))){
        perror(filename);
        exit(EXIT_FAILURE);
    }
    /* Initialize char array */
    int* freqs = (int*)malloc(256*sizeof(int));
    int i;
    for(i = 0; i<256;i++){
        freqs[i] = 0;
    }

    /* Iterate until EOF, incrmenting chararters as found*/
    int c = 0;
    while((c = getc(file)) != EOF){
        freqs[c] = freqs[c] + 1;
    }
    fclose(file);
    return freqs;
}

Node_List* createNodeList(int* freqs){
    /*Allocate Struct Array and List*/
    Node_List* list = init_Node_List();

    /*Copy over node information*/
    int i = 0;
    for(;i<256;i++){
        /*Insert non zero characters*/
        if(freqs[i] != 0){
            huff_Node* newNode = init_Huff_Node(freqs[i], i);
            insertNode(list, newNode);
        }
    }
    /* Return List */
    return list;
}

/*Creates a huffman tree frm list of freqs*/
huff_Node* createHTree(int* freqs){
    /*Initialize node list(add non-zero characters)*/
    Node_List* list = createNodeList(freqs);
    huff_Node* node1 = NULL;
    huff_Node* node2 = NULL;
    huff_Node* sumNode = NULL;
    int sum = 0;

    /*Until 1 node remains,*/
    while(list->head != list->tail){
        /*Pop 2 nodes*/
        node1 = popMin(list);
        node2 = popMin(list);

        /*Attach both to sum node */
        sum = (node1->freq)+(node2->freq);
        sumNode = init_Huff_Node(sum, 0);
        sumNode->left = node1;
        sumNode->right = node2;

        /*Reinsert node into list*/
        insertNode(list, sumNode);
    }
    /*Return Remaining Node*/
    huff_Node* tree = popMin(list);
    freeList(list);
    return tree;
}

void genCodesRecur(huff_Node* tree, codeIndex* index, int code, int len){
    /*If leaf*/
    if((tree->left == NULL)&&(tree->left == NULL)){
        index->codes[tree->c] = code;
        index->lens[tree->c] = len;
    }
    else{
        /*Traverse Right*/
        genCodesRecur(tree->right, index, (code*2)+1, len+1);
        /*Traverse Left*/
        genCodesRecur(tree->left, index, code*2, len+1);
    }
}

codeIndex* init_CodeIndex(){
    int* codes = (int*)malloc(256*sizeof(int));
    int* lens = (int*)malloc(256*sizeof(int));
    int i;
    for(i = 0; i<256;i++){
        codes[i] = 0;
        lens[i] = 0;
    }
    codeIndex* index = (codeIndex*)malloc(sizeof(codeIndex));
    index->codes = codes;
    index->lens = lens;
    return index;
}

void freeIndex(codeIndex* index){
    free(index->codes);
    free(index->lens);
    free(index);
}

/*Creates huffman codes from tree*/
codeIndex* genCodes(huff_Node* tree){
    /*Initialize code  and bit count*/
    codeIndex* index = init_CodeIndex();
    if(tree != NULL){
        /*Recursively Transverse Tree*/
        genCodesRecur(tree, index, 0, 0);
    }
    return index;
}

/*Pops the minimum value from the stack as node*/
huff_Node* popMin(Node_List* list){
    if(list->tail != NULL){
        huff_Node* minim = list->tail;
        if(minim->next != NULL){
            minim->next->prev = NULL;
            list->tail = minim->next;
        }
        else{
            list->head = NULL;
            list->tail = NULL;
        }
        list->items = list->items -1;
        return minim;
    }
    else{
        return NULL;
    }
}

void freeTree(huff_Node* tree){
    if(tree != NULL){
        if((tree->right != NULL) && (tree->left !=NULL)){
            huff_Node* right = tree->right;
            huff_Node* left = tree->left;
            freeTree(left);
            freeTree(right);
        }
        free(tree);
    }
}

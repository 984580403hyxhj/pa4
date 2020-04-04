#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hbt.h"
#include "lib.h"

void preorder(Tnode *head, FILE *fp)
{
    if(head == NULL) return;

    int pat = 0;
    fwrite(&(head->key),sizeof(int),1,fp);
    
    if(head->left) pat += 2;
    if(head->right) pat++;
    
    fwrite(&pat,sizeof(char),1,fp);
    //printf("%d,%d\n", head->key, pat);
    preorder(head->left,fp);
    preorder(head->right,fp);
}

void freetree(Tnode *head)
{
    if(head == NULL) return;

    

    freetree(head->left);
    freetree(head->right);

    free(head);
}

int main(int argc, char** argv)
{
    
    //print2D(treehead);
    if(!strcmp(argv[1],"-b")){
        Tnode *treehead = buildtree(argv[2]);
        FILE *output = fopen(argv[3],"wb+");
        if(output == NULL) return EXIT_FAILURE;

        preorder(treehead,output);
        fclose(output);
        freetree(treehead);
    }
    if(!strcmp(argv[1],"-e")){
        int first = 1;
        int second = 1;
        int third = 1;
        FILE *eval = fopen(argv[2],"rb");
        if(eval == NULL)
        {
            first = -1;
            printf("%d,%d,%d\n",first,second,third);
            return EXIT_FAILURE;
        }
        Tnode *head = preorder_tr(eval,&first,&second,&third);
        printf("%d,%d,%d\n",first,second,third);
        freetree(head);
        fclose(eval);
        if(first != 1) return EXIT_FAILURE;
    }




    return EXIT_SUCCESS;
}














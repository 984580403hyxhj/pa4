#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lib.h"
#include "hbt.h"
#define COUNT 10 

void print2DUtil(Tnode *root, int space) 
{ 
    // Base case 
    if (root == NULL) 
        return; 
  
    // Increase distance between levels 
    space += COUNT; 
  
    // Process right child first 
    print2DUtil(root->right, space); 
  
    // Print current node after space 
    // count 
    printf("\n"); 
    for (int i = COUNT; i < space; i++) 
        printf(" "); 
    printf("%d, %d\n", root->key, root->balance); 
  
    // Process left child 
    print2DUtil(root->left, space); 
} 
  
// Wrapper over print2DUtil() 
void print2D(Tnode *root) 
{ 
   // Pass initial space count as 0 
   print2DUtil(root, 0); 
} 


Tnode *clock_rotate(Tnode *head)
{
	Tnode* temp = head->left;
	head->left = head->left->right;
	temp->right = head;
	//head->balance = 0;
	//temp->balance = 0;
	return temp;
}

Tnode *c_clock_rotate(Tnode *head)
{
	Tnode *temp = head->right;
	head->right = head->right->left;
	temp->left = head;
	//head->balance = 0;
	//temp->balance = 0;
	return temp;
}

Tnode *rotate(Tnode *head)
{
	if(head->balance > 1)///l rotate
	{
		if(head->left->balance > 0)//ll
		{
			Tnode *temp = clock_rotate(head);
			temp->balance = 0;
			temp->right->balance = 0;
			return temp;
		}
		if(head->left->balance < 0)//lr
		{
			if(head->left->right->balance < 0){
				head->left = c_clock_rotate(head->left);
				Tnode *temp = clock_rotate(head);
				temp->balance = 0;
				temp->left->balance = 1;
				temp->right->balance = 0;
				return temp;
			}else if(head->left->right->balance > 0){
				head->left = c_clock_rotate(head->left);
				Tnode *temp = clock_rotate(head);
				temp->balance = 0;
				temp->left->balance = 0;
				temp->right->balance = -1;
				return temp;
			}else{
				head->left = c_clock_rotate(head->left);
				Tnode *temp = clock_rotate(head);
				temp->balance = 0;
				temp->left->balance = 0;
				temp->right->balance = 0;
				return temp;
			}
			
		}

	}
	if(head->balance < -1)//r rotate
	{
		if(head->right->balance < 0)//rr
		{
			Tnode *temp = c_clock_rotate(head);
			temp->balance = 0;
			temp->left->balance = 0;
			return temp;
		}
		if(head->right->balance > 0)//rl
		{
			/*head->right = clock_rotate(head->right);
			Tnode* temp = c_clock_rotate(head);

			return c_clock_rotate(head);*/
			if(head->right->left->balance > 0){
				head->right = clock_rotate(head->right);
				Tnode *temp = c_clock_rotate(head);
				temp->balance = 0;
				temp->left->balance = 0;
				temp->right->balance = -1;
				return temp;
			}else if(head->right->left->balance < 0){
				head->right = clock_rotate(head->right);
				Tnode *temp = c_clock_rotate(head);
				temp->balance = 0;
				temp->left->balance = 1;
				temp->right->balance = 0;
				return temp;
			}else{
				head->right = clock_rotate(head->right);
				Tnode *temp = c_clock_rotate(head);
				temp->balance = 0;
				temp->left->balance = 0;
				temp->right->balance = 0;
				return temp;
			}
		}
	}

	return NULL;
}

int height(Tnode *head)
{
	if(head == NULL) return 0;


	int left = height(head->left);
	int right = height(head->right);
	int max = right>left?right:left;
	int num = 1+max;

	return num;
}

Tnode *createNode(int key)
{
	Tnode *p = malloc(sizeof(Tnode));
	p->key = key;
	p->left = NULL;
	p->right = NULL;

	return p;
}

Tnode *insertnode(Tnode *head, int key, int *ifchange)
{
	if(head == NULL)
	{
		head = createNode(key);
		head->balance = 0;
		*ifchange = 1;
		return head;
	}

	if(key > head->key)
	{
		head->right = insertnode(head->right,key, ifchange);
	}else{
		head->left = insertnode(head->left,key, ifchange);
	}

	if(*ifchange == 1)
	{
		if(head->balance != 0)
		{
			*ifchange = 0;
		}
		if(key > head->key)
		{
			head->balance--;
		}else{
			head->balance++;
		}
		/*int temp =  height(head->left) - height(head->right);
		if(head->balance != temp){
			head->balance = temp;
		}else{
			*ifchange = 0;
		}*/
	}
	
	if(head->balance < -1 || head->balance > 1)
	{
		head = rotate(head);
		*ifchange = 0;
	}
	/*print2D(head);
	printf("////////////////////////////////////\n");*/
	return head;
}
////////////above all work//////////
///////////below all work//////

bool isleaf(Tnode *head)
{
	if(head->left || head->right)
	{
		return false;
	}else{
		return true;
	}
}

Tnode *deletetoright(Tnode *head, int *ifchange)
{
	if(head->right == NULL)
	{
		if(head->left)
		{
			*ifchange = 1;
			Tnode *temp = head->left;
			free(head);
			return temp;
		}else{
			*ifchange = 1;
			free(head);
			return NULL;
		}
	}
	head->right = deletetoright(head->right, ifchange);
	if(*ifchange == 1){
		if(head->balance == 0)
		{
			*ifchange = 0;
		}
		head->balance++;
	}
	if(head->balance < -1 || head->balance > 1)
	{
		head = rotate(head);
		*ifchange = 1;
	}

	return head;
}

Tnode *delete(Tnode *head, int key, int *ifchange)
{
	if(key == head->key)
	{
		if(isleaf(head))
		{
			free(head);
			*ifchange = 1;
			return NULL;
		}else
		{
			if(head->left&&head->right)
			{
				Tnode *temp = head->left;
				while(temp->right != NULL)
				{
					temp = temp->right;
				}
				head->key = temp->key; //swap value
				temp->key = key;
				head->left = deletetoright(head->left, ifchange);

				if(*ifchange == 1)
				{
					if(head->balance == 0){
						*ifchange = 0;
					}
					head->balance--;
					/*int tempbal = height(head->left) - height(head->right);
					if(tempbal != head->balance)
					{
						head->balance = tempbal;
					}else{
						*ifchange = 0;
					}*/
				}

				
				if(head->balance < -1 || head->balance > 1)
				{
					head = rotate(head);
				}

				return head;

			}else if(head->left)
			{
				Tnode* temp = head->left;
				*ifchange = 1;
				free(head);
				return temp;
			}else{
				Tnode* temp = head->right;
				*ifchange = 1;
				free(head);
				return temp;
			}
		}
	}

	if(key > head->key)
	{
		if(head->right == NULL) return head;
		head->right = delete(head->right,key, ifchange);
		if(*ifchange == 1){
			if(head->balance == 0){
				*ifchange = 0;
			}
			head->balance++;
		}
		//head->balance++;
	}else{
		if(head->left == NULL) return head;
		head->left = delete(head->left,key, ifchange);
		if(*ifchange == 1){
			if(head->balance == 0){
				*ifchange = 0;
			}
			head->balance--;
		}
		//head->balance--;
	}

	/*if(*ifchange == 1)
	{
		int tempbal = height(head->left) - height(head->right);
		if(tempbal != head->balance)
		{
			head->balance = tempbal;
		}else{
			*ifchange = 0;
		}
	}*/

	if(head->balance < -1 || head->balance > 1)
	{
		head = rotate(head);
		*ifchange = 1;
	}

	return head;

}
/////////above all work
/////////below all work

Tnode *buildtree(char *filename)
{
	FILE *fp = fopen(filename,"rb");

	if(fp == NULL) return NULL;

	int key = 0;
	char ascii = 0;
	Tnode *head = NULL;
	int *ifchange = malloc(sizeof(int));
	*ifchange = 0;
	while(1)
	{
		fread(&key,sizeof(int),1,fp);
		fread(&ascii,sizeof(char),1,fp);
		//printf("%d,%c\n",key, ascii);
		if(feof(fp)) break;
		if(ascii == 'i')
		{
			head = insertnode(head,key, ifchange);
			*ifchange = 0;
		}else{
			head = delete(head,key, ifchange);
			*ifchange = 0;
		}
		/*printf("%d, %c\n",key,ascii);
		print2D(head);
		//printf("////////////////////////////////////\n");
		printf("////////////////////////////////////////");*/
	}

	fclose(fp);
	return head;


}
/////////////////above all work


/////below is -e section

Tnode *preorder_tr(FILE *fp, int *first, int *second, int *third)
{
	if(feof(fp)) return NULL;
	int key = 0;
	int pat = 0;
	fread(&key,sizeof(int),1,fp);
	fread(&pat,sizeof(char),1,fp);

	Tnode *p = createNode(key);
	if(pat == 0) return p;
	if(pat == 1)
	{
		p->right = preorder_tr(fp, first, second, third);
		if(p->right->key < p->key)
		{
			*second = 0;
		}
	}
	else if(pat == 2)
	{
		p->left = preorder_tr(fp, first, second, third);
		if(p->left->key > p->key)
		{
			*second = 0;
		}
	}
	else if(pat == 3)
	{
		p->left = preorder_tr(fp, first, second, third);
		p->right = preorder_tr(fp, first, second, third);
		if(p->right->key < p->key)
		{
			*second = 0;
		}
		if(p->left->key > p->key)
		{
			*second = 0;
		}
	}
	else{
		*first = 0;
		return NULL;
	}

	p->balance = height(p->left) - height(p->right);
	if(p->balance < -1 || p->balance > 1)
	{
		*third = 0;
	}

	return p;

}








































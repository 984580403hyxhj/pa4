#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lib.h"
#include "hbt.h"

Tnode *clock_rotate(Tnode *head)
{
	Tnode* temp = head->left;
	head->left = head->left->right;
	temp->right = head;
	head->balance = 0;
	temp->balance = 0;
	return temp;
}

Tnode *c_clock_rotate(Tnode *head)
{
	Tnode *temp = head->right;
	head->right = head->right->left;
	temp->left = head;
	head->balance = 0;
	temp->balance = 0;
	return temp;
}

Tnode *rotate(Tnode *head)
{
	if(head->balance > 1)///l rotate
	{
		if(head->left->balance > 0)//ll
		{
			return clock_rotate(head);
		}
		if(head->left->balance < 0)//lr
		{
			head->left = c_clock_rotate(head->left);
			return clock_rotate(head);
		}

	}
	if(head->balance < -1)//r rotate
	{
		if(head->right->balance < 0)//rr
		{
			return c_clock_rotate(head);
		}
		if(head->right->balance > 0)//rl
		{
			head->right = clock_rotate(head->right);
			return c_clock_rotate(head);
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

Tnode *insertnode(Tnode *head, int key)
{
	if(head == NULL)
	{
		head = createNode(key);
		head->balance = 0;
		return head;
	}

	if(key > head->key)
	{
		head->right = insertnode(head->right,key);
	}else{
		head->left = insertnode(head->left,key);
	}

	head->balance = height(head->left) - height(head->right);
	if(head->balance < -1 || head->balance > 1)
	{
		head = rotate(head);
	}

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

Tnode *delete(Tnode *head, int key)
{
	if(key == head->key)
	{
		if(isleaf(head))
		{
			free(head);
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
				head->left = delete(head->left,key);

				head->balance = height(head->left) - height(head->right);
				if(head->balance < -1 || head->balance > 1)
				{
					head = rotate(head);
				}

				return head;
			}else if(head->left)
			{
				Tnode* temp = head->left;
				free(head);
				return temp;
			}else{
				Tnode* temp = head->right;
				free(head);
				return temp;
			}
		}
	}

	if(key > head->key)
	{
		if(head->right == NULL) return head;
		head->right = delete(head->right,key);
		//head->balance++;
	}else{
		if(head->left == NULL) return head;
		head->left = delete(head->left,key);
		//head->balance--;
	}

	head->balance = height(head->left) - height(head->right);
	if(head->balance < -1 || head->balance > 1)
	{
		head = rotate(head);
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
	while(1)
	{
		fread(&key,sizeof(int),1,fp);
		fread(&ascii,sizeof(char),1,fp);
		//printf("%d,%c\n",key, ascii);
		if(feof(fp)) break;
		if(ascii == 'i')
		{
			head = insertnode(head,key);
		}else{
			head = delete(head,key);
		}
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








































// binSearchTree.c
//

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node {
	unsigned long long num;
	void * pinfo;
	struct node * left;
	struct node * right;
	struct node * root;
} Tree;

Tree * addIt(Tree* nod, unsigned long long number);
Tree * findIt(Tree * nod, unsigned long long val);


// Autobalance TREE
Tree *addIt(Tree *nod, unsigned long long number)
{
	int left_bal = 0;
	int right_bal = 0;
	Tree* middle = nod;

	if (nod == NULL) {
		nod = (Tree*)malloc(sizeof(Tree));
		if (nod == NULL) {
			return NULL;
		}
		nod->num = number;
		nod->left = NULL;
		nod->root = NULL;
		nod->right = NULL;
		nod->pinfo = NULL;  // this pointer will be set externaly
		return nod;
	}

	while (middle != NULL)
	{
		if (number > middle->num)
		{
			left_bal = 0;
			right_bal = (middle->left == NULL) ? (right_bal + 1) : 0;
			if (middle->right == NULL)
			{
				// Hold your nodes level one
				Tree *right = addIt(NULL, number); // Add more one on right
				Tree *avo, *pai = middle->root;
				middle->right = right;
				right->root = middle;

				if ((right_bal == 2) && ((avo = middle->root->root) != NULL)) {
					// Hold your nodes level two
					// We have avo, pai, middle and right this is the moment for the balance
					// We need to make 5 changes where One is conditional
					if (avo->left == pai) avo->left = middle;		// Conditional
					if (avo->right == pai) avo->right = middle;		// Conditional
					middle->root = avo;
					middle->left = pai;
					pai->root = middle;
					pai->right = NULL;
				}

				return right;
			}

			// Goes to the Right
			middle = middle->right;
		}
		else if (number < middle->num)
		{
			left_bal = (middle->right == NULL) ? (left_bal + 1) : 0;
			right_bal = 0;
			if (middle->left == NULL)
			{
				// Hold your nodes level one
				Tree *left = addIt(NULL, number);
				Tree *avo, *pai = middle->root;
				middle->left = left;
				left->root = middle;

				if ((left_bal == 2) && ((avo = middle->root->root) != NULL)) {
					// Hold your nodes level two
					// We have avo, pai, middle and right, this is the moment for the balance
					// We need to make 5 changes where One is conditional
					if (avo->left == pai) avo->left = middle;		// Conditional
					if (avo->right == pai) avo->right = middle;		// Conditional
					middle->root = avo;
					middle->right = pai;
					pai->root = middle;
					pai->left = NULL;
				}

				return left;
			}

			// Goes to the Left
			middle = middle->left;
		}
		else
		{
			return middle;
		}
	}

	// Never reaches here
	assert(1 == 0);

	return NULL;
}

Tree *findIt(Tree *where, unsigned long long val)
{
	if (where == NULL)
	{
		return NULL;
	}

	if (where->num == val)
	{
		return where;
	}

	while (where != NULL)
	{
		if (val > where->num)
		{
			where = where->right;
		}
		else if (val < where->num)
		{
			where = where->left;
		}
		else
		{
			return where;
		}
	}

	return NULL;
}

void actInOrder(Tree *nod, void action(Tree *))
{
	Tree *aux = nod;

	if (nod == NULL)
	{
		return;
	}

	maxOnLeft:
	{
		while (nod->left != NULL)	// Runs max on left;
		{
			nod = nod->left;
		}
		
		action(nod);				// Acts;

		if (nod->right != NULL)
		{
			nod = nod->right;
			goto maxOnLeft;
		}

		while (nod->root != NULL)
		{
			aux = nod->root;

			if (aux->left == nod)      // Returns to root from left
			{
				action(aux);
				if (aux->right != NULL) // This is the middle of the tree?
				{
					nod = aux->right;   // Goes to the other side
					goto maxOnLeft;
				}

				if (aux->root == NULL)
				{
					return;        // Only ROOT has no root
				}
			}
			nod = aux;
		}
		return;                    // Only ROOT has no root

	}	// maxOnLeft:
}


void freeAllTree(Tree *root, void freeMe(Tree *))
{
	Tree *aux;
	Tree *nod = root;

	if (nod == NULL)
	{
		return;
	}


	run_max_on_left:
	{
		while (nod->left != NULL)  // Runs max on left;
		{
			nod = nod->left;
		}

		if (nod->right != NULL)
		{
			nod = nod->right;
			goto run_max_on_left;
		}

		while (nod->root != NULL)
		{
			aux = nod->root;
			if (aux->left == nod)
			{
				freeMe(aux->left);
				aux->left = NULL;
				if (aux->right != NULL)
				{
					nod = aux->right;
					goto run_max_on_left;
				}
			}
			else if (aux->right == nod)
			{
				assert(aux->left == NULL);
				freeMe(aux->right);
				aux->right = NULL;
			}
			else
			{
				assert(0);
			}
			nod = aux;
		}
		if (nod == root)
		{
			freeMe(root);
			return;
		}

	}	// run_max_on_left:
}

////////////////////////////////////////// Implementations  /////////////////////////////////////////////////////
int counter = 0;


void freeMeImplementation(Tree *t)
{
	if (t != NULL)
	{
		if (t->pinfo != NULL)
		{
			free(t->pinfo);
		}
		free(t);
	}
	counter--;
}

void printImplementation(Tree *t)
{
	printf(" %.2lld", t->num);
}

void counterImplementation(Tree *t)
{
	counter++;
}

//////////////////////////////////////////////////  Tests  ////////////////////////////////////////////////////////////////
#define MINIMUM_LEN 10;

int main(int argc, char ** argv)
{
	for (int i = 0; i < 10; i++)
	{
		Tree* root = NULL;
		int length = (rand() % 50);
		int val = 45;
		length += MINIMUM_LEN;

		root = addIt(NULL, val);
		printf("\nValues:  %.2d", val);

		for (int i = 0; i < length; i++)
		{
			val = rand() % 100;
			addIt(root, val);
			printf(" %.2d", val);
		}
		val = 63;
		addIt(root, val);
		printf(" %.2d", val);

		printf("\nInOrder:");
		actInOrder(root, printImplementation);

		actInOrder(root, counterImplementation);

		printf("\nTotal of nodes is : %d ", counter);

		if (findIt(root, 54) == NULL)
		{
			printf("\nThere is no 54 here.");
		}
		else
		{
			printf("\n54 is here.");
		}

		if (findIt(root, 63) == NULL)
		{
			printf("\nThere is no 63 here.");
		}
		else
		{
			printf("\n63 is here.");
		}


		freeAllTree(root, freeMeImplementation);

		printf("\nNumber of nodes to free : %d ", counter);

		assert(counter == 0);
	}
	return (EXIT_SUCCESS);
}


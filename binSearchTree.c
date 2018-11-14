// binSearchTree.c
//

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

typedef struct node {
	int deepth;
	unsigned long long num;
	void * pinfo;
	struct node * left;
	struct node * right;
	struct node * root;
} Tree;

Tree * addIt(Tree* nod, unsigned long long number);
Tree * findIt(Tree * nod, unsigned long long val);

#define MAX_RECURSIVE	25

// Rebalance TREE
// R E		<== Not 
// R I E	<== Not
// R I . E	<== Yes
void rebalance_right(Tree *rot_nod, Tree *end_nod)
{
	Tree *ini1_nod, *end1_nod;
	Tree *ini2_nod, *end2_nod;
	Tree *mid_nod;
	int cmp, endRight;

	if (end_nod->deepth - rot_nod->deepth == 0)
	{
		assert(rot_nod == end_nod);
		return;
	}

	if (end_nod->deepth - rot_nod->deepth == 1)
	{
		assert(rot_nod->right == end_nod);
		return;
	}

	if (end_nod->deepth - rot_nod->deepth == 2)
	{
		assert(rot_nod->right == end_nod->root);
		rot_nod->right->deepth = rot_nod->deepth + 1;
		return;
	}

	endRight = end_nod->deepth + 1;
	ini1_nod = rot_nod->right;
	end2_nod = end_nod;

	// R I . E
	// 1 2 3 4 5 6 7 8 9
	int aux = (rot_nod->deepth + end_nod->deepth);

	if (aux % 2 != 0)
	{
		aux++;
		endRight++;
	}
	aux /= 2;
	
	mid_nod = rot_nod;
	cmp = rot_nod->deepth;

	while (cmp != aux)
	{
		mid_nod = mid_nod->right;
		cmp++;
		endRight--;
	}

	// Breaks  rot->ini1 - > end1->mid->ini2 - > end2 
	end1_nod = mid_nod->root;
	ini2_nod = mid_nod->right;
	end1_nod->right = NULL;

	// mid will be the central node
	//             ini1--->end1
	//            /
	// rot -r-> mid
	//            \
	//             ini2-->end2

	// Six translations
	rot_nod->right = mid_nod;
	mid_nod->root = rot_nod;
	mid_nod->left = ini1_nod;
	mid_nod->right = ini2_nod;
	ini1_nod->root = mid_nod;
	ini2_nod->root = mid_nod;


	// Fix the deepth on the tips
	mid_nod->deepth = rot_nod->deepth + 1;
	ini1_nod->deepth = mid_nod->deepth + 1;
	ini2_nod->deepth = mid_nod->deepth + 1;

	aux = cmp - rot_nod->deepth;

	end1_nod->deepth = rot_nod->deepth + aux;
	rebalance_right(ini1_nod, end1_nod);

	end2_nod->deepth = endRight;
	rebalance_right(ini2_nod, end2_nod);

}


Tree *addIt(Tree *nod, unsigned long long number)
{
	int deep_level = 0;
	int left_bal = 0;
	int right_bal = 0;
	Tree* middle = nod;
	Tree* nod_bal_right = NULL;
	Tree* nod_bal_left = NULL;


	if (nod == NULL) {
		nod = (Tree*)malloc(sizeof(Tree));
		if (nod == NULL) {
			return NULL;
		}
		nod->deepth = 0;
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
			//right_bal = (middle->left == NULL) ? (right_bal + 1) : 0;
			if (middle->left == NULL)
			{
				if ((right_bal == 0) && (middle != nod))
				{
					nod_bal_right = middle;
					right_bal = 1;
				}
				else
				{
					right_bal++;
				}
			}
			else
			{
				right_bal = 0;
			}


			if (middle->right == NULL)
			{
				// Hold your nodes level one
				Tree *right = addIt(NULL, number); // Add more one on right
				right->root = middle;
				middle->right = right;
				right->deepth = deep_level + 1;

				if (right_bal >= MAX_RECURSIVE) 
				{
					rebalance_right(nod_bal_right, right);
				}

				//Tree *avo, *pai = middle->root;

				//if ((right_bal == 2) && ((avo = middle->root->root) != NULL)) {
				//	// Hold your nodes level two
				//	// We have avo, pai, middle and right this is the moment for the balance
				//	// We need to make 5 changes where One is conditional
				//	if (avo->left == pai) avo->left = middle;		// Conditional
				//	if (avo->right == pai) avo->right = middle;		// Conditional
				//	middle->root = avo;
				//	middle->left = pai;
				//	pai->root = middle;
				//	pai->right = NULL;
				//}

				return right;
			}

			// Goes to the Right
			middle = middle->right;
			deep_level++;
		}
		else if (number < middle->num)
		{
			//left_bal = (middle->right == NULL) ? (left_bal + 1) : 0;
			left_bal = 0;
			right_bal = 0;
			if (middle->left == NULL)
			{
				// Hold your nodes level one
				Tree *left = addIt(NULL, number);
				//Tree *avo, *pai = middle->root;
				middle->left = left;
				left->root = middle;
				left->deepth = deep_level + 1;

				//if ((left_bal == 2) && ((avo = middle->root->root) != NULL)) {
				//	// Hold your nodes level two
				//	// We have avo, pai, middle and right, this is the moment for the balance
				//	// We need to make 5 changes where One is conditional
				//	if (avo->left == pai) avo->left = middle;		// Conditional
				//	if (avo->right == pai) avo->right = middle;		// Conditional
				//	middle->root = avo;
				//	middle->right = pai;
				//	pai->root = middle;
				//	pai->left = NULL;
				//}

				return left;
			}

			// Goes to the Left
			middle = middle->left;
			deep_level++;
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

			if (aux->root == NULL)
			{
				return;        // Only ROOT has no root
			}

			if (aux->left == nod)      // Returns to root from left
			{
				action(aux);
				if (aux->right != NULL) // This is the middle of the tree?
				{
					nod = aux->right;   // Goes to the other side
					goto maxOnLeft;
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

void printInDeepthImplementation(Tree *t)
{
	for (int i = 0; i < t->deepth; i++)
	{
		printf("    .");
	}
	printf(" %.2lld\n", t->num);
}



void counterImplementation(Tree *t)
{
	counter++;
}


//////////////////////////////////////////////////  Tests  ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////  Tests  ////////////////////////////////////////////////////////////////

void printRandomic(void)
{
	for (int i = 0; i < 1; i++)
	{
		Tree* root = NULL;
		int length = (MAX_RECURSIVE + 1) * 2;
		int val = length;

		root = addIt(NULL, val);
		printf("\nValues:  %.2d", val);

		for (int i = 0; i < length; i++)
		{
			//val = rand() % MAX_RECURSIVE;
			val = 2 * i + 1;
			addIt(root, val);
			printf(" %.2d", val);
		}

		printf("\nInOrder:");
		actInOrder(root->left, printImplementation);
		printImplementation(root);
		actInOrder(root->right, printImplementation);

		counter = 1;
		actInOrder(root->left, counterImplementation);
		actInOrder(root->right, counterImplementation);
		printf("\nTotal of nodes is : %d ", counter);

		printf("\n================================== Print In Deepth =======================================");
		printf("\n012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n");
		actInOrder(root->left, printInDeepthImplementation);
		printInDeepthImplementation(root);
		actInOrder(root->right, printInDeepthImplementation);


		freeAllTree(root, freeMeImplementation);
		printf("\nNumber of nodes after free : %d ", counter);

		assert(counter == 0);
	}
}

// .
// 01  03  05  07  09  11  13  15  17  19  21  23  25  27  29  31  <== Odd
//  \__/    \__/    \__/    \__/    \__/    \__/    \__/    \__/   
//   02      06      10      14      18      22      26      30    <== Odd * 2
//    \______/        \______/        \______/        \______/     
//       04              12              20              28        <== Odd * 4
//        \______________/                \______________/         
//               08                              24                <== Odd * 8
//                \______________________________/                 
//                               16                                
//                               ||                                

// 1 - 04 - 1*4
// 2 - 08 - 2*4
// 4 - 16 - 4*4
// 8 - 32 - 8*4

#define MAX_SIZE_TREE 64
#define MAX_TREE_WIDTH	(MAX_SIZE_TREE/2-1)
int oddNumbers[MAX_SIZE_TREE / 2];	// Every odd number stay here
int odd_len = MAX_SIZE_TREE / 2;

//void print_lins(int factor, char *oddStr, char *evenStr)
void print_lins(int factor)
{
	char oddLeaf[MAX_SIZE_TREE+1];
	char evenLeaf[MAX_SIZE_TREE + 1];
	//////////////0         1         2         3         4         5         6         7         8         9         10        11        12        13
	//////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
	char fmt[] = "                                                                                                                                            ";

	int x = factor*4;
	assert((x+4) < sizeof(fmt));

	fmt[x+2] = 0;
	x = (x / 2) - 1;
	fmt[x++] = '%';
	fmt[x++] = '.';
	fmt[x++] = '2';
	fmt[x++] = 'd';

	for (int i = 0; i < odd_len/factor; i++)
	{
		printf(fmt, factor * oddNumbers[i]);
	}
	printf("\n");

	if (odd_len == factor)
	{
		return;
	}


	//print_lins(1, "_/  ", "  \\_");
	//print_lins(2, "___/    ", "    \\___");
	//print_lins(4, "_______/        ", "        \\_______");
	//print_lins(8, "_______________/                ", "                \\_______________");

	for (int i = 0; i < (4*factor); i++) 
	{
		if (i < (2 * factor)) {
			if (i == (2 * factor - 1)) {
				oddLeaf[i] = '/';
			} else {
				oddLeaf[i] = '_';
			}
			evenLeaf[i] = ' ';
		}
		else 
		{
			oddLeaf[i] = ' ';
			if (i == (2 * factor)) {
				evenLeaf[i] = '\\';
			} else {
				evenLeaf[i] = '_';
			}
		}
	}
	evenLeaf[(4 * factor)] = 0;
	oddLeaf[(4 * factor)] = 0;

	//assert(strcmp(evenLeaf, evenStr) == 0);
	//assert(strcmp(oddLeaf, oddStr) == 0);


	for (int i = 0; i < odd_len / factor; i++)
	{
		if (i % 2)
		{
			printf("%s", oddLeaf);
		}
		else
		{
			printf("%s", evenLeaf);
		}
	}
	printf("\n");

}

void print_staticaly()
{
	system("cls");

	// Line 1 creation
	for (int i = 0; i < odd_len; i++)
	{
		oddNumbers[i] = 2 * i + 1;
	}

	int bit = 1;

	while (bit < MAX_SIZE_TREE)
	{
		print_lins(bit);
		bit = bit << 1;
	}


	printf("\n");

}

//int testTree(int argc, char ** argv)
int main(int argc, char ** argv)
{
	print_staticaly();
	printRandomic();

	return (EXIT_SUCCESS);
}


/* OUTPUT */
/*

Values:  52 01 03 05 07 09 11 13 15 17 19 21 23 25 27 29 31 33 35 37 39 41 43 45 47 49 51 53 55 57 59 61 63 65 67 69 71 73 75 77 79 81 83 85 87 89 91 93 95 97 99 101 103
	InOrder: 01 03 05 07 09 11 13 15 17 19 21 23 25 27 29 31 33 35 37 39 41 43 45 47 49 51 52 53 55 57 59 61 63 65 67 69 71 73 75 77 79 81 83 85 87 89 91 93 95 97 99 101 103
	Total of nodes is : 53
	================================== Print In Deepth ====================================== =
	012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
	. 01
	.    .    . 03
	.    .    .    .    . 05
	.    .    .    .    .    .    . 07
	.    .    .    .    .    . 09
	.    .    .    .    .    .    . 11
	.    .    .    .    .    .    .    . 13
	.    .    .    . 15
	.    .    .    .    . 17
	.    .    .    .    .    .    . 19
	.    .    .    .    .    .    .    . 21
	.    .    .    .    .    . 23
	.    .    .    .    .    .    .    .    . 25
	.    . 27
	.    .    . 29
	.    .    .    .    . 31
	.    .    .    .    .    .    . 33
	.    .    .    .    .    . 35
	.    .    .    .    .    .    . 37
	.    .    .    .    .    .    .    . 39
	.    .    .    . 41
	.    .    .    .    . 43
	.    .    .    .    .    .    . 45
	.    .    .    .    .    .    .    . 47
	.    .    .    .    .    . 49
	.    .    .    .    .    .    .    .    . 51
	52
	. 53
	.    .    . 55
	.    .    .    .    . 57
	.    .    .    .    .    .    . 59
	.    .    .    .    .    . 61
	.    .    .    .    .    .    . 63
	.    .    .    .    .    .    .    . 65
	.    .    .    . 67
	.    .    .    .    . 69
	.    .    .    .    .    .    . 71
	.    .    .    .    .    .    .    . 73
	.    .    .    .    .    . 75
	.    .    .    .    .    .    .    .    . 77
	.    . 79
	.    .    . 81
	.    .    .    .    . 83
	.    .    .    .    .    .    . 85
	.    .    .    .    .    . 87
	.    .    .    .    .    .    . 89
	.    .    .    .    .    .    .    . 91
	.    .    .    . 93
	.    .    .    .    . 95
	.    .    .    .    .    .    . 97
	.    .    .    .    .    .    .    . 99
	.    .    .    .    .    . 101
	.    .    .    .    .    .    .    .    . 103

	Number of nodes after free : 0
*/



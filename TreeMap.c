#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TreeMap.h"

#define MAX(a, b) (((a) >= (b))?(a):(b))


/* Creeaza un arbore cu o serie de metode asociate
 *
 * return: arborele creat
 */
TTree* createTree(void* (*createElement)(void*),
				  void (*destroyElement)(void*),
				  void* (*createInfo)(void*),
				  void (*destroyInfo)(void*),
				  int compare(void*, void*)) {
	TTree* tree = (TTree*) malloc(sizeof(TTree));
	tree->createElement = *createElement;
	tree->destroyElement = *destroyElement;
	tree->createInfo = *createInfo;
	tree->destroyInfo = *destroyInfo;
	tree->compare = compare;
	tree->size = 0;
	tree->root = NULL;
	return tree;
}


/* Verifica daca un arbore este gol (vid)
 * 		1 - daca arborele este gol
 * 		0 - in caz contrar
 */
int isEmpty(TTree* tree) {
	if (tree == NULL || tree->root == NULL) {
		return 1;
	}
	return 0;
}


/* Cauta un anumit element in interiorul unui arbore
 *
 * tree: structura cu metodele asociate arborelui
 *   !folosit pentru a putea compara elementul curent cu cel cautat
 *
 * x: radacina arborelui curent (in care se face cautarea)
 * elem: elementul ce trebuie cautat
 */
TreeNode* search(TTree* tree, TreeNode* x, void* elem) {
	if (x == NULL) {
		return x;
	}
	if (tree->compare(x->elem, elem) == 0) {
		return x;
	}
	TreeNode* foundLeft = search(tree, x->left, elem);
	TreeNode* foundRight = search(tree, x->right, elem);
	if (foundLeft != NULL) {
		return foundLeft;
	}
	if (foundRight != NULL) {
		return foundRight;
	}
	return NULL;
}


/* Gaseste nodul cu elementul minim dintr-un arbore
 * avand radacina in x
 */
TreeNode* minimum(TreeNode* x) {
	if (x == NULL) {
		return x;
	}
	TreeNode* iter = x;
	while (iter->left != NULL) {
		iter = iter->left;
	}
	return iter;
}

/* Gaseste nodul cu elementul maxim dintr-un arbore
 * avand radacina in x
 */
TreeNode* maximum(TreeNode* x) {
	if (x == NULL) {
		return x;
	}
	TreeNode* iter = x;
	while (iter->right != NULL) {
		iter = iter->right;
	}
	return iter;
}


/* Functie pentru gasirea succesorului unui nod
 * (succesorul in inordine)
 */
TreeNode* successor(TreeNode* x) {
	if (x == NULL) {
		return x;
	}
	if (x->right != NULL) {
		return minimum(x->right);
	}
	TreeNode* temp = x->parent;
	TreeNode* iter = x;
	while (temp != NULL && iter == temp->right) {
		iter = temp;
		temp = temp->parent;
	}
	return temp;
}


/* Functie pentru gasirea predecesorului unui nod
 * (predecesorul in inordine)
 */
TreeNode* predecessor(TreeNode* x) {
	if (x == NULL) {
		return NULL;
	}
	if (x->left != NULL) {
		return maximum(x->left);
	}
	TreeNode* temp = x->parent;
	TreeNode* iter = x;
	while (temp != NULL && iter == temp->left) {
		iter = temp;
		temp = temp->parent;
	}
	return temp;
}


/* Actualizeaza inaltimea unui nod din arbore
 */
void updateHeight(TreeNode* x) {
	if (x == NULL) {
		return;
	}

	int leftHeight = 0;
	int rightHeight = 0;
	updateHeight(x->left);
	updateHeight(x->right);

	if (x != NULL) {
		if (x->left != NULL)  leftHeight  = x->left->height;
		if (x->right != NULL) rightHeight = x->right->height;
		x->height = MAX(leftHeight, rightHeight) + 1;
	}
}


/* Functie ce primeste adresa unui arbore si
 * a unui nod x si realizeaza o rotatie la stanga
 * a subarborelui care are varful in x
 *
 *   (x)	 Rotatie    (y)
 *	 / \     stanga     / \
 *  a  (y)    ---->   (x)  c
 *	   / \			  / \
 *    b   c          a   b
 */
void avlRotateLeft(TTree* tree, TreeNode* x) {
	if (tree == NULL || x == NULL) {
		return;
	}
	char direction = '\0';
	if(x->parent != NULL) {
		if (x->parent->left != NULL) {
			if (tree->compare(x->elem, x->parent->left->elem) == 0) {
				direction = 'l';
			} else {
				direction = 'r';
			}
		} else {
			direction = 'r';
		}
	}
	TreeNode* cpy = x;
	TreeNode* y = cpy->right;
	TreeNode* temp = y->left;
	if (temp != NULL) {
		temp->parent = cpy;
	}
	cpy->right = temp;
	y->left = cpy;
	y->parent = cpy->parent;
	cpy->parent = y;

	x = y;
	updateHeight(x->left);
	updateHeight(x->right);
	updateHeight(x);
	updateHeight(x->parent);
	if (direction == 'l') {
		x->parent->left = x;
	} else if (direction == 'r') {
		x->parent->right = x;
	}
	TreeNode* iter = x;
	while(iter->parent != NULL) {
		iter = iter->parent;
	}
	tree->root = iter;
}


/* Functie ce primeste adresa unui arbore si
 * a unui nod x si realizeaza o rotatie la dreapta
 * a subarborelui care are varful in y
 *
 *     (y)	 Rotatie    (x)
 *	   / \   dreapta    / \
 *   (x)  c   ---->    a  (y)
 *	 / \			      / \
 *  a   b                b   c
 */
void avlRotateRight(TTree* tree, TreeNode* y) {
	if (tree == NULL || y == NULL) {
		return;
	}
	char direction = '\0';
	if(y->parent != NULL) {
		if (y->parent->left != NULL) {
			if (tree->compare(y->elem, y->parent->left->elem) == 0) {
				direction = 'l';
			} else {
				direction = 'r';
			}
		} else {
			direction = 'r';
		}
	}
	TreeNode* cpy = y;
	TreeNode* x = cpy->left;
	TreeNode* temp = x->right;
	if (temp != NULL) {
		temp->parent = cpy;
	}
	cpy->left = temp;
	x->right = cpy;
	x->parent = cpy->parent;
	cpy->parent = x;

	y = x;
	updateHeight(y->left);
	updateHeight(y->right);
	updateHeight(y);
	updateHeight(y->parent);
	if (direction == 'l') {
		y->parent->left = y;
	} else if (direction == 'r') {
		y->parent->right = y;
	}
	TreeNode* iter = y;
	while (iter->parent != NULL) {
		iter = iter->parent;
	}
	tree->root = iter;
}

void avlRotateRightLeft(TTree* tree, TreeNode* x) {
	if (tree == NULL || x == NULL) {
		return;
	}
	char direction = '\0';
	if(x->parent != NULL) {
		if (x->parent->left != NULL) {
			if (tree->compare(x->elem, x->parent->left->elem) == 0) {
				direction = 'l';
			} else {
				direction = 'r';
			}
		} else {
			direction = 'r';
		}
	}
	TreeNode* cpy = x;
	TreeNode* P = x->parent;
	TreeNode* C = x->right;
	TreeNode* B = C->left;
	TreeNode* temp1 = B->left;
	TreeNode* temp2 = B->right;

	cpy->right = temp1;
	cpy->parent = B;
	B->left = cpy;
	B->right = C;
	C->left = temp2;
	C->parent = B;
	if (temp1 != NULL) {
		temp1->parent = cpy;
	}
	if (temp2 != NULL) {
		temp2->parent = C;
	}
	B->parent = P;

	x = B;
	updateHeight(x->left);
	updateHeight(x->right);
	updateHeight(x);
	updateHeight(x->parent);
	if (direction == 'l') {
		x->parent->left = x;
	} else if (direction == 'r') {
		x->parent->right = x;
	}
	TreeNode* iter = x;
	while(iter->parent != NULL) {
		iter = iter->parent;
	}
	tree->root = iter;
}

void avlRotateLeftRight(TTree* tree, TreeNode* x) {
	if (tree == NULL || x == NULL) {
		return;
	}
	char direction = '\0';
	if(x->parent != NULL) {
		if (x->parent->left != NULL) {
			if (tree->compare(x->elem, x->parent->left->elem) == 0) {
				direction = 'l';
			} else {
				direction = 'r';
			}
		} else {
			direction = 'r';
		}
	}
	TreeNode* cpy = x;
	TreeNode* A = x->left;
	TreeNode* B = A->right;
	TreeNode* P = x->parent;
	TreeNode* temp1 = B->left;
	TreeNode* temp2 = B->right;

	A->right = temp1;
	A->parent = B;
	B->left = A;
	B->right = cpy;
	cpy->left = temp2;
	cpy->parent = B;
	if (temp1 != NULL) {
		temp1->parent = A;
	}
	if (temp2 != NULL) {
		temp2->parent = cpy;
	}
	B->parent = P;

	x = B;
	updateHeight(x->left);
	updateHeight(x->right);
	updateHeight(x);
	updateHeight(x->parent);
	if (direction == 'l') {
		x->parent->left = x;
	} else if (direction == 'r') {
		x->parent->right = x;
	}
	TreeNode* iter = x;
	while(iter->parent != NULL) {
		iter = iter->parent;
	}
	tree->root = iter;
}

int heightNode(TreeNode* root) {
	if (root == NULL) {
		return 0;
	}
	return root->height;
}

/* Calculeaza factorul de echilibrare pentru un nod x
 * (AVL balance factor)
*/
int avlGetBalance(TreeNode *x) {
	if (x == NULL) {
		return 0;
	}
	return heightNode(x->left) - heightNode(x->right);
}


/* Functie pentru reechilibrarea unui arbore AVL
 * in urma unei inserari prin rotatii simple sau duble
 * si recalcularea inaltimii fiecarui nod intalnit parcurgand
 * arborele de jos in sus, spre radacina
 *
 */
void avlFixUp(TTree* tree, TreeNode* y) {
	if (y == NULL || (y->left == NULL && y->right == NULL)) {
		return;
	}
	avlFixUp(tree, y->left);
	avlFixUp(tree, y->right);
	int bf = avlGetBalance(y);
	if (bf > 1) {
		if (avlGetBalance(y->left) >= 0) {
			avlRotateRight(tree, y);
		} else if (avlGetBalance(y->left) <= -1) {
			avlRotateLeftRight(tree, y);
		}
	} else if (bf < -1) {
		if (avlGetBalance(y->right) >= 1) {
			avlRotateRightLeft(tree, y);
		} else if (avlGetBalance(y->right) <= -1) {
			avlRotateLeft(tree, y);
		}
	} else {
		return;
	}
	TreeNode* iter = y;
	while (iter->parent != NULL) {
		iter = iter->parent;
	}
	if(y->parent != NULL) {
		if (y->parent->left != NULL) {
			if (tree->compare(y->elem, y->parent->left->elem) == 0) {
				y->parent->left = y;
			}
		} else {
			y->parent->right = y;
		}
	}
	tree->root = iter;
}


/* Functie pentru crearea unui nod
 *
 * value: valoarea/cheia din cadrul arborelui
 * info: informatia/valoarea din dictionar
 */
TreeNode* createTreeNode(TTree *tree, void* value, void* info) {

	if (tree == NULL)
		return NULL;

	// Alocarea memoriei
	TreeNode* node = (TreeNode*) malloc(sizeof(TreeNode));

	// Setarea elementului si a informatiei
	node->elem = tree->createElement(value);
	node->info = tree->createInfo(info);

	// Initializarea legaturilor din cadrul arborelui
	node->parent = node->right = node->left = NULL;

	// Initializarea legaturilor din cadrul listei dublu inlantuite
	node->next = node->prev = node->end = NULL;


	// Inaltimea unui nod nou este 1
	// Inaltimea lui NULL este 0
	node->height = 1;

	return node;
}


/* Inserarea unul nou nod in cadrul multi-dictionarului
 * ! In urma adaugarii arborele trebuie sa fie echilibrat
 *
 */
void insert(TTree* tree, void* elem, void* info) {
	if (isEmpty(tree)) {
		tree->root = createTreeNode(tree, elem, info);
		tree->root->end = tree->root;
		tree->size++;
		return;
	}
	TreeNode* find = NULL;
	find = search(tree, tree->root, elem);
	if (find == NULL) {
		TreeNode* iter = tree->root;
		TreeNode* prevNode = tree->root;
		char direction = '\0';
		while (iter != NULL) {
			prevNode = iter;
			if (tree->compare(elem, iter->elem) == -1) {
				iter = iter->left;
				direction = 'l';
			} else {
				iter = iter->right;
				direction = 'r';
			}
		}
		iter = createTreeNode(tree, elem, info);
		iter->parent = prevNode;
		if (direction == 'l') {
			prevNode->left = iter;
		} else if (direction == 'r') {
			prevNode->right = iter;
		}
		TreeNode* contor = prevNode;
		while (contor->parent != NULL) {
			updateHeight(contor);
			contor = contor->parent;
		}
		iter->end = iter;
		TreeNode* pred = predecessor(iter);
		if (pred != NULL) {
			iter->prev = pred->end;
		}
		if (iter->prev != NULL) {
			iter->prev->next = iter;
		}
		iter->next = successor(iter);
		if (iter->next != NULL) {
			iter->next->prev = iter->end;
		}
		tree->root = contor;
		avlFixUp(tree, tree->root);
	} else if (find != NULL) {
		TreeNode* list = createTreeNode(tree, elem, info);
		TreeNode* temp = find->end->next;
		find->end->next = list;
		list->prev = find->end;
		list->next = temp;
		find->end = list;
		if (temp != NULL) {
			temp->prev = list;
		}
	}
	tree->size++;
}


/* Eliminarea unui nod dintr-un arbore
 *
 * ! tree trebuie folosit pentru eliberarea
 *   campurilor `elem` si `info`
 * */
void destroyTreeNode(TTree *tree, TreeNode* node){

	// Verificarea argumentelor functiei
	if(tree == NULL || node == NULL) return;

	// Folosirea metodelor arborelui
	// pentru de-alocarea campurilor nodului
	tree->destroyElement(node->elem);
	tree->destroyInfo(node->info);

	// Eliberarea memoriei nodului
	free(node);
}


/* Eliminarea unui nod din arbore
 *
 * elem: cheia nodului ce trebuie sters
 * 	! In cazul in care exista chei duplicate
 *	  se va sterge ultimul nod din lista de duplicate
 */
void delete(TTree* tree, void* elem) {
	if (isEmpty(tree)) {
		return;
	}
	TreeNode* find = search(tree, tree->root, elem);
	if (find == NULL) {
		return;
	} else if (find->end == find) {
		if (find == tree->root && find->right == NULL && find->left == NULL) {
			destroyTreeNode(tree, find);
			find = NULL;
			tree->root = find;
			tree->size--;
			return;
		}
		char direction = '\0';
		if(find->parent != NULL) {
			if (find->parent->left != NULL) {
				if (!(tree->compare(find->elem, find->parent->left->elem))) {
					direction = 'l';
				} else {
					direction = 'r';
				}
			} else {
				direction = 'r';
			}
		}
		if ((find->left == NULL) || (find->right == NULL)) {
			if (find->prev != NULL) {
				find->prev->next = find->next;
			}
			if (find->next != NULL) {
				if (find->prev != NULL) {
					find->next->prev = find->prev->end;
				} else {
					find->next->prev = NULL;
				}
				
			}
			TreeNode* temp = NULL;
			if (find->left != NULL) {
				temp = find->left;
			} else {
				temp = find->right;
			}
			if (temp == NULL) {
				temp = find;
				find = NULL;
				if (direction == 'l') {
					temp->parent->left = NULL;
				} else if (direction == 'r') {
					temp->parent->right = NULL;
				}
				destroyTreeNode(tree, temp);
				temp = NULL;
				if (tree->size > 2) {
					avlFixUp(tree, tree->root);
				}
			} else {
				TreeNode* iter = find;
				if (direction == 'l') {
					find->parent->left = temp;
				} else if (direction == 'r') {
					find->parent->right = temp;
				}
				find = temp;
				find->parent = iter->parent;
				destroyTreeNode(tree, iter);
				iter = NULL;
				if (tree->size > 2) {
					avlFixUp(tree, tree->root);
				}
			}
		} else {
			TreeNode* temp = successor(find);
			TreeNode* cpy = temp->right;
			TreeNode* par = temp->parent;
			if (direction == 'l') {
				find->parent->left = temp;
			} else if (direction == 'r') {
				find->parent->right = temp;
			}
			temp->parent = find->parent;
			if (find->parent == NULL) {
				tree->root = temp;
			}
			if (find->right != temp) {
				temp->right = find->right;
				if (find->right != NULL) {
					find->right->parent = temp;
				}
				par->left = cpy;
				if (cpy != NULL) {
					cpy->parent = par;
				}
			}
			temp->left = find->left;
			if (find->left != NULL) {
				find->left->parent = temp;
			}
			find->prev->next = find->next;
			temp->prev = find->prev;
			destroyTreeNode(tree, find);
			find = NULL;
			if (tree->size > 2) {
				avlFixUp(tree, tree->root);
			}
		}
	} else {
		TreeNode* temp = find->end;
		find->end = temp->prev;
		temp->prev->next = temp->next;
		if (temp->next != NULL) {
			temp->next->prev = temp->prev;
		}
		destroyTreeNode(tree, temp);
		temp = NULL;
	}
	tree->size--;
}


/* Eliberarea memoriei unui arbore
 */
void destroyTree(TTree* tree){

	/* Se poate folosi lista dublu intalntuita
	 * pentru eliberarea memoriei
	 */
	if (tree == NULL || tree->root == NULL) {
		return;
	}
	TreeNode* first = minimum(tree->root);
	TreeNode* temp = first;
	while (first != NULL) {
		first = first->next;
		destroyTreeNode(tree, temp);
		temp = first;
		tree->size--;
	}
	free(tree);
}
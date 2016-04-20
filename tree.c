#include <stdlib.h>
#include <stdio.h>

typedef struct tree {
	struct tree *up, *left, *right;
	int data;
}tree;

void insert(tree **root, int x) {
	if (*root == NULL) {
		*root = (tree*)malloc(sizeof(tree));
		(*root)->data = x;
		(*root)->up = (*root)->left = (*root)->right = NULL;
	}
	else if ((*root)->data > x) {
		if ((*root)->left) insert(&(*root)->left, x);
		else {
			tree *n = (tree*)malloc(sizeof(tree));
			n->data = x;
			n->left = n->right = NULL;
			n->up = (*root);
			(*root)->left = n;
		}
	}
	else {
		if ((*root)->right) insert(&(*root)->right, x);
		else {
			tree *n = (tree*)malloc(sizeof(tree));
			n->data = x;
			n->left = n->right = NULL;
			n->up = (*root);
			(*root)->right = n;
		}
	}
}

void preorder(tree *root) {
	if (root) {
		printf("%d ", root->data);
		preorder(root->left);
		preorder(root->right);
	}
}

void inorder(tree *root) {
	if (root) {
		inorder(root->left);
		printf("%d ", root->data);
		inorder(root->right);
	}
}

void postorder(tree *root) {
	if (root) {
		postorder(root->left);
		postorder(root->right);
		printf("%d ", root->data);
	}
}

tree* BSTmax(tree *root) {
	if (root) {
		while (root->right) root = root->right;
	}
	return root;
}

tree* BSTmin(tree *root) {
	if (root) {
		while (root->left) root = root->left;
	}
	return root;
}

tree* search(tree *root, int x) {
	if(root == NULL) return root;
	while (root) {
		if (root->data > x) root = root->left;
		else if (root->data < x) root = root->right;
		else break;
	}
	return root;
}
tree* succesor(tree *root, int x) {
	tree *n = search(root, x);
	if (n) {
		if (n->right)
			return BSTmin(n->right);
		else {
			tree *pn = n->up;
			while(pn && pn->left != n){
				n = pn;
				pn = n->up;
			}
			return pn;
		}
	}
	return n;
}

tree* predecessor(tree *root, int x) {
	tree *n = search(&(*root), x);
	if (n) {
		if (n->left)
			return BSTmax(n->left);
		else {
			tree *pn = n->up;
			while (pn && pn->right != n) {
				n = pn;
				pn = n->up;
			}
			return pn;
		}
	}
	return n;
}

void deleteBST(tree **root, tree **n){
	if(*root == NULL || *n == NULL) return;
	if((*n)->left == NULL && (*n)->right == NULL){
		if((*n)->up == NULL)
			(*root) = NULL;
		else if((*n)->up->left == (*n))
			(*n)->up->left = NULL;
		else
			(*n)->up->right = NULL;
		free(*n);
	}
	else if((*n)->left == NULL || (*n)->right == NULL){
		if((*n)->left == NULL){
			if((*n)->up == NULL)
				(*root)=(*n)->right;
			else if((*n)->up->left == (*n))
				(*n)->up->left = (*n)->right;
			else
				(*n)->up->right = (*n)->right;
		}
		else{
			if((*n)->up == NULL)
				(*root)=(*n)->left;
			else if((*n)->up->left == (*n))
				(*n)->up->left = (*n)->left;
			else
				(*n)->up->right = (*n)->left;	
		}
		free(*n);
	}
	else{
		tree *tmp = NULL;
		tmp = BSTmin((*n)->right);
		(*n)->data = tmp->data;
		deleteBST(&(*root), &tmp);
	}

}
int main() {
	tree *root = NULL, *n = NULL;
	int dana, c;
	printf("1 Dodaj\n2 Usun\n3 Wyszukaj\n4 Element minimalny\n5 Element maksymalny\n6 Nastepnik\n7 Poprzednik\n8 Preorde\n9 Inorder\n10 Postorder\n11 Zakoncz\n");
	do{
	scanf("%d", &c);
	switch(c){
		case 1:
			printf("\nPodaj liczbe: ");
			scanf("%d", &dana);
			insert(&root, dana);
			break;
		case 2:
			printf("\nPodaj liczbe: ");
			scanf("%d", &dana);
			n = search(root, dana);
			deleteBST(&root, &n);
			break;
		case 3:
			printf("\nPodaj liczbe: ");
			scanf("%d", &dana);
			n = search(root, dana);
			if(n) printf("\nDana jest w drzewie");
			else printf("\nDanej liczby nie ma w drzewie");
			break;
		case 4:
			if(n = BSTmin(root)) printf("\n%d", n->data);
			else printf("\nDrzewo puste!");
			break;
		case 5:
			if(n = BSTmax(root)) printf("\n%d", n->data);
			else printf("\nDrzewo puste!");
			break;
		case 6:
			printf("\nPodaj liczbe: ");
			scanf("%d", &dana);
			n = succesor(root, dana);
			if(n) printf("\n%d", n->data);
			else printf("\nNie ma nastepnika");
			break;
		case 7:
			printf("\nPodaj liczbe: ");
			scanf("%d", &dana);
			n = predecessor(root, dana);
			if(n) printf("\n%d", n->data);
			else printf("\nNie ma poprzednika");
			break;
		case 8:
			printf("\n");
			preorder(root);
			printf("\n");
			break;
		case 9:
			printf("\n");
			inorder(root);
			printf("\n");
			break;
		case 10:
			printf("\n");
			postorder(root);
			printf("\n");
			break;
		case 11:
			return 0;
			break;
		default:
			printf("\nWybrana operacja nie istnieje!");
			break;
	}
	}while(1);
	return 0;
}

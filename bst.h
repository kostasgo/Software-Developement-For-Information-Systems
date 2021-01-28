#ifndef bst_h
#define bst_h

// An AVL tree node
typedef struct TreeNodeType{
	double key;
  int index;
  int prediction;
  SparseV* x;
	struct TreeNodeType *left;
	struct TreeNodeType *right;

}TreeNode;

TreeNode* createTreeNode(double, int, int, SparseV*);
TreeNode* insertTree(TreeNode*, double, int, int, SparseV*);
int inOrderValidation(TreeNode *, char**, Hashtable*, Classifier*, ListNode**);
void deleteTree(TreeNode *);

#endif

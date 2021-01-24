#ifndef bst_h
#define bst_h

// An AVL tree node
typedef struct TreeNodeType{
	double key;
  int index;
  int prediction;
  double* x;
	struct TreeNodeType *left;
	struct TreeNodeType *right;

}TreeNode;

TreeNode* createTreeNode(double, int, int, double*);
TreeNode* insertTree(TreeNode*, double, int, int, double*);
int inOrderValidation(TreeNode *, char**, Hashtable*, Classifier*, ListNode**);
void deleteTree(TreeNode *);

#endif

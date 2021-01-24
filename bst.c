// C program to insert a node in AVL tree
#include <stdio.h>
#include <stdlib.h>


// An AVL tree node
typedef struct TreeNodeType{
	double key;
  int index;
  int prediction;
  double* x;
	struct TreeNodeType *left;
	struct TreeNodeType *right;

}TreeNode;



/* Helper function that allocates a new node with the given key and
	NULL left and right pointers. */
TreeNode* createTreeNode(double key, int index, int prediction, double* x){
	TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
	node->key = key;
  node->index = index;
  node->prediction = prediction;
  node->x=x;
	node->left = NULL;
	node->right = NULL;

	return(node);
}




TreeNode* insertTree(TreeNode* node, double key, int index, int prediction, double* x){
// Recursive function to insert a key in the subtree rooted
// with node and returns the new root of the subtree.
	if (node == NULL)
		return(createTreeNode(key,index,prediction,x));

	if (key < node->key)
		node->left = insertTree(node->left, key,index,prediction,x);
	else if (key >= node->key)
		node->right = insertTree(node->right, key,index,prediction,x);


	/* return the (unchanged) node pointer */
	return node;
}

// A utility function to print inOrder traversal
// of the tree.

int inOrder(TreeNode *root, char** array, Hashtable* table, Classifier* logReg, ListNode** speclist){
  int totalConflicts=0;
  int rightConflicts,leftConflicts;
	if(root != NULL)
	{
		rightConflicts=inOrder(root->right);


    //printf("%f %d\n", root->key, root->index);
    if(root->key!=0){
      char line[200];
      strcpy(line,array[root->index]);

      char* id1, *id2;
    	char delim[2] =",";
      int id1_isIn, id2_isIn;

    	char *token = strtok(line, delim);
    	id1=strdup(token);

    	token = strtok(NULL, delim);
      id2 = strdup(token);

      BucketData* data1=searchHashtable(table, id1);
    	BucketData* data2=searchHashtable(table, id2);

      if(data1->clique==NULL){
    		id1_isIn=0;
    	}
      else{
        id1_isIn=1;
      }

      if(data2->clique==NULL){
    		id2_isIn=0;
    	}
      else{
        id2_isIn=1;
      }

      //CASE 1: NONE OF THEM IS IN THE HASHTABLE

      if((!id1_isIn) && (!id2_isIn)){
        Specs* spec1 =createSpecs(id1);
        Specs* spec2 =createSpecs(id2);
        insertList(speclist, spec1);
        insertList(speclist, spec2);
        insertHashtable(table, spec1);
        insertHashtable(table, spec2);
      }

      //CASE 2: SPEC 1 IS IN, SPEC 2 NOT

      if((id1_isIn) && (!id2_isIn)){

        Specs* spec2=createSpecs(id2);
        insertList(speclist, spec2);
        insertHashtable(table, spec2);
        if(root->prediction ==1){
          mergeCliques(table, id1, id2);
        }
        else{
          updateNegatives(table, id1, id2);
        }
      }

      //CASE 3: SPEC 2 IS IN, SPEC 1 NOT

      if((!id1_isIn) && (id2_isIn)){

        Specs* spec1=createSpecs(id1);
        insertList(speclist, spec1);
        insertHashtable(table, spec1);
        if(root->prediction ==1){
          mergeCliques(table, id1, id2);
        }
        else{
          updateNegatives(table, id1, id2);
        }
      }

      //CASE 4: BOTH ARE IN THE HASHTABLE

      if((id1_isIn) && (id2_isIn)){

        if(root->prediction ==1){
          if(areNegatives(data1->clique, data2->clique)){
            totalConflicts++;
            int retrain=0;
            logisticRegression(logReg, (&root->x), &retrain, 10, 1);
          }
        }
        else{
          updateNegatives(table, id1, id2);
        }
      }

    }


		leftConflicts=inOrder(root->left);
	}
}

/* Driver program to test above function*/
int main(){
TreeNode *root = NULL;

/* Constructing tree given in the above figure */
root = insertTree(root, 9, 0, 1);
root = insertTree(root, 5, 1, 0);
root = insertTree(root, 4, 2 ,1);
root = insertTree(root, 8, 3 ,0);
root = insertTree(root, 8.7, 4 ,1);
root = insertTree(root, 3, 5 ,0);

/* The constructed AVL Tree would be
			30
		/ \
		20 40
		/ \	 \
	10 25 50
*/

printf("inOrder traversal of the constructed AVL"
		" tree is \n");
inOrder(root);

return 0;
}

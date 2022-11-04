#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int findHeight(Node * root) {
	//base case
	if(root == NULL) {
		return 0;
	}

	//calculate left and right heights
	int leftHeight = 1 + findHeight(root->left);
	int rightHeight = 1 + findHeight(root->right);

	//return greater of the two
	if(leftHeight > rightHeight) {
		return leftHeight;
	}
	return rightHeight;

}


bool equalPaths(Node * root)
{
    // Add your code below
		//base case
		if(root == NULL) {
			return true;
		}

		//if no children, then balanced
		if(root->left == NULL && root->right == NULL) {
			return true;
		}

		//if only left, check if left balanced
		if(root->left != NULL && root->right == NULL) {
			return equalPaths(root->left);
		}

		//if only right, check if right balanced
		if(root->left == NULL && root->right != NULL) {
			return equalPaths(root->right);
		}

		//if both, check if equal heights
		if(findHeight(root->left) == findHeight(root->right)) {
			return findHeight(root->left) && findHeight(root->right);
		}

		//if they were not equal, false
		return false;
		


}


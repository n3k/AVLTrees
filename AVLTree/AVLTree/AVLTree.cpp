// AVLTree.cpp : Defines the entry point for the console application.
//
#include <iostream>

using namespace std;

class Node 
{
	protected:
	    int height_value;
	public:
		Node *left;
		Node *right;
		Node *parent;
		int value;
		Node(int value) 
		{
			this->left = NULL;
			this->right = NULL;
			this->parent = NULL;
			this->value = value;
			this->height_value = -1;
		}	

		int heightDifference()
		{
			int leftHeight = -1;
			int rightHeight = -1;
			if (this->left != NULL) 
			{
				leftHeight = this->left->height();
			}
			if (this->right != NULL) 
			{
				rightHeight = this->right->height();
			}
			return abs(rightHeight - leftHeight);
		}

		bool hasChilds() 
		{			
			if (this->left || this->right)
				return true;
			return false;
		}

		unsigned int height() 
		{
			if (this->height_value != -1) return this->height_value;

			if (!this->left && !this->right) 
			{
				this->height_value = 0;
				return this->height_value;
			}
			if (!this->left)
			{
				this->height_value = this->right->height() + 1;
				return this->height_value;
			}
			if (!this->right) 
			{
				this->height_value = this->left->height() + 1;
				return this->height_value;	  
			}
			this->height_value = max(this->left->height(), this->right->height()) + 1;
			return this->height_value;
		}

		void resetHeight() 
		{
			this->height_value = -1;
		}
};

class BinaryTree
{	
	protected:
		Node *root;
		unsigned int elements;
		typedef void (*Callback)(BinaryTree *tree, Node *node);

		void refreshAllHeights()
		{
			this->preorder(this->resetHeight);
		}
		void _preorder(Node *aux, void *callback) 
		{	
			if (callback)
			{
				((Callback)callback)(this, aux);
			} else { cout << aux->value << endl; }

			if (aux->left) 
			{
				this->_preorder(aux->left, callback);
			}
			if (aux->right) 
			{
				this->_preorder(aux->right, callback);
			}
		}
		void _inorder(Node *aux, void *callback) 
		{			
			if (aux->left) 
			{
				this->_inorder(aux->left, callback);
			}
			
			if (callback)
			{
				((Callback)callback)(this, aux);
			} else { cout << aux->value << endl; }

			if (aux->right) 
			{
				this->_inorder(aux->right, callback);
			}
		}
		void _postorder(Node *aux, void *callback) 
		{			
			if (aux->left) 
			{
				this->_postorder(aux->left, callback);
			}
			if (aux->right) 
			{
				this->_postorder(aux->right, callback);
			}
			
			if (callback)
			{
				((Callback)callback)(this, aux);
			} else { cout << aux->value << endl; }
		}

	public:

		//// Defined Callbacks

		static void printValue(BinaryTree *tree, Node *node)
		{
			cout << "!!" << node->value << endl;
		}
		static void resetHeight(BinaryTree *tree, Node *node) 
		{
			node->resetHeight();
		}

		//// Tree Operations

		void insert(Node *newNode) 
		{
			//Check if exists a node with the same value in the tree
			if (this->search(newNode->value) != NULL) 
			{
				return;
			}

			if (this->root == NULL)
			{
				this->root = newNode;				
			} else {
				Node *aux = this->root;

				while ( aux->hasChilds() ) 
				{	
					if (newNode->value < aux->value)
					{
						if (aux->left != NULL) 
						{
							aux = aux->left;		
							continue;
						}						
					}
					if (newNode->value >= aux->value)
					{
						if (aux->right != NULL) 
						{
							aux = aux->right;		
							continue;
						}												
					}
					break;					
				}	

				if (newNode->value >= aux->value)
				{
					aux->right = newNode;					
				} else {
					aux->left = newNode;
				}
				//Set the parent
				newNode->parent = aux;
			}

			// Refresh all the heights
			this->refreshAllHeights();
		}	

		void remove(int value)
		{
			//Check if exists a node with the value in the tree
			Node *node = this->search(value);
			Node *aux = NULL;

			if (node == NULL) 
			{
				return;
			}

			Node *parent = node->parent;

			// If the node is a leaf
			if (!node->hasChilds()) 
			{
				if (node->value > parent->value) 
				{
					parent->right = NULL;
				} else {
					parent->left = NULL;		
				}					
			}			
			else  // If it is not a leaf...
			{
				if (node->left != NULL)
				{
					// Take the greater of the left side
					aux = node->left;
					while (aux->right != NULL) 
					{
						aux = aux->right;
					}
					if (aux->left && aux->parent != node) 
					{					
						aux->parent->right = aux->left;
						aux->left->parent = aux->parent;						
					}
					if (node->value > parent->value) 
					{
						parent->right = aux;
					} else {
						parent->left = aux;		
					}
					aux->right = node->right;
					if (aux->parent != node)
						aux->left = node->left;
				}
				else
				{
					// Take the lowest of the right side
					aux = node->right;
					while (aux->left != NULL) 
					{
						aux = aux->left;
					}
					if (aux->right) 
					{
						if (aux->parent != node) 
						{
							aux->parent->left = aux->right;
							aux->right->parent = aux->parent;
						}
					}
					if (node->value > parent->value) 
					{
						parent->right = aux;
					} else {
						parent->left = aux;		
					}
					aux->left = node->left;
					if (aux->parent != node)
						aux->right = node->right;
				}
			}			
			//Finally delete the node!
			delete node;	
			// Refresh all the heights
			this->refreshAllHeights();
		}

		Node* search(int value) 
		{
			if (this->root == NULL)
			{
				return NULL;				
			} else {
				Node *aux = this->root;

				while ( aux->hasChilds() ) 
				{	
					if (value < aux->value)
					{
						if (aux->left != NULL) 
						{
							aux = aux->left;		
							continue;
						}						
					}
					if (value > aux->value)
					{
						if (aux->right != NULL) 
						{
							aux = aux->right;		
							continue;
						}												
					}
					break;					
				}	

				if (value == aux->value)
				{
					return aux;
				} else {
					return NULL;
				}
			}
		}

		//// Traverse
				
		void preorder(void *callback) 
		{
			if (this->root != NULL)
			{
				this->_preorder(this->root, callback);
			} 
		}
		void inorder(void *callback)
		{
			if (this->root != NULL)
			{
				this->_inorder(this->root, callback);
			} 
		}
		void postorder(void *callback)
		{
			if (this->root != NULL)
			{
				this->_postorder(this->root, callback);
			} 
		}
		unsigned int height()
		{
			if (this->root == NULL) return 0;
			return this->root->height();
		}
};

class AVLTree: public BinaryTree
{   
	public:
		static void balance(BinaryTree *tree, Node *node)
		{			
			((AVLTree*)tree)->rebalance(node);
		}

		void insert(Node *newNode)
		{
			BinaryTree::insert(newNode);
			this->preorder(balance);
		}

		void remove(int value)
		{
			BinaryTree::remove(value);
			this->preorder(balance);
		}
	
		void rebalance(Node *node) 
		{
			while(node->heightDifference() > 1) 
			{
				int heightLeft = 0; 
				int heightRight = 0;
				if (node->left != NULL)
					heightLeft = node->left->height();
				if (node->right != NULL)
					heightRight = node->right->height();
				if (heightLeft > heightRight)
				{
					this->rotateRight(node);
				}
				else 
				{
					this->rotateLeft(node);
				}
				// Refresh all the heights
				this->refreshAllHeights();
			}
		}		
		void rotateLeft(Node *node) 
		{
			Node *newParent = node->right;
			Node *newChild = node->right->left;
			node->right = newChild;
			if (newChild != NULL)
				newChild->parent = node;
			if (node != this->root)
			{
				newParent->parent = node->parent;
				if (node->value > node->parent->value)
					node->parent->right = newParent;
				else node->parent->left = newParent;
			} 
			else 
			{
				newParent->parent = NULL;
				this->root = newParent;
			}			
			newParent->left = node;
			node->parent = newParent;
		}
		void rotateRight(Node *node) 
		{
			Node *newParent = node->left;
			Node *newChild = node->left->right;
			node->left = newChild;
			if (newChild != NULL)
				newChild->parent = node;
			if (node != this->root)
			{
				newParent->parent = node->parent;
				if (node->value > node->parent->value)
					node->parent->right = newParent;
				else node->parent->left = newParent;
			} 
			else 
			{
				newParent->parent = NULL;
				this->root = newParent;
			}			
			newParent->right = node;
			node->parent = newParent;
		}
};

int main(int argc, char **argv)
{
	/*
	BinaryTree *tree = new BinaryTree();
	Node *node1 = new Node(2);	
	Node *node2 = new Node(7);
	Node *node3 = new Node(5);
	Node *node4 = new Node(4);
	Node *node5 = new Node(6);
	Node *node6 = new Node(20);
	Node *node7 = new Node(15);
	Node *node8 = new Node(10);
	Node *node9 = new Node(19);
	Node *node10 = new Node(16);
	Node *node11 = new Node(50);
	Node *node12 = new Node(40);
	Node *node13 = new Node(100);
	Node *node14 = new Node(30);

	tree->insert(node1);	
	tree->insert(node2);	
	tree->insert(node3);	
	tree->insert(node4);
	tree->insert(node5);
	tree->insert(node6);
	tree->insert(node7);
	tree->insert(node8);
	tree->insert(node9);
	tree->insert(node10);
	tree->insert(node11);
	tree->insert(node12);
	tree->insert(node13);
	tree->insert(node14);
	
	tree->preorder(tree->printValue);

	cout << tree->height() << endl;

	tree->remove(50);
	tree->preorder(tree->printValue);

	delete tree;
	*/

	AVLTree *tree = new AVLTree();

	Node *node1 = new Node(1);	
	Node *node2 = new Node(2);
	Node *node3 = new Node(3);
	Node *node4 = new Node(4);
	Node *node5 = new Node(5);
	Node *node6 = new Node(6);
	Node *node7 = new Node(7);
	Node *node8 = new Node(8);
	Node *node9 = new Node(9);

	tree->insert(node1);	
	tree->insert(node2);	
	tree->insert(node3);	
	tree->insert(node4);
	tree->insert(node5);
	tree->insert(node6);
	tree->insert(node7);
	tree->insert(node8);
	tree->insert(node9);

	tree->preorder(tree->printValue);

	getchar();	

	return 0;
}
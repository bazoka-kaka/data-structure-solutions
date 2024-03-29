#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct AVLNode_t
{
    char *data;
    struct AVLNode_t *left,*right;
    int height;
}AVLNode;

typedef struct AVL_t
{
    AVLNode *_root;
    unsigned int _size;
}AVL;

AVLNode* _avl_createNode(char *value) {
    AVLNode *newNode = (AVLNode*) malloc(sizeof(AVLNode));
    newNode->data = (char*)malloc(sizeof(char) * 200);
    strcpy(newNode->data, value);
    newNode->height=1;
    newNode->left = newNode->right = NULL;
    return newNode;
}

AVLNode* _search(AVLNode *root, char* value) {
    while (root != NULL) {
        if (strcmp(value, root->data) < 0)
            root = root->left;
        else if (strcmp(value, root->data) > 0)
            root = root->right;
        else
            return root;
    }
    return root;
}

int _getHeight(AVLNode* node){
    if(node==NULL)
        return 0; 
    return node->height;
}

int _max(int a,int b){
    return (a > b)? a : b;
}

AVLNode* _rightRotate(AVLNode* pivotNode) {

    AVLNode* newParrent=pivotNode->left;
    pivotNode->left=newParrent->right;
    newParrent->right=pivotNode;

    pivotNode->height=_max(_getHeight(pivotNode->left),
                      _getHeight(pivotNode->right))+1;
    newParrent->height=_max(_getHeight(newParrent->left),
                       _getHeight(newParrent->right))+1;
    
    return newParrent;
}

AVLNode* _leftRotate(AVLNode* pivotNode) {

    AVLNode* newParrent=pivotNode->right;
    pivotNode->right=newParrent->left;
    newParrent->left=pivotNode;

    pivotNode->height=_max(_getHeight(pivotNode->left),
                      _getHeight(pivotNode->right))+1;
    newParrent->height=_max(_getHeight(newParrent->left),
                       _getHeight(newParrent->right))+1;
    
    return newParrent;
}

AVLNode* _leftCaseRotate(AVLNode* node){
    return _rightRotate(node);
}

AVLNode* _rightCaseRotate(AVLNode* node){
    return _leftRotate(node);
}

AVLNode* _leftRightCaseRotate(AVLNode* node){
    node->left=_leftRotate(node->left);
    return _rightRotate(node);
}

AVLNode* _rightLeftCaseRotate(AVLNode* node){
    node->right=_rightRotate(node->right);
    return _leftRotate(node);
}

int _getBalanceFactor(AVLNode* node){
    if(node==NULL)
        return 0;
    return _getHeight(node->left)-_getHeight(node->right);
}

AVLNode* _insert_AVL(AVL *avl,AVLNode* node, char* value) {
    
    if(node==NULL) // udah mencapai leaf
        return _avl_createNode(value);
    if(strcmp(value, node->data) <= 0)
        node->left = _insert_AVL(avl,node->left,value);
    else
      node->right = _insert_AVL(avl,node->right,value);
    
    node->height= 1 + _max(_getHeight(node->left),_getHeight(node->right)); 

    int balanceFactor=_getBalanceFactor(node);
    
    if(balanceFactor > 1 && strcmp(value, node->left->data) < 0)
        return _leftCaseRotate(node);
    if(balanceFactor > 1 && strcmp(value, node->left->data) > 0)
    return _leftRightCaseRotate(node);
    if(balanceFactor < -1 && strcmp(value, node->right->data) > 0)
        return _rightCaseRotate(node);
    if(balanceFactor < -1 && strcmp(value, node->right->data) < 0)
        return _rightLeftCaseRotate(node);
    
    return node;
}

AVLNode* _findMinNode(AVLNode *node) {
    AVLNode *currNode = node;
    while (currNode && currNode->left != NULL)
        currNode = currNode->left;
    return currNode;
}

AVLNode* _remove_AVL(AVLNode* node, char *value){
    if(node==NULL)
        return node;
    if(strcmp(value, node->data) > 0)
      node->right=_remove_AVL(node->right,value);
    else if(strcmp(value, node->data) < 0)
      node->left=_remove_AVL(node->left,value);
    else{
        AVLNode *temp;
        if((node->left==NULL)||(node->right==NULL)){
            temp=NULL;
            if(node->left==NULL) temp=node->right;  
            else if(node->right==NULL) temp=node->left;
            
            if(temp==NULL){
                temp=node;
                node=NULL;
            }
            else
                *node=*temp;   
            
            free(temp);
        }
        else{
            temp = _findMinNode(node->right);
            free(node->data);
            node->data = (char*)malloc(sizeof(char) * 200);
            strcpy(node->data, temp->data);
            node->right = _remove_AVL(node->right,temp->data);
        }    
    }

  if(node==NULL) return node;
    
    node->height=_max(_getHeight(node->left),_getHeight(node->right))+1;

    int balanceFactor= _getBalanceFactor(node);
    
    if(balanceFactor>1 && _getBalanceFactor(node->left)>=0) 
        return _leftCaseRotate(node);

    if(balanceFactor>1 && _getBalanceFactor(node->left)<0) 
        return _leftRightCaseRotate(node);
  
    if(balanceFactor < -1 && _getBalanceFactor(node->right)<=0) 
        return _rightCaseRotate(node);

    if(balanceFactor < -1 && _getBalanceFactor(node->right)>0) 
        return _rightLeftCaseRotate(node);
    
    return node;
}

void avl_init(AVL *avl) {
    avl->_root = NULL;
    avl->_size = 0u;
}

bool avl_isEmpty(AVL *avl) {
    return avl->_root == NULL;
}

bool avl_find(AVL *avl, char *value) {
    AVLNode *temp = _search(avl->_root, value);
    if (temp == NULL)
        return false;
    
    if (strcmp(temp->data, value) == 0)
        return true;
    else
        return false;
}

void avl_insert(AVL *avl, char* value){
    //if(!avl_find(avl,value)){
        avl->_root=_insert_AVL(avl,avl->_root,value);
        avl->_size++;
    //}

}

void avl_remove(AVL *avl, char *value){
    if(avl_find(avl,value)){
        avl->_root=_remove_AVL(avl->_root,value);
        avl->_size--;
    }

}

int  y = 0;

void preorder(AVLNode *root) {
    if (root) {
        preorder(root->left);
        if(y <= 0) return;
        --y;
        printf("%s\n", root->data);
        preorder(root->right);
    }
}

int main(){
  AVL avlku;
  avl_init(&avlku);
  //avl_insert(&avlku,1);
  //avl_remove(&avlku,1);
  int t;
  scanf("%d", &t);
  while(t--){
    int n;
    scanf("%d", &n);
    if(n == 1){
      char str[200];
      scanf(" %[^\n]", str);      
      avl_insert(&avlku, str);
    }else if(n == 2){
      char str[200];
      scanf(" %[^\n]", str);
      avl_remove(&avlku, str);
    }else{
      int a;
      scanf("%d", &a);      
      y = a;
      //inorder(avlku._root);
      //printf("\n\n");
      preorder(avlku._root);
      printf("---\n");
    }
  }
  return 0;
}
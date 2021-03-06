#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap* new=(TreeMap*)malloc(sizeof(TreeMap)); 
    new->current=NULL;
    new->root=NULL;
    new->lower_than = lower_than;
    return new;
}



void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode* aux=createTreeNode(key,value);
    Pair* p = searchTreeMap(tree,key);
        if(p!=NULL){
        return;
    }
    if(tree->lower_than (key,tree->current)==1){
        tree->current->left=aux;
        aux->parent=tree->current;
    }
    else{
        tree->current->right=aux;
        aux->parent=tree->current;
    }
    tree->current=aux;
    return;
}

TreeNode * minimum(TreeNode * x){
    if(x->left==NULL){
        return x;
    }
    while(x!= NULL){
        x=x->left;
        if(x->left==NULL){
            return x;
        }
    }
    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if(node->left==NULL && node->right==NULL){
        if(tree->lower_than (node->parent->pair->key,tree->current)==1){
            tree->current=node->parent;
            node->parent->left=NULL;
        }
        else{
            tree->current=node->parent;
            node->parent->right=NULL;
        }
    }
    else if(node->left!=NULL && node->right!=NULL){
        tree->current=minimum(node->right);
        node->pair->key=tree->current->pair->key;
        node->pair->value=tree->current->pair->value;
        removeNode(tree,tree->current);
    }
    else{
        if(node->left==NULL){
            if(tree->lower_than (node->parent->pair->key,tree->current)==1){
                node->parent->left=node->right;
                node->right->parent=node->parent;
            }
            else{
            node->parent->left=node->right;
            node->right->parent=node->parent;
            }
        }
        else{
            if(tree->lower_than (node->parent->pair->key,tree->current)==1){
                node->parent->right=node->left;
            }
            else{
                node->parent->right=node->right;
            }
            node->left->parent=node->parent;
        }
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* aux=tree->root;
    while(aux !=NULL){
        tree->current=aux;
        if(tree->lower_than (key,aux->pair->key)==1){
            aux=aux->left;
        }else if(tree->lower_than (aux->pair->key,key)==1){
            aux=aux->right;
        }else{
            tree->current=aux;
            return aux->pair;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    Pair* x=searchTreeMap(tree,key);
    if(x!=NULL){
        return x; 
    }
    printf("key: %d\n",*(int*) tree->current->pair->key);
    if(tree->lower_than(key,tree->current->pair->key)==1){
      return tree->current->pair;  
    }
    else{
        if(tree->lower_than(key,tree->current->pair->key)==0){
            Pair* p=nextTreeMap(tree);
            return p;
        }
    }
    return NULL;

}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode* x=minimum(tree->root);
    return x->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode* aux=tree->current;
    if (aux->right!=NULL) {
        tree->current=minimum(aux->right);
        return minimum(aux->right)->pair;
    }
    while(tree->current->parent!=NULL){
        if(tree->lower_than(aux->pair->key,tree->current->parent->pair->key)==1){
            tree->current=tree->current->parent;    
            return tree->current->pair;
        }
        tree->current=tree->current->parent;
    }
    return NULL;        
}

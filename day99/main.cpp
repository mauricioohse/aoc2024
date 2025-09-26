#include <stdio.h>

typedef struct node{
    int item;
    node* left;
    node* right;
} node_t;


node_t * Create_node(int value){
    node_t * n = new node_t;

    n->item = value;
    n->left = nullptr;
    n->right = nullptr;

    return n;
}

void traverse_left_preference(node_t * n){
    if (!n) return;
    printf("%d ",n->item);
    traverse_left_preference(n->left);
    traverse_left_preference(n->right);
}

void traverse_bottom_preference(node_t * n){
    if (!n) return;
    traverse_bottom_preference(n->left);
    traverse_bottom_preference(n->right);
    printf("%d ",n->item);
}

void traverse_bottommost_preference(node_t * n){
    if (!n) return;
    traverse_bottommost_preference(n->left);
    printf("%d ",n->item);
    traverse_bottommost_preference(n->right);
}

/*
    1
   / \ 
  2   3
 / \ / \ 
4  5 6  7
*/

node_t * print_branches_of_order_n(node_t* node,int n){
    
    node_t* rtn_left = nullptr;
    node_t* rtn_right = nullptr;

    if (!n || n < 0)
    {
        if ( node->item) printf("%d ", node->item);
        return node;
    }

    if (node->left)  rtn_left = print_branches_of_order_n(node->left, n-1);
    if (node->right) rtn_right = print_branches_of_order_n(node->right, n-1);

    return rtn_left? rtn_left : rtn_right? rtn_right : nullptr;
}

void print_node_levels(node_t* root){
    node_t* last_node = {0};
    int i = 0;
    do{
        last_node = print_branches_of_order_n(root, i);
        i++;
        printf("\n");
    }
    while(last_node!=nullptr);

}


int main(){

    node_t* root = Create_node(1);
    root->left = Create_node(2);
    root->right = Create_node(3);
    root->left->left = Create_node(4);
    root->left->right = Create_node(5);
    root->right->left = Create_node(6);
    root->right->right = Create_node(7);

    printf("left preference \n");
    traverse_left_preference(root);

    printf("\nbottom left preference \n");
    traverse_bottom_preference(root);

    printf("\nbottommost left preference \n");
    traverse_bottommost_preference(root);

    printf("\n print depth \n");
    print_node_levels(root);

    return  0;
}
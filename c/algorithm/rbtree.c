#include<stdio.h>

struct RBTreeNode
{
    char color;
    int key ;
    struct RBTreeNode *left ;
    struct RBTreeNode *right ;
    struct RBTreeNode *p ;
};

struct RBTree
{
    RBTreeNode * root;
    RBTreeNode nil;
    int num;
};

void  RBTreeInit(RBTree* T)
{
    T->nil.color = 'b';
    T->nil.key = -1;
    T->nil.left = T->nil.right = T->nil.p = NULL;
    T->num = 0;
    T->root = &(T->nil);
}


void LEFT_ROTATE(struct RBTree *T,struct RBTreeNode *x)//左旋转
{
    struct RBTreeNode *y ;
    y = x->right;
    x->right =y->left ;
    y->left->p = x ;
    y->p = x->p ;
    if(x->p == &T->nil)
        T->root = y ;
    else if (x == x->p->left)
        x->p->left = y ;
    else x->p->right = y ;
    y->left = x ;
    x->p = y ;

}

void RIGHT_ROTATE(struct RBTree*T, struct RBTreeNode *x)//右旋转
{
    struct RBTreeNode *y ;
    y = x->left;
    x->left = y->right ;
    y->right->p= x ;
    y->p = x->p ;
    if(x->p == &T->nil)
        T ->root = y ;
    else if (x == x->p->right)
        x->p->right = y ;
    else x->p->left = y ;
    y->right = x ;
    x->p = y ;
}

void RB_INSERT_FIXUP(struct RBTree *T,struct RBTreeNode *z)//调整
{
    struct RBTreeNode *y ;
    while (z->p->color == 'r')
    {
        if (z->p == z->p->p->left)
        {
            y = z->p->p->right ;
            if (y!=&T->nil && y->color == 'r')// case1
            {
                z->p->color = 'b';
                y->color = 'b';
                z->p->p->color = 'r';
                z = z->p->p;
            }
            else
            {
                if (z == z->p->right)//case2
                {
                    z = z->p ;
                    LEFT_ROTATE(T,z);
                }
                z->p->color = 'b';
                z->p->p->color = 'r';// change case2 into case3
                RIGHT_ROTATE(T,z->p->p);//case 3
            }
        }
        else // case 4-case 6
        {
            y = z->p->p->left ;
            if (y!=&T->nil&&y->color == 'r')// case4
            {
                z->p->color = 'b';
                y->color = 'b';
                z->p->p->color = 'r';
                z = z->p->p;
            }
            else

            {
                if (z == z->p->left)//case5
                {
                    z = z->p ;
                    RIGHT_ROTATE(T,z);
                }
                z->p->color = 'b';
                z->p->p->color = 'r';// change case5 into case6
                LEFT_ROTATE(T,z->p->p);//case 6
            }
        }
    }
    T->root->color = 'b';
}

void RB_INSERT(struct RBTree *T, struct RBTreeNode *z)//插入
{
    struct RBTreeNode *y ;
    y = &T->nil ;
    struct RBTreeNode *x ;
    x = T->root;
    while ( x != &T->nil ) // 查找合适的位置插入节点
    {
        y = x ;
        if (z->key<x->key)
            x = x->left ;
        else
            x = x->right ;
    }
    z->p = y ;
    if(y == &T->nil )
        T->root = z ;
    else if (z->key<y->key)
        y->left = z ;
    else
        y ->right = z ;
    z->left = &T->nil ;
    z->right = &T->nil ;
    z->color = 'r';
    RB_INSERT_FIXUP(T,z);//插入调整
    T->num ++;
}

void PrintInOrder (struct RBTree *Tree,struct RBTreeNode*T)
{
    if(T!=&Tree->nil)
    {
        printf("%d %c\n",T->key,T->color);
        PrintInOrder(Tree,T->left);
        PrintInOrder(Tree,T->right);
    }
}//中序打印

int main ()
{
    RBTree Tree;
    RBTreeInit(&Tree);
    RBTree* T = &Tree;
    int num ;
    printf("please input the number of RBTreeNode\n");
    scanf("%d",&num);
    printf("please input the value of RBTreeNode\n");
    for(int i = 0 ; i<num ; i++)
    {
        RBTreeNode* x = new RBTreeNode;
        x->color = 'r';
        x->p = x->left = x->right = NULL;
        printf("%d:",i+1);
        scanf("%d",&x->key);
        RB_INSERT(T,x);
    }
    printf("the structure of the RBtree is :\n");
    PrintInOrder(T,T->root);
    return 0;
}

//code author: Chinmay Shrikant Patil
//date: 22Feb,2019
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

struct Node{
    int arr[3][3];
    int value;
    int noc;
    struct Node** childs;
};

int nodes=1;

//function to print the puzzle
void printarr(int arr[3][3])
{
    int i,j;
    printf("  0 1 2\n");
    for(i=0;i<3;i++)
    {
        printf("%d ",i);
        for(j=0;j<3;j++)
        {
            switch(arr[i][j])
            {
                case 0: printf("_ ");
                        break;
                case -1: printf("X ");
                        break;
                case 1: printf("O ");
                        break;
            }
        }
        printf("\n");
    }

}

//function to calculate number of empty spaces in the puzzle
int emptyspaces(int arr[3][3])
{
    int i,j,num=0;
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            if(arr[i][j]==0)
                num++;
        }
    }
    return num;
}

//function to check the state of the puzzle, if anyone has won or draw
int checkState(struct Node* node)
{
    int i,j,max=0,min=0,sum,dg1,dg2;
    for(i=0;i<3;i++)
    {
        sum=0;
        for(j=0;j<3;j++)
        {
            sum=sum+node->arr[i][j];
        }
        if(sum>max)
            max=sum;
        if(sum<min)
            min=sum;
    }
    for(i=0;i<3;i++)
    {
        sum=0;
        for(j=0;j<3;j++)
        {
            sum=sum+node->arr[j][i];
        }
        if(sum>max)
            max=sum;
        if(sum<min)
            min=sum;
    }
    dg1=node->arr[0][0] + node->arr[1][1] + node->arr[2][2];
    if(dg1>max)
        max=dg1;
    if(dg1<min)
        min=dg1;
    dg2=node->arr[0][2] + node->arr[1][1] + node->arr[2][0];
    if(dg2>max)
        max=dg2;
    if(dg2<min)
        min=dg2;

    if(min==-3)
    {
        node->noc=0;
        node->value=-1;
        return 4;
    }
    if(max==3)
    {
        node->noc=0;
        node->value=1;
        return 5;
    }
    if(!emptyspaces(node->arr))
    {
        node->noc=0;
        node->value=0;
        return 9;
    }
    return 0;
}

//reccursive function that is called once initially to create the entire min-max tree
int recurr(struct Node* root,int wm)
{
    int i,j,min=1,max=-1,noc,t=0,p,q;
    struct Node* newNode;
    noc=emptyspaces(root->arr);
    root->noc=noc;
    root->childs=(struct Node** )malloc(sizeof(struct Node*)*noc);
    while(t<noc)
    {
        for(i=0;i<3;i++)
        {
            for(j=0;j<3;j++)
            {
                if(root->arr[i][j]==0)
                {
                    nodes++;
                    newNode=(struct Node* )malloc(sizeof(struct Node));
                    for(p=0;p<3;p++)
                    {
                        for(q=0;q<3;q++)
                        {
                            newNode->arr[p][q]=root->arr[p][q];
                        }
                    }
                    if(wm%2==1)
                        newNode->arr[i][j]=1;
                    else
                        newNode->arr[i][j]=-1;

                    if(!checkState(newNode))
                        recurr(newNode,wm+1);

                    if(newNode->value > max)
                        max=newNode->value;
                    if(newNode->value < min)
                        min=newNode->value;

                    root->childs[t]=newNode;
                    t++;
                }
            }
        }
    }
    if(wm%2==1)
        root->value=max;
    else
        root->value=min;

    return root->value;
}

int main(){
    int arr[3][3]={{0,0,0},{0,0,0},{0,0,0}};
    int count=1,i,j,p,q,t,max,check,state;
    struct Node *root=(struct Node*)malloc(sizeof(struct Node));
    printarr(arr);
    printf("Enter your move,i.e row and column\n");
    scanf("%d %d",&i,&j);
    arr[i][j]=-1;
    //printarr(arr);

    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            root->arr[i][j]=arr[i][j];
        }
    }

    recurr(root,1);
//    printf("total nodes in tree = %d\n",nodes);
    while(count<=8)
    {
        if(count%2==1)
        {
            max=0;
            t=root->noc;
////uncomment to see the state values of the children of the current node
//    for(i=0;i<t;i++)
//    {
//        printf("%d ",root->childs[i]->value);
//    }
//    printf("\n");
            while(t>0)
            {
                if(root->childs[t-1]->value > root->childs[max]->value)
                    max=t-1;
                t--;
            }

            root=root->childs[max];
            printarr(root->arr);
        }
        else if(count%2==0)
        {
            printf("Enter your move,i.e row and column\n");
            scanf("%d %d",&i,&j);
            for(p=0;p<3;p++)
                    {
                        for(q=0;q<3;q++)
                        {
                            arr[p][q]=root->arr[p][q];
                        }
                    }
            t=root->noc;
            arr[i][j]=-1;
            while(t>0)
            {
                check=1;
                for(p=0;p<3;p++)
                    {
                        for(q=0;q<3;q++)
                        {
                            if(root->childs[t-1]->arr[p][q] != arr[p][q])
                                check=0;
                        }
                    }
                if(check==1)
                    break;
                t--;
            }
            root=root->childs[t-1];
            //printarr(root->arr);
        }
        state=checkState(root);
        if(state==4)
        {
            printarr(root->arr);
            printf("\nUser(X) wins!\n");
            exit(0);
        }
        if(state==5)
        {
            printf("\nAI(O) wins!\n");
            exit(0);
        }
        if(state==9)
        {
            printarr(root->arr);
            printf("\nDraw!\n");
            exit(0);
        }
        count++;
    }
}

#include <iostream>

typedef struct _Node{
    int Key;
    struct _Node **Next;            ///vector de fii
    int nrFii;
}Node;
typedef struct _NodeB{
    int Key;
    struct _NodeB *first_child, *right_brother;
}NodeB;
Node* createNode(int Value)
{
    Node *p=(Node*)malloc(sizeof(Node));
    if(p==NULL)
    {
        printf("Not enough memory\n");
        exit(-1);
    }
    p->Key=Value;
    p->Next=(Node**)calloc(1,sizeof(Node*));
    p->nrFii=0;
    return p;
}
NodeB* createNodeB(int Value)
{
    NodeB *p=(NodeB*)malloc(sizeof(NodeB));
    if(p==NULL)
    {
        printf("Not enough memory\n");
        exit(-1);
    }
    p->Key=Value;
    p->first_child=NULL;
    p->right_brother=NULL;
    return p;
}
void PrettyPrint1(int *v, int n, int root, int nivel)
{
    /// functie recursiva de afisare
    for(int i=0;i<nivel;i++)
        printf(" ");
    printf("%d\n",root);

    for(int i=1;i<n;i++)
    {
        if(v[i]==root)           ///daca root e parintele nodului si nu am mai afisat acest nod, apelez
            PrettyPrint1(v, n, i,nivel+1); ///functia recursiva
    }
}
void PrettyPrint2(Node* root, int nivel)
{
    ///functie recursiva de afisare
    for(int i=0;i<nivel;i++)
        printf(" ");
    printf("%d\n",root->Key);
    if(root->nrFii>0)
        for(int i=0;i<root->nrFii;i++)              ///ma duc pe lista de fii ai fiecarui nod pornind de la radacina
            PrettyPrint2(root->Next[i],nivel+1);
}
void PrettyPrint3(NodeB* rootB, int nivel)
{
    ///functie recursiva de afisare
    if(rootB!=NULL)
    {
        for(int i=0;i<nivel;i++)
            printf(" ");
        printf("%d\n",rootB->Key);
        PrettyPrint3(rootB->first_child,nivel+1);       ///merg prima data pe primul fiu
        PrettyPrint3(rootB->right_brother,nivel);            ///apoi merg pe fratii din dreapta
    }
}
void T1(int *v, int n,Node** adrese, Node **root)
{
    ///folosesc vectorul de adrese ca sa am control asupra tuturor nodurilor
    Node *p=NULL;
    Node *q=NULL;
    for(int i=1;i<n;i++) {
        ///cazul radacinii
        if (v[i] == -1) {
            ///daca inca nu am ajuns la radacina, o creez si fac update la vectorul de adrese
            if (adrese[i] == NULL) {
                p = createNode(i);
                adrese[i] = p;
            }
            (*root) = adrese[i];
            continue;       ///ca sa treaca la urmatorul pas in for
        }
        ///daca inca nu a fost creat nodul cu key=i il creez
        if (adrese[i] == NULL) {
            p = createNode(i);
            adrese[i] = p;
            ///daca parintele inca nu a fost creat, il creez
            if (adrese[v[i]] == NULL) {
                q = createNode(v[i]);
                q->Next[q->nrFii++] = p;        ///fac legatura dintre parinte si copil
                adrese[v[i]] = q;               ///cresc si numarul de fii
            } else {
                ///daca deja exista fac doar legatura
                q = adrese[v[i]];
                q->Next = (Node **) realloc(q->Next, 2 * sizeof(Node *));
                q->Next[q->nrFii++] = p;
            }
        }
        else
        {   ///cazul in care a fost deja creat nodul cu key=i

            ///daca parintele lui nu a fost creat inca, il creez
            if(adrese[v[i]]==NULL)
            {
                q=createNode(v[i]);
                q->Next[q->nrFii++] = adrese[i];        ///fac legaatura
                adrese[v[i]] = q;
            }
            else
            {
                ///daca deja exista fac doar legatura
                q = adrese[v[i]];
                q->Next = (Node **) realloc(q->Next, 2 * sizeof(Node *));
                q->Next[q->nrFii++] = adrese[i];
            }
        }
    }
}
void T2(Node *root, NodeB *rootB)
{
    NodeB *q=NULL;
    NodeB *p=NULL;
    Node *next=NULL;

    if(root->nrFii>0)
    {
        ///prima data pun left child la fiecare nod, apoi pentru fiecare nod leg fratii in dreapta
        q=createNodeB(root->Next[0]->Key);
        rootB->first_child=q;
        T2(root->Next[0],rootB->first_child);       ///daca tot apelez recursiv aici, o sa imi puna prima data first
        for(int i=1;i<root->nrFii;i++)                  ///child la fiecare nod
        {
            p=createNodeB(root->Next[i]->Key);
            q->right_brother=p;                         ///aici leg fratii in dreapta; ma tot duc pe lista de fii de la
            T2(root->Next[i],q->right_brother);         ///structura de la arborele trecut (R2)
            q=q->right_brother;
        }
    }
}
void demo()
{
    int v[]={0,2,7,5,2,7,7,-1,5,2};   ///am pus v[0]=0 doar ca sa fie vectorul de parinti corect initializat

    int n=sizeof(v)/sizeof(v[0]);
    PrettyPrint1(v,n,7,0);
    printf("\n");

    Node **adrese=(Node**)calloc(n,sizeof(Node*));             ///ma folosesc de un vector de adrese
    Node *root=NULL;

    T1(v,n,adrese,&root);
    PrettyPrint2(root,0);
    printf("\n");

    NodeB *rootB;
    rootB=createNodeB(root->Key);

    T2(root,rootB);
    PrettyPrint3(rootB,0);
    printf("\n");

}
int main() {
    demo();
    return 0;
}

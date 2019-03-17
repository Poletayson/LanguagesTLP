#include "tree.h"

Tree* Tree::Cur;
Tree* Tree::F;

Tree::Tree()        //�� ��������� - ������ �������
{
    N = new Node ("", TypeEmpty);
    Right = Left = Parent = nullptr;
}



Tree::Tree(Tree *p, Tree *l, Tree *r, Node *n)
{
    N = n;
    Parent = p;
    Left = l;
    Right = r;
}

Tree::Tree(Node *n)
{
    N = n;
    Parent = nullptr;
    Left = nullptr;
    Right = nullptr;
}

void Tree::addLeft (Node *n)
{
    Tree *a = new Tree (this, nullptr, new Tree (), n);
    a->Right->Parent = a;    //�������� - �������
    Left = a;
}
void Tree::addRight (Node *n)
{
    Tree *a = new Tree (this, nullptr, new Tree (), n);
    a->Right->Parent = a;        //�������� - �������
    Right = a;
    //Cur = Right;
}
 Tree *Tree::Find (Tree *From, Node* n)     //���� �� ���� ������ �����
{
    Tree *cur = From;
    while (cur != nullptr)
         if (n->Id != cur->N->Id || n->TypeObj != cur->N->TypeObj)    //������ ������� �� ��������� � ��������
            cur = cur->Parent;
         else break;
    return cur;
}

 Tree *Tree::Find (QString name)     //���� �� ���� ������ �����
{
    Tree *cur = this;
    while (cur != nullptr)
         if (cur->N->Id != name)    //��� ������� ������� �� ��������� � ��������
            cur = cur->Parent;
         else break;
    return cur;
}

 Tree *Tree::Find (Node* n)
{
    return Find (this, n);
}


 Tree *Tree::FindOneLevel (Tree *From, Node* n)         //���� �� ����� ������
{
    Tree *cur = From;
    while (cur != nullptr)               //���� �� ����� �� ����� ��� �� ������ �������
        if (cur->Parent != nullptr)
            if (cur->Parent->Right == cur)      //������ �������
                break;
            else
                 if (n->Id != cur->N->Id || n->TypeObj != cur->N->TypeObj)    //������ ������� �� ��������� � ��������
                    cur = cur->Parent;
                 else break;
        else cur = cur->Parent;
    if (cur != nullptr)
        if (n->Id != cur->N->Id || n->TypeObj != cur->N->TypeObj)
            cur = nullptr;
    return cur;
}

 Tree *Tree::FindOneLevel (Node* n)
{
    return FindOneLevel (this, n);
}


 Tree *Tree::FindRightLeft (Tree *From, Node* n)        //���� � ������ ����
{
    Tree *cur = From->Right;
    while (cur != nullptr)
         if (n->Id != cur->N->Id || n->TypeObj != cur->N->TypeObj)    //������ ������� �� ��������� � ��������
            cur = cur->Left;
         else break;
    return cur;
}

 Tree *Tree::FindRightLeft (Node* n)
{
    return FindRightLeft (this, n);
}

 int Tree::semType(Lexem *lex)      //������������� ��� �������
{
     switch (lex->type)

     {
        case Tint:
         {
            return TypeInt;
         }
     case Tint8:
      {
         return TypeInt;
      }
     case Tint10:
      {
         return TypeInt;
      }
     case Tint16:
      {
         return TypeInt;
      }
     case Tchar:
      {
         return TypeChar;
      }
     case Tcchar:
      {
         return TypeChar;
      }
     case Tlong:
      {
         return TypeLong;
      }
     case Tmain:
      {
         return TypeMain;
      }
     case Tvoid:
      {
         return TypeVoid;
      }
     default:
         return TypeUnKnown;
     }
}

 bool Tree::semToTable (Node *n)  //�������� �� ������������, ��������� �������������� ������ � ������������� ����� � �������
 {
    if (n->TypeObj == Tfor)
    {
        Cur->addLeft(n);
        Cur = Cur->Left;
        return true;
    }
    if (!Cur->FindOneLevel(n))  //���� ��� ���
    {
        Cur->addLeft(n);
        Cur = Cur->Left;
        return true;
    }
    else return false;      //������������

 }

bool Tree::semFToTable (QString name)       //��������� ����� �������  � �������, �������� ������ ������ �������. ������������� ��������� �� ������ ���������
{
    Node *ptr = new Node (name, TypeFunc);  //�������
    if (!Cur->Find(name))  //���� ��� ���
    {
        Cur->addLeft(ptr);
        Cur = Cur->Left;
        F = Cur;                //��������� �� �������
        Cur->Right->Parent = Cur;
        Cur = Cur->Right;       //��������� �� ������ ������
        return true;
    }
    else return false;      //������������
}

void Tree::semRep (bool upLevel)     ///�������������� ��������� �� �������. ����������� �� ����� ������ � ����. �� ���� �������
{
    Tree *c = Cur;
    while (Cur != nullptr)  //���� �� ����� �� ����� �������
        if (Cur->Parent != nullptr)
             if (Cur->Parent->Right != Cur)    //�� ������ �������
                Cur = Cur->Parent;
             else break;
        else Cur = Cur->Parent;
    if (Cur != nullptr && upLevel)             //��������� �� ��������, ���� �������� �������
            Cur = Cur->Parent;
}
void Tree::semToRight()        //�������� ������� ������
{
    Cur->addRight(new Node ("", TypeEmpty));     //������ �������
    Cur = Cur->Right;
}

void Tree::semInc (Tree *func)       //��������� ����� ���������� �������
{
    func->N->ParamCount++;
}
void Tree::semInc ()       //��������� ����� ���������� �������
{
    F->N->ParamCount++;
}

void Tree::semSetParAmount (Tree* f, int n)
{
    f->N->ParamCount = n;
}

void Tree::semError(std::string err, Lexem* L)
{
    QTextStream cout (stdout);

    cout.setCodec(QTextCodec::codecForName("cp866"));

    std::cout<<"\n ������������� ������ � "<<L->str<<":"<<L->pos<<": "<<err;
}

int Tree::semTypeRes (int o1, int o2, Lexem* l)      //�������� ���������� ��������
{
    if (o1 == TypeInt && o2 == TypeInt)
        return TypeInt;
    if (o1 == TypeLong && o2 == TypeLong)
        return TypeLong;
    if (o1 == TypeLong && o2 == TypeInt)
        return TypeLong;
    if (o1 == TypeChar && o2 == TypeChar)
        return TypeChar;
    if (o1 == TypeChar && o2 == TypeInt)
        return TypeChar;
    if (o1 == TypeInt && o2 == TypeChar)
        return TypeInt;
    if (o1 != TypeUnKnown && o2 != TypeUnKnown)
        semError("�������������� �����", l);
    return TypeUnKnown;
}

int Tree::semTypeRes (int o1, int o2)      //�������� ���������� ��������
{
    if (o1 == TypeInt && o2 == TypeInt)
        return TypeInt;
    if (o1 == TypeInt && o2 == TypeChar)
        return TypeInt;

    if (o1 == TypeLong && (o2 == TypeLong || o2 == TypeInt || o2 == TypeChar))
        return TypeLong;

    if (o1 == TypeChar && o2 == TypeChar)
        return TypeChar;
    if (o1 == TypeChar && o2 == TypeInt)
        return TypeChar;

    return TypeUnKnown;
}

int Tree::semTypeResOnlyNum (int o1, int o2, Lexem* l)      //�������� ���������� ��������
{
    if (o1 == TypeInt && o2 == TypeInt)
        return TypeInt;
    if (o1 == TypeLong && o2 == TypeLong)
        return TypeLong;
    if (o1 == TypeLong && o2 == TypeInt)
        return TypeLong;
    if (o1 != TypeUnKnown && o2 != TypeUnKnown)
        semError("�������������� �����", l);
    return TypeUnKnown;
}

int Tree::semTypeResUn (int o1, Lexem* l)      //�������� ���������� ������� ��������
{
    if (o1 == TypeInt || o1 == TypeLong)
        return o1;
    if (o1 != TypeUnKnown)
        semError("�������������� �����", l);
    return TypeUnKnown;
}

void Tree::semTreeCopy (Tree* Fn)
{
    Tree *pos = this;   //���� ����� ��������� ���������
}
void Tree::semTreeDelete(Node *n)   //������� ��������� �� ��������� ����
{
    while (Cur != nullptr)
         if (n->Id != Cur->N->Id || n->TypeObj != Cur->N->TypeObj)    //������ ������� �� ��������� � ��������
         {
             if (Cur->Left != Q_NULLPTR)    //������� ���� ��������
                 delete Cur->Left;
             if (Cur->Right != Q_NULLPTR)
                 delete Cur->Right;
             Cur = Cur->Parent;             //��������� � ��������

         }
         else break;        //����� �� ������� �������

    if (Cur->Right != Q_NULLPTR)    //������� �������
        delete Cur->Right;
    if (Cur->Parent->Left == Cur)   //������ ������ ������� �������� ��������
        Cur->Parent->Left = Cur->Left;
    else if (Cur->Parent->Right == Cur)
        Cur->Parent->Right = Cur->Left;
}



#ifndef TREE_H
#define TREE_H
#include <node.h>
#include <iostream>
#include <QTextStream>
#include <QTextCodec>
#include <lexem.h>


class Tree
{
public:
    Node *N;
    Tree *Parent, *Left, *Right;
    static Tree * Cur;      //текущая вершина
    static Tree * F;        //текущая функция
    Tree();
    Tree(Node *n);
    Tree(Tree *p, Tree *l, Tree *r, Node *n);
    //~Tree();
    void addLeft (Node *n);             //вставить потомка
    void addRight (Node *n);
    Tree *Find (Tree *From, Node* n);   //поиск в таблице. Может вернуть null если нету
    Tree *Find (QString name);   //поиск в таблице. Может вернуть null если нету
    Tree *Find (Node* n);   //поиск в таблице. Может вернуть null если нету
    Tree *FindOneLevel (Tree *From, Node* n);   //поиск в таблице. Может вернуть null если нету
    Tree *FindOneLevel (Node* n);   //поиск в таблице. Может вернуть null если нету
    Tree *FindRightLeft (Tree *From, Node* n);   //поиск среди прямых потомков. Может вернуть null если нету
    Tree *FindRightLeft (Node* n);   //поиск среди прямых потомков. Может вернуть null если нету
    int semType (Lexem *lex);   //определение семантического типа
    ///проверка на дублирование на уровне, занесение в таблицу и перенос указателя
    bool semToTable (Node *n);  //проверка на дублирование, занесение идентификатора вместе с семантическим типом в таблицу
    ///занесение имени функции  в таблицу. Устанавливает указатель на правую созданную, F=эта функци
    bool semFToTable (QString name); //занесение имени функции вместе с типом возвращаемого значения в таблицу, создание пустой правой вершины. Возвращает указатель на созданную
    void semSetParAmount (Tree* f, int n);    //установить число параметров
    ///восстановление указателя на вершину. Поднимаемся по левым связям и поднимаемся на след уровень, если флаг
    void semRep (bool upLevel);
    ///копирует поддерево функции в позицию this
    void semTreeCopy (Tree* Fn);
    ///удаляем все поддерево функции, в которой сейчас находимся
    void semTreeDelete (Node* n);

    void semToRight ();     //уйти вправо
    void semInc (Tree *func);       //увеличить число параметров функции
    void semInc ();       //увеличить число параметров функции
    void semError (std::string err, Lexem* L);
    bool semSearch (Node* n);       //поиск узла
    bool semParAccord ();       ///проверка соотв. параметров
    static int semTypeRes (int o1, int o2);
    int semTypeRes (int o1, int o2, Lexem* l);      //проверка результата операции
    int semTypeResOnlyNum (int o1, int o2, Lexem* l);      //проверка результата для ТОЛЬКО числовых операций
    int semTypeResUn (int o1, Lexem* l);      //проверка результата для унарных операций
};

#endif // TREE_H

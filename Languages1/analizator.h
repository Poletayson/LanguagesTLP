#ifndef ANALIZATOR_H
#define ANALIZATOR_H

#include <iostream>
#include <QTextStream>
#include <qdebug.h>
#include "lexem.h"
#include "QList"
#include "scanner.h"
#include "locale.h"
#include "tree.h"

//namespace std:
class Analizator
{
public:

    Analizator();
    ///текущая вершина
    int cur;
    ///тип выражения. Устанавливается в A7()
    int tv;
    Scanner *scaner;
    QList <Lexem> *lex;
    bool right;
    Tree* T;                        //семантическое дерево
    bool InterpMainFlag;            //основной флаг интерпретации
    bool LocalInterpFlag;            //локальный флаг интерпретации
    ///Проверяем в Data(), нужно ли создавать узлы
    bool IsCycleTreeBuilding;       //еще строим дерево для цикла. Для объявлений будут создаваться узлы
    int deph;                       //глубина погружения в блок
    DataValue currenValue;      //последнее вычисленное значение
    std::string ErrorText;
    void toAnalize ();
    void S ();
    bool Data (bool isC);
    bool SOper (bool isC);
    bool Oper (bool isC);
    bool Cicle ();
    bool Prisv ();

    bool Function ();
    bool InitOfCicle ();
    bool EndOfIter ();
    DataValue* A1 ();   //возвращает значение выражения, либо Q_NULPTR
    DataValue* A2 ();
    DataValue* A3 ();
    DataValue* A4 ();
    DataValue* A5 ();
    DataValue* A6 ();
    DataValue* A7 ();
    ///выполнение операции
    DataValue* Operation (DataValue* a, DataValue* b, int operation);


};

#endif // ANALIZATOR_H

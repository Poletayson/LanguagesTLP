#ifndef NODE_H
#define NODE_H
#include <QString>

#define TypeVoid 0
#define TypeMain 1
#define TypeFunc 2
#define TypeInt 3
#define TypeLong 4
#define TypeChar 5

#define TypeUnKnown -1
#define TypeEmpty 10

enum DataType {TInt = 1, TLong, TChar};     //тип даных
union TDataValue        //Значение
{
    TDataValue(){DataInt=0;}
    int DataInt;
    long long DataLong;
    char DataChar;
};

class DataValue     //значение узла дерева
{
public:
    int Type;          //тип
    TDataValue Value;       //значение
    DataValue(){Type=TypeUnKnown;}
    DataValue(int t, TDataValue v);     //конструктор, включающий
    void setPosInProgram (int p);
    QString getValue ();            //получить значение в виде строки
};


class Node
{
public:
    DataValue Data;    //данные, соответствующие узлу
    QString Id;         //изображение объекта
    int TypeObj;          //тип
    int ParamCount;     //число параметров функции
    int PosInProgram;   //позиция тела функции в программе
    Node(QString I, int T, int c = 0, int pos = 0);
};
#endif // NODE_H

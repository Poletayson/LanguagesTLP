#include "node.h"

Node::Node(QString I, int T, int c, int pos)
{
    Id = I;
    TypeObj = T;
    ParamCount = c;
    PosInProgram = pos; //позиция в программе
}

DataValue::DataValue(int t, TDataValue v)
{
    Type = t;
    Value = v;
}

QString DataValue::getValue ()                  //получить значение в виде строки, в зависимости от типа
{
    switch (Type)
    {
        case TypeInt:
        {
            return QString::number(Value.DataInt);
            break;
        }
        case TypeLong:
        {
            return QString::number(Value.DataLong);
            break;
        }
        case TypeChar:
        {
            return "'"+QString::fromUtf8(&Value.DataChar)+"'";
            break;
        }
        default:
        {
            return "err";
            break;
        }
    }
}

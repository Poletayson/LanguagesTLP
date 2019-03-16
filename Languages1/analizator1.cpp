﻿#include "analizator.h"

Analizator::Analizator()
{
    scaner = new Scanner ("input.txt");
    lex = new QList <Lexem> ();
    cur = 0;
    right = true;
}

void Analizator::S ()
{
    int c;
    scaner->toScan(lex);         //получаем список лексем

    QString type;
    for (int i = 0; i < scaner->Errors.count(); i++)         //выводим лексические ошибки
    {
        std::cout<<'\n';
        std::cout<<"Lexical error in "<<scaner->Errors[i][0]<<":"<<scaner->Errors[i][1];
    }
    T = new Tree ();        //дерево
    T->Cur = T;
    InterpMainFlag = false;     //до main не интерпретируем
    deph = 0;         //глубина
    if(scaner->Errors.count() == 0)      //нет лексических ошибок
        while (cur < lex->count() - 1)      //пока не пройдем все лексемы
        {
            if((*lex)[cur].type != Tvoid)   //не void - значит данные
            {
               c = cur;
               if(!Data())
               {
                    cur = c;
                    right = false;
                    //ErrorText = ""
                    break;
               }
               if ((*lex)[cur].type != Tdt)    //РµСЃР»Рё РЅРµ С‚.Р·.
               {
                   right = false;
                   ErrorText = "Ожидался символ ;";
                   break;
               }
               cur++;
            }
            else        //void - значит описание функции
            {
                cur++;
                if ((*lex)[cur].type == Tmain)      //main
                {
                 ///
                    if (!T->semFToTable((*lex)[cur].image))     //маин не заносится
                    {   ///дублирование main
                        T->semError("Дублирование main", &(*lex)[cur]);
                    }
                 ///
                    cur++;
                    InterpMainFlag = true;  ////с этого момента начинаем интерпретацию
                    if ((*lex)[cur].type != Tls)
                    {
                        right = false;
                        ErrorText = "Ожидался символ (";
                        break;
                    }
                    cur++;
                    if ((*lex)[cur].type != Trs)
                    {
                        right = false;
                        ErrorText = "Ожидался символ )";
                        break;
                    }
                    cur++;
                }
                else    //Функция
                {
                    if ((*lex)[cur].type != Tid)          //РёРґРµРЅС‚РёС„РёРєР°С‚РѕСЂ
                    {
                        right = false;
                        ErrorText = "Ожидался идентификатор";
                        break;
                    }
                    else
            ///
                        if (!T->semFToTable((*lex)[cur].image))
                        {   ///дублирование функции
                            T->semError("Дублирование идентификатора", &(*lex)[cur]);
                        }
            ///
                    cur++;
                    if ((*lex)[cur].type != Tls)
                    {
                        right = false;
                        ErrorText = "Ожидался символ )";
                        break;
                    }
                    cur++;

                    if ((*lex)[cur].type != Trs)    //Не правая скобка
                        do
                        {
                        ///
                            int t1;
                            int curPtr = cur;
                            t1 = T->semType(&(*lex)[cur]);     //определим тип параметра
                        ///
                            if ((*lex)[cur].type != Tint && (*lex)[cur].type != Tchar)
                            {
                                if ((*lex)[cur].type != Tlong)
                                {
                                    right = false;
                                }
                                cur++;
                                if ((*lex)[cur].type != Tlong)
                                {
                                    right = false;
                                }
                                cur++;
                                if ((*lex)[cur].type != Tint)
                                {
                                    right = false;
                                }
                            }
                            cur++;
                            if ((*lex)[cur].type != Tid)
                            {
                                right = false;
                                ErrorText = "Ожидался идентификатор";
                                break;
                            }
                            else ///заносим в дерево + увеличим число параметров
                            {
                                Node *n = new Node ((*lex)[cur].image, t1);
                                if (!T->Cur->FindOneLevel(n))    //на этом уровне нет
                                {
                                    T->Cur->semToTable(n);   //помещаем в таблицу (дерево)
                                    T->semInc();        //увеличим число параметров
                                }
                                else
                                {
                                    T->semError("Дублирование имени параметра", &(*lex)[cur]);
                                }

                            }
                            ///
                            cur++;
                        } while ((*lex)[cur++].type == Tzap);
                    cur--;
                    if ((*lex)[cur].type != Trs)
                    {
                        right = false;
                        ErrorText = "Ожидался символ )";
                        break;
                    }
                    cur++;
                }
//далее должен идти сост. оператор
                if (!SOper ())    //проверяем на сост. оператор
                {
                    right = false;
                    break;
                }
                else    //был составной оператор. Восстановим уазатель
                {
///
                    T->semRep();
///
                }
            }
        }

    std::cout << "\n";
    std::cout.flush();


    if (right == true)
        if ((*lex)[cur].type == Tend)
            std::cout<<"\n Синтаксических ошибок не обнаружено\n";
        else;
    else
        std::cout<<"\n Обнаружена ошибка в "<<(*lex)[cur].str<<":"<<(*lex)[cur].pos<<": "<<ErrorText<<"\n";

    std::cout.flush();




    return;
}

bool Analizator::Data ()
{
    int t1;
    int curPtr = cur;
    t1 = T->semType(&(*lex)[cur]);              //определим тип данных

    //Tree *ptr = T->Find((*lex)[cur].image);     //ищем идентификатор
//    if (ptr == nullptr)     //ид не нашелся
//        T->semError("Необъявленный идентификатор", &(*lex)[cur]);
//    else
//        tv = ptr->N->TypeObj;       //тип объекта

    if ((*lex)[cur].type != Tint && (*lex)[cur].type != Tchar)
    {
        if ((*lex)[cur].type != Tlong)
        {
            right = false;
            ErrorText = "Ожидался тип";
            return false;
        }
        cur++;
        if ((*lex)[cur].type != Tlong)
        {
            right = false;
            ErrorText = "Ожидался long";
            return false;
        }
        cur++;
        if ((*lex)[cur].type != Tint)
        {
            right = false;
            ErrorText = "Ожидался int";
            return false;
        }

    }
    cur++;

    do
    {
        if ((*lex)[cur].type != Tid)
        {
            right = false;
            ErrorText = "Ожидался идентификатор";
            return false;
        }
///
//помещаем идентификатор в таблицу
        Node* nodePtr;
          nodePtr = new Node ((*lex)[cur].image, t1);             //новый узел для помещения в дерево
        if (t1 == TypeInt || t1 == TypeChar || t1 == TypeLong)
        {
            //nodePtr = new Node ((*lex)[cur].image, t1);             //новый узел для помещения в дерево

        }
        else
        {
            T->semError("Недопустимый тип", &(*lex)[cur]);
        }
///

        cur++;
        currenValue.Type = TypeUnKnown;         //тип будет неизвестен по умолчанию
        if ((*lex)[cur].type == Teq)    //инициализация
        {
            cur++;
            int type;
            DataValue* ptrVal = A1();
            if (ptrVal == Q_NULLPTR)
            {
                right = false;
                return false;
            }
            ///проверим типы при инициализации
            else
            {
                type = T->semTypeRes(t1, tv, &(*lex)[cur]);       //тип результата
            }

            nodePtr->Data = *ptrVal;    //устанавливаем значение для узла
            if ((InterpMainFlag || deph == 0) && type != TypeUnKnown)
            {
                std::cout<<"\n Инициализация: "<<nodePtr->Id.toStdString()<<" = "<<nodePtr->Data.getValue().toStdString();     //выведем значение новой переменной
                currenValue.Type = TypeUnKnown;     //намеренно портим тип - чтобы этим значением никто потом не воспользовался)
            }
            else {
                if (InterpMainFlag)
                    T->semError("Недопустимый тип", &(*lex)[cur]);
            }
            ///

        }
    ///

        T->Cur->semToTable(nodePtr);   //наконец, помещаем в таблицу (дерево)
    ///

    } while ((*lex)[cur++].type == Tzap);
    cur--;
    return true;
}

bool Analizator::SOper ()     //составной оператор
{
    if ((*lex)[cur].type != Tlf)
    {
        ErrorText = "Ожидался символ {";
        return false;
    }
    else
    {
        ///
        T->Cur->addLeft(new Node ("", TypeEmpty));//!!!!!!!!!!!!!!!
        T->Cur = T->Cur->Left;
        T->Cur = T->Cur->Right;    //уходим вправо //T->Cur->semToRight();
        ///
    }
    deph++;     //погружаемся в блок
    cur++;
    while ((*lex)[cur].type != Trf)
    {
        //c = cur;
        if ((*lex)[cur].type == Tint || (*lex)[cur].type == Tchar || (*lex)[cur].type == Tlong) //тип
        {
            if (!Data())         //значит данные
            {
                //cur = c;
                return false;
            }
       //действительно данные
            if ((*lex)[cur].type != Tdt)    //требуется т.з.
            {
                ErrorText = "Ожидался символ ;";
                return false;
            }
            cur++;

        }
        else    //значит оператор
        {
            if (!Oper())
                return false;
        }
    }
    if ((*lex)[cur].type != Trf)
    {
        ErrorText = "Ожидался символ }";
        return false;
    }
    cur++;

///
    deph--;     //поднимаемся из блока
    T->Cur->semRep();
    if (deph <= 0)      //вышли из блока верхнего уровня - снимаем флаг
        InterpMainFlag = false;
///
    return true;
}

bool Analizator::Oper ()     //оператор
{
    if ((*lex)[cur].type != Tdt)    //не пустой оператор
    {
        if ((*lex)[cur].type == Tfor)   //есть for
            if (!Cicle())         //не цикл
                return false;
            else;
        else
            if ((*lex)[cur].type == Tlf)   //есть {
                if (!SOper())       //Не сост. оператор
                    return false;
                else;
            else
                if ((*lex)[cur].type == Tid)   //идентификатор
                {
                    if ((*lex)[cur+1].type == Teq)    //после идет =
                        if (!Prisv()) //РЅРµ РїСЂРёСЃРІР°РёРІР°РЅРёРµ
                            return false;
                        else;
                    else if ((*lex)[cur+1].type == Tls) //после идет (
                            if (!Function()) //РЅРµ РІС‹СЂР°Р¶РµРЅРёРµ
                                return false;
                            else;
                    else
                    {
                        DataValue* ptrVal = A1();
                        if (ptrVal == Q_NULLPTR)//выражение?
                            return false;
                    }
                   if ((*lex)[cur].type != Tdt)
                            {
                                ErrorText = "Ожидался символ ;";
                                return false;
                            }
                    else cur++;
                }
        else
                {
                    ErrorText = "Недопустимый оператор";
                    return false;
                }

    }
    else cur++;     //пустой оператор
    return true;
}

bool Analizator::Cicle ()     //С†РёРєР»
{
    if ((*lex)[cur].type != Tfor)
    {
        ErrorText = "Ожидался for";
        return false;
    }
    ///
//    Node* nodePtr = new Node ((*lex)[cur].image, Tfor);
//        if (!T->semToTable(nodePtr))
//        {
//            T->semError("Ошибка добавления for", &(*lex)[cur]);
//        }
//        T = T->Cur->Right;
    ///
    cur++;
    if ((*lex)[cur].type != Tls)
    {
        ErrorText = "Ожидался символ (";
        return false;
    }
    cur++;
    if (!InitOfCicle())         //РЅРµ РёРЅРёС†РёР°Р»РёР·Р°С†РёСЏ
    {
        return false;
    }
    if ((*lex)[cur].type != Tdt)    //идет не т.з - ожидаем выражение
    {
        DataValue* ptrVal = A1();
        if (ptrVal == Q_NULLPTR) //не выражение
        {
            return false;
        }

    }
    if ((*lex)[cur].type != Tdt)
    {
        ErrorText = "Ожидался символ ;";
        return false;
    }
    cur++;

    if ((*lex)[cur].type != Trs)            //не закр. скобка - значит конец итерации
    {
        if ((*lex)[cur].type == Tid)
            if ((*lex)[cur+1].type == Teq)
                if (!Prisv())         //РЅРµ РІС‹СЂР°Р¶.
                {
                    //ErrorText = "Ожидалось присваивание";
                    return false;
                }
                else;
            else
            {
                DataValue* ptrVal = A1();
                if (ptrVal == Q_NULLPTR)
                {
                    return false;
                }
            }

    }
    if ((*lex)[cur].type != Trs)
    {
        ErrorText = "Ожидался символ )";
        return false;
    }
    cur++;
    if (!Oper())         //РЅРµ РѕРїРµСЂР°С‚РѕСЂ
    {
        //cur = c;        //РІРѕР·РІСЂР°С‰Р°РµРј СѓРєР°Р·Р°С‚РµР»СЊ
        return false;
    }
    return true;
}

bool Analizator::InitOfCicle ()     //инициализация цикла
{
    if ((*lex)[cur].type != Tdt)    //не т.з - не пусто
    {
        //c = cur;    //Р·Р°РїРѕРјРёРЅР°РµРј



        if ((*lex)[cur].type != Tid)        //идентиф. - значит данные
            if (!Data())         //РЅРµ РґР°РЅРЅС‹Рµ
                return false;
            else;
        else
            if (!Prisv())         //РЅРµ РґР°РЅРЅС‹Рµ
            {
                return false;
            }
        if ((*lex)[cur].type != Tdt)
        {
            ErrorText = "Ожидался символ ;";
            return false;
        }
    }
    cur++;
    return true;
}

bool Analizator::Prisv ()     //присваивание
{
    if ((*lex)[cur].type != Tid)
    {
        ErrorText = "Ожидался идентификатор";
        return false;
    }


/////
    Tree *ptr = T->Cur->Find((*lex)[cur].image);     //ищем идентификатор
    int t1;
    if (ptr == nullptr)     //ид не нашелся
        T->semError("Необъявленный идентификатор", &(*lex)[cur]);
    else
        t1 = ptr->N->TypeObj;       //тип объекта
////



    cur++;
    if ((*lex)[cur].type != Teq)
    {
        ErrorText = "Ожидался символ =";
        return false;
    }
    int curPtr = cur;
    cur++;

    DataValue* ptrVal = A1();
    if (ptrVal == Q_NULLPTR)    //после равно не выражение
    {
        return false;
    }

 ////
    if (ptr != nullptr)
    {
        int type = T->semTypeRes(t1, tv, &(*lex)[curPtr]);     //тип результата
        if (InterpMainFlag)      //тип определен и стоит флаг интерпретации
        {
            if (type != TypeUnKnown)
            {
                ptr->N->Data = *ptrVal;    //устанавливаем значение для узла
                std::cout<<"\n "<<ptr->N->Id.toStdString()<<" = "<<ptr->N->Data.getValue().toStdString();     //выведем значение новой переменной
                currenValue.Type = TypeUnKnown;     //намеренно портим тип - чтобы этим значением никто потом не воспользовался)
            }
            else {
                T->semError("Недопустимый тип", &(*lex)[cur]);
            }
        }
    }

////
    return true;
}


DataValue* Analizator::A1 ()     //A1
{
    //DataValue* result = new DataValue (TypeUnKnown, TDataValue());    //потом скопировать куда надо - в песледний A
    DataValue* result = A2();
    DataValue* ptr;
    if (result == Q_NULLPTR)
    {
        return Q_NULLPTR;
    }
    int t1 = tv;
    int curPtr;
    while ((*lex)[cur].type == Tvch)    //пока |
    {
        //int t = (*lex)[cur].type;
        curPtr = cur;
        cur++;
        ptr = A2();
        if (ptr == Q_NULLPTR)   //ошибка во втором операнде
        {
            return Q_NULLPTR;
        }
        else      //вычисляем  //ОСТАНОВИЛСЯ ЗДЕСЬ. Нужно интерпретировать вычисления
        {
            ////
                result = Operation(result, ptr, Tvch);  //значение
                if (result->Type == TypeUnKnown)
                    T->semError("Несоответствие типов", &(*lex)[curPtr]);
                //T->semTypeResOnlyNum(t1, tv, &(*lex)[curPtr]);
            ////
        }
    }
    return result;
}

DataValue* Analizator::A2 ()     //A2
{
    DataValue* result = A3();
    DataValue* ptr;
    if (result == Q_NULLPTR)
    {
        return Q_NULLPTR;
    }

    int t1 = tv;
    int curPtr;
    while ((*lex)[cur].type == Tamp)
    {
        curPtr = cur;
        cur++;

        ptr = A3();
        if (ptr == Q_NULLPTR)   //ошибка во втором операнде
        {
            return Q_NULLPTR;
        }
        else
        {
            ////
            result = Operation(result, ptr, Tamp);  //значение
            if (result->Type == TypeUnKnown)
                T->semError("Несоответствие типов", &(*lex)[curPtr]);
                //T->semTypeResOnlyNum(t1, tv, &(*lex)[curPtr]);

            ////
        }
    }
    return result;
}

DataValue* Analizator::A3 ()     //A3
{
    DataValue* result = A4();
    DataValue* ptr;
    if (result == Q_NULLPTR)
    {
        return Q_NULLPTR;
    }
    int t1 = tv;
    int curPtr;
    while ((*lex)[cur].type == Tlsd || (*lex)[cur].type == Trsd)
    {
        curPtr = cur;
        cur++;

        ptr = A4();
        if (ptr == Q_NULLPTR)   //ошибка во втором операнде
        {
            return Q_NULLPTR;
        }
        else
        {
            ////
            result = Operation(result, ptr, (*lex)[curPtr].type);  //значение
            if (result->Type == TypeUnKnown)
                T->semError("Несоответствие типов", &(*lex)[curPtr]);
                //T->semTypeResOnlyNum(t1, tv, &(*lex)[curPtr]);

            ////
        }
    }
    return result;
}
DataValue* Analizator::A4 ()     //A1
{
    DataValue* result = A5();
    DataValue* ptr;
    if (result == Q_NULLPTR)
    {
        return Q_NULLPTR;
    }
    int t1 = tv;
    int curPtr;
    while ((*lex)[cur].type == Tplus || (*lex)[cur].type == Tminus)
    {
        curPtr = cur;
        cur++;

        ptr = A5();
        if (ptr == Q_NULLPTR)   //ошибка во втором операнде
        {
            return Q_NULLPTR;
        }

        else
        {
            ////
            result = Operation(result, ptr, (*lex)[curPtr].type);  //значение
            if (result->Type == TypeUnKnown)
                T->semError("Несоответствие типов", &(*lex)[curPtr]);
                //T->semTypeResOnlyNum(t1, tv, &(*lex)[curPtr]);

            ////
        }
    }
    return result;
}
DataValue* Analizator::A5 ()     //A5
{
    DataValue* result = A6();
    DataValue* ptr;
    if (result == Q_NULLPTR)
    {
        return Q_NULLPTR;
    }
    int t1 = tv;
    int curPtr;
    while ((*lex)[cur].type == Tpers || (*lex)[cur].type == Tmul || (*lex)[cur].type == Tdel)
    {
        curPtr = cur;
        cur++;

        ptr = A6();
        if (ptr == Q_NULLPTR)   //ошибка во втором операнде
        {
            return Q_NULLPTR;
        }
        else
        {
            ////
            result = Operation(result, ptr, (*lex)[curPtr].type);  //значение
            if (result->Type == TypeUnKnown)
                T->semError("Несоответствие типов", &(*lex)[curPtr]);
                //T->semTypeResOnlyNum(t1, tv, &(*lex)[curPtr]);

            ////
        }
    }
    return result;
}

DataValue* Analizator::A6 ()     //A1
{
    bool f = false;
    int count = 0;
    while ((*lex)[cur].type == Ttil)
    {
        f = true;
        count++;
        cur++;
    }
    int curPtr = cur;
    DataValue* result = A7();
    if (result == Q_NULLPTR)
    {
        return Q_NULLPTR;
    }
    else    //сейчас проверим тип. Он уже установлен в A7
    {
    ////
        for (int i = 0; i < count; i++)
        {
            if (result->Type == TypeUnKnown)
            {
                T->semError("Неверный тип", &(*lex)[curPtr]);
                break;
            }
            switch (result->Type)
            {
                case TypeInt:
                {
                    result->Value.DataInt = ~(result->Value.DataInt);
                    break;
                }
                case TypeLong:
                {
                    result->Value.DataLong = ~(result->Value.DataLong);
                    break;
                }
                case TypeChar:
                {
                    result->Value.DataChar = ~(result->Value.DataChar);
                    break;
                }

            }
        }

    ////
    }

    return result;
}

DataValue* Analizator::A7 ()     //A7
{
    //09.03 дошли до сюда
    DataValue* result = new DataValue (TypeUnKnown, TDataValue());
    if ((*lex)[cur].type != Tint8 && (*lex)[cur].type != Tint10 && (*lex)[cur].type != Tint16
            && (*lex)[cur].type != Tcchar && (*lex)[cur].type != Tid)   //значит (
    {
        if ((*lex)[cur].type != Tls)
        {
            ErrorText = "Ожидалось выражение";
            return result;
        }
        cur++;
        result = A1();
        if (result == Q_NULLPTR)
            return result;
        if ((*lex)[cur].type != Trs)
        {
            ErrorText = "Ожидался символ )";
            result->Type = TypeUnKnown;
            return result;
        }
        cur++;
    }
    else    //не (A1)
    {
        ////

        if ((*lex)[cur].type != Tid)    //не id
        {
            tv = T->semType(&(*lex)[cur]);   //определим тип
            result->Type = tv;
            switch (tv)
            {
                case TypeChar: //символ
                {
                    result->Value.DataChar =  ((*lex)[cur].image).toUtf8().data()[1];
                    break;
                }
            case TypeInt: //символ
            {
                result->Value.DataInt = ((*lex)[cur].image).toInt(Q_NULLPTR, 0);   //0 - преобразование по стандартам C
                break;
            }
            case TypeLong: //символ
            {
                result->Value.DataLong = ((*lex)[cur].image).toInt(Q_NULLPTR, 0);   //0 - преобразование по стандартам C
                break;
            }
//                default:    //число
//                {
//                    if(tv == Tint8 || tv == Tint10 || tv == Tint16)
//                    {
//                        result->Value.DataInt =  ((*lex)[cur].image).toInt(Q_NULLPTR, 0);   //0 - преобразование по стандартам C
//                        break;
//                    }
//                }
            }
        }
        else        //id
        {
            tv = TypeUnKnown;
            Tree *ptr = T->Cur->Find((*lex)[cur].image);     //ищем идентификатор
            //int t1;
            if (ptr == Q_NULLPTR)     //ид не нашелся
                T->semError("Необъявленный идентификатор", &(*lex)[cur]);
            else    //с идентификатором все в порядке
            {
                tv = ptr->N->TypeObj;       //тип объекта
                result->Type = tv;
                switch (tv)
                {
                    case TypeChar: //символ
                    {
                        result->Value.DataChar = ptr->N->Data.Value.DataChar;
                        break;
                    }
                    case TypeInt:    //int
                    {
                        result->Value.DataInt = ptr->N->Data.Value.DataInt;
                        break;
                    }
                    case TypeLong:    //long
                    {
                        result->Value.DataLong = ptr->N->Data.Value.DataLong;
                        break;
                    }
                }
            }
        }
///
        cur++;
    }
    return result;
}

bool Analizator::Function ()     //РІС‹Р·РѕРІ С„СѓРЅРєС†РёРё
{
    Tree *ptr;
    int t1;
    if ((*lex)[cur].type != Tid)
    {
        ErrorText = "Ожидался идентификатор";
        return false;
    }
    else
    {
    /////
        ptr = T->Cur->Find(new Node ((*lex)[cur].image, TypeFunc));     //ищем функцию

        if (ptr == nullptr)     //ид не нашелся
            T->semError("Необъявленная функция", &(*lex)[cur]);
//        else
//            t1 = ptr->N->TypeObj;       //тип объекта
    ////
    }
    cur++;
    if ((*lex)[cur].type != Tls)
    {
        ErrorText = "Ожидался символ (";
        return false;
    }
    cur++;

    if ((*lex)[cur].type != Trs)        //РїР°СЂР°РјРµС‚СЂС‹ Р±СѓРґСѓС‚
    {
        DataValue* ptrVal = A1();
        if (ptrVal == Q_NULLPTR)    //не выражение
        {
            ErrorText = "Ожидалось выражение";
            return false;
        }
    }

    while ((*lex)[cur].type != Trs)
    {
        if ((*lex)[cur].type == Tzap) //Р·Р°РїСЏС‚Р°СЏ
        {
            cur++;
        }
        else
        {
            ErrorText = "Ожидался символ ,";
            return false;
        }

        DataValue* ptrVal = A1();
        if (ptrVal == Q_NULLPTR)
        {
            ErrorText = "Ожидалось выражение";
            return false;
        }

    }
    if ((*lex)[cur].type != Trs)
    {
        ErrorText = "Ожидался символ )";
        return false;
    }
    cur++;
    return true;
}

DataValue* Analizator::Operation (DataValue* a, DataValue* b, int operation)    //выполнение операции
{
    DataValue* result = new DataValue (TypeUnKnown, TDataValue());
    int type = Tree::semTypeRes(a->Type, b->Type);  //тип результата
    //int value = 0;
    TDataValue value;
    switch (operation)      //выполняем операцию в зависимоти от параметра
    {
        case Tvch:  //|
        {
            switch (type)
            {
                case TypeInt:
                {
                    value.DataInt = a->Value.DataInt | b->Value.DataInt;
                    break;
                }
                case TypeLong:
                {
                    value.DataLong = a->Value.DataLong | b->Value.DataLong;
                    break;
                }
                case TypeChar:
                {
                    value.DataChar = a->Value.DataChar | b->Value.DataChar;
                    break;
                }
            }
            break;
        }
        case Tamp:  //&
        {
            switch (type)
            {
                case TypeInt:
                {
                    value.DataInt = a->Value.DataInt & b->Value.DataInt;
                    break;
                }
                case TypeLong:
                {
                    value.DataLong = a->Value.DataLong & b->Value.DataLong;
                    break;
                }
                case TypeChar:
                {
                    value.DataChar = a->Value.DataChar & b->Value.DataChar;
                    break;
                }
            }
            break;
        }
        case Tlsd:  //<<
        {
            switch (type)
            {
                case TypeInt:
                {
                    value.DataInt = a->Value.DataInt << b->Value.DataInt;
                    break;
                }
                case TypeLong:
                {
                    value.DataLong = a->Value.DataLong << b->Value.DataLong;
                    break;
                }
                case TypeChar:
                {
                    value.DataChar = a->Value.DataChar << b->Value.DataChar;
                    break;
                }
            }
            break;
        }
        case Trsd:  //>>
        {
            switch (type)
            {
                case TypeInt:
                {
                    value.DataInt = a->Value.DataInt >> b->Value.DataInt;
                    break;
                }
                case TypeLong:
                {
                    value.DataLong = a->Value.DataLong >> b->Value.DataLong;
                    break;
                }
                case TypeChar:
                {
                    value.DataChar = a->Value.DataChar >> b->Value.DataChar;
                    break;
                }
            }
            break;
        }
        case Tminus:  //-
        {
            switch (type)
            {
                case TypeInt:
                {
                    value.DataInt = a->Value.DataInt - b->Value.DataInt;
                    break;
                }
                case TypeLong:
                {
                    value.DataLong = a->Value.DataLong - b->Value.DataLong;
                    break;
                }
                case TypeChar:
                {
                    value.DataChar = a->Value.DataChar - b->Value.DataChar;
                    break;
                }
            }
            break;
        }
        case Tplus:  //+
        {
            switch (type)
            {
                case TypeInt:
                {
                    value.DataInt = a->Value.DataInt + b->Value.DataInt;
                    break;
                }
                case TypeLong:
                {
                    value.DataLong = a->Value.DataLong + b->Value.DataLong;
                    break;
                }
                case TypeChar:
                {
                    value.DataChar = a->Value.DataChar + b->Value.DataChar;
                    break;
                }
            }
            break;
        }
        case Tdel:  // /
        {
            switch (type)
            {
                case TypeInt:
                {
                    value.DataInt = a->Value.DataInt / b->Value.DataInt;
                    break;
                }
                case TypeLong:
                {
                    value.DataLong = a->Value.DataLong / b->Value.DataLong;
                    break;
                }

            }
            break;
        }
        case Tmul:  //*
        {
            switch (type)
            {
                case TypeInt:
                {
                    value.DataInt = a->Value.DataInt * b->Value.DataInt;
                    break;
                }
                case TypeLong:
                {
                    value.DataLong = a->Value.DataLong * b->Value.DataLong;
                    break;
                }

            }
            break;
        }
        case Tpers:  //%
        {
            switch (type)
            {
                case TypeInt:
                {
                    value.DataInt = a->Value.DataInt % b->Value.DataInt;
                    break;
                }
                case TypeLong:
                {
                    value.DataLong = a->Value.DataLong % b->Value.DataLong;
                    break;
                }

            }
            break;
        }
        default:
        {
            type = TypeUnKnown;
            break;
        }
    }

    result->Type = type;
    result->Value = value;
    return result;
}

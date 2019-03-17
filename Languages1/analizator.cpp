#include "analizator.h"

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
    scaner->toScan(lex);         //�������� ������ ������

    QString type;
    for (int i = 0; i < scaner->Errors.count(); i++)         //������� ����������� ������
    {
        std::cout<<'\n';
        std::cout<<"Lexical error in "<<scaner->Errors[i][0]<<":"<<scaner->Errors[i][1];
    }
    T = new Tree ();        //������
    T->Cur = T;
    InterpMainFlag = false;     //�� main �� ��������������

    deph = 0;         //�������
    if(scaner->Errors.count() == 0)      //��� ����������� ������
        while (cur < lex->count() - 1)      //���� �� ������� ��� �������
        {
            if((*lex)[cur].type != Tvoid)   //�� void - ������ ������
            {
               c = cur;
               if(!Data(false))
               {
                    cur = c;
                    right = false;
                    //ErrorText = ""
                    break;
               }
               if ((*lex)[cur].type != Tdt)    //если не т.з.
               {
                   right = false;
                   ErrorText = "�������� ������ ;";
                   break;
               }
               cur++;
            }
            else        //void - ������ �������� �������
            {
                cur++;
                bool isF = false;       //����� �������
                if ((*lex)[cur].type == Tmain)      //main
                {
                 ///
                    if (!T->semFToTable((*lex)[cur].image))     //���� �� ���������
                    {   ///������������ main
                        T->semError("������������ main", &(*lex)[cur]);
                    }
                    std::cout<<"\n\n---���� � main---";
                 ///
                    cur++;
                    InterpMainFlag = true;  ////� ����� ������� �������� �������������
                    if ((*lex)[cur].type != Tls)
                    {
                        right = false;
                        ErrorText = "�������� ������ (";
                        break;
                    }
                    cur++;
                    if ((*lex)[cur].type != Trs)
                    {
                        right = false;
                        ErrorText = "�������� ������ )";
                        break;
                    }
                    cur++;
                }
                else    //�������
                {
                    isF = true;
                    if ((*lex)[cur].type != Tid)          //идентификатор
                    {
                        right = false;
                        ErrorText = "�������� �������������";
                        break;
                    }
                    else
            ///
                        if (!T->semFToTable((*lex)[cur].image))
                        {   ///������������ �������
                            T->semError("������������ ��������������", &(*lex)[cur]);
                        }
            ///
                    cur++;
                    if ((*lex)[cur].type != Tls)
                    {
                        right = false;
                        ErrorText = "�������� ������ (";
                        break;
                    }
                    cur++;

                    if ((*lex)[cur].type != Trs)    //�� ������ ������
                        do
                        {
                        ///
                            int t1;
                            int curPtr = cur;
                            t1 = T->semType(&(*lex)[cur]);     //��������� ��� ���������
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
                                ErrorText = "�������� �������������";
                                break;
                            }
                            else ///������� � ������ + �������� ����� ����������
                            {
                                Node *n = new Node ((*lex)[cur].image, t1);
                                if (!T->Cur->FindOneLevel(n))    //�� ���� ������ ���
                                {
                                    T->Cur->semToTable(n);   //�������� � ������� (������)
                                    T->semInc();        //�������� ����� ����������
                                }
                                else
                                {
                                    T->semError("������������ ����� ���������", &(*lex)[cur]);
                                }

                            }
                            ///
                            cur++;
                        } while ((*lex)[cur++].type == Tzap);
                    cur--;
                    if ((*lex)[cur].type != Trs)
                    {
                        right = false;
                        ErrorText = "�������� ������ )";
                        break;
                    }
                    cur++;
                    T->F->N->PosInProgram = cur;    //�������� ��� ���� ������� ������� �����
                }
//����� ������ ���� ����. ��������

                if (!SOper (false))    //��������� �� ����. ��������
                {
                    right = false;
                    break;
                }
                else    //��� ��������� ��������. ����������� �������� � ������
                {
///
                    //T->Cur = T->F;
                    T->semRep(true);
                    if (!isF)
                        std::cout<<"\n\n---����� �� main---";
///
                }
            }
        }

    std::cout << "\n";
    std::cout.flush();


    if (right == true)
        if ((*lex)[cur].type == Tend)
            std::cout<<"\n �������������� ������ �� ����������\n";
        else;
    else
        std::cout<<"\n ���������� ������ � "<<(*lex)[cur].str<<":"<<(*lex)[cur].pos<<": "<<ErrorText<<"\n";

    std::cout.flush();




    return;
}

bool Analizator::Data (bool isC)
{
    int t1;
    int curPtr = cur;
    t1 = T->semType(&(*lex)[cur]);              //��������� ��� ������

    if ((*lex)[cur].type != Tint && (*lex)[cur].type != Tchar)
    {
        if ((*lex)[cur].type != Tlong)
        {
            right = false;
            ErrorText = "�������� ���";
            return false;
        }
        cur++;
        if ((*lex)[cur].type != Tlong)
        {
            right = false;
            ErrorText = "�������� long";
            return false;
        }
        cur++;
        if ((*lex)[cur].type != Tint)
        {
            right = false;
            ErrorText = "�������� int";
            return false;
        }

    }
    cur++;

    do
    {
        if ((*lex)[cur].type != Tid)
        {
            right = false;
            ErrorText = "�������� �������������";
            return false;
        }
///
//�������� ������������� � �������
        Node* nodePtr;
        if (!isC || IsCycleTreeBuilding)   //�� �� � �����, ���� ����������� �� �����, ���� ��� ������ ������
        {
            nodePtr = new Node ((*lex)[cur].image, t1);             //����� ���� ��� ��������� � ������
            if (t1 == TypeInt || t1 == TypeChar || t1 == TypeLong)
            {
                //nodePtr = new Node ((*lex)[cur].image, t1);             //����� ���� ��� ��������� � ������

            }
            else
            {
                T->semError("������������ ���", &(*lex)[cur]);
            }
        }
        else        //������� ���������� � ��� ������� ������
        {
            Tree *ptr = T->Cur->Find((*lex)[cur].image);     //���� �������������
            if (ptr == nullptr)     //�� �� �������
                T->semError("������������� �������������", &(*lex)[cur]);
            else
                nodePtr = ptr->N;   //����� �������������. ����� �������� � ��� ���������
        }
///

        cur++;
        currenValue.Type = TypeUnKnown;         //��� ����� ���������� �� ���������
        if ((*lex)[cur].type == Teq)    //�������������
        {
            cur++;
            int type;
            DataValue* ptrVal = A1();
            if (ptrVal == Q_NULLPTR)
            {
                right = false;
                return false;
            }
            ///�������� ���� ��� �������������
            else
            {
                type = T->semTypeRes(t1, tv, &(*lex)[cur]);       //��� ����������
            }

            nodePtr->Data = *ptrVal;    //������������� �������� ��� ����
            if ((InterpMainFlag || deph == 0) && type != TypeUnKnown)
            {
                std::cout<<"\n �������������: "<<nodePtr->Id.toStdString()<<" = "<<nodePtr->Data.getValue().toStdString();     //������� �������� ����� ����������
                currenValue.Type = TypeUnKnown;     //��������� ������ ��� - ����� ���� ��������� ����� ����� �� ��������������)
            }
            else {
                if (InterpMainFlag)
                    T->semError("������������ ���", &(*lex)[cur]);
            }
            ///

        }
    ///
    if (!isC || IsCycleTreeBuilding)   //�� � �����
        T->Cur->semToTable(nodePtr);   //�������, �������� � ������� (������)
//    else  //������ �� ������, �������� �����
//        T->Cur = T->Left;      //� �����, ��������� �� ����� ����
    ///

    } while ((*lex)[cur++].type == Tzap);
    cur--;
    return true;
}

bool Analizator::SOper (bool isC)     //��������� ��������
{
    if ((*lex)[cur].type != Tlf)
    {
        ErrorText = "�������� ������ {";
        return false;
    }
    else
    {
        ///
        if (!isC)    //���� �� �� � �����
        {
            T->Cur->addLeft(new Node ("", TypeEmpty));//��������� ������ ���� ����� � ������ ������ �� ����
            T->Cur = T->Cur->Left;
            T->Cur = T->Cur->Right;    //������ ������ //T->Cur->semToRight();
        }
//        else  //� �����, ���� �� ������� ����� ����� ������
//        {
//            //if (T->Cur->N->TypeObj != TypeEmpty)    //
//            //{
//            //    T->Cur->addLeft(new Node ("", TypeEmpty));//��������� ������ ���� �����
//            //    T->Cur = T->Cur->Left;
//            //}
//            T->Cur = T->Cur->Right;    //������ ������ ������
//        }
        ///
    }
    deph++;     //����������� � ����
    cur++;
    while ((*lex)[cur].type != Trf) //���� �� ������ �. ������
    {
        //c = cur;
        if ((*lex)[cur].type == Tint || (*lex)[cur].type == Tchar || (*lex)[cur].type == Tlong) //���
        {
            if (!Data(isC))         //������ ������
            {
                //cur = c;
                return false;
            }
       //������������� ������
            if ((*lex)[cur].type != Tdt)    //��������� �.�.
            {
                ErrorText = "�������� ������ ;";
                return false;
            }
            cur++;

        }
        else    //������ ��������
        {
            if (!Oper(false))
                return false;
        }
    }
    if ((*lex)[cur].type != Trf)
    {
        ErrorText = "�������� ������ }";
        return false;
    }
    cur++;

///
    deph--;     //����������� �� �����
    if (!isC)   //���� �� ����������� ����� - ����������� ���������
        T->Cur->semRep(true);
    if (deph <= 0)      //����� �� ����� �������� ������ - ������� ����
        InterpMainFlag = false;
///
    return true;
}

bool Analizator::Oper (bool isC)     //��������
{
    if ((*lex)[cur].type != Tdt)    //�� ������ ��������
    {
        if ((*lex)[cur].type == Tfor)   //���� for
            if (!Cicle())         //�� ����
                return false;
            else;
        else
            if ((*lex)[cur].type == Tlf)   //���� {
                if (!SOper(isC))       //�� ����. ��������
                    return false;
                else;
            else
                if ((*lex)[cur].type == Tid)   //�������������
                {
                    if ((*lex)[cur+1].type == Teq)    //����� ���� =
                        if (!Prisv()) //не присваивание
                            return false;
                        else;
                    else if ((*lex)[cur+1].type == Tls) //����� ���� (
                            if (!Function())
                                return false;
                            else if (InterpMainFlag)   //����� �������. ����� �������� �����. �������� ������� ��������
                            {
                                if (!semErr)
                                {
                                    cur = T->F->N->PosInProgram;    //��������� � �����
                                    if (!SOper (false))    //��������� �� ����. ��������
                                    {
                                        right = false;
                                        return false;
                                    }
                                    else    //��� ��������� ��������. ������ ��������� �� ������ ������� � ������ ���������
                                    {
                                        ///
                                        T->Cur->semTreeDelete(new Node (T->F->N->Id, TypeEmpty));
                                        cur = RetAddress.takeLast();//addrReturn;   //������ ���������
                                        ///
                                    }
                                    if (InterpMainFlag)
                                    {
                                        std::cout<<"\n---������� ����������";
                                    }
                                }
                                else
                                {
                                    if (stage == 1) //������ ���� ���������
                                    {
                                        T->Cur->semTreeDelete(new Node (T->F->N->Id, TypeEmpty));
                                    }
                                }
                            }
                    else
                    {
                        DataValue* ptrVal = A1();
                        if (ptrVal == Q_NULLPTR)//���������?
                            return false;
                    }
                   if ((*lex)[cur].type != Tdt)
                            {
                                ErrorText = "�������� ������ ;";
                                return false;
                            }
                    else cur++;
                }
        else
                {
                    ErrorText = "������������ ��������";
                    return false;
                }

    }
    else cur++;     //������ ��������
    return true;
}

bool Analizator::Cicle ()     //����
{
    int usl, IterEnd, code;
    IsCycleTreeBuilding = true;            //���� ������ ������
    if ((*lex)[cur].type != Tfor)
    {
        ErrorText = "�������� for";
        return false;
    }
    ///
    if (InterpMainFlag) //��������� ���� for � ������ ������
    {
        std::cout<<"\n ���� for";     //
        Node* nodePtr = new Node ((*lex)[cur].image, Tfor);
        if (!T->semToTable(nodePtr))
        {
            T->semError("������ ���������� for", &(*lex)[cur]);
        }
        T->Cur = T->Cur->Right;
    }
    ///


    cur++;
    if ((*lex)[cur].type != Tls)
    {
        ErrorText = "�������� ������ (";
        return false;
    }
    cur++;
    if (!InitOfCicle())         //не инициализация
    {
        return false;
    }
    bool localFlag = InterpMainFlag;
    InterpMainFlag = false; //��������� �������������
    usl = cur;  ///
    if ((*lex)[cur].type != Tdt)    //���� �� �.� - ������� ���������
    {
        DataValue* ptrVal = A1();
        if (ptrVal == Q_NULLPTR) //�� ���������
        {
            return false;
        }

    }
    if ((*lex)[cur].type != Tdt)
    {
        ErrorText = "�������� ������ ;";
        return false;
    }
    cur++;

    if ((*lex)[cur].type != Trs)            //�� ����. ������ - ������ ����� ��������
    {
        IterEnd = cur;  ///����� ��������
        if ((*lex)[cur].type == Tid)
            if ((*lex)[cur+1].type == Teq)
                if (!Prisv())         //�� ������������.
                {
                    //ErrorText = "��������� ������������";
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
        ErrorText = "�������� ������ )";
        return false;
    }
    cur++;
    code = cur;

    if (!Oper(true))         //�� ��������
    {
        //cur = c;        //возвращаем указатель
        return false;
    }

    IsCycleTreeBuilding = false;            //������ ������ ���� ���������
    //��� ���������, ��������������
///������������� �����
///
    bool ptrFlag;
    InterpMainFlag = localFlag; //���������� ���� �������������
    int LocalCur = cur;
    if (InterpMainFlag) //�������������� ������ ���� ����� ���������� ����
    {
        do
        {
            cur = usl;

            if ((*lex)[cur].type != Tdt)    //���� �� �.� - ������� ���������
            {
                DataValue* ptrVal = A1();
                if (ptrVal == Q_NULLPTR) //�� ���������
                {
                    return false;
                }
                if (ptrVal->Value.DataInt == 0)//����� ����� ������ ���� == 0
                    localFlag = false;
                else
                    localFlag = true;
                std::cout<<"\n �������� ��������� ���������: " << ptrVal->Value.DataInt;     //
            }
            else
                localFlag = true;   //�� ��������� - true

            if (!localFlag)
            {
                std::cout<<"\n ������� �� �����";
                break;
            }

            cur = code;    //���� ��������� ����� ��������




            if (localFlag && InterpMainFlag)        //��������������, ���� ������� ����
                if (!Oper(true))         //�� ��������
                {
                    //cur = c;        //возвращаем указатель
                    return false;
                }

            cur = IterEnd; //������ �������� ����� ��������


            if ((*lex)[cur].type != Trs)            //�� ����. ������ - ������ ����� ��������
            {
                if ((*lex)[cur].type == Tid)
                    if ((*lex)[cur+1].type == Teq)
                        if (!Prisv())         //не выраж.
                        {
                            //ErrorText = "��������� ������������";
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
        } while (localFlag && InterpMainFlag);
    }

///������������� �����
    //IsCycle = false;
    cur = LocalCur;
    if (InterpMainFlag)
        T->Cur->semRep(true);       //������ �� ����� �����
    return true;
}

bool Analizator::InitOfCicle ()     //������������� �����
{
    if ((*lex)[cur].type != Tdt)    //�� �.� - �� �����
    {
        if ((*lex)[cur].type != Tid)        //�������. - ������ ������
            if (!Data(false))         //не данные
                return false;
            else;
        else
            if (!Prisv())         //не данные
            {
                return false;
            }
        if ((*lex)[cur].type != Tdt)
        {
            ErrorText = "�������� ������ ;";
            return false;
        }
    }
    cur++;
    return true;
}

bool Analizator::Prisv ()     //������������
{
    if ((*lex)[cur].type != Tid)
    {
        ErrorText = "�������� �������������";
        return false;
    }


/////
    Tree *ptr = T->Cur->Find((*lex)[cur].image);     //���� �������������
    int t1;
    if (ptr == nullptr)     //�� �� �������
        T->semError("������������� �������������", &(*lex)[cur]);
    else
        t1 = ptr->N->TypeObj;       //��� �������
////



    cur++;
    if ((*lex)[cur].type != Teq)
    {
        ErrorText = "�������� ������ =";
        return false;
    }
    int curPtr = cur;
    cur++;

    DataValue* ptrVal = A1();
    if (ptrVal == Q_NULLPTR)    //����� ����� �� ���������
    {
        return false;
    }

 ////
    if (ptr != nullptr)
    {
        int type = T->semTypeRes(t1, tv, &(*lex)[curPtr]);     //��� ����������
        if (InterpMainFlag)      //��� ��������� � ����� ���� �������������
        {
            if (type != TypeUnKnown)
            {
                ptrVal->Type = type;
                ptr->N->Data = *ptrVal;    //������������� �������� ��� ����
                std::cout<<"\n "<<ptr->N->Id.toStdString()<<" = "<<ptr->N->Data.getValue().toStdString();     //������� �������� ����� ����������
                currenValue.Type = TypeUnKnown;     //��������� ������ ��� - ����� ���� ��������� ����� ����� �� ��������������)
            }
            else {
                T->semError("������������ ���", &(*lex)[cur]);
            }
        }
    }

////
    return true;
}


DataValue* Analizator::A1 ()     //A1
{
    //DataValue* result = new DataValue (TypeUnKnown, TDataValue());    //����� ����������� ���� ���� - � ��������� A
    DataValue* result = A2();
    DataValue* ptr;
    if (result == Q_NULLPTR)
    {
        return Q_NULLPTR;
    }
    int t1 = tv;
    int curPtr;
    while ((*lex)[cur].type == Tvch)    //���� |
    {
        //int t = (*lex)[cur].type;
        curPtr = cur;
        cur++;
        ptr = A2();
        if (ptr == Q_NULLPTR)   //������ �� ������ ��������
        {
            return Q_NULLPTR;
        }
        else      //���������  //����������� �����. ����� ���������������� ����������
        {
            ////
                result = Operation(result, ptr, Tvch);  //��������
                if (result->Type == TypeUnKnown)
                    T->semError("�������������� �����", &(*lex)[curPtr]);
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
        if (ptr == Q_NULLPTR)   //������ �� ������ ��������
        {
            return Q_NULLPTR;
        }
        else
        {
            ////
            result = Operation(result, ptr, Tamp);  //��������
            if (result->Type == TypeUnKnown)
                T->semError("�������������� �����", &(*lex)[curPtr]);
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
        if (ptr == Q_NULLPTR)   //������ �� ������ ��������
        {
            return Q_NULLPTR;
        }
        else
        {
            ////
            result = Operation(result, ptr, (*lex)[curPtr].type);  //��������
            if (result->Type == TypeUnKnown)
                T->semError("�������������� �����", &(*lex)[curPtr]);
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
        if (ptr == Q_NULLPTR)   //������ �� ������ ��������
        {
            return Q_NULLPTR;
        }

        else
        {
            ////
            result = Operation(result, ptr, (*lex)[curPtr].type);  //��������
            if (result->Type == TypeUnKnown)
                T->semError("�������������� �����", &(*lex)[curPtr]);
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
        if (ptr == Q_NULLPTR)   //������ �� ������ ��������
        {
            return Q_NULLPTR;
        }
        else
        {
            ////
            result = Operation(result, ptr, (*lex)[curPtr].type);  //��������
            if (result->Type == TypeUnKnown)
                T->semError("�������������� �����", &(*lex)[curPtr]);
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
    else    //������ �������� ���. �� ��� ���������� � A7
    {
    ////
        for (int i = 0; i < count; i++)
        {
            if (result->Type == TypeUnKnown)
            {
                T->semError("�������� ���", &(*lex)[curPtr]);
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
    //09.03 ����� �� ����
    DataValue* result = new DataValue (TypeUnKnown, TDataValue());
    if ((*lex)[cur].type != Tint8 && (*lex)[cur].type != Tint10 && (*lex)[cur].type != Tint16
            && (*lex)[cur].type != Tcchar && (*lex)[cur].type != Tid)   //������ (
    {
        if ((*lex)[cur].type != Tls)
        {
            ErrorText = "��������� ���������";
            return result;
        }
        cur++;
        result = A1();
        if (result == Q_NULLPTR)
            return result;
        if ((*lex)[cur].type != Trs)
        {
            ErrorText = "�������� ������ )";
            result->Type = TypeUnKnown;
            return result;
        }
        cur++;
    }
    else    //�� (A1)
    {
        ////

        if ((*lex)[cur].type != Tid)    //�� id
        {
            tv = T->semType(&(*lex)[cur]);   //��������� ���
            result->Type = tv;
            switch (tv)
            {
                case TypeChar: //������
                {
                    result->Value.DataChar =  ((*lex)[cur].image).toUtf8().data()[1];
                    break;
                }
            case TypeInt: //������
            {
                result->Value.DataInt = ((*lex)[cur].image).toInt(Q_NULLPTR, 0);   //0 - �������������� �� ���������� C
                break;
            }
            case TypeLong: //������
            {
                result->Value.DataLong = ((*lex)[cur].image).toInt(Q_NULLPTR, 0);   //0 - �������������� �� ���������� C
                break;
            }
//                default:    //�����
//                {
//                    if(tv == Tint8 || tv == Tint10 || tv == Tint16)
//                    {
//                        result->Value.DataInt =  ((*lex)[cur].image).toInt(Q_NULLPTR, 0);   //0 - �������������� �� ���������� C
//                        break;
//                    }
//                }
            }
        }
        else        //id
        {
            tv = TypeUnKnown;
            Tree *ptr = T->Cur->Find((*lex)[cur].image);     //���� �������������
            //int t1;
            if (ptr == Q_NULLPTR)     //�� �� �������
                T->semError("������������� �������������", &(*lex)[cur]);
            else    //� ��������������� ��� � �������
            {
                tv = ptr->N->TypeObj;       //��� �������
                result->Type = tv;
                switch (tv)
                {
                    case TypeChar: //������
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

bool Analizator::Function ()     //����� �������
{
    Tree *ptr;
    Node *nodePtr;
    int t1;
    int ParCount;
    stage = 0;      //������� ������
    semErr = false; //���� ������ ���
    if ((*lex)[cur].type != Tid)
    {
        ErrorText = "�������� �������������";
        return false;
    }
    else
    {
    /////
        ptr = T->Cur->Find(new Node ((*lex)[cur].image, TypeFunc));     //���� �������

        if (ptr == nullptr)     //�� �� �������
        {
            T->semError("������������� �������", &(*lex)[cur]);
            semErr = true;
        }
    ////

    }

    if (ptr != Q_NULLPTR)       //������� ���� �������
        ParCount = ptr->N->ParamCount;  //����� ���������� ���������� �������
    if (InterpMainFlag  && ptr != Q_NULLPTR)
    {
        std::cout<<"\n\n---����� ������� "<<ptr->N->Id.toStdString();
        stage = 1;      //������ ������: ������� �������. ������ � ������ ����� ���������

//    }

//    if (ptr != Q_NULLPTR)       //������� ���� �������
//    {
        //����� �������. ����� ����������� �� ��������� � �������� �������
        //�������� ���� � ������. ��� ����� ������
        T->F = ptr;
        T->Cur->addLeft(new Node (ptr->N->Id, TypeEmpty, ptr->N->ParamCount, ptr->N->PosInProgram));
        T->Cur = T->Cur->Left;
        T->Cur = T->Cur->Right;    //������ ������


        ptr = ptr->Right;   //���������� � ������ ������� �� ���� ����������
        ptr = ptr->Left;
     }



    cur++;
    if ((*lex)[cur].type != Tls)
    {
        ErrorText = "�������� ������ (";
        return false;
    }
    cur++;

    if ((*lex)[cur].type != Trs)        //����������� ���������
    {
        DataValue* ptrVal = A1();
        if (ptrVal == Q_NULLPTR)    //�� ���������
        {
            ErrorText = "��������� ���������";
            return false;
        }
        if (ptr != Q_NULLPTR)       //�������� ������������
        {
            if (ParCount > 0)   //��� ����� ���������
            {
                if (InterpMainFlag)
                {
                    nodePtr = new Node (ptr->N->Id, ptr->N->TypeObj);   //����� ����
                    nodePtr->Data = *ptrVal;         //�������� ��������
                    T->Cur->addLeft(nodePtr);       //��������� ������ ���� ����, � �� ��� �� ����� - �����
                    T->Cur = T->Cur->Left;
                }

                ptr = ptr->Left;
                ParCount--;     //���������� ������
            }
            else
            {
                T->semError("���������� ������ ��� �����", &(*lex)[cur]);
                semErr = true;
            }
        }
    }

    while ((*lex)[cur].type != Trs)
    {
        if ((*lex)[cur].type == Tzap) //запятая
        {
            cur++;
        }
        else
        {
            ErrorText = "�������� ������ ,";
            return false;
        }

        DataValue* ptrVal = A1();
        if (ptrVal == Q_NULLPTR)
        {
            ErrorText = "��������� ���������";
            return false;
        }
        if (ptr != Q_NULLPTR)       //������� ���� �������
        {
            if (ParCount > 0)   //��� ����� ���������
            {
                if (InterpMainFlag)
                {
                    nodePtr = new Node (ptr->N->Id, ptr->N->TypeObj);   //����� ����
                    nodePtr->Data = *ptrVal;         //�������� ��������
                    T->Cur->addLeft(nodePtr);
                    T->Cur = T->Cur->Left;
                }
                ptr = ptr->Left;
                ParCount--;     //���������� ������
            }
            else
            {
                T->semError("���������� ������ ��� �����", &(*lex)[cur]);
                semErr = true;
            }
        }

    }
    if (ptr != Q_NULLPTR)       //������� ���� �������
        if(ParCount > 0)
        {
            T->semError("���������� ������ ��� �����", &(*lex)[cur]);
            semErr = true;
        }
    if ((*lex)[cur].type != Trs)
    {
        ErrorText = "�������� ������ )";
        return false;
    }
    //��������� ��������� ��� �������
    cur++;
    if (InterpMainFlag)
        RetAddress.push_back(cur); //��������� ����� ��������
    addrReturn = cur;   //����� ��������
    return true;
}

DataValue* Analizator::Operation (DataValue* a, DataValue* b, int operation)    //���������� ��������
{
    DataValue* result = new DataValue (TypeUnKnown, TDataValue());
    int type = Tree::semTypeRes(a->Type, b->Type);  //��� ����������
    //int value = 0;
    TDataValue value;
    switch (operation)      //��������� �������� � ���������� �� ���������
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


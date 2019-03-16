#include <QCoreApplication>
#include <lexem.h>
#include <scanner.h>
#include <analizator.h>
#include <QList>
#include <iostream>
#include <QTextStream>
#include <stdio.h>
#include <locale.h>
//using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //QList <Lexem> Lexems;
    setlocale(LC_ALL, "rus");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("IBM 866"));
    //system("chcp 1251");
    //Scanner scaner ("input.txt");

    //scaner.toScan(&Lexems);
    Analizator A;
    A.S();
    system("pause");
    return 0;
}

#ifndef SCANNER_H
#define SCANNER_H

#include <qfile.h>
#include <QString>
#include <QList>
#include <lexem.h>
#include <QMultiMap>
#include <iostream>
#include <QTextStream>

#define MaxLenth 10


class Scanner
{
public:
    QFile *file;
    QString *text;
    QMultiMap <int, QString> Types;
    QMultiMap <int, QString> KeyWords;          //ключевые слова
    QMultiMap <int, QChar> Symbols;          //символы
    QMultiMap <int, QChar> IgnoreSymbols;   //игнорируемые символы

    int lexLenth;
    QList <int*> Errors;
    Scanner();
    Scanner(QString fileName = "input.txt");
    void toScan (QList <Lexem> *lex);
};

#endif // SCANNER_H


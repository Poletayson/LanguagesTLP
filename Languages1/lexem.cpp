#include "lexem.h"

Lexem::Lexem()
{

}

Lexem::Lexem(int t, QString i)
{
    image = i;
    type = t;
}

void Lexem::setPosInProgram (int p)
{
    posInProgram = p;
}


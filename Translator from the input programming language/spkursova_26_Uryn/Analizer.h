#pragma once
#include "DataTypes.h"

class Analizer
{
private:
	DataType* Data;
	Lexema GetNextLexem(FILE* f);
public:
	Analizer(DataType* Data) : Data(Data) {}
	int AnalisisLexems(FILE* FIn); // analisis number of tokens
	void PrintLexemsInFile();	// print all tokens in file
};


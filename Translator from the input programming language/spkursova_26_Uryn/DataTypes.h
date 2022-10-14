#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <Windows.h>
#include <io.h>

#define IsOperation(k) (k < 12)

using namespace std;

enum TypeOfLex
{
	LAdd,
	LSub,
	LMul,
	LDiv,
	LMod,
	LEqu,
	LNotEqu,
	LGreate,
	LLess,
	LNot,
	LAnd,
	LOr,

	LBegProg,
	LProgram,
	LProgramName,
	LStart,
	LData,
	LVarType,
	LIdentifier,
	LEnd,

	LScan,
	LPrint,

	LIf,
	LElse,
	LWhile,
	LDo,

	LNewValue,

	LDLine,
	LLBraket,
	LRBraket,
	LTrLBraket,
	LTrRBraket,
	LNumber,
	LSeparator,
	LComma,
	LEOF,
	LLComentar,
	LRComentar,
	LUnknown
};

struct LexAndName
{
	TypeOfLex type;
	std::string name;
};

//DATA
typedef struct Lexem
{
	string name;
	int value;
	int line;
	TypeOfLex type;
}Lexema;

typedef struct L
{
	vector<Lexema> LexTable;			//Таблиця лексем

	vector<string> IdTable;		//Таблиця ідентифікаторів
	string ftype = "u26";
	string TName = "_XXXXXXXX";
	vector<LexAndName> LexName =
	{
		{LAdd,"+"},
		{LSub,"-"},
		{LMul,"*" },
		{LDiv,"/"},
		{LMod,"%"},
		{LEqu,"eg"},
		{LNotEqu,"ne"},
		{LGreate,">="},
		{LLess,"<="},
		{LNot,"!!"},
		{LAnd,"&&"},
		{LOr,"||"},

		{LBegProg,""},
		{LProgram,"name"},
		{LProgramName,""},
		{LTrLBraket,"<"},
		{LTrRBraket,">"},
		{LStart,"body"},
		{LData,"data"},
		{LVarType,"integer32"},
		{LIdentifier,""},
		{LEnd,"end"},

		{LScan,"scan"},
		{LPrint,"print"},

		{LIf,"if"},
		{LElse,"else"},
		{LWhile,"while"},
		{LDo,"do"},

		{LNewValue,"::"},

		{LLBraket,"("},
		{LRBraket,")"},
		{LDLine,"_"},
		{LNumber,""},
		{LSeparator,";"},
		{LComma,","},
		{LLComentar,"$*"},
		{LRComentar,""},
		{LEOF,"EndFile"}
	};

	bool IsPresentInput,					//Ознаки присутності операторів
		IsPresentOutput,
		IsPresentMod,
		IsPresentAnd,
		IsPresentOr,
		IsPresentNot,
		IsPresentEqu,
		IsPresentGreate,
		IsPresentLess,
		IsPresentDiv;

	bool EnableProgN = 1, PosLStart = 0;

	int numberErrors;

	std::string InputFileName;
	std::string OutputFileName;
}DataType;

bool directory_exists(const std::string& directory);
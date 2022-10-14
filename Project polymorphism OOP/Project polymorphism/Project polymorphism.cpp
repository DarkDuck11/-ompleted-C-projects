#include <stdlib.h>
#include <iostream>

using namespace std;

class CCloth
{
public:
	char* name;
	bool status = true;

	CCloth(char* name1) {
		name = new char[sizeof(name1) + 1];
		name = name1;
	}


	void Print() {
		if (status == true) {
			cout << "На складі присутній одняг" << endl;
		}
		else
			cout << "одягу немає" << endl;
		cout << "Назва одягу- " << name << endl;

	}

	virtual ~CCloth() {
		delete[]name;
	}

	bool getStatus() {
		return status;
	}

	void setStatuse(bool statuse1) {
		status = statuse1;
	}

	virtual void FullInfo() = 0;
};



class CPants : virtual public CCloth
{
public:
	char* rozmir;


	CPants(char* name1, char* rozmir1) : CCloth(name1) {
		rozmir = new char[sizeof(rozmir1) + 1];
		rozmir = rozmir1;
	}

	~CPants() {
		delete[]rozmir;
	}



	void Print() {
		cout << "розмір одягу- " << rozmir << endl;
	}

	void info() {
		cout << "штани" << endl;
	}

	virtual void FullInfo() {
		cout << "---------------------" << endl <<
			"назва одягу- " << name << endl;
		if (status == true) {
			cout << "На складі присутній одняг" << endl;
		}
		else
			cout << "одягу немає" << endl;
		Print();
		info();
		cout << "---------------------" << endl;
	}
};





class CShorts : virtual public CCloth
{
public:
	char* color;


	CShorts(char* name1, char* color1) : CCloth(name1) {
		color = new char[sizeof(color1) + 1];
		color = color1;
	}

	~CShorts() {
		delete[]color;
	}

	void Print() {
		cout << "колір- " << color << endl;
	}

	void info() {
		cout << "шорти" << endl;
	}

	void FullInfo() {
		cout << "---------------------" << endl <<
			"назва одягу- " << name << endl;
		if (status == true) {
			cout << "На складі присутній одняг" << endl;
		}
		else
			cout << "одягу немає" << endl;
		Print();
		info();
		cout << "---------------------" << endl;
	}
};





class CPantsShorts : public CPants, CShorts
{
public:
	char* firma;

	CPantsShorts(char* name1, char* rozmir1, char* color1, char* firma1) : CPants(name1, rozmir1), CCloth(name1), CShorts(name1, color1) {
		firma = new char[sizeof(firma1) + 1];
		firma = firma1;
	}

	~CPantsShorts() {
		delete[]firma;
	}

	void Print() {
		cout << "розмір- " << rozmir << endl <<
			"колір- " << color << endl <<
			"фірма-" << firma << endl;
	}

	void info() {
		cout << "комплект штани та шорти" << endl;
	}

	void FullInfo() {
		cout << "---------------------" << endl <<
			"назва одягу- " << name << endl;
		if (status == true) {
			cout << "На складі присутній одняг" << endl;
		}
		else
			cout << "одягу немає" << endl;
		Print();
		info();
		cout << "---------------------" << endl;
	}
};




//main.cpp

int main()
{
	setlocale(LC_ALL, "Ukrainian");

	CCloth* sit0 = new CPants((char*)"Завужені джинси", (char*)"L");
	sit0->setStatuse(true);
	sit0->FullInfo();

	CCloth* sit1 = new CShorts((char*)"Gucci Worldwide", (char*)"білий");
	sit1->setStatuse(true);
	sit1->FullInfo();

	CCloth* sit2 = new CPantsShorts((char*)"NIKE NSW HE PANT TEARAWAY PK", (char*)"М", (char*)"чорний", (char*)"NIKE");
	sit2->setStatuse(true);
	sit2->FullInfo();

	CPants* sit3 = new CPants((char*)"Versace Jeans Couture", (char*)"М");
	sit3->setStatuse(true);
	sit3->FullInfo();

	CShorts* sit4 = new CShorts((char*)"Adidas Essentials Linear", (char*)"сірий");
	sit4->setStatuse(true);
	sit4->FullInfo();

	CCloth* sit5 = new CPantsShorts((char*)"Apres Mer", (char*)"М", (char*)"білий", (char*)"Dolce&Gabbana");
	sit5->setStatuse(true);
	sit5->FullInfo();

	delete sit0;
	delete sit1;
	delete sit2;
	delete sit3;
	delete sit4;
	delete sit5;


	return 0;
}

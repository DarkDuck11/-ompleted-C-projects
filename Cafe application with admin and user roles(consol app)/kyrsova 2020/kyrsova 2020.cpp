#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
using namespace std;

class CEdit
{
protected:
	string info1, info2, info3, info4, findInfo, info, repInfo, edInfo;
	bool isFound = false;
public:
	void Show_Info(char fileName[], string header1, string header2, string header3, string header4) //Показати інформацію
	{
		fstream infile(fileName); //Відкрити файл для зчитування
		while (!infile)
		{
			cout << "Cannot open file!\n";
			exit(0);
		}
		cout << setw(15) << left << header1 << setw(15) << header2 << setw(15) << header3 << setw(15) << header4 << '\n'; //Вивід шапки таблиці
		while (!infile.eof())
		{
			infile >> info1 >> info2 >> info3 >> info4; //Зчитування даних з файлу для виводу
			if (!infile.eof())
				cout << setw(15) << left << info1 << setw(15) << info2 << setw(15) << info3 << setw(15) << info4 << '\n'; //Вивід даних
		}
		system("pause");
		infile.close(); //Закрити файл для зчитування
	}
	void Write_Info(char fileName[], string header1, string header2, string header3, string header4) //Записати інформацію
	{
		cout << header1;
		cin >> info1;
		cout << header2;
		cin >> info2;
		cout << header3;
		cin >> info3;
		cout << header4;
		cin >> info4;
		fstream outfile(fileName, ios::app); //Відкрити файл для запису
		if (!outfile)
		{
			cout << "Cannot open file!\n";
			exit(0);
		}
		outfile << setw(9) << left << info1 << ' ' << setw(9) << info2 << ' ' << setw(9) << info3 << ' ' << setw(9) << info4 << '\n'; //Запис
		cout << "Data was successfully writen.\n";
		system("pause");
		outfile.close();
	}
	void Find_Info(char fileName[]) //Знайти інформацію
	{
		cout << "Information to find : "; //Ввести інформацію для пошуку
		cin >> findInfo;
		fstream infile(fileName); //Відкрити файл для зчитування
		while (!infile)
		{
			cout << "Cannot open file!\n";
			exit(0);
		}
		while (!infile.eof())
		{
			infile >> info;
			if (findInfo == info)
			{
				cout << "Information was successfully found.\n";
				isFound = true;
			}
		}
		if (!isFound)
			cout << "Not found!\n";
		system("pause");
		infile.close(); //Закрити файл для зчитування
	}
	void Edit_Info(char fileName[], char fileName1[]) //Редагувати інформацію
	{
		cout << "Information to edit : "; //Ввести слово, яке треба замінити
		cin >> edInfo;
		cout << "Information to replace by : "; //Ввести слово, яким замінити
		cin >> repInfo;
		fstream infile(fileName); //Відкрити файл для зчитування
		while (!infile)
		{
			cout << "Cannot open file!\n";
			exit(0);
		}
		fstream outfile(fileName1, ios::out); //Відкрити файл перезапису
		if (!outfile)
		{
			cout << "Cannot open file!\n";
			exit(0);
		}
		while (!infile.eof())
		{
			infile >> info1 >> info2 >> info3 >> info4; //Зчитування даних з файлу для виводу
			if (edInfo == info1)
			{
				info1 = repInfo;
				isFound = true;
			}
			if (edInfo == info2)
			{
				info2 = repInfo;
				isFound = true;
			}
			if (edInfo == info3)
			{
				info3 = repInfo;
				isFound = true;
			}
			if (edInfo == info4)
			{
				info4 = repInfo;
				isFound = true;
			}
			if (!infile.eof())
				outfile << setw(9) << left << info1 << ' ' << setw(9) << info2 << ' ' << setw(9) << info3 << ' ' << setw(9) << info4 << '\n'; //Перезапис
		}
		if (!isFound)
			cout << "Not found!\n";
		else
			cout << "Data was successfully edited.\n";
		system("pause");
		infile.close(); //Закрити файл для рзчитування
		outfile.close(); //Закрити файл для перезапису
		remove(fileName);
		rename(fileName1, fileName);
	}
	void Delete_Info(char fileName[]) //Видалити інформацію
	{
		fstream outfile(fileName, ios::out); //Відкрити файл для видалення
		cout << "Data was successfully deleted.\n";
		system("pause");
		outfile.close();
	}
};

class CPrimary : public CEdit
{
public:
	char Primary(char choice, string name)
	{
		if (choice == '1') //Прочитати інформацію з файлу
		{
			Show_Info("Primary information.txt", " Restaurant name", "Director", "Opening date", "Price($)");
			return choice = 'p';
		}
		if ((choice == '2') && (name == "admin")) //Записати інформацію, операція доступна тільки для адміністратора
		{
			Write_Info("Primary information.txt", "Restaurant name : ", "Director : ", "Opening date : ", "Price($) : ");
			return choice = 'p';
		}
		if (choice == '3') //Знайти інформацію
		{
			Find_Info("Primary information.txt");
			return choice = 'p';
		}

		if ((choice == '4') && (name == "admin")) //Редагувати інформацію, операція доступна тільки для адміністратора
		{
			Edit_Info("Primary information.txt", "Primary information1.txt");
			return choice = 'p';
		}
		if ((choice == '5') && (name == "admin")) //Видалити інформацію, операція доступна тільки для адміністратора
		{
			Delete_Info("Primary information.txt");
			return choice = 'p';
		}
		if ((choice == 'b') || (choice == 'B')) //Повернення до попереднього меню
		{
			return choice = 'm';
		}
		if ((choice == 'q') || (choice == 'Q')) //Вихід з програми
			exit(0);
		else
			return choice = 'p';
	}
};

class CFinances : public CEdit
{
public:
	char Finances(char choice, string name)
	{
		if (choice == '1') //Прочитати інформацію з файлу
		{
			Show_Info("Finances.txt", "Date", "Income($)", "Expense($)", "Total($)");
			return choice = 'f';
		}

		if ((choice == '2') && (name == "admin")) //Записати інформацію у файл, операція доступна тільки для адміністратора
		{
			Write_Info("Finances.txt", "Date : ", "Income($) : ", "Expense($) : ", "Total($) : ");
			return choice = 'f';
		}
		if (choice == '3') //Знайти інформацію
		{
			Find_Info("Finances.txt");
			return choice = 'f';
		}
		if ((choice == '4') && (name == "admin")) //Редагувати інформацію, операція доступна тільки для адміністратора
		{
			Edit_Info("Finances.txt", "Finances1.txt");
			return choice = 'f';
		}
		if ((choice == '5') && (name == "admin")) //Видалити інформацію, операція доступна тільки для адміністратора
		{
			Delete_Info("Finances.txt");
			return choice = 'f';
		}
		if ((choice == 'b') || (choice == 'B')) //Повернення до попереднього меню
		{
			return choice = 'm';
		}
		if ((choice == 'q') || (choice == 'Q')) //Вихід з програми
			exit(0);
		else
			return choice = 'f';
	}
};

class CTimetable : public CEdit
{
public:
	char Timetable(char choice, string name)
	{
		if (choice == '1') //Прочитати інформацію з файлу
		{
			Show_Info("Timetable.txt", "Day", "Date", "Opening", "Closing");
			return choice = 't';
		}
		if ((choice == '2') && (name == "admin")) //Записати інформацію у файл, операція доступна тільки для адміністратора
		{
			Write_Info("Timetable.txt", "Day : ", "Date : ", "Opening : ", "Closing : ");
			return choice = 't';
		}
		if (choice == '3') //Знайти інформацію
		{
			Find_Info("Timetable.txt");
			return choice = 't';
		}
		if ((choice == '4') && (name == "admin")) //Редагувати інформацію, операція доступна тільки для адміністратора
		{
			Edit_Info("Timetable.txt", "Timetable1.txt");
			return choice = 't';
		}
		if ((choice == '5') && (name == "admin")) //Видалити інформацію, операція доступна тільки для адміністратора
		{
			Delete_Info("Timetable.txt");
			return choice = 't';
		}
		if ((choice == 'b') || (choice == 'B')) //Повернення до попереднього меню
		{
			return choice = 'm';
		}
		if ((choice == 'q') || (choice == 'Q')) //Вихід з програми
			exit(0);
		else
			return choice = 't';
	}
};

class CGoods : public CEdit
{
public:
	char Goods(char choice, string name)
	{
		if (choice == '1') //Прочитати інформацію з файлу
		{
			Show_Info("Goods.txt", "Goods", "Number(pcs)", "Price($)", "ID");
			return choice = 'g';
		}
		if ((choice == '2') && (name == "admin")) //Записати інформацію у файл, операція доступна тільки для адміністратора
		{
			Write_Info("Goods.txt", "Goods : ", "Number(pcs) : ", "Price($) : ", "ID : ");
			return choice = 'g';
		}
		if (choice == '3') //Знайти інформацію
		{
			Find_Info("Goods.txt");
			return choice = 'g';
		}
		if ((choice == '4') && (name == "admin")) //Редагувати інформацію, операція доступна тільки для адміністратора
		{
			Edit_Info("Goods.txt", "Goods1.txt");
			return choice = 'g';
		}

		if ((choice == '5') && (name == "admin")) //Видалити інформацію, операція доступна тільки для адміністратора
		{
			Delete_Info("Goods.txt");
			return choice = 'g';
		}
		if ((choice == 'b') || (choice == 'B')) //Повернення до попереднього меню
		{
			return choice = 'm';
		}
		if ((choice == 'q') || (choice == 'Q')) //Вихід з програми
			exit(0);
		else
			return choice = 'g';
	}
};

class CInventory : public CEdit
{
public:
	char Inventory(char choice, string name)
	{
		if (choice == '1') //Прочитати інформацію з файлу
		{
			Show_Info("Inventory.txt", "Object", "Number(pcs)", "Price($)", "ID");
			return choice = 'i';
		}
		if ((choice == '2') && (name == "admin")) //Записати інформацію у файл, операція доступна тільки для адміністратора
		{
			Write_Info("Inventory.txt", "Object : ", "Number(pcs) : ", "Price($) : ", "ID : ");
			return choice = 'i';
		}

		if (choice == '3') //Знайти інформацію
		{
			Find_Info("Inventory.txt");
			return choice = 'i';
		}
		if ((choice == '4') && (name == "admin")) //Редагувати інформацію, операція доступна тільки для адміністратора
		{
			Edit_Info("Inventory.txt", "Inventory1.txt");
			return choice = 'i';
		}
		if ((choice == '5') && (name == "admin")) //Видалити інформацію, операція доступна тільки для адміністратора
		{
			Delete_Info("Inventory.txt");
			return choice = 'i';
		}
		if ((choice == 'b') || (choice == 'B')) //Повернення до попереднього меню
		{
			return choice = 'm';
		}
		if ((choice == 'q') || (choice == 'Q')) //Вихід з програми
			exit(0);
		else
			return choice = 'i';
	}
};

class CWorkers : public CEdit
{
public:
	char Workers(char choice, string name)
	{
		if (choice == '1') //Прочитати інформацію з файлу
		{
			Show_Info("Workers.txt", "Name", "Profession", "Salary($)", "Age");
			return choice = 'w';
		}
		if ((choice == '2') && (name == "admin")) //Записати інформацію у файл, операція доступна тільки для адміністратора
		{
			Write_Info("Workers.txt", "Name : ", "Profession : ", "Salary($) : ", "Age : ");
			return choice = 'w';
		}
		if (choice == '3') //Знайти інформацію
		{
			Find_Info("Workers.txt");
			return choice = 'w';
		}
		if ((choice == '4') && (name == "admin")) //Редагувати інформацію, операція доступна тільки для адміністратора
		{
			Edit_Info("Workers.txt", "Workers1.txt");
			return choice = 'w';
		}
		if ((choice == '5') && (name == "admin")) //Видалити інформацію, операція доступна тільки для адміністратора
		{
			Delete_Info("Workers.txt");
			return choice = 'w';
		}
		if ((choice == 'b') || (choice == 'B')) //Повернення до попереднього меню
		{
			return choice = 'm';
		}

		if ((choice == 'q') || (choice == 'Q')) //Вихід з програми
			exit(0);
		else
			return choice = 'w';
	}
};

class CMain : public CPrimary, public CFinances, public CTimetable, public CGoods, public CInventory, public CWorkers
{
private:
	char choice;
	string name;
	float money;
public:
	void getChoice()
	{
		cin >> choice;
	}
	void getName()
	{
		cout << "Enter your name : "; //Ввід ім'я користувача
		cin >> name;
	}
	void getMoney()
	{
		cout << "Enter your money ($) : "; //Ввід грошового балансу користувача
		cin >> money;
	}
	void Menu0()
	{
		system("cls"); //Очистити екран
		system("color 1F");
		cout << " Restaurant management : \n"; //Стартове меню
		cout << "Main menu - press 'm'\n"; //Перехід в головне меню кнопкою 'm'
		cout << "Quit - press 'q'\n"; //Вихід з програми кнопкою 'q'
	}
	void Menu1()
	{
		system("cls"); //Очистити екран
		cout << "Authorization : \n"; //Меню авторизації
	}
	void Menu2()
	{
		system("cls"); //Очистити екран
		cout << " Restaurant management -> Main menu : \n";
		cout << "Primary information - press 'p'\n"; //Перехід в розділ основної інформації кнопкою 'p'
		if (name == "admin") //Операція доступна тільки для адміністратора
			cout << "Finances - press 'f'\n"; //Перехід в розділ фінансів кнопкою 'f'
		cout << "Timetable - press 't'\n"; //Перехід в меню графіку роботи кнопкою 't'
		cout << "Goods - press 'g'\n"; //Перехід в меню товарів кнопкою 'g'
		if (name == "admin") //Операція доступна тільки для адміністратора
			cout << "Inventory - press 'i'\n"; //Перехід в меню інвентарю кнопкою 'i'
		if (name == "admin") //Операція доступна тільки для адміністратора
			cout << "Workers - press 'w'\n"; //Перехід в меню працівників кнопкою 'w'
		cout << "Back - press 'b'\n"; //Повернення до попереднього меню кнопкою 'b'
		cout << "Quit - press 'q'\n"; //Вихід з програми кнопкою 'q'
	}
	void Menu3(string dirName, string name)
	{
		system("cls"); //Очистити екран
		cout << " Restaurant management -> Main menu -> " << dirName << " : \n";
		cout << "Show data - press '1'\n"; //Показати інформацію
		if (name == "admin") //Операція доступна тільки для адміністратора
			cout << "Write data - press '2'\n"; //Записати інформацію
		cout << "Find data - press '3'\n"; //Знайти інформацію
		if (name == "admin") //Операція доступна тільки для адміністратора
			cout << "Edit data - press '4'\n"; //Знайти інформацію
		if (name == "admin") //Операція доступна тільки для адміністратора
			cout << "Delete data - press '5'\n"; //Видалити інформацію
		cout << "Back - press 'b'\n"; //Повернутися до попереднього меню
		cout << "Quit - press 'q'\n"; //Вихід з програми кнопкою 'q'
	}
	int Main()
	{
		while (true)
		{
			Menu0(); //Стартове меню
			getChoice();
			if ((choice == 'm') || (choice == 'M')) //Перехід в меню авторизації
			{
				Menu1(); //Меню авторизації
				getName();
				getMoney();
			}
			while ((choice == 'm') || (choice == 'M')) //Перехід в головне меню
			{
				Menu2();
				getChoice();
				while ((choice == 'p') || (choice == 'P')) //Перехід в розділ основної інформації
				{
					Menu3("Primary information", name);
					getChoice();
					choice = Primary(choice, name);
				}
				while (((choice == 'f') || (choice == 'F')) && (name == "admin")) //Перехід в розділ фінансів
				{
					Menu3("Finances", name);
					getChoice();
					choice = Finances(choice, name);
				}
				while ((choice == 't') || (choice == 'T')) //Перехід в меню графіку роботи
				{
					Menu3("Timetable", name);
					getChoice();
					choice = Timetable(choice, name);
				}
				while ((choice == 'g') || (choice == 'G')) //Перехід в меню товарів
				{
					Menu3("Goods", name);
					getChoice();
					choice = Goods(choice, name);
				}
				while (((choice == 'i') || (choice == 'I')) && (name == "admin")) //Перехід в меню інвентарю
				{
					Menu3("Inventory", name);
					getChoice();
					choice = Inventory(choice, name);
				}
				while (((choice == 'w') || (choice == 'W')) && (name == "admin")) //Перехід в меню працівників
				{
					Menu3("Workers", name);
					getChoice();
					choice = Workers(choice, name);
				}
				if ((choice == 'b') || (choice == 'B')) //Повернення до попереднього меню
					break;
				if ((choice == 'q') || (choice == 'Q')) //Вихід з програми
					exit(0);
				else
					choice = 'm';
			}
			if ((choice == 'q') || (choice == 'Q')) //Вихід з програми
				exit(0);
		}
	}
};
int main()
{
	CMain main;
	main.Main();
	return 0;
}

#include <iostream>
using namespace std;

int tabs = 0; //Для створення табуляції
float sum = 0;
int i = 0;

//Структура ветки
struct Branch
{
public:

	int Data; //Поле даних
	Branch* LeftBranch; //вказівник на сусідню відтку
	Branch* RightBranch; //вказівник на сусідню відтку
};

//Функція внесення даних
void Add(int aData, Branch*& aBranch)
{
	//Якщо вітки не існує
	if (!aBranch)
	{ //створюємо вітку і задаємо значення
		aBranch = new Branch;
		aBranch->Data = aData;
		aBranch->LeftBranch = 0;
		aBranch->RightBranch = 0;
		return;
	}
	else //Иначе сверим вносимое
		if (aBranch->Data > aData)
		{ //Якщо воно менше того, що в цій вітці - додаєм в ліво
			Add(aData, aBranch->LeftBranch);
		}
		else
		{ //інакще в вітку справа
			Add(aData, aBranch->RightBranch);
		};
}

void Rahunok(Branch*& aBranch)// функція обрахунку середнього значення листків
{
	if (NULL == aBranch)
		return;
	if (aBranch->LeftBranch == NULL) {
		if (aBranch->RightBranch == NULL) {
			sum += aBranch->Data;
			i++;
		}
	}
	Rahunok(aBranch->RightBranch);
	Rahunok(aBranch->LeftBranch);
}

float Rahunok1(Branch*& aBranch, int k) // функція обрахунку середнього значення вузлів
{
	float sum = 0;
	if ((aBranch != NULL) && (k > 0)) {
		sum += Rahunok1(aBranch->LeftBranch, k - 1);
		sum += aBranch->Data;
		sum += Rahunok1(aBranch->RightBranch, k - 1);
	}
	return sum;
}

void print1() //Функція виводу середнього значення
{
	cout << "Середнє значення листків " << sum / i << endl;
}

void pr_obh(Branch*& aBranch)// функція прямого обходу
{
	if (NULL == aBranch)    return;

	cout << aBranch->Data << " ";
	pr_obh(aBranch->LeftBranch); //Обхід лівої частини  
	pr_obh(aBranch->RightBranch); //Обхіб правої частини
}

void zv_obh1(Branch*& aBranch)// фнкція зворотнього обходу
{
	if (NULL == aBranch)    return;

	zv_obh1(aBranch->RightBranch); //Обхіб правої частини
	zv_obh1(aBranch->LeftBranch); //Обхід лівої частини 
	if (aBranch->RightBranch == NULL) {
		if (aBranch->LeftBranch == NULL) {
			cout << aBranch->Data << " ";
		}
	}
}

void sm_obh2(Branch*& aBranch)// функція симетричьного обходу
{
	if (NULL == aBranch)    return;

	sm_obh2(aBranch->LeftBranch); //Обхід лівої частини
	if ((aBranch->LeftBranch == NULL && aBranch->RightBranch != NULL) || (aBranch->RightBranch == NULL && aBranch->LeftBranch != NULL))
	{
		cout << aBranch->Data << " ";
	}
	sm_obh2(aBranch->RightBranch); //Обхіб правої частини
}

void print(Branch* aBranch)//Функція виводу дерева
{
	if (!aBranch) return; //Якщо вітки не існує - виходимо. Виводимо нічого
	tabs += 5; //Інакще збільшуєм лічильник рекурсивно визванних процедур
	//які будуть рахувати відступи для виводу

	print(aBranch->LeftBranch); //Виведем відку і її підвітки зліва

	for (int i = 0; i < tabs; i++) cout << " "; //Потім відступи
	cout << aBranch->Data << endl; //Дані цієї вітки


	print(aBranch->RightBranch);//Виведем відку і її підвітки справа

	tabs -= 5; //після зменшуєм кількість відступів
	return;
}

void is_Empty(Branch*& aBranch) // перевірка чи дерево заповнене
{
	if (!aBranch)
	{
		cout << "Дерево пусте...";
	}
	else
	{
		cout << "Дерево заповнене...";
	}
}

Branch* del_elem(Branch*& aBranch, int aData) {// функція видалення елемента з дерева
	if (aBranch == NULL)
		return aBranch;

	if (aData == aBranch->Data) {

		Branch* tmp;
		if (aBranch->RightBranch == NULL)
			tmp = aBranch->LeftBranch;
		else {

			Branch* ptr = aBranch->RightBranch;
			if (ptr->LeftBranch == NULL) {
				ptr->LeftBranch = aBranch->LeftBranch;
				tmp = ptr;
			}
			else {

				Branch* pmin = ptr->LeftBranch;
				while (pmin->LeftBranch != NULL) {
					ptr = pmin;
					pmin = ptr->LeftBranch;
				}
				ptr->LeftBranch = pmin->RightBranch;
				pmin->LeftBranch = aBranch->LeftBranch;
				pmin->RightBranch = aBranch->RightBranch;
				tmp = pmin;
			}
		}

		delete aBranch;
		return tmp;
	}
	else if (aData < aBranch->Data)
		aBranch->LeftBranch = del_elem(aBranch->LeftBranch, aData);
	else
		aBranch->RightBranch = del_elem(aBranch->RightBranch, aData);
	return aBranch;
}

int main()
{
	setlocale(LC_ALL, "rus");
	Branch* Root = 0;
	int vel;
	int element;
	int k;

	cout << "Введіть кількість елементів дерева: ";
	cin >> vel;
	cout << endl;

	for (int i = 0; i < vel; i++)
	{
		cout << "Ведіть новий елемент дерева: ";
		cin >> element;
		Add(element, Root);
		print(Root);
		cout << endl;
	}

	cout << "Прямий обхід дерева: " << endl;
	pr_obh(Root);
	cout << endl;

	cout << "зворотній обхід листків дерева: " << endl;
	zv_obh1(Root);
	cout << endl;

	cout << "симетричьний обхід вузлів які мають одного нащадка дерева: " << endl;
	sm_obh2(Root);
	cout << endl;

	cout << "Видалим елемент бінарного дерева:" << endl;
	cout << "Введіть елемет, який хочете видалити: ";
	cin >> element;
	del_elem(Root, element);
	vel--;
	print(Root);

	Rahunok(Root);
	print1();

	cout << "Середнє значення вузлів " << (Rahunok1(Root, vel)) / vel << endl;

	cout << "Переіврка чи дерево заповнене: " << endl;
	is_Empty(Root);
	cout << endl;

	return 0;
}

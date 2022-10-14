#include <iostream>
#include <iomanip>


using namespace std;

template <typename T>// сворюємо шаблон для полегшення роботи з типами даних
class Stack
{
private:
	T* stackPtr;
	int size;
	int top;
public:
	Stack() {// конструкрор класу
		size = 0;
		stackPtr = new T[size];
		top = -1;
	}

	~Stack() {// декуструктор
		delete[] stackPtr;
	}

	bool push(const T value) {
		size++;//додаємдо одне значення для зміни розміра 
		stackPtr = (T*)realloc(stackPtr, size * sizeof(int)); //змінюєм ромір змінної stackPtr
		top++;// збільшуєм розмір для того число записувалось у наступну комірку 
		stackPtr[top] = value;//присвоюєм значення введене з клавіатури масиву stackPtr
		return true;
	}

	T pop() {
		if (top == -1) return false;//умова через яку вилучення не відбувається
		T res = stackPtr[top];
		top--; // зменшуєм одне значення для зміни розміра
		size--;// зменшуємо розмір для того число записувалось у наступну комірку
		stackPtr = (T*)realloc(stackPtr, size * sizeof(int));//змінюєм ромір змінної stackPtr, для того щоб стек не займав багато пам'яті
		return res;
	}

	void printStack() {
		for (int ix = size - 1; ix >= 0; ix--)// цикл для виводу на екран вімст стеку
			cout << " " << setw(4) << stackPtr[ix] << endl;
		cout << endl;
	}


	int get_size() {
		return size;
	}
};

int main()
{
	setlocale(LC_ALL, "ukr");
	Stack <int> myStack;//створюєм об'єкт класу з чилами цілого типу

	int temp;

	while (true)// нескінченний цикл
	{
		cout << "Додати елементи в  стек: ";
		cin >> temp;
		if (temp % 2 == 0) {// умова для парних чисел
			cout << "\nДоданий елемент: " << temp << endl;
			myStack.push(temp);//функція для заповлення стеку парними числами
		}
		else {
			cout << "\nВилучений елемент: " << myStack.pop() << endl;// функція для вилучення елементів з стреку
		}

		cout << "\nВміст стеку: " << endl;
		myStack.printStack();

	}
	return 0;
}

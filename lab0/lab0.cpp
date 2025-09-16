#include <iostream>
#include <locale.h>
#include <string>
using namespace std;

struct Tube {
    string Name;
    float Dlina;
    int Diametr;
    bool Check;
};

struct Ks {
    string name;
    int countceh;
    int countworkceh;
    string klass;
};

void menu() {
    setlocale(LC_ALL, "Russian");
    while (true) {
        cout << "1. Добавить трубу \n2. Добавить КС \n3. Просмотр всех объектов \n4. Редактировать трубу \n5. Редактировать КС \n6. Сохранить \n7. Загрузить \n0. Выход\n";
        int number;
        cin >> number;
        switch (number) {
        case 1:
        {
            break;
        }
        case 2:
        {
            break;
        }
        case 3:
        {   
            break;
        }
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            break;
        }
        case 7:
        {
            break;
        }
        case 8:
        {
            break;
        }

        }

    }
}

int main() { // Добавлена функция main()
    menu(); // Вызов функции menu()
    return 0;
}
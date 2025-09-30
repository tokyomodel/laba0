#include <iostream>
#include <locale.h>
#include <string>
#include <fstream>
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

void addTube(Tube& tube) {
    cout << "Введите километровую отметку трубы: ";
    cin.ignore();
    getline(cin, tube.Name);
    cout << "Eсли хотите ввести нецелое число - используйте точку.\n";
    cout << "Введите длину трубы (км): ";
    while (!(cin >> tube.Dlina) || tube.Dlina <= 0) {
        cout << "Ошибка! Введите положительное число: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Введите диаметр трубы (мм): ";
    while (!(cin >> tube.Diametr) || tube.Diametr <= 0 || (tube.Diametr - int(tube.Diametr)) != 0) {
        cout << "Ошибка! Введите положительное число: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    tube.Check = false;
    cout << "Труба была успешно добавлена.\n";
    return;
}
void editTube(Tube& tube) {
    if (tube.Name.empty()) {
        cout << "Труба не найдена. Сначала добавьте трубу. :)\n";
        return;
    }
    tube.Check = !tube.Check;
    cout << "Статус ремонта трубы изменен на: " << (tube.Check ? "Работает" : "В ремонте") << "\n";
}
void showTube(const Tube& tube) {
    if (tube.Name.empty()) {
        cout << "Труба не найдена. Сначала добавьте трубу. :)\n";
        return;
    }
    cout << "Труба: " << tube.Name
        << ", Длина: " << tube.Dlina << " км"
        << ", Диаметр: " << tube.Diametr << " мм"
        << ", Статус: " << (tube.Check ? "работает" : "в ремонте") << "\n";
}
void addKs(Ks& ks) {
    cout << "Введите название КС: ";
    cin.ignore();
    getline(cin, ks.name);
    cout << "Введите количество цехов: ";
    while (!(cin >> ks.countceh) || ks.countceh <= 0) {
        cout << "Oшибка! Введите положительное целое число: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Введите количество работающих цехов: ";
    while (!(cin >> ks.countworkceh) || ks.countworkceh < 0 || ks.countworkceh > ks.countceh) {
        cout << "Oшибка! Введите число от 0 до " << ks.countceh << ": ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Введите класс станции: ";
    cin.ignore();
    getline(cin, ks.klass);

    cout << "КС успешно добавлена!\n";
}
void editKs(Ks& ks) {
    if (ks.name.empty()) {
        cout << "Сначала добавьте КС!\n";
        return;
    }
    int choice;
    cout << "1. Запустить цех \n2. Остановить цех \nВыберите действие: ";
    cin >> choice;
    if (choice == 1) {
        if (ks.countworkceh < ks.countceh) {
            ks.countworkceh++;
            cout << "Цех запущен. Работающих цехов: " << ks.countworkceh << "\n";
        }
        else {
            cout << "Все цехи уже работают!!!!!!!!\n";
        }
    }
    else if (choice == 2) {
        if (ks.countworkceh > 0) {
            ks.countworkceh--;
            cout << "Цех остановлен. Работающих цехов: " << ks.countworkceh << "\n";
        }
        else {
            cout << "Работающих цехов нет. Все спят!\n";
        }
    }
}
void showKs(const Ks& ks) {
    if (ks.name.empty()) {
        cout << "КС не добавлена\n";
        return;
    }
    cout << "КС: " << ks.name
        << ", Цехов: " << ks.countceh 
        << ", Работает: " << ks.countworkceh
        << ", Класс: " << ks.klass << "\n";
}
void saveFile(const Tube& tube, const Ks& ks) {
    ofstream file("data.txt");
    if (!file.is_open()) {
        cout << "Произошла ошибка открытия файла для записи!\n";
        return;
    }
    file << tube.Name << "\n" << tube.Dlina << "\n" << tube.Diametr << "\n" << tube.Check << "\n";
    file << ks.name << "\n" << ks.countceh << "\n" << ks.countworkceh << "\n" << ks.klass << "\n";
    file.close();
    cout << "Файл был успешно сохранен!\n";
}
void loadFile(Tube& tube, Ks& ks) {
    ifstream file("data.txt");
    if (!file.is_open()) {
        cout << "Произошла ошибка открытия файла для чтения!\n";
        return;
    }
    getline(file, tube.Name);
    file >> tube.Dlina >> tube.Diametr >> tube.Check;
    file.ignore();
    getline(file, ks.name);
    file >> ks.countceh >> ks.countworkceh >> ks.klass;
    file.ignore();
    getline(file, ks.klass);
    file.close();
    cout << "Данные были успешно загружены из файла!^^\n";
}
void showAll(const Tube& tube, const Ks& ks) {
    cout << "Все записанные объекты\n";
    showTube(tube);
    showKs(ks);
    cout << "----------------------\n";
}
void menu() {
    setlocale(LC_ALL, "Russian");
    Tube tube;
    Ks ks;
    while (true) {
        cout << "1. Добавить трубу \n2. Добавить КС \n3. Просмотр всех объектов \n4. Редактировать трубу \n5. Редактировать КС \n6. Сохранить \n7. Загрузить \n8. Выход\n";
        int number;
        cin >> number;
        switch (number) {
        case 1:
        
            addTube(tube);
            break;
        
        case 2:
        
            addKs(ks);
            break;
        
        case 3:
         
            showAll(tube, ks);
            break;
        
        case 4:
        
            editTube(tube);
            break;
        
        case 5:
        
            editKs(ks);
            break;
        
        case 6:
        
            saveFile(tube, ks);
            break;
        
        case 7:
        
            loadFile(tube, ks);
            break;
        
        case 8:
        
            cout << "Выход из программы...\n";
            return;        
        default:
            cout << "Ошибка! Введенное значение должно быть от 1 до 8. Попробуйте снова!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

    }
}

int main() {
    menu(); 
    return 0;
}
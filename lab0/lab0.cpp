#include <iostream>
#include <locale.h>
#include <string>
#include <fstream>
#include <sstream>
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

int inputInt(const string& prompt, int minVal = 1) {
    string input;
    int value;
    while (true) {
        cout << prompt;
        getline(cin, input);
        bool isValid = !input.empty();
        for (char c : input) {
            if (!isdigit(c)) {
                isValid = false;
                break;
            }
        }
        if (isValid) {
            try {
                value = stoi(input);
                if (value >= minVal) {
                    return value;
                }
                else {
                    cout << "Ошибка! Число должно быть не меньше " << minVal << ". Попробуйте снова: ";
                }
            }
            catch (...) {
                cout << "Ошибка! Введите целое число: ";
            }
        }
        else {
            cout << "Ошибка! Введите целое положительное число: ";
        }
    }
}
float inputFloat(const string& prompt, float minVal = 0.001f) {
    string input;
    float value;
    while (true) {
        cout << prompt;
        getline(cin, input);

        if (input.empty()) {
            cout << "Ошибка! Введите число: ";
            continue;
        }
        bool hasDecimal = false;
        bool isValid = true;

        for (size_t i = 0; i < input.length(); i++) {
            if (input[i] == '.') {
                if (hasDecimal) { // больше одной точки
                    isValid = false;
                    break;
                }
                hasDecimal = true;
            }
            else if (!isdigit(input[i])) {
                isValid = false;
                break;
            }
        }

        if (isValid) {
            try {
                value = stof(input);
                if (value >= minVal) {
                    return value;
                }
                else {
                    cout << "Ошибка! Число должно быть не меньше " << minVal << ". Попробуйте снова: ";
                }
            }
            catch (...) {
                cout << "Ошибка! Введите число: ";
            }
        }
        else {
            cout << "Ошибка! Введите число (например: 12.5): ";
        }
    }
}

int inputIntRange(const string& prompt, int minVal, int maxVal) {
    string input;
    int value;
    while (true) {
        cout << prompt;
        getline(cin, input);


        bool isValid = !input.empty();
        for (char c : input) {
            if (!isdigit(c)) {
                isValid = false;
                break;
            }
        }

        if (isValid) {
            try {
                value = stoi(input);
                if (value >= minVal && value <= maxVal) {
                    return value;
                }
                else {
                    cout << "Ошибка! Число должно быть от " << minVal << " до " << maxVal << ". Попробуйте снова: ";
                }
            }
            catch (...) {
                cout << "Ошибка! Введите целое число: ";
            }
        }
        else {
            cout << "Ошибка! Введите целое число: ";
        }
    }
}

void addTube(Tube& tube) {
    cout << "Введите километровую отметку трубы: ";
    getline(cin, tube.Name);

    cout << "Если хотите ввести нецелое число - используйте точку.\n";
    tube.Dlina = inputFloat("Введите длину трубы (км): ");
    tube.Diametr = inputInt("Введите диаметр трубы (мм): ");

    tube.Check = false;
    cout << "Труба была успешно добавлена.\n";
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
    getline(cin, ks.name);

    ks.countceh = inputInt("Введите количество цехов: ");
    ks.countworkceh = inputIntRange("Введите количество работающих цехов: ", 0, ks.countceh);

    cout << "Введите класс станции: ";
    getline(cin, ks.klass);

    cout << "КС успешно добавлена!\n";
}

void editKs(Ks& ks) {
    if (ks.name.empty()) {
        cout << "Сначала добавьте КС!\n";
        return;
    }

    int choice = inputIntRange("1. Запустить цех \n2. Остановить цех \nВыберите действие: ", 1, 2);

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
    file >> ks.countceh >> ks.countworkceh;
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

        int choice = inputIntRange("", 1, 8);


        switch (choice) {
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
        }
    }
}

int main() {
    menu();
    return 0;
}
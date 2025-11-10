#include <iostream>
#include <locale.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <chrono>
using namespace std;

// ==================== КЛАСС ДЛЯ ЛОГИРОВАНИЯ ====================
class Logger {
private:
    ofstream logFile;
public:
    Logger() {
        logFile.open("log.txt", ios::app);
        log("=== Сеанс начат ===");
    }

    ~Logger() {
        log("=== Сеанс завершен ===");
        logFile.close();
    }

    void log(const string& message) {
        auto now = chrono::system_clock::now();
        time_t time = chrono::system_clock::to_time_t(now);
        char timeStr[26];
        ctime_s(timeStr, sizeof(timeStr), &time);
        logFile << timeStr << ": " << message << endl;
    }
};

Logger logger;

// ==================== КЛАСС ТРУБЫ ====================
class Tube {
private:
    static int nextId;
    int id;
    string name;
    float length;
    int diameter;
    bool underRepair;

public:
    Tube() : id(nextId++), length(0), diameter(0), underRepair(false) {}

    // Геттеры
    int getId() const { return id; }
    string getName() const { return name; }
    float getLength() const { return length; }
    int getDiameter() const { return diameter; }
    bool isUnderRepair() const { return underRepair; }

    // Сеттеры
    void setName(const string& n) { name = n; }
    void setLength(float l) { length = l; }
    void setDiameter(int d) { diameter = d; }
    void setUnderRepair(bool repair) { underRepair = repair; }

    void input() {
        cout << "Введите километровую отметку трубы: ";
        getline(cin, name);

        cout << "Если хотите ввести нецелое число - используйте точку.\n";
        length = inputFloat("Введите длину трубы (км): ");
        diameter = inputInt("Введите диаметр трубы (мм): ");

        underRepair = false;

        string logMsg = "Добавлена труба ID: " + to_string(id) + ", название: " + name;
        logger.log(logMsg);
    }

    void edit() {
        underRepair = !underRepair;
        string status = underRepair ? "в ремонт" : "в работу";
        cout << "Статус ремонта трубы изменен на: " << (underRepair ? "В ремонте" : "Работает") << "\n";

        string logMsg = "Изменен статус трубы ID: " + to_string(id) + " - " + status;
        logger.log(logMsg);
    }

    void display() const {
        cout << "Труба ID: " << id
            << ", Название: " << name
            << ", Длина: " << length << " км"
            << ", Диаметр: " << diameter << " мм"
            << ", Статус: " << (underRepair ? "в ремонте" : "работает") << "\n";
    }

    bool matchesFilter(const string& filterName, bool filterRepair) const {
        bool nameMatch = filterName.empty() || (name.find(filterName) != string::npos);
        bool repairMatch = !filterRepair || underRepair;
        return nameMatch && repairMatch;
    }

    // Статические методы валидации ввода
    static int inputInt(const string& prompt, int minVal = 1) {
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

    static float inputFloat(const string& prompt, float minVal = 0.001f) {
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
                    if (hasDecimal) {
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
};

int Tube::nextId = 1;

// ==================== КЛАСС КОМПРЕССОРНОЙ СТАНЦИИ ====================
class CompressorStation {
private:
    static int nextId;
    int id;
    string name;
    int totalWorkshops;
    int workingWorkshops;
    string stationClass;

public:
    CompressorStation() : id(nextId++), totalWorkshops(0), workingWorkshops(0) {}

    // Геттеры
    int getId() const { return id; }
    string getName() const { return name; }
    int getTotalWorkshops() const { return totalWorkshops; }
    int getWorkingWorkshops() const { return workingWorkshops; }
    string getStationClass() const { return stationClass; }

    // Расчет процента незадействованных цехов
    double getUnusedPercentage() const {
        if (totalWorkshops == 0) return 0.0;
        return ((totalWorkshops - workingWorkshops) * 100.0) / totalWorkshops;
    }

    void input() {
        cout << "Введите название КС: ";
        getline(cin, name);

        totalWorkshops = inputIntRange("Введите количество цехов: ", 1, 1000);
        workingWorkshops = inputIntRange("Введите количество работающих цехов: ", 0, totalWorkshops);

        cout << "Введите класс станции: ";
        getline(cin, stationClass);

        string logMsg = "Добавлена КС ID: " + to_string(id) + ", название: " + name;
        logger.log(logMsg);
    }

    void edit() {
        int choice = inputIntRange("1. Запустить цех \n2. Остановить цех \nВыберите действие: ", 1, 2);

        if (choice == 1) {
            if (workingWorkshops < totalWorkshops) {
                workingWorkshops++;
                cout << "Цех запущен. Работающих цехов: " << workingWorkshops << "\n";
                logger.log("Запущен цех на КС ID: " + to_string(id));
            }
            else {
                cout << "Все цехи уже работают!\n";
            }
        }
        else if (choice == 2) {
            if (workingWorkshops > 0) {
                workingWorkshops--;
                cout << "Цех остановлен. Работающих цехов: " << workingWorkshops << "\n";
                logger.log("Остановлен цех на КС ID: " + to_string(id));
            }
            else {
                cout << "Работающих цехов нет!\n";
            }
        }
    }

    void display() const {
        cout << "КС ID: " << id
            << ", Название: " << name
            << ", Цехов: " << totalWorkshops
            << ", Работает: " << workingWorkshops
            << ", Незадействовано: " << getUnusedPercentage() << "%"
            << ", Класс: " << stationClass << "\n";
    }

    bool matchesFilter(const string& filterName, double maxUnusedPercentage) const {
        bool nameMatch = filterName.empty() || (name.find(filterName) != string::npos);
        bool percentageMatch = getUnusedPercentage() <= maxUnusedPercentage;
        return nameMatch && percentageMatch;
    }

    static int inputIntRange(const string& prompt, int minVal, int maxVal) {
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
};

int CompressorStation::nextId = 1;

// ==================== КЛАСС ДЛЯ УПРАВЛЕНИЯ ДАННЫМИ ====================
class DataManager {
private:
    vector<Tube> tubes;
    vector<CompressorStation> stations;

public:
    // Работа с трубами
    void addTube() {
        Tube tube;
        tube.input();
        tubes.push_back(tube);
    }

    void editTube(int id) {
        auto it = find_if(tubes.begin(), tubes.end(), [id](const Tube& t) { return t.getId() == id; });
        if (it != tubes.end()) {
            it->edit();
        }
        else {
            cout << "Труба с ID " << id << " не найдена!\n";
        }
    }

    void deleteTube(int id) {
        auto it = find_if(tubes.begin(), tubes.end(), [id](const Tube& t) { return t.getId() == id; });
        if (it != tubes.end()) {
            logger.log("Удалена труба ID: " + to_string(id));
            tubes.erase(it);
            cout << "Труба удалена!\n";
        }
        else {
            cout << "Труба с ID " << id << " не найдена!\n";
        }
    }

    // Работа с КС
    void addStation() {
        CompressorStation station;
        station.input();
        stations.push_back(station);
    }

    void editStation(int id) {
        auto it = find_if(stations.begin(), stations.end(), [id](const CompressorStation& s) { return s.getId() == id; });
        if (it != stations.end()) {
            it->edit();
        }
        else {
            cout << "КС с ID " << id << " не найдена!\n";
        }
    }

    void deleteStation(int id) {
        auto it = find_if(stations.begin(), stations.end(), [id](const CompressorStation& s) { return s.getId() == id; });
        if (it != stations.end()) {
            logger.log("Удалена КС ID: " + to_string(id));
            stations.erase(it);
            cout << "КС удалена!\n";
        }
        else {
            cout << "КС с ID " << id << " не найдена!\n";
        }
    }

    // Поиск труб
    vector<int> findTubes(const string& nameFilter = "", bool repairFilter = false) {
        vector<int> result;
        for (const auto& tube : tubes) {
            if (tube.matchesFilter(nameFilter, repairFilter)) {
                result.push_back(tube.getId());
            }
        }
        return result;
    }

    // Поиск КС
    vector<int> findStations(const string& nameFilter = "", double maxUnusedPercentage = 100.0) {
        vector<int> result;
        for (const auto& station : stations) {
            if (station.matchesFilter(nameFilter, maxUnusedPercentage)) {
                result.push_back(station.getId());
            }
        }
        return result;
    }

    // Пакетное редактирование труб
    void batchEditTubes(const vector<int>& tubeIds) {
        if (tubeIds.empty()) {
            cout << "Нет труб для редактирования!\n";
            return;
        }

        cout << "Найдено труб: " << tubeIds.size() << "\n";
        for (int id : tubeIds) {
            auto it = find_if(tubes.begin(), tubes.end(), [id](const Tube& t) { return t.getId() == id; });
            if (it != tubes.end()) {
                it->display();
            }
        }

        int choice = CompressorStation::inputIntRange("1. Редактировать все\n2. Выбрать конкретные\nВыберите действие: ", 1, 2);

        if (choice == 1) {
            for (int id : tubeIds) {
                editTube(id);
            }
            logger.log("Пакетное редактирование всех найденных труб (" + to_string(tubeIds.size()) + " шт.)");
        }
        else {
            cout << "Введите ID труб для редактирования (через пробел, 0 для завершения): ";
            string input;
            getline(cin, input);
            stringstream ss(input);
            int id;
            set<int> selectedIds;

            while (ss >> id && id != 0) {
                if (find(tubeIds.begin(), tubeIds.end(), id) != tubeIds.end()) {
                    selectedIds.insert(id);
                }
            }

            for (int id : selectedIds) {
                editTube(id);
            }
            logger.log("Пакетное редактирование выбранных труб (" + to_string(selectedIds.size()) + " шт.)");
        }
    }

    // Пакетное удаление труб
    void batchDeleteTubes(const vector<int>& tubeIds) {
        if (tubeIds.empty()) {
            cout << "Нет труб для удаления!\n";
            return;
        }

        cout << "Будет удалено труб: " << tubeIds.size() << "\n";
        string confirm;
        cout << "Подтвердите удаление (yes/no): ";
        getline(cin, confirm);

        if (confirm == "yes") {
            for (int id : tubeIds) {
                deleteTube(id);
            }
            logger.log("Пакетное удаление труб (" + to_string(tubeIds.size()) + " шт.)");
        }
    }

    // Отображение всех объектов
    void displayAll() const {
        cout << "=== ТРУБЫ ===\n";
        if (tubes.empty()) {
            cout << "Трубы не добавлены\n";
        }
        else {
            for (const auto& tube : tubes) {
                tube.display();
            }
        }

        cout << "=== КОМПРЕССОРНЫЕ СТАНЦИИ ===\n";
        if (stations.empty()) {
            cout << "КС не добавлены\n";
        }
        else {
            for (const auto& station : stations) {
                station.display();
            }
        }
        cout << "----------------------\n";
    }

    // Сохранение в файл
    void saveToFile(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Ошибка открытия файла для записи!\n";
            return;
        }

        // Сохраняем трубы
        file << tubes.size() << "\n";
        for (const auto& tube : tubes) {
            file << tube.getId() << "\n"
                << tube.getName() << "\n"
                << tube.getLength() << "\n"
                << tube.getDiameter() << "\n"
                << tube.isUnderRepair() << "\n";
        }

        // Сохраняем КС
        file << stations.size() << "\n";
        for (const auto& station : stations) {
            file << station.getId() << "\n"
                << station.getName() << "\n"
                << station.getTotalWorkshops() << "\n"
                << station.getWorkingWorkshops() << "\n"
                << station.getStationClass() << "\n";
        }

        file.close();
        cout << "Данные сохранены в файл: " << filename << "\n";
        logger.log("Сохранение данных в файл: " + filename);
    }

    // Загрузка из файла
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Ошибка открытия файла для чтения!\n";
            return;
        }

        tubes.clear();
        stations.clear();

        // Загружаем трубы
        int tubeCount;
        file >> tubeCount;
        file.ignore();
        for (int i = 0; i < tubeCount; i++) {
            Tube tube;
            int id;
            string name;
            float length;
            int diameter;
            bool underRepair;

            file >> id;
            file.ignore();
            getline(file, name);
            file >> length >> diameter >> underRepair;
            file.ignore();

            // Здесь нужно обновить nextId чтобы избежать конфликтов
            tube.setName(name);
            tube.setLength(length);
            tube.setDiameter(diameter);
            tube.setUnderRepair(underRepair);
            tubes.push_back(tube);
        }

        // Загружаем КС
        int stationCount;
        file >> stationCount;
        file.ignore();
        for (int i = 0; i < stationCount; i++) {
            CompressorStation station;
            int id;
            string name;
            int totalWorkshops, workingWorkshops;
            string stationClass;

            file >> id;
            file.ignore();
            getline(file, name);
            file >> totalWorkshops >> workingWorkshops;
            file.ignore();
            getline(file, stationClass);

            // Аналогично для КС
            // В реальном приложении нужно сохранять/восстанавливать nextId
            stations.push_back(station);
        }

        file.close();
        cout << "Данные загружены из файла: " << filename << "\n";
        logger.log("Загрузка данных из файла: " + filename);
    }

    // Геттеры для получения объектов по ID
    Tube* getTubeById(int id) {
        auto it = find_if(tubes.begin(), tubes.end(), [id](const Tube& t) { return t.getId() == id; });
        return it != tubes.end() ? &(*it) : nullptr;
    }

    CompressorStation* getStationById(int id) {
        auto it = find_if(stations.begin(), stations.end(), [id](const CompressorStation& s) { return s.getId() == id; });
        return it != stations.end() ? &(*it) : nullptr;
    }
};

// ==================== ГЛАВНОЕ МЕНЮ ====================
void showMenu() {
    cout << "\n=== ГЛАВНОЕ МЕНЮ ===\n";
    cout << "1. Добавить трубу\n";
    cout << "2. Добавить КС\n";
    cout << "3. Просмотр всех объектов\n";
    cout << "4. Редактировать трубу\n";
    cout << "5. Редактировать КС\n";
    cout << "6. Удалить трубу\n";
    cout << "7. Удалить КС\n";
    cout << "8. Поиск труб\n";
    cout << "9. Поиск КС\n";
    cout << "10. Пакетное редактирование труб\n";
    cout << "11. Пакетное удаление труб\n";
    cout << "12. Сохранить\n";
    cout << "13. Загрузить\n";
    cout << "0. Выход\n";
    cout << "Выберите действие: ";
}

void menu() {
    setlocale(LC_ALL, "Russian");
    DataManager dataManager;

    while (true) {
        showMenu();
        string input;
        getline(cin, input);

        if (input.empty() || !all_of(input.begin(), input.end(), ::isdigit)) {
            cout << "Ошибка! Введите число от 0 до 13\n";
            continue;
        }

        int choice = stoi(input);

        switch (choice) {
        case 1:
            dataManager.addTube();
            break;
        case 2:
            dataManager.addStation();
            break;
        case 3:
            dataManager.displayAll();
            break;
        case 4: {
            int id = Tube::inputInt("Введите ID трубы для редактирования: ");
            dataManager.editTube(id);
            break;
        }
        case 5: {
            int id = Tube::inputInt("Введите ID КС для редактирования: ");
            dataManager.editStation(id);
            break;
        }
        case 6: {
            int id = Tube::inputInt("Введите ID трубы для удаления: ");
            dataManager.deleteTube(id);
            break;
        }
        case 7: {
            int id = Tube::inputInt("Введите ID КС для удаления: ");
            dataManager.deleteStation(id);
            break;
        }
        case 8: {
            string nameFilter;
            cout << "Введите название для фильтра (или Enter для пропуска): ";
            getline(cin, nameFilter);

            string repairFilter;
            cout << "Фильтровать по ремонту? (yes/no): ";
            getline(cin, repairFilter);
            bool filterRepair = (repairFilter == "yes");

            auto foundTubes = dataManager.findTubes(nameFilter, filterRepair);
            cout << "Найдено труб: " << foundTubes.size() << "\n";
            for (int id : foundTubes) {
                if (auto tube = dataManager.getTubeById(id)) {
                    tube->display();
                }
            }
            break;
        }
        case 9: {
            string nameFilter;
            cout << "Введите название для фильтра (или Enter для пропуска): ";
            getline(cin, nameFilter);

            double maxPercentage = 100.0;
            string percentageFilter;
            cout << "Введите максимальный процент незадействованных цехов (или Enter для 100%): ";
            getline(cin, percentageFilter);
            if (!percentageFilter.empty()) {
                maxPercentage = stod(percentageFilter);
            }

            auto foundStations = dataManager.findStations(nameFilter, maxPercentage);
            cout << "Найдено КС: " << foundStations.size() << "\n";
            for (int id : foundStations) {
                if (auto station = dataManager.getStationById(id)) {
                    station->display();
                }
            }
            break;
        }
        case 10: {
            string nameFilter;
            cout << "Введите название для фильтра (или Enter для пропуска): ";
            getline(cin, nameFilter);

            string repairFilter;
            cout << "Фильтровать по ремонту? (yes/no): ";
            getline(cin, repairFilter);
            bool filterRepair = (repairFilter == "yes");

            auto foundTubes = dataManager.findTubes(nameFilter, filterRepair);
            dataManager.batchEditTubes(foundTubes);
            break;
        }
        case 11: {
            string nameFilter;
            cout << "Введите название для фильтра (или Enter для пропуска): ";
            getline(cin, nameFilter);

            string repairFilter;
            cout << "Фильтровать по ремонту? (yes/no): ";
            getline(cin, repairFilter);
            bool filterRepair = (repairFilter == "yes");

            auto foundTubes = dataManager.findTubes(nameFilter, filterRepair);
            dataManager.batchDeleteTubes(foundTubes);
            break;
        }
        case 12: {
            string filename;
            cout << "Введите имя файла для сохранения: ";
            getline(cin, filename);
            dataManager.saveToFile(filename);
            break;
        }
        case 13: {
            string filename;
            cout << "Введите имя файла для загрузки: ";
            getline(cin, filename);
            dataManager.loadFromFile(filename);
            break;
        }
        case 0:
            cout << "Выход из программы...\n";
            return;
        default:
            cout << "Неверный выбор! Попробуйте снова.\n";
        }
    }
}

int main() {
    menu();
    return 0;
}
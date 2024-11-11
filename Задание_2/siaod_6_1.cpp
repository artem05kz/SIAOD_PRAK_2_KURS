#include <iostream>
#include <vector>
#include <list>
#include <Windows.h>
using namespace std;

class HashTable {
private:
    vector<list<pair<string, string>>> table;
    int size;
    int num_elements;

public:
    HashTable(int size = 10) : size(size), num_elements(0) {
        table.resize(size);
    }

    unsigned int hash(const string& key) {
        unsigned int hash_value = 0;
        for (int i = 0; i < key.length(); i++) {
            hash_value += (unsigned int)key[i];
        }
        return hash_value % size;
    }

    void insert(const string& key, const string& value) {
        int index = hash(key);
        table[index].push_back(make_pair(key, value));
        num_elements++;
        if (num_elements / size > 0.75) {
            resize();
        }
    }

    void remove(const string& key) {
        int index = hash(key);
        for (auto it = table[index].begin(); it != table[index].end(); it++) {
            if (it->first == key) {
                table[index].erase(it);
                num_elements--;
                break;
            }
        }
    }

    string search(const string& key) {
        int index = hash(key);
        for (auto it = table[index].begin(); it != table[index].end(); it++) {
            if (it->first == key) {
                return it->second;
            }
        }
        return "";
    }

    void resize() {
        vector<list<pair<string, string>>> old_table = table;
        size *= 2;
        table.resize(size);
        num_elements = 0;
        for (auto& list : old_table) {
            for (auto& pair : list) {
                insert(pair.first, pair.second);
            }
        }
    }

    void print() {
        for (int i = 0; i < size; i++) {
            cout << "Index " << i << ": ";
            if (table[i].empty()) {
                cout << "Empty"; 
            }
            else {
                for (auto& pair : table[i]) {
                    cout << "(" << pair.first << ", " << pair.second << ") ";
                }
            }
            cout << endl;
        }
    }
};

class DataSet {
private:
    vector<pair<string, string>> data;
    HashTable hash_table;

public:
    DataSet() : hash_table(10) {}

    void insert(const string& key, const string& value) {
        data.push_back(make_pair(key, value));
        hash_table.insert(key, value);
    }

    void remove(const string& key) {
        hash_table.remove(key);
        data.erase(remove_if(data.begin(), data.end(), [&](const pair<string, string>& p) { return p.first == key; }));
    }

    string search(const string& key) {
        return hash_table.search(key);
    }

    void print() {
        cout << "Содержимое хеш-таблицы:" << endl;
        hash_table.print(); 
        /*cout << "Содержимое вектора данных:" << endl;
        for (const auto& pair : data) {
            cout << "(" << pair.first << ", " << pair.second << ") " << "\n";
        }*/
        cout << endl;
    }
};

void menu(DataSet& data)
{
    string command;
    while (true) {
        cout << "Введите команду (insert, remove, search, print, exit): ";
        cin >> command;

        if (command == "insert") {
            string key, value;
            cout << "Введите ключ: ";
            cin >> key;
            cout << "Введите значение: ";
            cin >> value;
            data.insert(key, value);
            cout << "Элемент успешно вставлен." << endl;
        }
        else if (command == "remove") {
            string key;
            cout << "Введите ключ для удаления: ";
            cin >> key;
            data.remove(key);
            cout << "Элемент успешно удален." << endl;
        }
        else if (command == "search") {
            string key;
            cout << "Введите ключ для поиска: ";
            cin >> key;
            string value = data.search(key);
            if (value.empty()) {
                cout << "Элемент с данным ключом не найден." << endl;
            }
            else {
                cout << "Значение элемента: " << value << endl;
            }
        }
        else if (command == "print") {
                data.print();
        }
        else if (command == "exit") {
            break;
        }
        else {
            cout << "Неизвестная команда." << endl;
        }
    }
}
int main() {
    setlocale(LC_ALL, "RU");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
   DataSet data;

    data.insert("09.03.01", "МГУ");
    data.insert("10.05.01", "СПбГУ");
    data.insert("11.03.03", "МГТУ");
    data.insert("12.04.01", "МФТИ");
    data.insert("31.23.12", "ИИТ");
    data.insert("09.09.09", "МОС");
    data.insert("01.05.04", "ТХТ");
    data.insert("04.06.05", "ИКБ");
    data.print();
    
    menu(data);
    cout << "\n";

    return 0;
}
    
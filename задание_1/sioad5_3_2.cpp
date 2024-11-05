#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <clocale>
#include <chrono>
using namespace std;

struct Reader {
    int ticket_number;     // Номер читательского билета (ключ)
    char full_name[50];    // ФИО
    char city[30];         // Город
};
// структура данных в оперативной памяти
struct IndexEntry {
    int key;         // Номер читательского билета
    long offset;     // Смещение в бинарном файле
};
vector<IndexEntry> indexTable;  // Таблица индексов

// функции для построения таблицы индексов
void build_index_table(const string& binary_filename) {
    ifstream binary_file(binary_filename, ios::binary);
    if (!binary_file.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
        return;
    }

    Reader reader;
    long offset = 0;

    while (binary_file.read(reinterpret_cast<char*>(&reader), sizeof(Reader))) {
        indexTable.push_back({ reader.ticket_number, offset });
        offset += sizeof(Reader);
    }

    binary_file.close();
}
//функция поиска по таблице индексов
bool index_search(int key, long& offset) {
    for (const auto& entry : indexTable) {
        if (entry.key == key) {
            offset = entry.offset;
            return true;
        }
    }
    return false;
}
// Функция для чтения записи по смещению
Reader read_record_by_offset(const string& binary_filename, long offset) {
    ifstream binary_file(binary_filename, ios::binary);
    if (!binary_file.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
        throw runtime_error("Ошибка открытия файла");
    }

    binary_file.seekg(offset, ios::beg);
    Reader reader;
    binary_file.read(reinterpret_cast<char*>(&reader), sizeof(Reader));
    binary_file.close();
    return reader;
}


// Функция для вывода всех записей из бинарного файла
void display_all_records(const  string& binary_filename) {
    ifstream binary_file(binary_filename, ios::binary);

    if (!binary_file.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
        return;
    }

    Reader reader;
    int index = 0;

    // Чтение записей до конца файла
    while (binary_file.read(reinterpret_cast<char*>(&reader), sizeof(Reader))) {
        cout << "Запись #" << ++index << "\n";
        cout << "Номер билета: " << reader.ticket_number << "\n";
        cout << "ФИО: " << reader.full_name << "\n";
        cout << "Город: " << reader.city << "\n";
        cout << "---------------------------\n";
    }

    binary_file.close();
}

// Предусловие: бинарный файл существует
// Постусловие: возвращает true, если запись найдена, иначе false. Индекс записи записан в record_index
bool linear_search_in_file(const  string& binary_filename, int key, int& record_index) {
    ifstream binary_file(binary_filename, ios::binary);

    if (!binary_file.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
        return false;
    }

    Reader reader;
    int index = 0;

    // Чтение записей и линейный поиск
    while (binary_file.read(reinterpret_cast<char*>(&reader), sizeof(Reader))) {
        if (reader.ticket_number == key) {
            record_index = index;
            binary_file.close();
            return true;
        }
        ++index;
    }

    binary_file.close();
    return false;
}


// Предусловие: файл должен быть доступен для записи, count > 0
// Постусловие: сгенерирован текстовый файл с count записями
void generate_text_file(const  string& filename, int count) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
        return;
    }

    srand(time(nullptr));
    vector<int> used_keys;

    for (int i = 0; i < count; ++i) {
        int key;
        do {
            key = 10000 + rand() % 90000; // Генерация уникального ключа (5-значный номер)
        } while (find(used_keys.begin(), used_keys.end(), key) != used_keys.end());

        used_keys.push_back(key);

        string full_name = "Reader_" + to_string(i + 1);
        string city = "City_" + to_string(i % 10 + 1);

        file << key << " " << full_name << " " << city << "\n";
    }

    file.close();
}

// Предусловие: текстовый файл должен существовать
// Постусловие: сгенерирован бинарный файл на основе текстового файла
void convert_to_binary(const  string& text_filename, const  string& binary_filename) {
    ifstream text_file(text_filename);
    ofstream binary_file(binary_filename, ios::binary);

    if (!text_file.is_open() || !binary_file.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
        return;
    }

    Reader reader;
    while (text_file >> reader.ticket_number >> reader.full_name >> reader.city) {
        binary_file.write(reinterpret_cast<char*>(&reader), sizeof(Reader));
    }

    text_file.close();
    binary_file.close();
}

// Предусловие: бинарный файл существует, индекс записи корректен
// Постусловие: выводит на экран содержимое записи
void display_record(const  string& binary_filename, int record_index) {
    ifstream binary_file(binary_filename, ios::binary);

    if (!binary_file.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
        return;
    }

    binary_file.seekg(record_index * sizeof(Reader), ios::beg);

    Reader reader;
    binary_file.read(reinterpret_cast<char*>(&reader), sizeof(Reader));

    cout << "Номер билета: " << reader.ticket_number << "\n";
    cout << "ФИО: " << reader.full_name << "\n";
    cout << "Город: " << reader.city << "\n";

    binary_file.close();
}

// Предусловие: бинарный файл должен быть отсортирован по ключам
// Постусловие: возвращает true, если запись найдена, иначе false. Индекс записи записан в record_index
bool binary_search_in_file(const  string& binary_filename, int key, int& record_index) {
    ifstream binary_file(binary_filename, ios::binary);

    if (!binary_file.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
        return false;
    }

    binary_file.seekg(0, ios::end);
    int total_records = binary_file.tellg() / sizeof(Reader);
    int left = 0, right = total_records - 1;

    while (left <= right) {
        int mid = (left + right) / 2;
        binary_file.seekg(mid * sizeof(Reader), ios::beg);

        Reader reader;
        binary_file.read(reinterpret_cast<char*>(&reader), sizeof(Reader));

        if (reader.ticket_number == key) {
            record_index = mid;
            binary_file.close();
            return true;
        }
        else if (reader.ticket_number < key) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    binary_file.close();
    return false;
}

int main() {
    setlocale(LC_ALL, "RU");
    const string text_filename = "readers.txt";
    const string binary_filename = "readers.bin";
    const int record_count = 10000;

    // 1. Генерация текстового файла
    generate_text_file(text_filename, record_count);

    // 2. Преобразование в бинарный файл
    convert_to_binary(text_filename, binary_filename);

    // 3. Построение таблицы индексов
    build_index_table(binary_filename);
    display_all_records("readers.bin");

    // 4. Поиск записи по ключу
    int key_to_search;
    cout << "Введите номер читательского билета для поиска: ";
    cin >> key_to_search;

    long offset;
    auto start = chrono::high_resolution_clock::now();
    index_search(key_to_search, offset);
    Reader reader = read_record_by_offset(binary_filename, offset);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = end - start;
    cout << "Номер билета: " << reader.ticket_number << "\n";
    cout << "ФИО: " << reader.full_name << "\n";
    cout << "Город: " << reader.city << "\n";

    cout << "Время выполнения поиска: " << elapsed.count() << " секунд\n";

    return 0;
}

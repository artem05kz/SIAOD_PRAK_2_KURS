#include <iostream>
#include <Windows.h>
#include <vector>
#include <sstream>
#include <string>
using namespace std;
// Функция для проверки, является ли строка целым числом
bool isNumber(const  string& str) {
    for (char const& c : str) {
        if ( isdigit(c) == 0) return false;
    }
    return true;
}

// Функция для разделения строки на слова и возврата массива целых чисел
 vector<int> extractNumbers(const  string& sentence) {
     vector<int> numbers;
     stringstream ss(sentence);
     string word;

    while (ss >> word) {
        if (isNumber(word)) {
            numbers.push_back( stoi(word));
        }
    }
    return numbers;
}

int main() {
    setlocale(LC_ALL, "RU");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string sentence = "23 45 те55кст 1/2 слово 78";
    vector<int> numbers = extractNumbers(sentence);
    cout << "Пример строки: " << sentence << endl;
    cout << "Массив целых чисел: ";
    for (int num : numbers) {
         cout << num << " ";
    }
    return 0;
}

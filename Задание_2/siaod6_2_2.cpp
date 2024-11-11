#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>

using namespace std;
// Функция для вычисления таблицы сдвигов по эвристике хорошего суффикса
vector<int> goodSuffix(const  string& pattern) {
    int m = pattern.size();
    vector<int> shift(m + 1, m); // Инициализация смещений размером m+1
    vector<int> border(m + 1, 0);

    int i = m, j = m + 1;
    border[i] = j;

    while (i > 0) {
        while (j <= m && pattern[i - 1] != pattern[j - 1]) {
            if (shift[j] == m) {
                shift[j] = j - i;
            }
            j = border[j];
        }
        i--;
        j--;
        border[i] = j;
    }

    for (int i = 0; i <= m; i++) {
        if (shift[i] == m) {
            shift[i] = j;
        }
        if (i == j) {
            j = border[j];
        }
    }

    return shift;
}

// Функция для поиска подстроки в строке с использованием алгоритма Бойера-Мура
vector<int> boyerMooreSearch(const  string& text, const  string& pattern) {
    int n = text.size();
    int m = pattern.size();

    if (m == 0) return {}; // Если подстрока пустая, возвращаем пустой результат

    vector<int> result; // Массив для хранения индексов вхождений
    vector<int> shift = goodSuffix(pattern);

    int s = 0; // Смещение относительно строки
    while (s <= n - m) {
        int j = m - 1;

        // Сравнение с конца образца
        while (j >= 0 && pattern[j] == text[s + j]) {
            j--;
        }

        // Если образец полностью совпал с подстрокой в тексте
        if (j < 0) {
            result.push_back(s);
            s += shift[0];
        }
        else {
            // Иначе сдвигаем строку по таблице сдвигов
            s += shift[j + 1];
        }
    }

    return result;
}

int main() {
    setlocale(LC_ALL, "RU");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    string text = "ABOBQWERTBOBQWERTBOB";
    string pattern = "C";
    cout << "строка - " << text << endl;
    cout << "подстрока - " << pattern << endl;
    vector<int> positions = boyerMooreSearch(text, pattern);

    cout << "Позиции вхождений: ";
    for (int pos : positions) {
        cout << pos << " ";
    }
    return 0;
}

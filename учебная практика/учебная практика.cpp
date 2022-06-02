#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include <clocale>
#include <ctime>
using namespace std;

class words_lengths_count   // класс для хранения количества слов каждой длины 
{
public:
    int length = 0; //длина слова
    int count = 0; //количество слов этой длины
};

string edit_word(string word) //редактируем каждое слово из файла
{
    for (int i = 0; i < word.length(); i++) //перебираем символы
    {
        if (!(isalnum(word[i]) and word[i] <= 127)) //если не число и не буквы латиницы
        {
            if (word.length() == 1) //если длина слова 1 символ
                word.erase(i); //стираем его
            else { //если больше символов
                word.erase(i, 1); //удаляем символ и все что справа перемещаем левее
                i--; //возвращаемся к тому же месту символа, чтобы проверить следующий
            }
        }
    }
    return word; 
}
void input(vector<string>& words, string& path_original) 
{                                                     
    ifstream original; //создаем объект класса ifstream
    bool check = false; // для проверки ввода пути
    while (check == false) {
        check = true;
        original.open(path_original); // открываем файл по введенному пути
        if (!original.is_open()) // если не открылся
        {
            cout << "Ошибка доступа к файлу." << endl;
            cout << "Введите полный путь к текстовому файлу" << endl;
            getline(cin, path_original);
            check = false;
        }
    }

    while (!original.eof()) // пока файл не закончится    
    {
        string new_word; //новое слово
        original >> new_word; //записываем новое слово из файла
        new_word = edit_word(new_word); //редактируем слово 
        if (new_word.length() > 0) { // если длина слова не 0
            words.push_back(new_word); //записываем слово в вектор со словами
        }
    }
    original.close(); //закрываем файл с оригиналом текста
}
void qsort(vector <string>& vec, int left, int right) { //быстрая сортировка
    int i = left, j = right; //левый и правый элементы от которых будем двигаться
    string tmp; //временная переменная для перестановки двух элементов местами
    int pivot = vec[(left + right) / 2].length(); //опорный элемент, средний в рассматриваемой части вектора

    while (i <= j) { //пока левый индекс не больше правого
        while (vec[i].length() > pivot) //пока длина рассматриваемого слова слева больше опорного
            i++; //увеличиваем счетчик чтобы посмотреть следующее
        while (vec[j].length() < pivot) // пока длина рассматриваемого слова справа меньше опорного
            j--; //уменьшаем счетчик чтобы посмотреть следующее
        if (i <= j) { //дошли до слов которые можно поменять местами
            tmp = vec[i]; //меняем
            vec[i] = vec[j];
            vec[j] = tmp;
            i++; //переходим к следующим
            j--;
        }
    };

    if (left < j) // пока есть что сортировать левее от опорного
        qsort(vec, left, j); //сортируем
    if (i < right) //пока есть что сортировать правее от опорного
        qsort(vec, i, right); //сортируем

}
void result(vector<string>& words, string path_result)
{
    ofstream result; //открываем файл для записи
    bool check = false; // для проверки ввода пути
    while (check == false) {
        check = true;
        result.open(path_result); // открываем файл по введенному пути
        if (!result.is_open()) // если не открылся
        {
            cout << "Ошибка доступа к файлу." << endl;
            cout << "Введите полный путь к текстовому файлу" << endl;
            getline(cin, path_result);
            check = false;
        }
    }
 
    for (int i = 0; i < words.size(); i++) // перебираем слова в векторе слов
        result << words[i] << " "; //записываем их в файл для результата
    result.close(); //закрываем файл с результатом
}
int find_length(vector <words_lengths_count> vtr, int number) // ищем есть ли длина в векторе длин
{
    for (int i = 0; i < vtr.size(); i++)  // перебираем длины
    {
        if (vtr[i].length == number) // если есть такая длина
            return i; // возвращаем ее номер
    }
    return -1; // иначе возвращаем -1
}
void find_lengths_count_words(vector<string>& words, vector<words_lengths_count>& word_length_count)
{
    for (int i = 0; i < words.size(); i++) //перебираем слова
    {
        int finded = find_length(word_length_count, words[i].length()); //ищем есть ли в уже записанных длинах длина рассматриваемого слова
        if (finded == -1) //если нет
        {
            words_lengths_count new_length;  //создаем новую длину
            new_length.length = words[i].length(); //записываем ее
            new_length.count++; //прибавляем счетчик количества слов этой длины
            word_length_count.push_back(new_length);
        }
        else
        {
            word_length_count[finded].count++; //прибавляем счетчик количества слов этой длины
        }
    }
}
void analysis(vector<string>& words, int time, string path_original, string path_analysis)
{
    ifstream original; //оригинальный для чтения
    ofstream analysis; // анализ для записи
    bool check = false; // для проверки ввода пути
    while (check == false) {
        check = true;
        analysis.open(path_analysis); // открываем файл по введенному пути
        if (!analysis.is_open()) // если не открылся
        {
            cout << "Ошибка доступа к файлу." << endl;
            cout << "Введите полный путь к текстовому файлу" << endl;
            getline(cin, path_analysis);
            check = false;
        }
    }
   
    analysis << "Исходный текст: " << endl;
    cout << "Исходный текст: " << endl;
    original.open(path_original); // открываем файл с оригинальным текстом
    while (!original.eof()) // пока не конец файла
    {
        string line; // строка
        getline(original, line); //считываем строку из файла с ориг текстом
        analysis << line << endl; // записываем в файл анализа
        cout << line << endl;
    }
    original.close(); // закрываем файл с оригинальным текстом

    //выводим в файл анализа и на консоль информацию о варианте задания
    analysis << endl << "Вариант 11: латиница, по количеству символов в слове, по убыванию, учитывать числа, быстрая сортировка." << endl << "Количество слов : " << words.size()
        << endl << "Время сортировки: " << time << " мс" << endl << "Статистика(количество слов каждой длины) :" << endl;
    cout << endl << "Вариант 11: латиница, по количеству символов в слове, по убыванию, учитывать числа, быстрая сортировка." << endl << "Количество слов : " << words.size()
        << endl << "Время сортировки: " << time << " мс" << endl << "Статистика(количество слов каждой длины) :" << endl;

    vector<words_lengths_count> word_length_count; //вектор с длинами и количеством слов каждой длины
    find_lengths_count_words(words, word_length_count); // ищем длины и считаем слова каждой длины

    for (int i = 0; i < word_length_count.size();  i++) //перебираем длины по убыванию
    {   //выводим в файл анализа и на консоль каждую длину и количество слов
        analysis << word_length_count[i].length << " - " << word_length_count[i].count << endl; 
        cout << word_length_count[i].length << " - " << word_length_count[i].count << endl; 
    }
    analysis.close();
}

int main()
{
    setlocale(LC_ALL, "Russian");

    cout << "Введите путь к оригинальному файлу с текстом: " << endl;
    string path_original;
    getline(cin, path_original);

    vector <string> words; // вектор с текстом по словам
    input(words, path_original); //ввод текста

    int time0 = clock(); // включаем счетчик времени
       qsort(words, 0, words.size() - 1);//сортируем
    int time = clock() - time0; //выключаем счетчик времени и считаем время

    cout << "Введите путь к файлу для результата: " << endl;
    string path_result;
    getline(cin, path_result);
    result(words, path_result); //записываем отсортированные слова в выходной файл
    cout << "Введите путь к файлу для анализа: " << endl; 
    string path_analysis;
    getline(cin, path_analysis);
    analysis(words, time, path_original, path_analysis); //записываем анализ в файл с анализом
}

/* cymbols  words
0. 3289
1. 3535
2. 4738
3. 5763
4. 6901     1122
5. 8776 
6. 9393
7. 11145
8. 13700  
9. 15304
*/
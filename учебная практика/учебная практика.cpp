#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include <clocale>
#include <ctime>
using namespace std;
// класс для хранения количества слов каждой длины 
class WordsLengthsCount   
{
public:
    int length = 0; //длина слова
    int count = 0; //количество слов этой длины
};

//функция для удаления лишних символов из слов (вызывается в input)
string editWord(string word) 
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

//функция, считывающая слова из входного файла и записывающая их в вектор слов (вызывается в main)
void input(vector<string>& words, string& pathOriginal) 
{                                                     
    ifstream original; //создаем объект класса ifstream
    bool check = false; // для проверки ввода пути
    while (check == false) {
        check = true;
        original.open(pathOriginal); // открываем файл по введенному пути
        if (!original.is_open()) // если не открылся
        {
            cout << "Ошибка доступа к файлу." << endl;
            cout << "Введите полный путь к текстовому файлу" << endl;
            getline(cin, pathOriginal);
            check = false;
        }
    }

    while (!original.eof()) // пока файл не закончится    
    {
        string newWord; //новое слово
        original >> newWord; //записываем новое слово из файла
        newWord = editWord(newWord); //редактируем слово 
        if (newWord.length() > 0) { // если длина слова не 0
            words.push_back(newWord); //записываем слово в вектор со словами
        }
    }
    original.close(); //закрываем файл с оригиналом текста
}

//функция быстрой сортировки (вызывается в main)
void qsort(vector <string>& vec, int left, int right) { 
    int i = left, j = right; //левый и правый элементы от которых будем двигаться
    string temp; //временная переменная для перестановки двух элементов местами
    int pivot = vec[(left + right) / 2].length(); //опорный элемент, средний в рассматриваемой части вектора

    while (i <= j) { //пока левый индекс не больше правого
        while (vec[i].length() > pivot) //пока длина рассматриваемого слова слева больше опорного
            i++; //увеличиваем счетчик чтобы посмотреть следующее
        while (vec[j].length() < pivot) // пока длина рассматриваемого слова справа меньше опорного
            j--; //уменьшаем счетчик чтобы посмотреть следующее
        if (i <= j) { //дошли до слов которые можно поменять местами
            temp = vec[i]; //меняем
            vec[i] = vec[j];
            vec[j] = temp;
            i++; //переходим к следующим
            j--;
        }
    };

    if (left < j) // пока есть что сортировать левее от опорного
        qsort(vec, left, j); //сортируем
    if (i < right) //пока есть что сортировать правее от опорного
        qsort(vec, i, right); //сортируем

}

//функция для вывода результата в файл результата (вызывается в main)
void result(vector<string>& words, string pathResult)
{
    ofstream result; //открываем файл для записи
    bool check = false; // для проверки ввода пути
    while (check == false) {
        check = true;
        result.open(pathResult); // открываем файл по введенному пути
        if (!result.is_open()) // если не открылся
        {
            cout << "Ошибка доступа к файлу." << endl;
            cout << "Введите полный путь к текстовому файлу" << endl;
            getline(cin, pathResult);
            check = false;
        }
    }
 
    for (int i = 0; i < words.size(); i++) // перебираем слова в векторе слов
        result << words[i] << " "; //записываем их в файл для результата
    result.close(); //закрываем файл с результатом
}

//функция, проверяющая, есть ли искомая длина в векторе со всеми длинами слов (вызывается в findLengthsCountWords)
int findLength(vector <WordsLengthsCount> vec, int length) 
{
    for (int i = 0; i < vec.size(); i++)  // перебираем длины
    {
        if (vec[i].length == length) // если есть такая длина
            return i; // возвращаем ее номер
    }
    return -1; // иначе возвращаем -1
}

//функция для вычисления статистики, т.е. количества слов каждой длины (вызывается в analysis)
void findLengthsCountWords(vector<string>& words, vector<WordsLengthsCount>& wordsLengthsCount)
{
    for (int i = 0; i < words.size(); i++) //перебираем слова
    {
        int finded = findLength(wordsLengthsCount, words[i].length()); //ищем есть ли в уже записанных длинах длина рассматриваемого слова
        if (finded == -1) //если нет
        {
            WordsLengthsCount new_length;  //создаем новую длину
            new_length.length = words[i].length(); //записываем ее
            new_length.count++; //прибавляем счетчик количества слов этой длины
            wordsLengthsCount.push_back(new_length);
        }
        else
        {
            wordsLengthsCount[finded].count++; //прибавляем счетчик количества слов этой длины
        }
    }
}

//функция для вывода анализа в файл анализа и на консоль (вызывается в main)
void analysis(vector<string>& words, int time, string pathOriginal, string pathAnalysis)
{
    ifstream original; //оригинальный для чтения
    ofstream analysis; // анализ для записи
    bool check = false; // для проверки ввода пути
    while (check == false) {
        check = true;
        analysis.open(pathAnalysis); // открываем файл по введенному пути
        if (!analysis.is_open()) // если не открылся
        {
            cout << "Ошибка доступа к файлу." << endl;
            cout << "Введите полный путь к текстовому файлу" << endl;
            getline(cin, pathAnalysis);
            check = false;
        }
    }
   
    analysis << "Исходный текст: " << endl;
    cout << "Исходный текст: " << endl;
    original.open(pathOriginal); // открываем файл с оригинальным текстом
    while (!original.eof()) // пока не конец файла
    {
        string line; // строка
        getline(original, line); //считываем строку из файла с ориг текстом
        analysis << line << endl; // записываем в файл анализа
        cout << line << endl;
    }
    original.close(); // закрываем файл с оригинальным текстом

    //выводим в файл анализа и на консоль информацию о варианте задания и тд
    analysis << endl << "Вариант 11: латиница, по количеству символов в слове, по убыванию, учитывать числа, быстрая сортировка." << endl << "Количество слов : " << words.size()
             << endl << "Время сортировки: " << time << " мс" << endl << "Статистика(количество слов каждой длины) :" << endl;

    cout << endl << "Вариант 11: латиница, по количеству символов в слове, по убыванию, учитывать числа, быстрая сортировка." << endl << "Количество слов : " << words.size()
         << endl << "Время сортировки: " << time << " мс" << endl << "Статистика(количество слов каждой длины) :" << endl;

    vector<WordsLengthsCount> wordsLengthsCount; //вектор с длинами и количеством слов каждой длины
    findLengthsCountWords(words, wordsLengthsCount); // ищем длины и считаем слова каждой длины

    for (int i = 0; i < wordsLengthsCount.size();  i++) //перебираем длины по убыванию
    {   //выводим в файл анализа и на консоль каждую длину и количество слов
        analysis << wordsLengthsCount[i].length << " - " << wordsLengthsCount[i].count << endl; 
        cout << wordsLengthsCount[i].length << " - " << wordsLengthsCount[i].count << endl; 
    }
    analysis.close();
}

int main()
{
    setlocale(LC_ALL, "Russian");

    cout << "Введите путь к файлу с оригиналом текста: " << endl;
    string pathOriginal;
    getline(cin, pathOriginal);

    vector <string> words; // вектор с текстом по словам
    input(words, pathOriginal); //ввод текста и запись по словам в вектор 

    int time0 = clock(); // включаем счетчик времени
       qsort(words, 0, words.size() - 1);//быстрая сортировка слов в векторе по длине по убыванию
    int time = clock() - time0; //выключаем счетчик времени и считаем время

    cout << "Введите путь к файлу для результата: " << endl;
    string pathResult;
    getline(cin, pathResult);
    result(words, pathResult); //записываем отсортированные слова в выходной файл
    cout << "Введите путь к файлу для анализа: " << endl; 
    string pathAnalysis;
    getline(cin, pathAnalysis);
    analysis(words, time, pathOriginal, pathAnalysis); //записываем анализ в файл с анализом и на консоль
}

/* cymbols  words   time
0. 3289     583     2
1. 3535     711     3
2. 4738     793     2
3. 5763     966     4
4. 6901     1122    4
5. 8776     1279    5
6. 9393     2007    5
7. 11145    1841    6
8. 13700    2518    6
9. 15304    2887    12
*/
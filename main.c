#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Ращупкин Евгений КЭ-203
28 вариант
Lab6
В текстовом файле хранится текст.
Осуществить прямой поиск введенного пользователем слова с использованием алгоритма
Боуера и Мура
*/

typedef struct SubString
{
   int Length;
   char *String;
   int *SkipArr;
} SubString;

//Открытие файла
int FileOpen(FILE **file)
{
   // Открытие файла с режимом доступа «только чтение» и привязка к нему потока данных
   printf("Opening: ");
   *file = fopen("input.txt", "r");

   // Проверка открытия файла
   if (*file == NULL)
   {
      printf("ERROR\n");
      return -1;
   }

   else
   {
      printf("completed\n");
      return 1;
   }
}

//Чтение файла построчно
int FilePrint(FILE **file)
{
   //Переменная, в которую поочередно будут помещаться считываемые строки
   char str[100];
   //Указатель, в который будет помещен адрес массива, в который считана строка, или NULL если достигнут коней файла или произошла ошибка
   char *estr;
   //Счётчик линий
   int line = 0;
   printf("Lines reading:\n");
   //Чтение (построчно) данных из файла в бесконечном цикле
   while (1)
   {
      //Чтение одной строки  из файла
      estr = fgets(str, sizeof(str), *file);
      //Добавляем к счётчику строк 1
      line++;
      //Проверка на конец файла или ошибку чтения
      if (estr == NULL)
      {
         //Проверяем, что именно произошло: кончился файл или это ошибка чтения
         if (feof(*file) != 0)
         {
            //Если файл закончился, выводим сообщение о завершении чтения и выходим из бесконечного цикла
            printf("\nFile reading complited\n");
            break;
         }
         else
         {
            //Если при чтении произошла ошибка, выводим сообщение об ошибке и выходим из бесконечного цикла
            printf("\nERROR reading\n");
            return -1;
         }
      }
      //Если файл не закончился, и не было ошибки чтения выводим считанную строку  на экран
      printf("   %5d. %s", line, str);
   }
   //Переставляем указатель на 0 строку для дальнейшей работы
   fseek(*file, 0, SEEK_SET);
   return 1;
}

int StringBMSearch(int *skiparr, char *string, char *desiredstring, int size)
{
   
   int stringlen = strlen(string);
   
   int start = 0;
   int position = start + size - 1;
   
   int step = 0;

   for (position; position < stringlen; position += step)
   {  
      printf("%d \n", position);
      int i = size - 1;
      int bufposition = position;
      for(i; i >= 0; i--)
      {
         if (string[bufposition] == desiredstring[i])
         {
            bufposition--;
            if (bufposition == position - size + 1)
            {
               printf("%d position\n", position);
               return position;
            }    
         }
         else
         {  
            step = size;
            for (int j = 0; j < size; j++)
            {
               if (string[bufposition] == desiredstring[j])
               {
                  step = skiparr[j];
                  break;
               }  
            }
            break;
         }
      }
   }
   return -1;
}

//Перебор строк для поиска алгоритмом Боуера и Мура
int FileBMSearch(FILE **file, int *skiparr, char *desiredstring, int desiredstringsize, int *coincidence)
{
   char str[100];
   char *estr;
   int line = 0;
   printf("Lines searching:\n");
   while (1)
   {
      estr = fgets(str, sizeof(str), *file);
      line++;
      if (estr == NULL)
      {
         if (feof(*file) != 0)
         {
            printf("\nFile searching complited\n");
            break;
         }
         else
         {

            printf("\nERROR reading\n");
            return -1;
         }
      }

      printf("%10d   %5d. %s", strlen(str), line, str);
      //Перераспределяем память для массива совпадений
      printf ("%d aaaadded\n", coincidence[line - 1]);
      
      coincidence = (int*)realloc(coincidence, line * sizeof(int));

      puts("Pererasp\n");

      coincidence[line - 1] = StringBMSearch(skiparr, str, desiredstring, desiredstringsize);
      printf ("%d added\n", coincidence[line - 1]);
   }
   fseek(*file, 0, SEEK_SET);
   return 1;
}

//Закрытие файла
int FileClose(FILE **file)
{
   printf("Closing a file: ");
   if (fclose(*file) == EOF)
   {
      printf("ERROR\n");
      return -1;
   }

   else
   {
      printf("completed.\n");
      return 1;
   }
}

char *StringGet(int *len)
{
   *len = 0;                               // изначально строка пуста
   int capacity = 1;                       // ёмкость контейнера динамической строки (1, так как точно будет '\0')
   char *s = (char *)malloc(sizeof(char)); // динамическая пустая строка

   char c = getchar(); // символ для чтения данных

   // читаем символы, пока не получим символ переноса строки (\n)
   while (c != '\n')
   {
      s[(*len)++] = c; // заносим в строку новый символ

      // если реальный размер больше размера контейнера, то увеличим его размер
      if (*len >= capacity)
      {
         capacity++;                                      // увеличиваем ёмкость строки на 1
         s = (char *)realloc(s, capacity * sizeof(char)); // создаём новую строку с увеличенной ёмкостью
      }
      c = getchar(); // считываем следующий символ
   }

   s[*len] = '\0'; // завершаем строку символом конца строки

   return s; // возвращаем указатель на считанную строку
}

void StringSkipArrayFill(int *arr, char *string, int size)
{
   //Функция заполняет с конца заполняет элементы последовательно с 0, заменяет на минимальное оставшееся
   int i = size - 1;
   for (i; i >= 0; i--)
   {
      arr[i] = size - i - 1;
      int j = size - 1;
      int end = i;
      for (j; j >= end; j--)
      {
         if (string[j] == string[i])
         {
            arr[i] = arr[j];
            break;
         }
      }
   }
}

void StringSkipArrayPrint(int *arr, char *string, int size)
{
   for (int i = 0; i < size; i++)
   {
      printf("%3c", string[i]);
   }
   printf("\n");
   for (int i = 0; i < size; i++)
   {
      printf("%3d", arr[i]);
   }
   printf("\n");
}

void PrintArray(int *arr)
{
   int n = sizeof(arr)/sizeof(int);
   for (int i = 0; i < n; i++)
   {
      printf("%d ", arr[i]);
   }
   printf("\n");
}

void main()
{
   //Переменная, в которую будет помещен указатель на созданный поток данных
   FILE *file;

   FileOpen(&file);

   FilePrint(&file);

   SubString str;

   printf("Enter string:\n>>");
   str.String = StringGet(&str.Length);
   str.SkipArr = malloc(str.Length * sizeof(int));
   printf("You've wrote: %s, len = %d\n", str.String, str.Length);
   StringSkipArrayFill(str.SkipArr, str.String, str.Length);
   StringSkipArrayPrint(str.SkipArr, str.String, str.Length);

   int *arrcoincidences;
   FileBMSearch(&file, str.SkipArr, str.String, str.Length, arrcoincidences);


   PrintArray(arrcoincidences);

   FileClose(&file);
}
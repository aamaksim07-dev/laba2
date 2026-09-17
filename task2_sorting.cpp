#define _CRT_SECURE_NO_WARNINGS		// разрешаем привычный scanf

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <windows.h>

/* Максимальный размер массива. Больше брать нельзя: на наборе
   "половина возрастает, половина убывает" быстрая сортировка
   вызывает саму себя примерно n/2 раз и стека перестаёт хватать. */
#define MAXN 30000

int etalon[MAXN];	// исходный набор данных
int items[MAXN];	// копия, которую сортируем

/* Сортировка Шелла (реализация из задания).
   Проверка j >= 0 поставлена первой: иначе items[j] читается
   до проверки индекса, то есть за границей массива. */
void shell(int *items, int count)
{
	int i, j, gap, k;
	int x, a[5];

	a[0]=9; a[1]=5; a[2]=3; a[3]=2; a[4]=1;

	for(k=0; k < 5; k++) {
		gap = a[k];
		for(i=gap; i < count; ++i) {
			x = items[i];
			for(j=i-gap; (j >= 0) && (x < items[j]); j=j-gap)
				items[j+gap] = items[j];
			items[j+gap] = x;
		}
	}
}

/* Быстрая сортировка (из задания), вызов: qs(items, 0, count-1); */
void qs(int *items, int left, int right)
{
	int i, j;
	int x, y;

	i = left; j = right;

	/* выбор компаранда */
	x = items[(left+right)/2];

	do {
		while((items[i] < x) && (i < right)) i++;
		while((x < items[j]) && (j > left)) j--;

		if(i <= j) {
			y = items[i];
			items[i] = items[j];
			items[j] = y;
			i++; j--;
		}
	} while(i <= j);

	if(left < j) qs(items, left, j);
	if(i < right) qs(items, i, right);
}

/* Функция сравнения для стандартной qsort */
int cmp(const void *p, const void *q)
{
	return *(int*)p - *(int*)q;
}

/* Заполняет etalon одним из четырёх наборов данных */
void fill(int n, int kind)
{
	int i;

	for(i=0; i<n; i++)
	{
		if(kind==0) etalon[i]=rand();			// случайный
		else if(kind==1) etalon[i]=i+1;			// возрастающий
		else if(kind==2) etalon[i]=n-i;			// убывающий
		else if(i<n/2) etalon[i]=i+1;			// половина вверх,
		else etalon[i]=n-i;						// половина вниз
	}
}

/* Копирует эталон в рабочий массив перед каждой сортировкой */
void copy(int n)
{
	int i;

	for(i=0; i<n; i++)
		items[i]=etalon[i];
}

/* Среднее время одного копирования массива. Копирование входит в замер
   сортировки, поэтому его нужно вычесть из результата. */
double copy_time(int n)
{
	clock_t start;
	int count=0;

	start = clock();
	do {
		copy(n);
		count++;
	} while(clock()-start < CLOCKS_PER_SEC/10);

	return (double)(clock()-start)/CLOCKS_PER_SEC/count;
}

/* Среднее время одной сортировки. Одна сортировка небольшого массива
   выполняется быстрее, чем тикает таймер (около 1 мс), поэтому сортируем
   в цикле, пока не наберётся 0,2 секунды, и делим на число повторов. */
double measure(int n, int algorithm)
{
	clock_t start;
	double time;
	int count=0;

	start = clock();
	do {
		copy(n);
		if(algorithm==0) shell(items, n);
		else if(algorithm==1) qs(items, 0, n-1);
		else qsort(items, n, sizeof(int), cmp);
		count++;
	} while(clock()-start < CLOCKS_PER_SEC/5);

	time = (double)(clock()-start)/CLOCKS_PER_SEC/count - copy_time(n);

	return time > 0 ? time : 0;
}

int main(void)
{
	const char *names[] = {
		"Случайный массив",
		"Возрастающий массив",
		"Убывающий массив",
		"Половина возрастающая, половина убывающая" };
	int n, kind;

	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	SetConsoleOutputCP(1251);		// вывод по-русски
	setlocale(LC_NUMERIC, "Russian");	// запятая в дробях
	srand((unsigned)time(NULL));

	printf("Введите размер массива: ");
	scanf("%d", &n);
	if(n < 2 || n > MAXN) return 0;

	for(kind=0; kind<4; kind++)
	{
		fill(n, kind);
		printf("\n%d. %s:\n", kind+1, names[kind]);
		printf("Сортировка Шелла:    %f секунд\n", measure(n, 0));
		printf("Быстрая сортировка:  %f секунд\n", measure(n, 1));
		printf("qsort:               %f секунд\n", measure(n, 2));
	}

	return 0;
}

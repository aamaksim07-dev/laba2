#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <windows.h>

#define MAXN 10000		// максимальный размер матрицы

/* Матрицы объявлены глобально: три массива 10000 x 10000
   занимают около 1,2 ГБ, на стеке столько места нет. */
int a[MAXN][MAXN], b[MAXN][MAXN], c[MAXN][MAXN];

void matrix_multiplication(int n)
{
	clock_t start, end;
	int i=0, j=0, r;
	int elem_c;

	srand((unsigned)time(NULL));
	while(i<n)
	{
		j=0;	// без сброса j заполнялась бы только первая строка
		while(j<n)
		{
			a[i][j]=rand()% 100 + 1;	// заполняем матрицы случайными числами
			b[i][j]=rand()% 100 + 1;
			c[i][j]=0;
			j++;
		}
		i++;
	}

	start = clock();

	/* Порядок циклов i-r-j: элементы b и c перебираются
	   по строкам, подряд. При обычном порядке i-j-r матрица b
	   читается по столбцам, и программа работает дольше. */
	for(i=0;i<n;i++)
	{
		for(r=0;r<n;r++)
		{
			elem_c=a[i][r];
			for(j=0;j<n;j++)
				c[i][j]=c[i][j]+elem_c*b[r][j];
		}
	}

	end = clock();

	printf("%d\t%f сек.\n", n, (double)(end-start)/CLOCKS_PER_SEC);
}

int main(void)
{
	int sizes[] = { 100, 200, 400, 1000, 2000, 4000, 10000 };
	int count = sizeof(sizes) / sizeof(sizes[0]);
	int k;

	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	SetConsoleOutputCP(1251);		// вывод по-русски
	setlocale(LC_NUMERIC, "Russian");	// запятая в дробях

	printf("Размер\tВремя\n");

	for(k=0; k<count; k++)
		matrix_multiplication(sizes[k]);

	return 0;
}

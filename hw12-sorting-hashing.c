/*
 * hw12-sorting-hashing.c
 *
 *  Homework 12: Sorting & Hashing
 *  Department of Computer Science at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ARRAY_SIZE			13	/* prime number */
#define MAX_HASH_TABLE_SIZE 	MAX_ARRAY_SIZE

/* �ʿ信 ���� �Լ� �߰� ���� */
int initialize(int **a);			// ���� ������ ������ �迭 ���� �Ҵ� �� �ʱ�ȭ
int freeArray(int *a);				// ���� �Ҵ�� �迭 �Ҵ� ����
void printArray(int *a);			// �迭 ���

int selectionSort(int *a);			// ���� ����
int insertionSort(int *a);			// ���� ����
int bubbleSort(int *a);				// ���� ����
int shellSort(int *a);				// �� ����
/* recursive function���� ���� */	
int quickSort(int *a, int n);		// �� ����


/* hash code generator, key % MAX_HASH_TABLE_SIZE */
int hashCode(int key);				// �ؽ� �Լ�

/* array a�� ���� hash table�� �����. */
int hashing(int *a, int **ht);		// �ؽ� ���̺� �Ҵ�

/* hash table���� key�� ã�� hash table�� index return */
int search(int *ht, int key);		// �ؽ� ���̺��� Ű ��ġ ã��


int main()
{
	char command;
	int *array = NULL;
	int *hashtable = NULL;
	int key = -1;
	int index = -1;

	srand(time(NULL));

	printf("[----- 2021041018 �忹�� -----]\n");
	
	do{
		printf("----------------------------------------------------------------\n");
		printf("                        Sorting & Hashing                       \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize       = z           Quit             = q\n");
		printf(" Selection Sort   = s           Insertion Sort   = i\n");
		printf(" Bubble Sort      = b           Shell Sort       = l\n");
		printf(" Quick Sort       = k           Print Array      = p\n");
		printf(" Hashing          = h           Search(for Hash) = e\n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initialize(&array);
			break;
		case 'q': case 'Q':
			freeArray(array);
			freeArray(hashtable);
			break;
		case 's': case 'S':
			selectionSort(array);
			break;
		case 'i': case 'I':
			insertionSort(array);
			break;
		case 'b': case 'B':
			bubbleSort(array);
			break;
		case 'l': case 'L':
			shellSort(array);
			break;
		case 'k': case 'K':
			printf("Quick Sort: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);
			quickSort(array, MAX_ARRAY_SIZE);
			printf("----------------------------------------------------------------\n");
			printArray(array);

			break;

		case 'h': case 'H':
			printf("Hashing: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);
			hashing(array, &hashtable);
			printArray(hashtable);
			break;

		case 'e': case 'E':
			printf("Your Key = ");		// �ؽ� ���̺��� ��ġ ã�� �� �Է�
			scanf("%d", &key);
			printArray(hashtable);
			index = search(hashtable, key);
			printf("key = %d, index = %d,  hashtable[%d] = %d\n", key, index, index, hashtable[index]);
			break;

		case 'p': case 'P':
			printArray(array);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

/* ����/�ؽ̿� �̿�� ���� �迭 ���� �Ҵ� �� �ʱ�ȭ */
int initialize(int** a)	// ���� ������ ���
{
	int *temp = NULL;

	/* array�� NULL�� ��� �޸� �Ҵ� */
	if(*a == NULL) {
		temp = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*a = temp;  /* �Ҵ�� �޸��� �ּҸ� ���� --> main���� �迭�� control �Ҽ� �ֵ��� ��*/
	} else
		temp = *a;

	/* �������� �迭�� ������ ���� */
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		temp[i] = rand() % MAX_ARRAY_SIZE;

	return 0;
}

/* ���� �Ҵ�� �迭 �Ҵ� ���� */
int freeArray(int *a)
{
	if(a != NULL)	// ���� �Ҵ�Ǿ��� �޸� ����
		free(a);
	return 0;
}

/* �迭 �� ��� */
void printArray(int *a)
{
	if (a == NULL) {
		printf("nothing to print.\n");
		return;
	}
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)	// �迭�� ���� �� ���
		printf("a[%02d] ", i);
	printf("\n");
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		printf("%5d ", a[i]);
	printf("\n");
}

/* ���� ���� */
int selectionSort(int *a)
{
	int min;
	int minindex;
	int i, j;

	printf("Selection Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);
	/* �ڽ� ���� ������ ���� �� ���� �ּ��� ���� ���� �ε����� ������� ����� �����Ѵ�. */
	for (i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		minindex = i;							// �񱳸� ���� ������ ����
		min = a[i];
		/* ���� ��ġ�� ���� ���� �ε��� ���� ���� ���� �ּڰ��� ã�´�. */
		for(j = i+1; j < MAX_ARRAY_SIZE; j++)
		{
			if (min > a[j])
			{
				min = a[j];
				minindex = j;
			}
		}
		/** ���� �ε��� ���� �ּڰ��� �ִ� �ε����� ����ְ�,
		 * �ּڰ��� ���� �ε����� ����ִ´�. (�ڸ� ��ü)*/
		a[minindex] = a[i];
		a[i] = min;
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);
	return 0;
}

/* ���� ���� */
int insertionSort(int *a)
{
	int i, j, t;

	printf("Insertion Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);
	/** ���� ���� �� ���� ��ġ ������ ū ���� �� ���� ��ġ�� ���� ��� �ڸ���ü�Ѵ�.
	 * ���� ��ġ ���� ���� ���ʿ� ���̻� �� ū ���� ���� ������ �ݺ��Ͽ� ���� ��ġ�� ����ȴ�.
	 * ���� ���ʿ������� ���ĵǾ� ������. */
	for(i = 1; i < MAX_ARRAY_SIZE; i++)
	{
		t = a[i];	// ���� �� ����
		j = i;
		/** ������ ���� ���� ������ ũ�ٸ� �ش� ���� ���� �ڸ��� �ִ´�(���������� �ű��.)
		 * �� ������ ������ ��� ���ؼ� ���� ������ ũ�ٸ� ���������� ���� �ű��. */
		while (a[j-1] > t && j > 0)
		{
			a[j] = a[j-1];
			j--;
		}
		/* ���� ���� ���������� ū ���� �ִ� ��ġ�� �����Ѵ�. (�ڸ� ��ü) */
		a[j] = t;
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

/* ���� ���� */
int bubbleSort(int *a)
{
	int i, j, t;

	printf("Bubble Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	/** �ѹ� ������ ���� ������ �ִ��� �迭�� ���� �������� ����.
	 * ���� �ѹ� ������ ���� ������ ���ϴ� ���� ���� �ϳ��� �پ���. */
	for(i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		for (j = 1; j < MAX_ARRAY_SIZE; j++) // j = 0 -> j = 1�� ����
		{	
			/* �ٷ� ������ ���� ���ؼ� ������ ���� �� ũ�ٸ� �ڸ���ü�Ѵ�. */
			if (a[j-1] > a[j])
			{
				t = a[j-1];
				a[j-1] = a[j];
				a[j] = t;
			}
		}
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

/* �� ���� */
int shellSort(int *a)
{
	int i, j, k, h, v;

	printf("Shell Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	/** ���� ������ ���� �������� �ڵ尡 ����ϴ�.
	 * h=1�� �� ������ �ݺ��� ������. (h=1�� �� h/2�� �ϸ� 0�� ��)
	*/
	for (h = MAX_ARRAY_SIZE/2; h > 0; h /= 2)
	{
		for (i = 0; i < h; i++)
		{
			for (j = i + h; j < MAX_ARRAY_SIZE; j += h)
			{
				/* hĭ�� ������ ���� ���Ͽ� ���� ������ hĭ ������(������) ���� ���� �� ũ�ٸ�
				 * �ش� ��ġ ���� ���� �� �ε����� ����ְ�,
				 * �׺��� �� hĭ ����(2hĭ ����)�� ���� ���� ���� ���Ͽ� �� �� ������ ���� ũ�ٸ�
				 * �� ���� �ռ��� hĭ �������� ���� ����ְ�...
				 * ������ hĭ��ŭ ������ ��ġ�鿡 �� ū ���� ���� ������ �ݺ� */
				v = a[j];
				k = j;
				while (k > h-1 && a[k-h] > v)
				{
					a[k] = a[k-h];
					k -= h;
				}
				/* ���������� �� ū ���� �־��� ��ġ�� ���� ���� ����ִ´�. */
				 a[k] = v;
			}
		}
	}
	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

/* �� ���� (recursive) */
int quickSort(int *a, int n)
{
	int v, t;
	int i, j;

	if (n > 1)
	{
		v = a[n-1];	// �Ǻ�: �� �� �������� ���� �������� ������.
		i = -1;
		j = n - 1;

		while(1)
		{
			while(a[++i] < v);	// �Ǻ����� �� ū ���� ������ ������ ���� = i
			while(a[--j] > v);	// �Ǻ����� �� ���� ���� ������ ������ ���� = j

			if (i >= j) break;
			/* i < j�ε� a[i] > a[j]�̹Ƿ� ��ü */
			t = a[i];
			a[i] = a[j];
			a[j] = t;
		}
		/* �Ǻ��� �Ǻ����� ū ���� ���� ���� ������� �α� ���� �ڸ� ��ü */
		t = a[i];
		a[i] = a[n-1];
		a[n-1] = t;

		/** �Ǻ����� ���� ����, �Ǻ����� ū �����ʿ� ���� ���������� ����
		 * ������ ��쿡�� �Ǻ��� �� �κп����� �� ������ �ε��� */
		quickSort(a, i);
		quickSort(a+i+1, n-i-1);
	}


	return 0;
}

/* �ؽ� �Լ�: ���� �Լ� */
int hashCode(int key) {
   return key % MAX_HASH_TABLE_SIZE;
}

/* �ؽ� ���̺� �� �ִ� �Լ� */
int hashing(int *a, int **ht)
{
	int *hashtable = NULL;

	/* hash table�� NULL�� ��� �޸� �Ҵ� */
	if(*ht == NULL) {
		hashtable = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*ht = hashtable;  /* �Ҵ�� �޸��� �ּҸ� ���� --> main���� �迭�� control �Ҽ� �ֵ��� ��*/
	} else {
		hashtable = *ht;	/* hash table�� NULL�� �ƴѰ��, table ��Ȱ��, reset to -1 */
	}

	/* �ؽ� ���̺� ��Ŷ�� ��� -1�� �ʱ�ȭ */
	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		hashtable[i] = -1;

	/*
	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		printf("hashtable[%d] = %d\n", i, hashtable[i]);
	*/

	int key = -1;
	int hashcode = -1;
	int index = -1;
	for (int i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		key = a[i];
		hashcode = hashCode(key);	// �ؽ� �Լ��� �ؽ� ���̺� ����� ��ġ ����
		/*
		printf("key = %d, hashcode = %d, hashtable[%d]=%d\n", key, hashcode, hashcode, hashtable[hashcode]);
		*/
		/* �ش� ��ġ ��Ŷ�� ����ٸ� �׳� �Ҵ� */
		if (hashtable[hashcode] == -1)
		{
			hashtable[hashcode] = key;
		/** ��� ���� �ʴٸ� ��Ŷ�� �ϳ��� ���Ը� �����ϹǷ� �浹 �� �����÷ΰ� �߻�
		 * ���� ��������� ��� ���� ��Ŷ ����, ���� ��� �ִ� ���� �Ҵ� */
		} else 	{

			index = hashcode;

			while(hashtable[index] != -1)
			{
				index = (++index) % MAX_HASH_TABLE_SIZE;
				/*
				printf("index = %d\n", index);
				*/
			}
			hashtable[index] = key;
		}
	}

	return 0;
}

/* �ؽ� ���̺��� �� ã�� �Լ� */
int search(int *ht, int key)
{
	/* �ؽ� �Լ��� �Էµ� ���� ��ġ ��� */
	int index = hashCode(key);

	/* �ؽ� �Լ��� ���� ��ġ�� ���� �ִٸ� ���� */
	if(ht[index] == key)
		return index;

	/* �� ��ġ�� ������ �����÷� �߻����� ���� ��� �������� �Űܰ��� �̾߱��̹Ƿ� 
	 * ���� ������ ��� ���� ��Ŷ �����ϸ鼭 ���� ��� �ִ��� �����ϰ� ���� */
	while(ht[++index] != key)
	{
		index = index % MAX_HASH_TABLE_SIZE;
	}
	return index;
}




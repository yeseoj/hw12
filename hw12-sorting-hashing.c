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

/* 필요에 따라 함수 추가 가능 */
int initialize(int **a);			// 정수 값들을 저장할 배열 동적 할당 및 초기화
int freeArray(int *a);				// 동적 할당된 배열 할당 해제
void printArray(int *a);			// 배열 출력

int selectionSort(int *a);			// 선택 정렬
int insertionSort(int *a);			// 삽입 정렬
int bubbleSort(int *a);				// 버블 정렬
int shellSort(int *a);				// 셸 정렬
/* recursive function으로 구현 */	
int quickSort(int *a, int n);		// 퀵 정렬


/* hash code generator, key % MAX_HASH_TABLE_SIZE */
int hashCode(int key);				// 해시 함수

/* array a에 대한 hash table을 만든다. */
int hashing(int *a, int **ht);		// 해시 테이블 할당

/* hash table에서 key를 찾아 hash table의 index return */
int search(int *ht, int key);		// 해시 테이블에서 키 위치 찾기


int main()
{
	char command;
	int *array = NULL;
	int *hashtable = NULL;
	int key = -1;
	int index = -1;

	srand(time(NULL));

	printf("[----- 2021041018 장예서 -----]\n");
	
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
			printf("Your Key = ");		// 해시 테이블에서 위치 찾을 값 입력
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

/* 정렬/해싱에 이용될 값의 배열 동적 할당 및 초기화 */
int initialize(int** a)	// 이중 포인터 사용
{
	int *temp = NULL;

	/* array가 NULL인 경우 메모리 할당 */
	if(*a == NULL) {
		temp = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*a = temp;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else
		temp = *a;

	/* 랜덤값을 배열의 값으로 저장 */
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		temp[i] = rand() % MAX_ARRAY_SIZE;

	return 0;
}

/* 동적 할당된 배열 할당 해제 */
int freeArray(int *a)
{
	if(a != NULL)	// 동적 할당되었던 메모리 해제
		free(a);
	return 0;
}

/* 배열 값 출력 */
void printArray(int *a)
{
	if (a == NULL) {
		printf("nothing to print.\n");
		return;
	}
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)	// 배열의 원소 값 출력
		printf("a[%02d] ", i);
	printf("\n");
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		printf("%5d ", a[i]);
	printf("\n");
}

/* 선택 정렬 */
int selectionSort(int *a)
{
	int min;
	int minindex;
	int i, j;

	printf("Selection Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);
	/* 자신 포함 뒤쪽의 값들 중 가장 최소인 값을 앞쪽 인덱스에 순서대로 끌어와 정렬한다. */
	for (i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		minindex = i;							// 비교를 위해 변수에 저장
		min = a[i];
		/* 현재 위치의 값을 뒤쪽 인덱스 값과 비교해 그중 최솟값을 찾는다. */
		for(j = i+1; j < MAX_ARRAY_SIZE; j++)
		{
			if (min > a[j])
			{
				min = a[j];
				minindex = j;
			}
		}
		/** 현재 인덱스 값을 최솟값이 있던 인덱스에 집어넣고,
		 * 최솟값을 현재 인덱스에 집어넣는다. (자리 교체)*/
		a[minindex] = a[i];
		a[i] = min;
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);
	return 0;
}

/* 삽입 정렬 */
int insertionSort(int *a)
{
	int i, j, t;

	printf("Insertion Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);
	/** 앞쪽 값들 중 현재 위치 값보다 큰 값은 그 다음 위치의 값과 계속 자리교체한다.
	 * 현재 위치 기존 값은 앞쪽에 더이상 더 큰 값이 없을 때까지 반복하여 멈춘 위치에 저장된다.
	 * 따라서 앞쪽에서부터 정렬되어 나간다. */
	for(i = 1; i < MAX_ARRAY_SIZE; i++)
	{
		t = a[i];	// 기존 값 저장
		j = i;
		/** 앞쪽의 값이 기존 값보다 크다면 해당 값을 기존 자리에 넣는다(오른쪽으로 옮긴다.)
		 * 그 앞쪽의 값과도 계속 비교해서 기존 값보다 크다면 오른쪽으로 값을 옮긴다. */
		while (a[j-1] > t && j > 0)
		{
			a[j] = a[j-1];
			j--;
		}
		/* 기존 값을 마지막으로 큰 값이 있던 위치에 저장한다. (자리 교체) */
		a[j] = t;
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

/* 버블 정렬 */
int bubbleSort(int *a)
{
	int i, j, t;

	printf("Bubble Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	/** 한번 정렬을 끝낼 때마다 최댓값이 배열의 가장 뒤쪽으로 간다.
	 * 따라서 한번 정렬을 끝낼 때마다 비교하는 원소 수도 하나씩 줄어든다. */
	for(i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		for (j = 1; j < MAX_ARRAY_SIZE; j++) // j = 0 -> j = 1로 수정
		{	
			/* 바로 직전의 값과 비교해서 직전의 값이 더 크다면 자리교체한다. */
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

/* 셸 정렬 */
int shellSort(int *a)
{
	int i, j, k, h, v;

	printf("Shell Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	/** 삽입 정렬의 향상된 버전으로 코드가 비슷하다.
	 * h=1이 될 때까지 반복해 나간다. (h=1일 때 h/2를 하면 0이 됨)
	*/
	for (h = MAX_ARRAY_SIZE/2; h > 0; h /= 2)
	{
		for (i = 0; i < h; i++)
		{
			for (j = i + h; j < MAX_ARRAY_SIZE; j += h)
			{
				/* h칸씩 떨어진 곳과 비교하여 기존 값보다 h칸 떨어진(이전의) 곳의 값이 더 크다면
				 * 해당 위치 값을 기존 값 인덱스에 집어넣고,
				 * 그보다 더 h칸 이전(2h칸 이전)의 곳과 기존 값을 비교하여 또 더 이전의 값이 크다면
				 * 그 값은 앞서서 h칸 떨어졌던 곳에 집어넣고...
				 * 앞쪽의 h칸만큼 떨어진 위치들에 더 큰 값이 없을 때까지 반복 */
				v = a[j];
				k = j;
				while (k > h-1 && a[k-h] > v)
				{
					a[k] = a[k-h];
					k -= h;
				}
				/* 마지막으로 더 큰 값이 있었던 위치에 기존 값을 집어넣는다. */
				 a[k] = v;
			}
		}
	}
	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

/* 퀵 정렬 (recursive) */
int quickSort(int *a, int n)
{
	int v, t;
	int i, j;

	if (n > 1)
	{
		v = a[n-1];	// 피봇: 이 값 기준으로 왼쪽 오른쪽이 나뉜다.
		i = -1;
		j = n - 1;

		while(1)
		{
			while(a[++i] < v);	// 피봇보다 더 큰 값이 나오기 시작한 지점 = i
			while(a[--j] > v);	// 피봇보다 더 작은 값이 나오기 시작한 지점 = j

			if (i >= j) break;
			/* i < j인데 a[i] > a[j]이므로 교체 */
			t = a[i];
			a[i] = a[j];
			a[j] = t;
		}
		/* 피봇을 피봇보다 큰 값과 작은 값의 경계점에 두기 위해 자리 교체 */
		t = a[i];
		a[i] = a[n-1];
		a[n-1] = t;

		/** 피봇보다 작은 왼쪽, 피봇보다 큰 오른쪽에 대해 독립적으로 정렬
		 * 각각의 경우에도 피봇은 각 부분에서의 맨 마지막 인덱스 */
		quickSort(a, i);
		quickSort(a+i+1, n-i-1);
	}


	return 0;
}

/* 해시 함수: 제산 함수 */
int hashCode(int key) {
   return key % MAX_HASH_TABLE_SIZE;
}

/* 해시 테이블에 값 넣는 함수 */
int hashing(int *a, int **ht)
{
	int *hashtable = NULL;

	/* hash table이 NULL인 경우 메모리 할당 */
	if(*ht == NULL) {
		hashtable = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*ht = hashtable;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else {
		hashtable = *ht;	/* hash table이 NULL이 아닌경우, table 재활용, reset to -1 */
	}

	/* 해시 테이블 버킷을 모두 -1로 초기화 */
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
		hashcode = hashCode(key);	// 해시 함수로 해시 테이블에 저장될 위치 지정
		/*
		printf("key = %d, hashcode = %d, hashtable[%d]=%d\n", key, hashcode, hashcode, hashtable[hashcode]);
		*/
		/* 해당 위치 버킷이 비었다면 그냥 할당 */
		if (hashtable[hashcode] == -1)
		{
			hashtable[hashcode] = key;
		/** 비어 있지 않다면 버킷에 하나의 슬롯만 존재하므로 충돌 및 오버플로가 발생
		 * 선형 조사법으로 계속 다음 버킷 조사, 그중 비어 있는 곳에 할당 */
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

/* 해시 테이블에서 값 찾는 함수 */
int search(int *ht, int key)
{
	/* 해시 함수로 입력된 값의 위치 계산 */
	int index = hashCode(key);

	/* 해시 함수로 계산된 위치에 값이 있다면 리턴 */
	if(ht[index] == key)
		return index;

	/* 그 위치에 없으면 오버플로 발생으로 인해 계속 다음으로 옮겨갔단 이야기이므로 
	 * 같은 원리로 계속 다음 버킷 조사하면서 값이 어디 있는지 조사하고 리턴 */
	while(ht[++index] != key)
	{
		index = index % MAX_HASH_TABLE_SIZE;
	}
	return index;
}




#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define THREADS_COUNT 3
#define SHOPS_COUNT 5

typedef struct
{
	int _products;
	pthread_mutex_t _mutex;
} Shop;

typedef struct
{
	int _shoping_list, _rank;
} Customer;

Shop list[SHOPS_COUNT];
pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;
short complete = 0;

void output()
{
	pthread_mutex_lock(&output_mutex);
	for (int i = 0; i < SHOPS_COUNT; i++)
	{
		printf("SHOP: %d\tPRODUCTS: %d\n", i + 1, list[i]._products);
	}
	printf("\n");
	pthread_mutex_unlock(&output_mutex);
}

void* deliverer_work()
{
	srand(time(NULL));
	while (complete != THREADS_COUNT)
	{
		for (int i = 0; i < SHOPS_COUNT; i++)
		{
			if (pthread_mutex_trylock(&list[i]._mutex) == 0)
			{
				list[i]._products += rand() % 5000 + 1000;
				sleep(2);
				pthread_mutex_unlock(&list[i]._mutex);
			}
		}
	}
	pthread_exit(NULL);
}
void* customer_work(void* args)
{
	Customer arg = (*(Customer*)(args));

	while (arg._shoping_list != 0)
	{
		for (int i = 0; i < SHOPS_COUNT; i++)
		{
			if (pthread_mutex_trylock(&list[i]._mutex) == 0 && list[i]._products != 0)
			{
				if (arg._shoping_list >= list[i]._products)
				{
					arg._shoping_list -= list[i]._products;
					list[i]._products = 0;
				}
				else
				{
					list[i]._products -= arg._shoping_list;
					arg._shoping_list = 0;
					pthread_mutex_lock(&output_mutex);
					complete++;
					pthread_mutex_unlock(&output_mutex);
				}
				sleep(3);
				pthread_mutex_unlock(&list[i]._mutex);
			}
		}
	}
	pthread_mutex_lock(&output_mutex);
	printf("%d\t%d\n", arg._rank, arg._shoping_list);
	pthread_mutex_unlock(&output_mutex);
	pthread_exit(NULL);
}
int main(void)
{
	srand(time(NULL));

	for (int i = 0; i < SHOPS_COUNT; i++)
	{
		list[i]._products = rand() % 5000 + 1000;
		pthread_mutex_init(&list[i]._mutex, NULL);
	}
	output();

	pthread_t deliverer;
	pthread_t customers[THREADS_COUNT];
	Customer args[THREADS_COUNT];

	for (int i = 0; i < THREADS_COUNT; i++)
	{
		args[i]._shoping_list = rand() % 10000 + 1000;
		args[i]._rank = i;
		printf("%d\t%d\n", args[i]._rank, args[i]._shoping_list);
		pthread_create(&customers[i], NULL, customer_work, (void*)(&args[i]));
	}
	pthread_create(&deliverer, NULL, deliverer_work, NULL);
	printf("\n");
	for (int j = 0; j < THREADS_COUNT; j++)
	{
		pthread_join(customers[j], NULL);
	}
	pthread_join(deliverer, NULL);
	pthread_exit(NULL);
	return 0;
}
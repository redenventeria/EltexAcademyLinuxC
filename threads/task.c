#include<stdio.h>
#include<threads.h>
#include<stdlib.h>
#include<malloc.h>
#include<time.h>
#include<math.h>

#define SHOP_N 5
#define CUSTOMER_N 3

mtx_t shop_mtx[SHOP_N];
int shop[SHOP_N];

thrd_t customer[CUSTOMER_N];
thrd_t restock;

int is_done = 0;
int total_start;
int total_eaten = 0;
int total_restocked = 0;
int total_left = 0;

int customer_func(void *a)
{
    int n = *(int*)a;
    int value = 90001 + rand() % 20000;
    struct timespec slp_time = {2, 0};
    while(value != 0)
    {
        for(size_t i = 0; i < SHOP_N; i++)
        {
            if(shop[i] == 0)
                continue;
            
            // Critical section.
            if(mtx_trylock(&shop_mtx[i]) != thrd_busy) {
                int eaten = fmin(value, shop[i]);
                value -= eaten;
                shop[i] -= eaten;
                printf("Customer %d ate %d in %luth shop, %d remaining\n", n, eaten, i, value);
                total_eaten += eaten;
                mtx_unlock(&shop_mtx[i]);
                break;
            }


        }
        thrd_sleep(&slp_time, NULL);
    }

    return 0;

}

int restock_func(void *a)
{
    struct timespec slp_time = {1, 0};
    while(!is_done)
    {
        for(size_t i = 0; i < SHOP_N; i++)
        {

            // Critical section
            if(mtx_trylock(&shop_mtx[i]) != thrd_busy) {
                int restock_value = 4001 + rand() % 2000;
                shop[i] += restock_value;
                total_restocked += restock_value;
                printf("Shop %lu restoked, new value is %d\n", i, shop[i]);
                mtx_unlock(&shop_mtx[i]);
                break;
            }


        }
        thrd_sleep(&slp_time, NULL);
    }

    return 0;
}

int main()
{
    int stat;
    for(int i = 0; i < SHOP_N; i++) {
        stat = mtx_init(&shop_mtx[i], mtx_plain);
        shop[i] = 9001 + rand() % 2000;
        total_start += shop[i];
    }

    int customer_num[CUSTOMER_N];
    for(int i = 0; i < CUSTOMER_N; i++)
    {
        customer_num[i] = i;
        thrd_create(&customer[i], customer_func, &customer_num[i]);
    }
    thrd_create(&restock, restock_func, NULL);


    int res = 0;
    for(int i = 0; i < CUSTOMER_N; i++)
    {
        thrd_join(customer[i], &res);
    }
    
    is_done = 1;
    thrd_join(restock, &res);

    for(int i = 0; i < SHOP_N; i++)
        total_left += shop[i];

    printf("\nEnd results:\nstart: %d\neaten: %d\nrestocked: %d\nleft: %d\n", \
           total_start, total_eaten, total_restocked, total_left);
    
    printf("start + restocked - eaten = left\n");
    printf("%d + %d - %d = %d\n", total_start, total_restocked, total_eaten, total_left);
    printf("%d = %d\n", total_start + total_restocked - total_eaten, total_left);
    return 0;
}
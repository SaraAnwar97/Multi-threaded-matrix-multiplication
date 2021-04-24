#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define N 100
int r1, c1,r2,c2,i,j,k;
int array1[N][N];
int array2[N][N];
int result1[N][N];
int result2[N][N];
int main(){
    FILE* f;
    f = fopen("input.txt", "r");
    fscanf(f, "%d%d", &r1, &c1);
    for(i=0; i<r1; i++)
        for(j=0; j<c1; j++)
            fscanf(f, "%d", &array1[i][j]);
    
    fscanf(f, "%d%d", &r2, &c2);
    if (c1 != r2 ){
        printf("Matrices can't be multiplied with each other\n");
        exit(1);
    }
    for(i=0; i<r2; i++)
        for(j=0; j<c2; j++)
            fscanf(f, "%d", &array2[i][j]);
    fclose(f);
    
    
    printf("Matrix 1:\n");
    for(i=0; i<r1; i++){
        for(j=0; j<c1; j++)
            printf ("%d\t", array1[i][j]);
        printf("\n");
    }
    printf("Matrix 2:\n");
    for(i=0; i<r2; i++){
        for(j=0; j<c2; j++)
            printf ("%d\t", array2[i][j]);
        printf("\n");
    }
    
    
    for(i=0; i<r1; ++i)
        for(j=0; j<c2; ++j)
        {
            result1[i][j] = 0;
        }
    
    int nthreads=r1;
    void *threadmul1(void *arg)
    {     int *p=(int *)arg;
        for(i=*p;i<(*p+(N/nthreads));i++)
            for(j=0; j<c2; j++)
                for(k=0; k<r2; k++)
                    result1[i][j]+=array1[i][k]*array2[k][j];
    }
    int rownos[nthreads];
    pthread_t tid[nthreads];
    
    clock_t start1, end1;
    double cpu_time_used1;
    start1 = clock();
    for(i=0;i<nthreads;i++)
    { rownos[i]=i*(N/nthreads);
        pthread_create(&tid[i],NULL,threadmul1,&rownos[i]);
    }
    for(i=0;i<nthreads;i++)
        pthread_join(tid[i],NULL);
    end1 = clock();
    cpu_time_used1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC;
    int nthreads2=r1*c2;
    
    void *threadmul2(void *arg)
    {
        int *p=(int *)arg;
        for(i=*p;i<r1;i++)
            for(j=0; j<c2; j++)
                for(k=0; k<r2; k++)
                    result2[i][j]+=array1[i][k]*array2[k][j];
    }
    int threads[nthreads2];
    pthread_t tid2[nthreads2];
    
    clock_t start2, end2;
    double cpu_time_used2;
    start2 = clock();
    for(i=0;i<nthreads2;i++)
    { threads[i]=i*(nthreads2);
        pthread_create(&tid2[i],NULL,threadmul2,&threads[i]);
    }
    for(i=0;i<nthreads2;i++)
        pthread_join(tid2[i],NULL);
    end2 = clock();
    cpu_time_used2 = ((double) (end2 - start2)) / CLOCKS_PER_SEC;
    FILE *f2;
    f2 = fopen("output.txt", "w");
    fprintf(f2,"Output Matrix 1:\n");
    for(i=0; i<r1; i++) {
        for(j=0; j<c2; j++)
        {
            fprintf(f2,"%d\t ", result1[i][j]);
        }
        fprintf(f2,"\n");}
    fprintf(f2,"threadmul1() took %f seconds to execute \n", cpu_time_used1);
    fprintf(f2,"Output Matrix 2:\n");
    for(i=0; i<r1; i++) {
        for(j=0; j<c2; j++)
        {
            fprintf(f2,"%d\t ", result2[i][j]);
        }
        fprintf(f2,"\n");}
    fprintf(f2,"threadmul2() took %f seconds to execute \n", cpu_time_used2);
    return 0;
}

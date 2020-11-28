//
// Created by yann on 2020/11/27.
//

#include <memory.h>
#include "stdio.h"

//typedef int (*Comparator)(int a, int b);
//
//void swap02(int *A, int i, int j)
//{
//    int temp;
//    temp=A[i];
//    A[i]=A[j];
//    A[j]=temp;
//}
//
//int partition02(int* A, int left, int right, Comparator comparator1, Comparator comparator2 )
//{
//    int i,j;
//    int pivo = A[ (left+right)/2 ];
//
//    i = left; j = right;
//    while( 1 )
//    {
//        while( comparator1(A[i], pivo)){ i++; }
//        while( comparator2(A[j], pivo) ){ j--; }
//        if ( i < j )
//            swap02(A, i, j);
//        else
//            break;
//    }
//    return i;
//}
//
//void quisort(int* A, int start, int end, Comparator comparator1, Comparator comparator2)
//{
//    int i;
//    if( end > start )
//    {
//        i = partition02( A, start, end, comparator1, comparator2);
//        quisort( A, start, i - 1,comparator1, comparator2 );
//        quisort( A, i + 1, end, comparator1, comparator2);
//    }
//}
//
//int _ascending (int a, int b) { return b<a; }
//int _descending (int a, int b) { return b>a; }
//
//int main01() {
//
//
//    int A[10] = {72, 6, 57, 88, 60, 42, 83, 73, 48, 85};
////    quisort(A, 0, 9, 10);
//    return 0;
//}

int main01(){

    char c[] = "eat,eat";
    char *tmp1;
    char *tmp2;
    tmp1 = strtok(c, ",");
    tmp2 = strtok(NULL, ",");
    if (!strcmp(tmp1, tmp2))
        printf("%s, %s, %d", tmp1, tmp2, strcmp(tmp2, tmp1));

    printf("%d", 8/3);
}

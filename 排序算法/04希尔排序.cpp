#include<stdio.h>  
#include<stdlib.h>  
#include<iostream>

using namespace std;

/*
希尔排序（Shell Sort）
第一个突破O(n^2)的排序算法；是简单【插入排序】的改进版；它与插入排序的不同之处在于，它会优先比较距离较远的元素。希尔排序又叫缩小增量排序

(1)算法简介

希尔排序的核心在于间隔序列的设定。既可以提前设定好间隔序列，也可以动态的定义间隔序列。
动态定义间隔序列的算法是《算法（第4版》的合著者Robert Sedgewick提出的。
(2)算法描述和实现

先将整个待排序的记录序列分割成为若干子序列分别进行直接插入排序，具体算法描述：
<1>.选择一个增量序列t1，t2，…，tk，其中ti>tj，tk=1；
<2>.按增量序列个数k，对序列进行k 趟排序；
<3>.每趟排序，根据对应的增量ti，将待排序列分割成若干长度为m 的子序列，分别对各子表进行直接插入排序。
	仅增量因子为1 时，整个序列作为一个表来处理，表长度即为整个序列的长度。
*/
static int len = 0;
void print(const char* str, const int* arr){
	cout<<str<<endl;
	for(int i = 0; i < len; i++){
		cout<<arr[i]<<" ";
	}
	cout<<endl;
}
int main()  
{  
	int arr[] = {3,44,38,5,47,15,36,26,27,2,46,4,19,50,48};
	len = sizeof(arr)/sizeof(int);
	print("===================== 初始化数组:",arr);
	int temp,change,i,j;
	int total = 0;
	for(int div = len / 2;div >= 1; div=div / 2){
		for(i = div; i < len; i++){
			temp = arr[i];
			j = i - div;
			while(j >= 0 && arr[j] > temp){
				arr[j+div] = arr[j];
				j = j - div;
				total++;
			}
			arr[j+div] = temp;
		}
	}
	print("===================== 1.排序=>:",arr);
	cout<<"遍历次数:"<<total<<endl;
    return 0;  
} 


/*
运行结果：
liaohw@ubuntu:~/sort$ ./a.out 
===================== 初始化数组:
3 44 38 5 47 15 36 26 27 2 46 4 19 50 48 
===================== 1.排序=>:
2 3 4 5 15 19 26 27 36 38 44 46 47 48 50 
遍历次数:16
*/
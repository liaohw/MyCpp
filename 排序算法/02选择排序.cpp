#include<stdio.h>  
#include<stdlib.h>  
#include<iostream>

using namespace std;

/*
(1)算法简介

选择排序(Selection-sort)是一种简单直观的排序算法。
它的工作原理：
首先在未排序序列中找到最小（大）元素，存放到排序序列的起始位置，
然后再从剩余未排序元素中继续寻找最小（大）元素，然后放到已排序序列的末尾。
以此类推，直到所有元素均排序完毕。
(2)算法描述和实现

n个记录的直接选择排序可经过n-1趟直接选择排序得到有序结果。具体算法描述如下：
<1>.初始状态：无序区为R[1..n]，有序区为空；
<2>.第i趟排序(i=1,2,3...n-1)开始时，当前有序区和无序区分别为R[1..i-1]和R(i..n）。该趟排序从当前无序区中-选出关键字最小的记录 R[k]，将它与无序区的第1个记录R交换，使R[1..i]和R[i+1..n)分别变为记录个数增加1个的新有序区和记录个数减少1个的新无序区；
<3>.n-1趟结束，数组有序化了。

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
	int total = 0;
	int change = 0;
	int min,temp;
	for(int i = 0; i < len - 1; i++){
		min = i;
		for(int j = i + 1; j < len; j++){
			if(arr[min] > arr[j]){
				min = j;
			}
			total++;
		}
		temp = arr[i]; arr[i] = arr[min]; arr[min] = temp;
		change++;
	}
	print("===================== 1.排序=>:",arr);
	cout<<"遍历次数:"<<total<<", 交换次数:"<<change<<endl;
    return 0;  
} 


/*
运行结果：
liaohw@ubuntu:~/sort$ ./a.out            
===================== 初始化数组:
3 44 38 5 47 15 36 26 27 2 46 4 19 50 48 
===================== 1.排序=>:
2 3 4 5 15 19 26 27 36 38 44 46 47 48 50 
遍历次数:105, 交换次数:14
*/
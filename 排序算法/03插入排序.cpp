#include<stdio.h>  
#include<stdlib.h>  
#include<iostream>

using namespace std;

/*
(1)算法简介

插入排序（Insertion-Sort）的算法描述是一种简单直观的排序算法。
它的工作原理是通过构建有序序列，对于未排序数据，在已排序序列中从后向前扫描，找到相应位置并插入。
插入排序在实现上，通常采用in-place排序（即只需用到O(1)的额外空间的排序），
因而在从后向前扫描过程中，需要反复把已排序元素逐步向后挪位，为最新元素提供插入空间。
(2)算法描述和实现

一般来说，插入排序都采用in-place在数组上实现。具体算法描述如下：
<1>.从第一个元素开始，该元素可以认为已经被排序；
<2>.取出下一个元素，在已经排序的元素序列中从后向前扫描；
<3>.如果该元素（已排序）大于新元素，将该元素移到下一位置；
<4>.重复步骤3，直到找到已排序的元素小于或者等于新元素的位置；
<5>.将新元素插入到该位置后；
<6>.重复步骤2~5。
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
	int temp,total,change;
	{
		int arr[] = {3,44,38,5,47,15,36,26,27,2,46,4,19,50,48};
		len = sizeof(arr)/sizeof(int);
		print("===================== 初始化数组:",arr);
		total = change = 0;
		for(int i = 1; i < len; i++){
			temp = arr[i];
			int j = i - 1;
			while(j >= 0 && arr[j] > temp){
				arr[j+1] = arr[j];
				j--;
				total++;
				change++;
			}
			arr[j+1] = temp;
			change++;
		}
		print("===================== 1.排序=>:",arr);
		cout<<"遍历次数:"<<total<<", 交换次数:"<<change<<endl;
	}

	/*
	改进排序： 
		 查找插入位置时使用二分查找的方式
	*/
	{
		int arr[] = {3,44,38,5,47,15,36,26,27,2,46,4,19,50,48};
		len = sizeof(arr)/sizeof(int);
		total = change = 0;
		int left,right;
		for(int i = 1; i < len; i++){
			temp = arr[i]; left = 0; right = i - 1;
			while(left <= right){
				int mid = (left + right) / 2;
				if(temp < arr[mid]){
					right = mid - 1;
				}
				else{
					left = mid + 1;
				}
				total++;
			}
			int j = i - 1;
			while(j >= left){
				arr[j+1] = arr[j];
				j--;
				change++;
			}
			arr[left] = temp;
			change++;
		}
		print("===================== 2.排序(二分查找优化)=>:",arr);
		cout<<"遍历次数:"<<total<<", 交换次数:"<<change<<endl;
	}

	
    return 0;  
} 


/*
运行结果：
liaohw@ubuntu:~/sort$ ./a.out 
===================== 初始化数组:
3 44 38 5 47 15 36 26 27 2 46 4 19 50 48 
===================== 1.排序=>:
2 3 4 5 15 19 26 27 36 38 44 46 47 48 50 
遍历次数:44, 交换次数:58
===================== 2.排序(二分查找优化)=>:
2 3 4 5 15 19 26 27 36 38 44 46 47 48 50 
遍历次数:43, 交换次数:58
*/
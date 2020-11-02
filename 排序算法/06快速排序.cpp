#include<stdio.h>  
#include<stdlib.h>  
#include<iostream>

using namespace std;

/*
(1)算法简介
快速排序的基本思想：通过一趟排序将待排记录分隔成独立的两部分，其中一部分记录的关键字均比另一部分的关键字小，
则可分别对这两部分记录继续进行排序，以达到整个序列有序。

(2)算法描述和实现
快速排序使用分治法来把一个串（list）分为两个子串（sub-lists）。具体算法描述如下：
<1>.从数列中挑出一个元素，称为 "基准"（pivot）；
<2>.重新排序数列，所有元素比基准值小的摆放在基准前面，所有元素比基准值大的摆在基准的后面（相同的数可以到任一边）。
	在这个分区退出之后，该基准就处于数列的中间位置。这个称为分区（partition）操作；
<3>.递归地（recursive）把小于基准值元素的子数列和大于基准值元素的子数列排序。

*/
static int len = 0;
void print(const char* str, const int* arr){
	cout<<str<<endl;
	for(int i = 0; i < len; i++){
		cout<<arr[i]<<" ";
	}
	cout<<endl;
}
// 3,44,38,5,47,15
// 3,|44,38,5,15,47
// 3,|15,38,5,|44,47
void quickSort(int arr[], int left, int right){
	if(left >= right) return;
	int data = arr[left];
	int i = left + 1;
	int j = right;
	while(i != j){
		while(arr[j] > data && i < j){
			j--;
		}
		while(arr[i] < data && i < j){
			i++;
		}
		if(i < j){
			int temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
	}
	if(data > arr[i]){
		arr[left] = arr[i];
		arr[i] = data;
	}

	quickSort(arr,left,i-1);
	quickSort(arr,i,right);
}
int main()  
{  
	int arr[] = {3,44,38,5,47,15,36,26,27,2,46,4,19,50,48};
	len = sizeof(arr)/sizeof(int);
	print("===================== 初始化数组:",arr);	
	quickSort(arr,0,len-1);
	
	print("===================== 1.排序=>:",arr);
    return 0;  
} 


/*
运行结果：
liaohw@ubuntu:~/sort$ ./a.out 
===================== 初始化数组:
3 44 38 5 47 15 36 26 27 2 46 4 19 50 48 
===================== 1.排序=>:
2 3 4 5 15 19 26 27 36 38 44 46 47 48 50 
*/
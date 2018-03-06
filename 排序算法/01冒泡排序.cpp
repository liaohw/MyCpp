#include<stdio.h>  
#include<stdlib.h>  
#include<iostream>

using namespace std;

/*
(1)算法描述

冒泡排序是一种简单的排序算法。它重复地走访过要排序的数列，一次比较两个元素，如果它们的顺序错误就把它们交换过来。
走访数列的工作是重复地进行直到没有再需要交换，也就是说该数列已经排序完成。
这个算法的名字由来是因为越小的元素会经由交换慢慢“浮”到数列的顶端。

(2)算法描述和实现

具体算法描述如下：
<1>.比较相邻的元素。如果第一个比第二个大，就交换它们两个；
<2>.对每一对相邻元素作同样的工作，从开始第一对到结尾的最后一对，这样在最后的元素应该会是最大的数；
<3>.针对所有的元素重复以上的步骤，除了最后一个；
<4>.重复步骤1~3，直到排序完成。
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
		for(int i = 0; i < len; i++){
			for(int j = 0; j < len - i - 1; j++){
				if(arr[j] > arr[j+1]){
					temp = arr[j+1]; arr[j+1] = arr[j]; arr[j] = temp;
					change++;
				}
				total++;
			}
		}
		print("===================== 1.冒泡排序=>:",arr);
		cout<<"遍历次数:"<<total<<", 交换次数:"<<change<<endl;
	}

	/*
	改进冒泡排序： 
		设置一标志性变量pos,用于记录每趟排序中最后一次进行交换的位置。
		由于pos位置之后的记录均已交换到位,故在进行下一趟排序时只要扫描到pos位置即可。
	*/
	{
		int arr[] = {3,44,38,5,47,15,36,26,27,2,46,4,19,50,48};
		len = sizeof(arr)/sizeof(int);
		total = change = 0;
		int i = len -1;
		while(i > 0){
			int pos = 0;
			for(int j = 0; j < i; j++){
				if(arr[j] > arr[j+1]){
					pos = j;
					temp = arr[j+1]; arr[j+1] = arr[j]; arr[j] = temp;
					change++;
				}
				total++;
			}
			i = pos;
		}
		print("===================== 2.冒泡排序optimized=>:",arr);
		cout<<"遍历次数:"<<total<<", 交换次数:"<<change<<endl;
	}

	
	/*
	进一步优化：
		每趟排序中进行正向和反向两遍冒泡的方法一次可以得到两个最终值(最大者和最小者)。
	*/
	{
		int arr[] = {3,44,38,5,47,15,36,26,27,2,46,4,19,50,48};
		len = sizeof(arr)/sizeof(int);
		total = change = 0;
		int high = len -1;
		int low = 0;
		int pos;
		while(low < high){
			pos = 0;
			for(int j = low; j < high; j++){
				if(arr[j] > arr[j+1]){
					pos = j;
					temp = arr[j+1]; arr[j+1] = arr[j]; arr[j] = temp;
					change++;
				}
				total++;
			}
			high = pos;
			for(int j = high; j > low; j--){
				if(arr[j] < arr[j-1]){
					pos = j;
					temp = arr[j];  arr[j] = arr[j-1]; arr[j-1] = temp;
					change++;
				}
				total++;
			}
			low = pos;
		}
		print("===================== 3.冒泡排序双项optimized=>:",arr);
		cout<<"遍历次数:"<<total<<", 交换次数:"<<change<<endl;
	}
    return 0;  
} 


/*
运行结果：
liaohw@ubuntu:~/sort$ ./a.out 
===================== 初始化数组:
3 44 38 5 47 15 36 26 27 2 46 4 19 50 48 
===================== 1.冒泡排序=>:
2 3 4 5 15 19 26 27 36 38 44 46 47 48 50 
遍历次数:105, 交换次数:44
===================== 2.冒泡排序optimized=>:
2 3 4 5 15 19 26 27 36 38 44 46 47 48 50 
遍历次数:77, 交换次数:44
===================== 3.冒泡排序双项optimized=>:
2 3 4 5 15 19 26 27 36 38 44 46 47 48 50 
遍历次数:63, 交换次数:44
*/
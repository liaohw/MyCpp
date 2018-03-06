#include<stdio.h>  
#include<stdlib.h>  
#include<iostream>

using namespace std;

/*
和选择排序一样，归并排序的性能不受输入数据的影响，但表现比选择排序好的多，因为始终都是O(n log n）的时间复杂度。代价是需要额外的内存空间。

(1)算法简介
归并排序是建立在归并操作上的一种有效的排序算法。该算法是采用分治法（Divide and Conquer）的一个非常典型的应用。
归并排序是一种稳定的排序方法。将已有序的子序列合并，得到完全有序的序列；即先使每个子序列有序，再使子序列段间有序。
若将两个有序表合并成一个有序表，称为2-路归并。

(2)算法描述和实现
具体算法描述如下：
<1>.把长度为n的输入序列分成两个长度为n/2的子序列；
<2>.对这两个子序列分别采用归并排序；
<3>.将两个排序好的子序列合并成一个最终的排序序列。
*/
static int len = 0;
void print(const char* str, const int* arr){
	cout<<str<<endl;
	for(int i = 0; i < len; i++){
		cout<<arr[i]<<" ";
	}
	cout<<endl;
}
void merge(int arr[], int left, int mid, int right){
	
}
void mergeSort(int arr[],int left,int right){
	if(left >= right) return;
	int mid = (left + right) / 2;
	mergeSort(arr,left,mid);
	mergeSort(arr,mid+1,right);
	merge(arr,left,mid,right);
}
int main()  
{
	int arr[] = {3,44,38,5,47,15,36,26,27,2,46,4,19,50,48};
	len = sizeof(arr)/sizeof(int);
	print("===================== 初始化数组:",arr);	
	mergeSort(arr,0,len-1);
	
	print("===================== 1.排序=>:",arr);
	cout<<"遍历次数:"<<total<<", 交换次数:"<<change<<endl;
    return 0;  
} 


/*
运行结果：

*/
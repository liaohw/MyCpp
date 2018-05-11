#include<stdio.h>  
#include<stdlib.h>  
#include<iostream>

using namespace std;

/*
(1)算法简介
堆排序（Heapsort）是指利用堆这种数据结构所设计的一种排序算法。
堆积是一个近似完全二叉树的结构，并同时满足堆积的性质：即子结点的键值或索引总是小于（或者大于）它的父节点。

(2)算法描述和实现
具体算法描述如下：
<1>.将初始待排序关键字序列(R1,R2....Rn)构建成大顶堆，此堆为初始的无序区；
<2>.将堆顶元素R[1]与最后一个元素R[n]交换，此时得到新的无序区(R1,R2,......Rn-1)和新的有序区(Rn),且满足R[1,2...n-1]<=R[n]；
<3>.由于交换后新的堆顶R[1]可能违反堆的性质，因此需要对当前无序区(R1,R2,......Rn-1)调整为新堆，然后再次将R[1]与无序区最后一个元素交换，
	得到新的无序区(R1,R2....Rn-2)和新的有序区(Rn-1,Rn)。
	不断重复此过程直到有序区的元素个数为n-1，则整个排序过程完成。
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

*/
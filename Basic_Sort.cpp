

/*************************
 *冒泡排序
 *选择排序
 *插入排序
	-----·直接插入
	-----·折半插入
 *堆排序
 *快速排序
	-----·begin和end（递归和栈）‘
	-----·prev和cur（递归和栈）
	-----·三数取中
	-----·区间数数量达到一定个数，就使用插入排序
 *希尔排序
 *归并排序
 *计数排序
 *基数排序
	-----·最高位优先MSD
	-----·最低位优先LSD
 *************************/

#include <iostream>
#include <assert.h>
#include<stack>
using namespace std;

/*#################<<<<《冒泡排序》>>>>############################*/
/*时间复杂度 N^2  */
void BubbleSort(int *array, int size)
{
	assert(array && size > 0);
	int i, j;
	bool exchange = false;
	for (i = 0; i < size - 1; ++i)         //控制比较所少轮回
	{
		for (j = 0; j < size - i - 1; ++j) //控制每一轮回的比较
		{
			if (array[j]>array[j + 1])
			{
				swap(array[j], array[j + 1]);
				exchange = true;
			}
		}
		if (exchange == false)
		{//说明没有发生变换，是有序的序列
			return;
		}
	}
}
/*#################<<<<《选择排序》>>>>############################*/
/*时间复杂度：N^2 */
void SelectSort(int* array,int size)
{
	assert(array && size > 0);
	int begin = 0;
	int end = size - 1;
	int min,max,left, right;
	for (; begin < size-1,end>=0; ++begin,--end)
	{
		min = begin;
		max = end;
		left = begin;
		right = end;
		while (left<right)//当只有一个数据，则不必要排序
		{ //9 4 8 5 0--0 4 8 5 9
			if (array[min]>array[left])
			{
				swap(array[min], array[left]);
			}
			if (array[max] < array[left])
			{
				swap(array[max], array[left]);
			}
			++left;
		}
	}
}
/*#################<<<<《直接插入排序》>>>>########################*/
/*时间复杂度为 N^2 */
void InsertSort(int* array, int size)
{
	assert(array && size > 0);
	int begin,end,tmp;
	for (begin = 1; begin < size; ++begin)
	{
		end = begin - 1;
		tmp = array[begin];
		while (end >= 0 && tmp < array[end])
		{
			array[end + 1] = array[end];
			--end;
		}
		if (array[end + 1] != tmp)  //自己加上去的，没必要自己给自己赋值，
		{						    //即使数组中含有多个相同元素也可以，如：9 0 4 3 8 8 2 8  
			array[end + 1] = tmp;
		}
	}
}
/*#################<<<<《折半插入排序》>>>>########################*/
/*时间复杂度为 N^2 */
void InsertSort_Half(int *array, int size)
{
	assert(array && size > 0);
	int begin,tmp,left,right,mid,k;
	for (begin = 1; begin < size; begin++)
	{
		tmp = array[begin];
		left = 0;
		right = begin - 1;
		while (left <= right)
		{
			mid = left + (right-left) / 2;  //此处不可以是(left+right-left)/2；否则会在left = mid + 1; 
			if (array[mid] < tmp)      //陷入死循环，(left+right-left)/2+1也不可以
			{
				left = mid + 1; 
			}
			else
			{
				right = mid - 1;
			}
		}
		for (k = begin - 1; k >= left; --k)
		{
			array[k + 1] = array[k];
		}
		array[left] = tmp;
	}
}
/*#################<<<<《堆排序》>>>>##############################*/
/*
*时间复杂度  N*logN + N*logN = 2N*logN 常数级省略简等于 N*logN
（最坏情形是有N个数可能调整，假设每次每个数都需要从根节点开始折半调整，即高度）
*应用：随机访问前提下
*/
//（向下调整）：时间复杂度 N*logN 
void seftDown(int* array, int size, int root)
{
	if (array == NULL || size <= 0 || root < 0 || root>size - 1)
	{
		return;
	}
	int left = root * 2 + 1;
	while (left < size)   //给的是左闭右开区间，如果左边越界，那么右边就肯定也越界了
	{
		int right = left + 1;
		int max = left;
		if (right<size && array[max]<array[right])
		{
			max = right;
		}
		if (array[max] > array[root])
		{
			swap(array[max], array[root]);
			root = max;
			left = root * 2 + 1;
		}
		else
		{
			break;
		}
	}
}
void Heap(int* array, int size) //堆排序
{
	if (array == NULL || size <= 0)
	{
		return;
	}
	int begin;
	for (begin = size / 2 - 1; begin >= 0; --begin)//第一次向下调整
	{
		seftDown(array, size, begin);
	}
	int end = size-1;
	while (end)
	{
		swap(array[0], array[end]);
		seftDown(array,end,0);
		//由于之前已经调整过了，此时只需要从根节点向下与头节点进行比较就好
		//其他已经成堆了
		--end;
	}
}


/*#################<<<<《快速排序》>>>>###########################*/
/*：时间复杂度：N*logN（最坏情形是已经有序地序列或者接近有序）适用范围广，比较快*/

/**************************************************
 *方法一：递归和非递归的实现（begin和end方式）
 *方法二：递归和非递归的实现（prev和cur方式）
 *方法三：三数取中
 *方法四：当区间的数达到一定的数量，就采用插入排序
 *************************************************/

//方法一：递归实现（一种划分算法的实现）
void _QuickSort1(int* array, int left,int right)
{
	int key = array[right];
	int begin = left;     //从左到右找比array[key]大的数与array[begin]交换
	int end = right-1;    //从右到左找比array[key]小的数与array[end]交换
	if (left >= right)
	{
		return;
	}
	while (begin<end)
	{
		while (begin < end && array[begin] <= key) //等号处理相等元素
		{
			++begin;
		}
		while (begin < end && array[end] >= key)
		{
			--end;
		}
		if (begin < end)
		{
			swap(array[begin], array[end]);
			++begin;
			--end;
		}
	}
	if (array[begin] < key)  //最后begin停的下标对应的元素不一定是大于key的元素
	{						 //升序的时候（0123456789），最后begin和end同指向8，不应该直接将8和9交换
		begin++;             //begin++之后begin变成了right，
	}
	swap(array[begin], array[right]);//最后把array[right]交换到中间
	_QuickSort1(array, left, begin - 1); //递归求左子问题（闭区间）
	_QuickSort1(array, begin+1,right);//递归求右子问题（闭区间）
}
//非递归实现：压栈
int _NonR_QuickSort1(int *array, int left, int right)
{										//在NonR_QuickSort1_or_2函数中调用
	assert(array);
	int key = array[right];
	int begin = left;
	int end = right - 1;
	while (begin < end)
	{
		while (begin < end && array[begin] <= key)
		{
			++begin;
		}
		while (begin < end && array[end] >= key)
		{
			--end;
		}
		if (begin < end)
		{
			swap(array[begin],array[end]);
			++begin;
			--end;
		}
	}
	if (array[begin] < key)
	{
		++begin;
	}
	swap(array[begin],array[right]);
	return begin;
}

//三数取中实现
int GetMidIndex(int* array, int left, int right)
{
	int mid = left + (right - left) / 2;
	int k = left;
	if (array[mid] < array[k])
	{
		k = mid;
	}
	if (array[right] < array[k])//找出最小元素下标
	{
		k = right;
	}
	if (k != left)
	{
		swap(array[k], array[left]);
	}
	if (mid != right && array[mid] < array[right])
	{
		swap(array[mid], array[right]);
	}
	return array[right];
}
//方法二：递归实现：时间复杂度 N*logN 应用单链表排序（要求时间复杂度为 N*logN ）
void _QuickSort2(int* array, int left, int right)
{
	assert(array);
	if (left >= right)
	{
		return;
	}
	//方法三：优化：三数取中法
	//int key = GetMidIndex(array, left, right);
	//方法四：再优化：当区间的数达到一定的数量（13，如果给的太大，就类似插入排序了），就采用插入排序
	if (right - left < 3)
	{
		InsertSort(array + left, right - left + 1);
		//递归区间可能是下标5到right，所以相应的插入排序也是在此区间进行
		return;
	}
	int key = array[right];
	int prev = left - 1;
	int cur = left;
	while (cur < right)
	{
		if (array[cur] < key)
		{
			++prev;
			if (prev != cur)
			{
				swap(array[prev], array[cur]);
			}
		}
		++cur;
	}
	while (array[prev] < array[right])//最坏情形是当prev走到right就停止
	{ //9, 5, 0, 4, 2, 3, 4, 8, 5, 7 ：第一轮比较交换到5042345897时，
		//cur指向9，prev指向最后一个5，此时直接执行swap(array[right], array[prev]);
		//还不能达到左边元素都比7小，右边元素都比7大，所以prev要向右边找到一个大于等于7的数
		//才能与array[right]进行交换
		++prev;
	}
	swap(array[right], array[prev]);
	_QuickSort2(array, left, prev - 1);
	_QuickSort2(array, prev + 1, right);
}
//非递归实现：（处理递归函数栈溢出的问题）
int _NonR_QuickSort2(int* array, int left, int right)
{                                //在NonR_QuickSort1_or_2函数中调用
	assert(array);
	if (left >= right)
	{
		return -1;
	}
	int key = array[right];
	int cur = left;
	int prev = left - 1;
	while (cur < right)
	{
		if (array[cur] < key)
		{
			++prev;
			if (prev != cur)
			{
				swap(array[prev], array[cur]);
			}
		}
		++cur;
	}
	while (array[prev] < array[right])
	{
		++prev;
	}
	swap(array[right], array[prev]);
	return prev;
}
//压栈
void NonR_QuickSort1_or_2(int* array, int size)
{
	assert(array && size > 0);
	stack<int> s;
	s.push(size - 1);//右边界
	s.push(0);     //左边界
	int left;
	int right;
	while (!s.empty())
	{
		left = s.top();
		s.pop();
		right = s.top();
		s.pop();
		int key = _NonR_QuickSort2(array, left, right);//key为每一轮比较后prev的下标
		if (key - 1 > left)  //当左子问题有两个或者两个以上的数才进行插入
		{
			s.push(key - 1);
			s.push(left);
		}
		if (right - 1 > key) //当右子问题有两个或者两个以上的数才进行插入
		{
			s.push(right);
			s.push(key + 1);
		}
	}
}

/*#################<<<<《希尔排序》>>>>############################*/
// ：时间复杂度 平均：N^1.3 最好：N  最坏：N^2 
void ShellSort(int *array, int size)
{
	assert(array && size > 0);
	int gap = size;
	int begin, end, tmp;
	int left = 0;
	int right = size - 1;
	while (gap > 1)
	{
		gap = gap / 3 + 1;
		for (end = left + gap; end <= right; end++)
		{
			if (array[end]<array[end - gap])
			{
				tmp = array[end];
				begin = end - gap;
				while (begin >= left && array[begin] > tmp)
				{
					array[begin+gap] = array[begin];
					begin -= gap;
				}
				array[begin + gap] = tmp;
			}
		}
	}
}

/*#################<<<<《归并排序》>>>>############################*/
//时间复杂度 N*logN   应用：外排
void MergeSort(int *array, int size)
{
	assert(array && size > 0);
	int *tmp = new int[size];//辅助数组
	memset(tmp,0,size);
	int left = 0;
	int right = size - 1;
	int mid = left + (right - left) / 2;
	for (int begin = 0; begin <= mid; begin++)
	{
		tmp[begin] = array[begin];
	}
	InsertSort(tmp, mid+1);//对数组前半部分元素进行排序
	for (int begin = 0; begin <= mid; begin++)
	{
		array[begin] = tmp[begin];
	}
	int begin, i;
	for (begin = mid+1, i = 0; begin <= right; begin++, i++)//回赋给原数组
	{
		tmp[i] = array[begin];
	}
	InsertSort(tmp, right-mid);//对数组后半部分原进行排序
	for (begin = mid+1,i = 0; begin <= right; begin++,i++)//回赋给原数组
	{
		array[begin] = tmp[i];
	}
	int left1 = mid + 1;
	int index = 0;
	while (left <= mid && left1 <= right)//对两半已经排好序的元素进行合并
	{
		if (array[left] < array[left1])
		{
			tmp[index] = array[left];
			++left;
			++index;
		}
		else
		{
			tmp[index] = array[left1];
			++index;
			++left1;
		}
	}
	while (left1 <= right) //右半部分还有剩余元素，合并剩余元素
	{
		tmp[index] = array[left1];
		left1++;
		index++;
	}
	while (left <= mid) //左半部分还有剩余元素，合并剩余元素
	{
		tmp[index] = array[left];
		left++;
		index++;
	}
	for (begin = 0; begin <= right; begin++)//从辅助数组回赋给原数组
	{
		array[begin] = tmp[begin];
	}
}

/*#################<<<<《计数排序》>>>>###########################*/
//要相应开辟很大的空间，根据最大数进行开辟空间，若要排序的数没有重复的数，则可以用位图
void CountSort(int *array, int size)
{
	assert(array && size > 0);
	int min = array[0], max = array[0];
	int i = 1;
	for (; i < size; i++)
	{
		if (array[i] > max)
		{
			max = array[i];
		}
		if (array[i] < min)
		{
			min = array[i];
		}
	}
	int len = max - min + 1;
	int *count = new int[len];
	memset(count, 0, sizeof(int)*len);
	for (i = 0; i < size; i++)
	{
		count[array[i] - min]++;
	}
	memset(array,0,sizeof(int)*size);
	int index = 0;
	for (i = 0; i < len; i++)
	{
		while (count[i] != 0)
		{
			array[index] = i + min;
			index++;
			count[i]--;
		}
	}
}

/*#################<<<<《基数排序》>>>>###########################*/
//时间复杂度 多少位*N，应用：数比较集中的排序
//最高位优先MSD（MostSignificant Digit first）
int GetDigit(int value, int d)
{
	while (--d)
	{
		value /= 10;
	}
	return value%10;
}
const int size = 10;
void _RadixSort_MSD(int* array, int left, int right, int d)  //d为处理的位数，百位、十位、个位
{
	assert(array);
	if (d <= 0 || left >= right)
	{
		return;
	}
	int count[size];
	int *auxArray = new int[right-left+1]; //辅助数组
	memset(count, 0, sizeof(count));
	memset(auxArray, 0, sizeof(int)*(right-left+1));
	int i, j;
	for (i = left; i <= right; i++)
	{
		count[GetDigit(array[i], d)]++;
	}
	for (i = 1; i < size; i++)
	{
		count[i] += count[i - 1];
	}
	for (i = left; i <= right; i++)
	{//大的排在数组前面，count顺着执行保证大的数排在桶底，
	 //每次还对桶内多个元素（一个元素则不需要排列）进行排序
		j = GetDigit(array[i], d);  //取元素array[i]第d位的值
		auxArray[count[j] - 1]  = array[i];
		count[j]--;
	}
 	for (i = left,j=0; i <= right; i++,++j)
	{
		array[i] = auxArray[j];//从辅助数组顺序写回原数组
	}
	for (i = 0; i < size; i++)
	{
		_RadixSort_MSD(array, count[i], count[i + 1] - 1, d - 1);
	}
}

//最低位优先LSD（Least Significant Digit first）
int _GetMaxBit(int* array, int size)
{//求最大数的总共位数
	int count = 1,flag = 0;
	int i = 0,tmp;
	for (; i < size; i++)
	{
		flag = count;
		tmp = array[i];
		while (tmp / 10)
		{
			count++;
			tmp /= 10;
		}
		if (count > flag)
		{
			flag = count;
		}
	}
	return flag;
}
void RadixSort_LSD(int* array, int size)
{
	int maxBit = _GetMaxBit(array, size);
	int* count = new int[size];
	int* auxArray = new int[size];

	int radix = 1;
	for (int bit = 1; bit <= maxBit; ++bit)
	{
		memset(count, 0, sizeof(int)* size);
		memset(auxArray, 0, sizeof(int)* size);
		for (int i = 0; i < size; ++i)
		{
			int k = (array[i]/radix) % 10;
			count[k]++;   
		}
		for (int i = 1; i < size; ++i)
		{
			count[i] += count[i - 1];
		}
		for (int i = size - 1; i >= 0; --i)
		{//小的排在数组前面，count倒着执行保证大的数排在桶底
			int k = (array[i] / radix) % 10;
			auxArray[--count[k]] = array[i];
		}
		memcpy(array, auxArray, sizeof(int)*size);//拷贝回原数组
		radix *= 10;
		//radix = pow(10.0,bit-1);
	}
}
void TestAllSort()
{
	//int array[] = { 11, 0, 1, 4, 22, 0, 4, 8, 0, 7 };
	int array[] = { 90, 35, 40, 46, 27, 53, 64, 88, 75, 67 };
	//int array[] = { 1, 325, 403, 436, 237, 9, 664, 887, 755, 67 };
	//int array[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	int size = sizeof(array) / sizeof(array[0]);
	//BubbleSort(array,size);      //冒泡排序
	//SelectSort(array,size);      //选择排序
	//InsertSort(array,size);      //直接插入
	//InsertSort_Half(array,size); //折半插入
	//Heap(array, size);		   //堆排序
	//QuickSort(array,size);       //快速排序（六种：begin和end（递归和栈）、prev和cur（递归和栈）、
								   //三数取中、区间数数量达到一定个数，就使用插入排序）
	//ShellSort(array, size);	   //希尔排序（ gap = (gap/3)/2 ）
	//MergeSort(array,size);       //归并排序
	//CountSort(array,size);       //计数排序
	//_RadixSort_MSD(array, 0, size - 1, 3);//MSD
	RadixSort_LSD(array,size);				//LSD
	for (int i = 0; i < size; i++)
	{
		cout << array[i] << " ";
	}
	cout << endl;
}
int main()
{
	TestAllSort();
	//Schedule();
	//getchar();
	system("pause");
	return 0;
}
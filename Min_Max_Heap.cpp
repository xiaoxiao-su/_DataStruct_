

//最小堆（建立最小堆、最大堆；实现插入、删除、排序、查找）
//用仿函数在最大堆和最小堆之间进行切换
#include <iostream>
#include <vector>
using namespace std;

////仿函数：实现最小堆与最大堆之间的切换////
template<class T>
class Less       //小于函数，建立最小堆
{
public:
	bool operator() (const T& left, const T& right)//重载()
	{
		return right < left;
	}
};

template<class T>
class Greater   //小于函数，建立最大堆
{
public:
	bool operator() (const T& left, const T& right)
	{
		return right > left;
	}
};

template<class T,template<class> class Compare>  //模板的模板类型
class Heap					 //用template<class> class Compare：限定了第二个参数必须是模板类类型
{
public:
	Heap()                   //建立空堆
	{}
	Heap(T* array, int size) //建立堆
	{
		int i = 0;
		_array.reserve(size);//知道要开辟空间的大小后，reserve函数直接帮跟我们开辟好，所需空间大小
				     //这样避免每次插入数据遇到空间不够时候，一点一点扩大容量，从而提高效率
		for (; i < size; i++)
		{
			_array.push_back(array[i]);//拷贝值
		}
		int begin = size / 2 - 1;
		for (; begin >= 0; begin--)       //从倒数第一个非叶子节点开始向下调整
		{
			seftDown(begin, size);
		}
	}
	Heap(vector<T> array)   //建立最小堆 
	{
		swap(_array, array);
		int begin = size / 2 - 1;
		for (; begin >= 0; begin--)  
		{
			seftDown(begin, size);
		}
	}
	//方法一：
	void seftDown_flag(int root, int size)	 //向下调整,借助标记flag
	{
		int flag = 1;
		while (flag)
		{
			int left = root * 2 + 1;
			int right = root * 2 + 2;
			if (left > size-1)
			{
				return;
			}
			int key = left;    //记录最小值（关键码最小）或者最大值（关键码最大）
 			if (right < size && Compare<T>()(_array[left], _array[right]))
			{///////////////////Compare()为临时对象，类名+（）：调用类默认构造函数创建临时对象
				key = right;
			}
			flag = 0;
			if (Compare<T>()(_array[root], _array[key]))
			{
				swap(_array[key], _array[root]);
				root = key;
				flag = 1;
			}
		}
	}
	//方法二：
        void seftDown(int root, int size)      //向下调整
	{
		int left = root * 2 + 1;      ////
		while (left < size)	      ////
		{	
			int right = left + 1; 
			int key = left;  
 			if (right < size && Compare<T>()(_array[left], _array[right]))
			{
				key = right;
			}
			if (Compare<T>()(_array[root], _array[key]))
			{
				swap(_array[key], _array[root]);
				root = key;
				left = root * 2 + 1;  ////
			}
			else
			{
				break;  	     ////
			}
		}
	}
	void Insert(const T& data)
	{
		_array.push_back(data);   //把数据尾插到数组后面
		int begin = _array.size() - 1;
		seftUp(begin);
	}
	void seftUp(int root)		 //向上调整（插入的元素只需跟该棵子树的根节点比较）
	{
		int flag = 1;
		while (flag)
		{
			int parent = (root - 1) / 2;
			flag = 0;
			if (_array[parent] > _array[root])
			{
				flag = 1;
				swap(_array[parent], _array[root]);
				root = parent;
			}
		}
	}
	void RemoveHead()   	    //删除第一个节点（下标为0的头结点）: 先用最后一个元素代替首元素
	{
		int size = _array.size();
		_array[0] = _array[size - 1];
		_array.pop_back(); //把最后一个数据删掉
		int begin = 0;
		size = _array.size();
		seftDown(begin, size);
	}
	const T& GetTop()         //返回堆顶元素
	{
		return _array[0];
	}
	bool Empty()             //判断堆是否为空堆
	{
		return size == 0;
	}
	void ChangeHeapHead(int value)
	{
		_array[0] = value;
	}
	//方法一：
	void HeapSort(int root, int size)                   //降序则建立最小堆，升序则建立最大堆，直接在数组中进行操作
	{
		int tmp = size - 1;
		while(tmp)
		{
			swap(_array[root], _array[tmp]);    //将数组中第一个和最后一个数据进行交换
			seftDown(root, tmp);		    //向下调整
			tmp--;				    //有效个数减减
		}
	}
	//方法二：
	void HeapSort_Stack(int *Array, int root, int size) //降序则建立最小堆，升序则建立最大堆，借助辅助数组
	{
		int num = size;
		while (!_array.empty())
		{
			Array[size - 1] = _array.front();
			RemoveHead();
			seftDown(root, --size);
		}
		for (int i = 0; i < num; i++)
		{
			_array.push_back(Array[i]);
		}
	}
	void _Print()
	{
		int index = 0,size = _array.size();
		while (index < size)
		{
			cout << _array[index] << " ";
			index++;
		}
		cout << endl;
	}
private:
	vector<T> _array;   //动态顺序表
};

//扩展题：十个数（或者更多数）中查找最大的前五个数。
//延伸：当在有大于4G个数中查找最大的前k个数，此时内存是存不完这些数据的，那么就可以借助建立堆

void SearchNumHeap(int array[],int size,int k)
{
	int Array[5] = {0};
	for (int i = 0; i < k; i++)
	{
		Array[i] = array[i];
	}
	Heap<int, Less> hp(Array, k);   //对十个数中的前五个数建最小堆
	for (int i = k; i < size; i++)
	{
		int tmp = hp.GetTop();
		if (array[i] > tmp)
		{
			hp.ChangeHeapHead(array[i]);
			hp.seftDown(0, k);
		}
	}
	hp._Print();
}
void TestHeap()
{
	int array[10] = { 10, 16, 18, 13, 12, 15, 17, 9, 8, 19 };
	//Heap<int, Less> com(array, 10);//最小堆
	//Heap<int, Greater> com(array, 10);//最大堆
	//com.HeapSort(0,10);
	//com.HeapSort_Stack(array, 0, 10);
	//com._Print();
	SearchNumHeap(array, 10, 5);
}
int main()
{
	//TestMin_Heap();
	TestHeap();
	system("pause");
	return 0;
}
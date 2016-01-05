/***************************************************************************************
 *位图：腾讯面试题：给40亿个无符号整形数，并且没有排过序，现在怎么判断给定的一个数是否
 *在这40亿个数里面【腾讯】
 *思路：如果内存够的话，40亿个整型使用位图存储需要500M左右的空间。
 **************************************************************************************/

//BitMap：位图是一个数组的每个数据的每个二进制位表示一个数据，0表示数据不存在，1表示数据存在

#include <iostream>
using namespace std;

template<class T>
class BitSet
{
public:
	BitSet(size_t num)
		:_size(num / 32 + 1)   //当num不整除32时，分配的空间会少，所以要加上1
	{
		SetVector = new T[_size];	         //若有100个数开辟4个T空间的大小
		memset(SetVector, 0, sizeof(T)*_size);   //初始化
	}
	void Set(size_t num)
	{
		size_t index = num / 32;//定位到哪个T区间（如：T是int，那么定位到哪个整型区间）
		size_t pos = num % 32;  //定位到该T空间中的具体哪一位
					//（假如上面定位到数据在第1个整型区间，那么接着定位在整型32位中的哪一位）
		SetVector[index] |= (1 << (31 - pos));
		printf("0x%x\n", SetVector[index]);
	}
	void Reset(size_t num)
	{
		size_t index = num / 32;
		size_t pos = num % 32;
		SetVector[index] &= ~(1 << 31 - pos);
		printf("0x%x\n", SetVector[index]);
	}
	void Clear()
	{
		memset(SetVector, 0, sizeof(T)*_size);
	}
private:
	T* SetVector;  
	size_t _size;  
};
void TestBitSet()
{
	BitSet<int> bt1(100);//有100个数
	bt1.Set(10);         //有一个数据为10
	//bt1.Set(8); 	     //有一个数据为8
	bt1.Set(1);  	     //有一个数据为1
	bt1.Reset(1);
	/*bt1.Reset(8);
	bt1.Reset(10);*/
	//bt1.Clear();
}
int main()
{
	TestBitSet();
	getchar();
	return 0;
}
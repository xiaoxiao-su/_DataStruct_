
/******************************************
 *散列表（哈希表）--> 散列函数
 *解决哈希冲突的方法：
	（1）闭散列方法：a）线性探测 b）二次探测
	（2）开散列方法：链式结构
 ******************************************/

/////////////////（1）闭散列方法：////////////////// 

#include<iostream>
using namespace std;

enum State       //标记数据的状态
{
	EMPTY,   //标记无数据
	EXIST,   //标记数据存在
	DELETE,  //标记数据被删除，此时数据还是保存在数组里面的
};

template<class k>
class HashTableClose
{
public:
	HashTableClose(const k& capacity)
		:_size(0)
		, _capacity(capacity)
	{
		_tables = new k[_capacity];
		_states = new State[_capacity];
		memset(_tables, k(), sizeof(k)*_capacity); //初始化
		memset(_states, EMPTY, sizeof(State)*_capacity);
	}
	~HashTableClose()
	{
		delete[] _tables;
		delete[] _states;
	}
	//////线性探测（一级探测）///////
	k HashFunc1(const k& key)
	{
		return key % _capacity;         //防止越界
	}
	bool Insert1(const k& key)
	{
		_CheckCapacity();
		k pos = HashFunc1(key);         //定位到插入处
		while (_states[pos] == EXIST)   //如果pos位置已经存放别的数据了，那么往后寻找空位置
		{
			if (_states[pos] != DELETE && _tables[pos] == key)  //数组里已经有想要插入的数据
			{                                                   //不能存放相同数据
				return false;
			}
			pos++;
			if (pos == _capacity)
			{
				pos = 0;        //从开始处循环查找空位置
			}
		}
		_tables[pos] = key;
		_states[pos] = EXIST;
		++_size;
		return true;
	}
	/////////二次探测/////////
	k HashFunc2(const k& lastHash, int i)
	{
		return (lastHash + 2 * i - 1) % _capacity;//lastHash为上一个插入的位置
	}
	bool Insert2(const k& key)
	{
		_CheckCapacity();
		k pos = HashFunc1(key);
		int i = 1;
		while (_states[pos] == EXIST)            //如果pos位置已经存放别的数据了，那么往后寻找空位置
		{
			if (_states[pos] != DELETE && _tables[pos] == key) 
			{                                                   
				return false;
			}
			pos = HashFunc2(pos, i++);   //为了数组里存储的数据不那么拥挤,容易出现冲突
			if (pos == _capacity)
			{
				pos = 0;            //从开始处循环查找空位置
			} 
		}
		_tables[pos] = key;
		_states[pos] = EXIST;
		++_size;
		return true;
	}
	void Print()
	{
		int index = 0;
		for (; index < _capacity; index++) //不是顺序存储，要遍历一遍，所以不能用_size
		{
			cout << "[" << _tables[index] << "," << _states[index] << "]  ";
		}
	}
	bool Remove(const k& key)
	{
		k pos = HashFunc1(key);
		int i = 1;
		while (_states[pos] != EMPTY)  
		{
			if (_states[pos] == EXIST && _tables[pos] == key)   
			{                                                  
				_states[pos] = DELETE;//实际数据还在数组里面，只是标记变了而已
				--_size;
				return true;
			}
			pos = HashFunc2(pos, i++);
			if (pos == _capacity)
			{
				pos = 0; 
			}
		}
		return false;
	}
protected:
	void _CheckCapacity()     //扩大容量
	{ 
		if ((float)_size / (float)_capacity >= 0.8)   //当有效数据个数达到总容量的80%则及时进行扩容
		{
			int n = _capacity;
			_capacity = 2 * _capacity + 1;
			k* tmp_tables = new k[_capacity];
			State* tmp_states = new State[_capacity];
			memset(tmp_tables, k(), sizeof(k)*_capacity);    
			memset(tmp_states, EMPTY, sizeof(State)*_capacity);
			int index = 0;
			while (index != n)
			{
				if (_states[index] != EMPTY)
				{
					int pos = HashFunc1(_tables[index]);//线性探测（一级探测）：将数据重新定位到新开辟的数组里面
					tmp_tables[pos] = _tables[index];
					tmp_states[pos] = _states[index];
				}
				index++;
			}
			delete[] _tables;
			delete[] _states;
			_tables = tmp_tables;
			_states = tmp_states;
		}
	}
private:
	k* _tables;     //存放数据
	State* _states; //标记数据状态
	k _size;        //有效个数
	k _capacity;    //容量
};


////////////（2）开散列方法//////////////

#include <vector>
template<class k, class v>
struct HashTableOpenNode                               //每个节点是一个结构体
{
	k _key;
	v _value;
	HashTableOpenNode<k, v>* _next;

	HashTableOpenNode(const k& key,const v& value) //构造函数
		:_key(key)
		, _value(value)
		, _next(NULL)
	{}
};
const size_t PrimeSize = 28;
static const unsigned long _Prime[PrimeSize] =
{//防止哈希冲突而验证出来最应该开辟空间的大小
	53ul, 97ul, 193ul, 389ul, 769ul,
	1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
	49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
	1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
	50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
	1610612741ul, 3221225473ul, 4294967291ul
};

template<class k,class v>
class HashTableOpen
{
	typedef HashTableOpenNode<k,v> Node;
public:
	HashTableOpen()
		:_size(0)
	{}
public:
	bool Insert(const k& key, const v& value)   
	{//插入数据（例如：一个中文对应的所有英文应该插到同一条链上，查找的时候会很高效）
		unsigned long num = _GetPrime();
		_CheckCapacity(num);
		size_t pos = HashFunc(key,num);
		Node* tmp = new Node(key, value);
		Node* begin = _tables[pos];
		while (begin)   //如果该条链上已经有相同的数据，那么就不用重复插入了
		{
			if (begin->_key == key && begin->_value == value)
			{
				return false;
			}
			begin = begin->_next;
		}
		tmp->_next = _tables[pos];//进行头插
		_tables[pos] = tmp;
		++_size;
		return true;
	}
	bool Find(const k& key)   //查找（例如：一个中文对应的所有英文，反之也成立）
	{
		size_t pos = HashFunc(key,_tables.size());
		Node* begin = _tables[pos];
		while (begin)
		{
			if (begin->_key == key)
			{
				cout << "[" << begin->_key << ",";
				cout << begin->_value << "]->";
			}
			begin = begin->_next;
		}
	}
	bool Remove(const k& key) //删除（例如：删除一个中文对应的所有英文，反之也成立）
	{
		size_t pos = HashFunc(key, _tables.size());
		Node* begin = _tables[pos];
		while (begin)
		{
			if (begin->_key == key)
			{
				Node* del = begin;
				begin = begin->_next;
				delete del;
			}
			begin = begin->_next;
		}
	}
	size_t HashFunc(const k& key,const k& capacity)
	{
		return key % capacity;
	}
	void _Print()    //打印哈希表
	{
		for (int i = 0; i < _tables.size(); i++)
		{
			Node* begin = _tables[i];
			printf("HashTable[%d]->", i);
			while (begin)
			{
				cout << "[" << begin->_key << ",";
				cout << begin->_value << "]->";
				begin = begin->_next;
			}
			cout <<"NULL"<< endl;
		}
	}
protected:
	unsigned long _GetPrime()        //获取质数
	{
		for (int i = 0; i<PrimeSize; i++)
		{
			if (_Prime[i]>_size)
			{
				return _Prime[i];
			}
		}
		return _Prime[PrimeSize];
	}
	void _CheckCapacity(size_t num)   //若一次性多开辟几个字节
	{
		if (num > _tables.size())
		{
			vector<Node*> NewTables;
			NewTables.assign(num,NULL);//将NewTables的每个元素初始化为NULL
			for (int i = 0; i < _tables.size(); i++)//拷贝值
			{
				while (_tables[i])
				{
					Node* begin = _tables[i];
					_tables[i] = _tables[i]->_next;
					size_t pos = HashFunc(begin->_key,num);
					begin->_next = NewTables[pos];//进行头插
					NewTables[pos] = begin;
				}
			}
			_tables.swap(NewTables);
		}
	}
private:
	vector<Node*> _tables;
	size_t _size;
};
void TestHashTableOpen()
{
	HashTableOpen<int, double> open;
	open.Insert(1, 1.1);
	open.Insert(1, 1.2);
	open.Insert(2, 2.2);
	open.Insert(2, 2.4);
	open.Insert(1, 1.1);
	open.Insert(20, 1.6);
	open.Insert(30, 1.1);
	open._Print();
}
int main()
{
	//TestHashTableClose();
	TestHashTableOpen();
	system("pause");
	return 0;
}


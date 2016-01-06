

/**************************
 *广义表是非线性的结构，是线性表的一种扩展，是有n个元素组成有限序列。
 *广义表的定义是递归的，因为在表的描述中又得到了表，允许表中有表。
 *----------<1> A = ()
 *----------<2> B = (a,b)
 *----------<3> C = (a,b,(c,d))
 *----------<4> D = (a,b,(c,d),(e,(f),h)) 
 *----------<5> E = (((),()))
 *
 *广义表的基本操作：
 *----------建立广义表
 *----------拷贝广义表
 *----------遍历广义表
 *----------求广义表的节点个数、长度、深度
 **************************/

#include <iostream>
using namespace std;

enum NodeType
{
	HeadType,  //头节点
	ValueType, //值节点
	SubType,   //子表节点
};
struct GeneralNode
{
	GeneralNode* _next;
	NodeType _type;  //方便遍历的时候知道该节点是何节点
	union
	{//要不值节点，要不子表节点
		char _value;
		GeneralNode* _sublink;
	};

	GeneralNode(NodeType type,char value = '\0')
		:_next(NULL)
		, _type(type)
	{
		if (_type == SubType)
		{
			_sublink = NULL;
		}
		else if (_type == ValueType)
		{
			_value = value;
		}
	}
};
class GeneralLists
{
public:
	GeneralLists(const char* str)         //构造函数
		:_Link(NULL)
	{
		_GreadList(_Link, str);//建立广义表
	}
	GeneralLists(const GeneralLists& g)   //拷贝构造函数
		:_Link(NULL)
	{
		//DiList(_Link,g._Link);
		_Link = DiList(g._Link);
	}
	void DiList(GeneralNode*& link, GeneralNode* gbegin)//拷贝
	{
		GeneralNode* begin = link;
		while (gbegin)
		{
			if (gbegin->_type == HeadType)
			{
				GeneralNode* HeadNode = new GeneralNode(HeadType);//开辟头节点
				link = HeadNode;
				begin = link;
			}
			if (gbegin->_type == ValueType)   //值节点
			{
				begin->_next = new GeneralNode(ValueType, gbegin->_value);
				begin = begin->_next;
			}
			if (gbegin->_type == SubType)     //子表节点
			{
				begin->_next = new GeneralNode(SubType);
				begin = begin->_next;
				DiList(begin->_sublink, gbegin->_sublink);
			}
			gbegin = gbegin->_next;
		}
	}
	GeneralNode* DiList(GeneralNode* gbegin)//以返回值的形式
	{
		GeneralNode* HeadNode = new GeneralNode(HeadType);//开辟头节点
		//HeadNode作为最终返回值
		GeneralNode* cur = gbegin->_next;
		GeneralNode* begin = HeadNode;
		while (cur)
		{
			if (cur->_type == ValueType)   //值节点
			{
				begin->_next = new GeneralNode(ValueType, cur->_value);
			}
			else if (cur->_type == SubType)     //子表节点
			{
				begin->_next = new GeneralNode(SubType);
				begin->_next->_sublink = DiList(cur->_sublink);
			}
			cur = cur->_next;
			begin = begin->_next;
		}
		return HeadNode;
	}

	//方法一：
	//GeneralLists& operator= (const GeneralLists& g)   //赋值操作符重载函数
	//{
	//	if (&g != this)
	//	{
	//		this->_Distory(); //先摧毁原表
	//		DiList(_Link,g._Link);
	//		//_Link = DiList(g._Link);//以返回值的形式
	//	}
	//	return *this;
	//}

	//方法二：
	GeneralLists& operator= (GeneralLists g)   //(注意参数列表不可有引用和const)赋值操作符重载函数
	{//g是实参对象的拷贝，调用拷贝构造函数开辟空间对g进行初始化，完后g的指向和
	 //实参对象指向不同
		swap(_Link,g._Link);//出了这个函数的作用域后，g._Link会被释放
		return *this;
	}
	~GeneralLists()       //析构函数
	{
		_Distory();
	}
	void _Distory()       //摧毁广义表
	{
		Distory(_Link);
	}
	void Distory(GeneralNode*& link)
	{
		while (link)
		{
			if (link->_type == SubType)//摧毁子表
			{
				Distory(link->_sublink);
			}
			GeneralNode* del = link; //第一个必然是头节点
			link = link->_next;
			delete del;
		}
	}
	void _Print()
	{
		_Print(_Link);
	}
	void _Print(GeneralNode* begin)//递归遍历
	{//((b,c,d))
		while (begin)
		{
			if (begin->_type == HeadType)
			{
				cout << "(";
			}
			if (begin->_type == ValueType)
			{
				cout << begin->_value;
				if (begin->_next != NULL)
				{
					cout << ",";
				}
			}
			if (begin->_type == SubType)
			{
				_Print(begin->_sublink);
				if (begin->_next != NULL)
				{
					cout << ",";
				}
			}
			begin = begin->_next;
		}
		cout << ")";
	}
	void _Size()
	{
		cout << _Size(_Link) << "  ";
	}
	int _Size(GeneralNode* begin)//广义表值个数
	{
		int Scount = 0;
		while (begin)
		{
			if (begin->_type == ValueType)
			{
				Scount++;
			}
			if (begin->_type == SubType)
			{
				Scount += _Size(begin->_sublink);//上层节点个数加上子表里包含的节点的个数
			}
			begin = begin->_next;
		}
		return Scount;
	}
	int Length()   //广义表长度，节点个数（不包括子表里面的）
	{
		GeneralNode* begin = _Link;
		int Lcount = 0;
		while (begin->_next != NULL)
		{
			Lcount++;
			begin = begin->_next;
		}
		return Lcount;
	}
	void Depth()
	{
		cout << Depth(_Link) << "  ";
	}

	int Depth(GeneralNode* begin) //广义表的深度（即多少层）
	{
		int depth = 1;
		while (begin)
		{
			if (begin->_type == SubType)
			{
				int subdepth = Depth(begin->_sublink);
				if(subdepth+1>depth)
				{
					depth = subdepth+1;
				}
			}
			begin = begin->_next;
		}
		return depth;
	}
protected:
	void _GreadList(GeneralNode*& link,const char*& str)//建立广义表
	{
		if (*str != '(')
		{
			cout << "Invalid string" << endl;
			return;
		}
		str++;
		GeneralNode* HeadNode = new GeneralNode(HeadType);//开辟头节点
		link = HeadNode;
		GeneralNode* begin = HeadNode;

		//(a, ((b), (c), d), e)
		while (*str != '\0')
		{
			if (*str == ',')
			{
 				str++;
			}
			if (*str != ')'&& *str != '(')//扩展：要是其他字符比如空格，可以写一个过滤函数
			{
				GeneralNode* valueNode = new GeneralNode(ValueType,*str);
				//开辟值节点
				str++;
				begin->_next = valueNode;
				begin = begin->_next; 
			}
			else if (*str == ')')  //递归结束条件
			{
				str++;
				return;
			}
			else if (*str == '(')  //递归构造子表
			{
				GeneralNode* subNode = new GeneralNode(SubType);//开辟子表节点
				begin->_next = subNode;
				begin = begin->_next;
				_GreadList(subNode->_sublink, str); //递归调用子表
			}
		}
	}
	
private:
	GeneralNode* _Link;
};
void Test1()
{
	const char* s1 = "(a,((b),(c),d),e)";
	GeneralLists g1(s1);  //构造函数
	GeneralLists g2(g1);  //拷贝构造函数

	g1._Print();
	cout << endl;
	g2._Print();

	cout << endl;
	cout << "Size : ";
	g1._Size();
	g2._Size();

	//深度
	cout << "Depth : ";
	g1.Depth();
	g2.Depth();

	//长度
	cout << "Length : ";
	cout << g1.Length() << "  ";
	cout << g2.Length() << "  ";
}

int main()
{
	Test1();
	system("pause");
	return 0;
}

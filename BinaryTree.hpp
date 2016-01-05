#pragma once

/*************************************
 *二叉树的建立
 *二叉树的遍历
 *		------先序遍历（递归和非递归实现）
 *		------中序遍历（递归和非递归实现）
 *		------后序遍历（递归和非递归实现）
 *		------中序遍历（非递归实现）
 ************************************/
#include <queue>
#include <stack>

template<class T>
struct TreeNode          //普通树节点结构（孩子兄弟表示法）
{
	T _value;
	TreeNode<T>* _leftNode;   //左孩子
	TreeNode<T>* _brotherNode;//右兄弟
};

template<class T>
struct BinaryTreeNode    //二叉树节点结构
{
	T _value;
	BinaryTreeNode<T>* _leftNode; //左子树
	BinaryTreeNode<T>* _rightNode;//右子树

	BinaryTreeNode(const T& value)//构造函数
		:_value(value)
		, _leftNode(NULL)
		, _rightNode(NULL)
	{}
};

template<class T>
class BinaryTree
{
public:
	BinaryTree(char* str)
		:_root(NULL)
	{
		_GreateTree(_root, str);
	}
	BinaryTreeNode<T>* _Copy(BinaryTreeNode<T>* cur)
	{
		BinaryTreeNode<T>* root = NULL;
		if (cur)
		{
			root = new BinaryTreeNode<T>(cur->_value);
			BinaryTreeNode<T>* begin = root;
			begin->_leftNode = _Copy(cur->_leftNode);
			begin->_rightNode = _Copy(cur->_rightNode);
		}
		return root;
	}
	/*优化版：
	BinaryTree(BinaryTree<T>* cur)        //拷贝构造函数，用一颗树去初始化另外一颗树
	{
		swap(_root, cur);
		return *this;
	}*/
	//普通版：
	BinaryTree(const BinaryTree<T>& cur)  //拷贝构造函数，参数是引用
	{
		_root = _Copy(cur._root);
	}
	/*优化版：
	BinaryTree<T>& operator= (BinaryTree<T>* cur) //赋值操作符重载函数，参数不是引用
	{
		if (this != &cur)
		{
			swap(_root, cur);
		}
		return *this;
	}*/
	//普通版：
	void _Distory(BinaryTreeNode<T>* tmp)
	{
		if (tmp)
		{
			_Distory(tmp->_leftNode);
			_Distory(tmp->_rightNode);
			delete tmp;
		}
	}
	BinaryTree<T>& operator= (const BinaryTree<T>& cur) //赋值操作符重载函数
	{
		if (this != &cur)
		{
			_Distory(_root);//或者显示调用析构函数this->~BinaryTree();
			_root = _Copy(cur._root);
		}
		return *this;
	}
	~BinaryTree()
	{
		_Distory(_root);
	}
	void _GreateTree(BinaryTreeNode<T>*& root, char*& str) //构造一棵二叉树
	{
		if (*str != '#' && *str != '\0')
		{
			root = new BinaryTreeNode<T>(*str);
			_GreateTree(root->_leftNode, ++str); //遍历创建左子树
			_GreateTree(root->_rightNode, ++str);//遍历创建右子树
		}
	}
	void _Print()      //调用保护成员函数
	{
		cout << "先序遍历：";
		PreOrder(_root);
		cout << endl;
		cout << "中序遍历：";
		InOrder(_root);
		cout << endl;
		cout << "后序遍历：";
		BackOrder(_root);
		cout << endl;
		cout << "层序遍历：";
		LevelOrder(_root);
		cout << endl;
	}
	void _NonR_Print()
	{
		NonR_PreOrder(_root);
		cout << endl;
		NonR_InOrder(_root);
		cout << endl;
		NonR_PostOrder(_root);
		cout << endl;
	}
	int Size()
	{
		cout << "节点个数：";
		return size(_root);
	}
	int size(BinaryTreeNode<T>* root)   //节点个数（递归）
	{
		int count = 0;
		if (root)
		{
			count = 1;
			count += size(root->_leftNode); //递归进左子树
			count += size(root->_rightNode);//递归进右子树
		}
		return count;
	}
	int Depth()
	{
		cout << "深度：";
		return depth(_root);
	}
	int depth(BinaryTreeNode<T>* root)    //深度（递归）
	{
		if (root == NULL)
		{
			return 0;
		}
		int leftNum = 1 + _Depth(root->_leftNode);
		int rightNum = 1 + _Depth(root->_rightNode);
		return leftNum > rightNum ? leftNum : rightNum;
	}
protected:
	void PreOrder(BinaryTreeNode<T>* cur)  //先序遍历（递归方法一）
	{
		if (cur)
		{
			cout << cur->_value << " ";    //最先访问根节点
		}
		if (cur->_leftNode)
		{
			PreOrder(cur->_leftNode);
		}
		if (cur->_rightNode)
		{
			PreOrder(cur->_rightNode);
		}
	}
	void NonR_PreOrder(BinaryTreeNode<T>* root)   //先序遍历（非递归方法一）
	{
		stack<BinaryTreeNode<T>*> s;
		if (root)
		{
			s.push(root);
		}
		BinaryTreeNode<T>* cur = NULL;
		while (!s.empty())
		{
			cur = s.top();
			cout << cur->_value << " ";
			s.pop();
			if (cur->_rightNode)  //先让右孩子先于左边孩子入栈
			{
				s.push(cur->_rightNode);
			}
			if (cur->_leftNode)
			{
				s.push(cur->_leftNode);
			}
		}
	}
	void InOrder(BinaryTreeNode<T>* root)     //中序遍历（递归）
	{
		if (root)
		{
			if (root->_leftNode)
			{
				InOrder(root->_leftNode);
			}
			cout << root->_value << " ";    //中间访问根节点
			if (root->_rightNode)
			{
				InOrder(root->_rightNode);
			}
		}
	}
	void NonR_InOrder(BinaryTreeNode<T>* root)   //中序遍历（非递归）
	{
		stack<BinaryTreeNode<T>*> s;
		BinaryTreeNode<T>* cur = root;
		while (cur || !s.empty())//控制循环条件是数据
		{
			while (cur)	//将左边元素全部入栈，如果cur为NULL，那么就直接出数据并且打印数据
			{
				s.push(cur);
				cur = cur->_leftNode;
			}
			BinaryTreeNode<T>* top = s.top();
			cout << top->_value << " ";
			s.pop();
			cur = top->_rightNode;//右孩子可能还是一颗子树
		}
	}
	void BackOrder(BinaryTreeNode<T>* root)     //后序遍历
	{
		if (root)
		{
			if (root->_leftNode)
			{
				BackOrder(root->_leftNode);
			}
			if (root->_rightNode)
			{
				BackOrder(root->_rightNode);
			}
			cout << root->_value << " ";//最后访问根节点
		}
	}
	void NonR_PostOrder(BinaryTreeNode<T>* root)   //后序遍历（非递归）
	{
		stack<BinaryTreeNode<T>*> s;
		BinaryTreeNode<T>* cur = root;
		BinaryTreeNode<T>* visitedNode = NULL;
		while (cur || !s.empty())
		{
			while (cur)    //将左边元素全部入栈
			{
				s.push(cur);
				cur = cur->_leftNode;
			}
			BinaryTreeNode<T>* top = s.top();//此时不能立即访问top节点，因为该节点有可能还有右子树
			if (top->_rightNode == NULL || top->_rightNode == visitedNode)
			{
				cout << top->_value << " ";
				s.pop();
				visitedNode = top;//保存上一次被访问的节点
			}
			else
			{
				cur = top->_rightNode;
			}
		}
	}
	void LevelOrder(BinaryTreeNode<T>* root)  //层序遍历（队列）
	{
		queue<BinaryTreeNode<T>* > q;
		if (root)
		{
			q.push(root);
		}
		BinaryTreeNode<T>* begin = NULL;
		while (!q.empty())    //栈为空则表示访问结束
		{
			begin = q.front();//保存栈中的第一个元素以访问其左右孩子
			cout << begin->_value << " ";
			q.pop();
			if (begin->_leftNode)
			{
				q.push(begin->_leftNode);
			}
			if (begin->_rightNode)
			{
				q.push(begin->_rightNode);
			}
		}
	}
private:
	BinaryTreeNode<T>* _root;//根节点
};

void TestBinaryTree1()
{
	char* str1 = "123##4##56";
	BinaryTree<char> t1(str1);
	t1._Print();
	cout << endl;
	/*cout << t1.Size() << endl;
	cout << t1.Depth() << endl;*/
	BinaryTree<char> t2(t1);
	t2._Print();
	cout << endl;
	char* str3 = "456##7##89";
	BinaryTree<char> t3(str3);
	t3 = t2;
	t3._Print();
	cout << endl;
}

/*类似例题：逆序打印单链表：递归和非递归（入栈）*/

void TestBinaryTree2()
{
	char* str = "123##4##56";
	BinaryTree<char> t1(str);
	/*int a[10] = {1,2,3,-1,-1,4,-1.-1,5,6};
	BinaryTree<int> i1(a,10,-1);*/
	t1._NonR_Print();
}

//扩展
//三叉链表存储二叉树节点结构
template<class T>
struct BinaryTreeNode_PNode
{
	BinaryTreeNode_PNode<T>* _leftNode;  //左孩子
	BinaryTreeNode_PNode<T>* _rightNode; //右孩子
	BinaryTreeNode_PNode<T>* _parentNode;//父节点
	T _data;

	BinaryTreeNode_PNode(const T& data)
		:_data(data)
		, _leftNode(NULL)
		, _rightNode(NULL)
		, _parentNode(NULL)//父节点为i，则左孩子为2*i+1,右孩子为2*i+2
	{}
};
template<class T>
class BinaryTree_P
{
public:
	BinaryTree_P(T* array,int size)
		:_root(NULL)
	{
		int index = 0;
		_CreateBinaryTree_P(_root, array, size, index);
	}
protected:
	void _CreateBinaryTree_P(BinaryTreeNode_PNode<T>*& root, T* array, int size, int& index)
	{
		if (index < size && array[index] != '#')
		{   //先创建根节点，标识以先序遍历顺序构造二叉树
			root = new BinaryTreeNode_PNode<T>(array[index]);
			_CreateBinaryTree_P(root->_leftNode, array, size, ++index);
			_CreateBinaryTree_P(root->_rightNode, array, size, ++index);
		}
	}
private:
	BinaryTreeNode_PNode<T>* _root;
};

void TestBinaryTree_P()
{
	int array[20] = { 1, 2, 3, '#', '#', 4, '#', '#', 5, 6 };
	BinaryTree_P<int> tree(array,10);
}
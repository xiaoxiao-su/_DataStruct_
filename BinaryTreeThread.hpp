

/***********************************************
 *线索化二叉树
 *
 *-------先序线索化
 *-----------------先序遍历
 *-------中序线索化
 *-----------------先序遍历、中序遍历
 *-----------------查找中序遍历下的第一个节点
 *-----------------查找中序遍历下的最后一个节点
 *-----------------查找中序遍历current节点的后继
 *-----------------查找中序遍历current节点的前驱
 *-------后序线索化
************************************************/

#include<iostream>
using namespace std;

enum type
{
	THREAD,//线索化
	LINK   //儿女节点
};

template<class T>
struct BinaryTreeThreadNode
{
	T _data;
	BinaryTreeThreadNode<T>* _leftNode;
	BinaryTreeThreadNode<T>* _rightNode;
	type leftTag; //为THREAD时左节点存放前驱，为LINK时存放左孩子
	type rightTag;//为THREAD时左节点存放后继，为LINK时存放右孩子

	BinaryTreeThreadNode(const T& data)
		:_data(data)
		, _leftNode(NULL)
		, _rightNode(NULL)
		, leftTag(LINK)
		, rightTag(LINK)
	{}
};

template<class T>
class BinaryTreeThread
{
public:
	BinaryTreeThread(T* array, int size)
		:_root(NULL)
	{
		int index = 0;
		_CreateBinaryTreeThreadPre(_root, array, size, index);
	}
	void PrevOrderThread()  //先序线索化（三种遍历顺序：先序、中序、后序）
	{
		BinaryTreeThreadNode<T>* prev = NULL;
		PrevOrderGreadThread(_root, prev);
	}
	void InOrderThread()   //中序线索化（三种遍历顺序：先序、中序、后序）
	{
		BinaryTreeThreadNode<T>* prev = NULL;
		InOrderGreadThread(_root, prev);
	}
	BinaryTreeThreadNode<T>* First(BinaryTreeThreadNode<T>* cur)    //中序线索化，查找中序遍历下的第一个节点
	{
		while (cur && cur->leftTag == LINK)
		{
			cur = cur->_leftNode;
		}
		return cur;
	}
	BinaryTreeThreadNode<T>* Last(BinaryTreeThreadNode<T>* cur)    //中序线索化，查找中序遍历下的最后一个节点
	{
		while (cur && cur->rightTag == LINK)
		{
			cur = cur->_rightNode;
		}
		return cur;
	}
	BinaryTreeThreadNode<T>* Post(BinaryTreeThreadNode<T>* current)//中序线索化，中序遍历current节点的后继
	{
		if (current && current->_rightNode != NULL)
		{
			First(current->_rightNode);
		}
		return NULL;
	}

	BinaryTreeThreadNode<T>* Prev(BinaryTreeThreadNode<T>* current)//中序线索化，中序遍历current节点的前驱
	{
		if (current && current->_leftNode != NULL)
		{
			Last(current->_leftNode);
		}
		return NULL;
	}
	void HostOrderThread()   //后序线索化（三种遍历顺序：先序、中序、后序）
	{
		BinaryTreeThreadNode<T>* prev = NULL;
		HostOrderGreadThread(_root, prev);
	}
	void Print()
	{
		/*Non_RPreOrder_In();
		cout << endl;
		Non_RInOrder_In();
		cout << endl;*/
		
		/*Non_RPreOrder_Pre();
		cout << endl;
		Non_RInOrder_Pre();
		cout << endl;*/
	}
protected:
	void _CreateBinaryTreeThreadPre(BinaryTreeThreadNode<T>*& root,
		T* array, int size, int& index)//先序构造一棵二叉树（没有线索化）
	{
		if (index < size && array[index] != '#')
		{
			root = new BinaryTreeThreadNode<T>(array[index]);
			_CreateBinaryTreeThreadPre(root->_leftNode, array, size, ++index);
			_CreateBinaryTreeThreadPre(root->_rightNode, array, size, ++index);
		}
	}
	void PrevOrderGreadThread(BinaryTreeThreadNode<T>*& cur,
		BinaryTreeThreadNode<T>*& prev)//先序线索化二叉树
	{
		if (cur)
		{
			if (cur->_leftNode == NULL)//左孩子为NULL
			{
				cur->leftTag = THREAD;
				cur->_leftNode = prev;
			}
			if (prev && prev->_rightNode == NULL)
			{
				prev->rightTag = THREAD;
				prev->_rightNode = cur;
			}
			prev = cur;		 //保存上一个节点
			if (cur->leftTag == LINK)//左孩子存在则线索化左子树
			{
				PrevOrderGreadThread(cur->_leftNode, prev);
			}
			if (cur->rightTag == LINK)
			{
				PrevOrderGreadThread(cur->_rightNode, prev);
			}
		}
	}
	void InOrderGreadThread(BinaryTreeThreadNode<T>*& cur,
		BinaryTreeThreadNode<T>*& prev)                   //中序线索化二叉树
	{
		if (cur)
		{
			InOrderGreadThread(cur->_leftNode, prev); //中序线索化左子树
			if (cur->_leftNode == NULL)
			{
				cur->leftTag = THREAD;
				cur->_leftNode = prev;
			}
			if (prev && prev->_rightNode == NULL)
			{
				prev->rightTag = THREAD;
				prev->_rightNode = cur;
			}
			prev = cur;
			InOrderGreadThread(cur->_rightNode, prev);//中序线索化右子树
		}
	}
	void HostOrderGreadThread(BinaryTreeThreadNode<T>*& cur,
		BinaryTreeThreadNode<T>*& prev)		         //后序线索化二叉树
	{
		if (cur)
		{
			HostOrderGreadThread(cur->_leftNode, prev);
			HostOrderGreadThread(cur->_rightNode, prev);
			if (cur->_leftNode == NULL)
			{
				cur->leftTag = THREAD;
				cur->_leftNode = prev;
			}
			if (prev && prev->_rightNode == NULL)
			{
				prev->rightTag = THREAD;
				prev->_rightNode = cur;
			}
			prev = cur;
		}
	}

	//******先序线索化，遍历*******//

	void Non_RPreOrder_Pre()  //先序线索化，先序遍历
	{
		BinaryTreeThreadNode<T>* cur = _root;
		while (cur)
		{
			cout << cur->_data << " ";
			if (cur->leftTag == LINK)
			{
				cur = cur->_leftNode;
			}
			else if (cur->rightTag == LINK)
			{
				cur = cur->_rightNode;
			}
			else if (cur && cur->rightTag == THREAD)
			{
				cur = cur->_rightNode;//开始访问右节点
			}
		}
	}

	//************中序线索化，遍历******************//

	void Non_RPreOrder_In()    //中序线索化，先序遍历（非递归）
	{
		BinaryTreeThreadNode<T>* cur = _root;
		while (cur)
		{
			cout << cur->_data << " ";
			if (cur->leftTag == LINK)   //向下访问左节点
			{
				cur = cur->_leftNode;
			}
			else if (cur->rightTag == LINK)//向下访问右节点
			{
				cur = cur->_rightNode;
			}			
			else
			{
				while (cur && cur->rightTag == THREAD) //直到有右节点的时候
				{
					cur = cur->_rightNode;
				}
				if (cur)
				{
					cur = cur->_rightNode; //访问右子树
				}
			}
		}
	}

	void Non_RInOrder_In()  //中序线索化，中序遍历（非递归）
	{
		BinaryTreeThreadNode<T>* cur = _root;
		while (cur)
		{
			while (cur->leftTag != THREAD)//找到左边最后一个左孩子
			{
				cur = cur->_leftNode;
			}
			cout << cur->_data << " ";    //访问根节点
			while (cur->rightTag == THREAD)
			{
				cur = cur->_rightNode;     
				cout << cur->_data << " ";//没有右孩子则遍历右节点
			}
			cur = cur->_rightNode;    //右孩子作为一棵子树去遍历
		}
	}
	//******后序线索化，遍历*******//
	//void Non_RPostOrder_Post()    //后序线索化，后序遍历（非递归）
	//{ //用三叉链表实现：即每个链表里都包含一个指向父节点的指针_parent
	//	BinaryTreeThreadNode<T>* cur = _root;
	//	BinaryTreeThreadNode<T> visit = NULL;   //标记已经访问过的节点
	//	if (cur)
	//	{
	//		while (cur && cur->leftTag == LINK)
	//		{
	//			cur = cur->_leftNode;
	//		}
	//		while (cur && cur->leftTag == THREAD)
	//		{
	//			cout << cur->_data << " ";
	//			visit = cur;
	//			cur = cur->_rightNode;
	//		}
	//		if (cur && cur->_rightNode == visit)
	//		{
	//			//cur = cur->_parent;
	//			cur = cur->_rightNode;
	//		}
	//		if (cur == root)
	//		{
	//			return;
	//		}
	//	}
	//}
private:
	BinaryTreeThreadNode<T>* _root;
};
void TestBinaryTreeThread()
{
	//int array[20] = { 1, 2, 3, '#', '#', 4, '#', '#', 5, 6 };
	int array[20] = { 1, 2, 3, '#', '#', 4, '#', 5, '#', '#', 6 ,7};
	//BinaryTreeThread<int> tree(array, 10);
	BinaryTreeThread<int> tree(array, 12);
	//tree.PrevOrderThread();//先序线索化
	//tree.InOrderThread();//中序线索化
	tree.HostOrderThread();//后序线索化
	tree.Print();
}
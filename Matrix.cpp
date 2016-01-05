
/**************************************************************************************************
 *《对称矩阵》：设一个N*N的方阵A，A中任意元素Aij，当且仅当Aij == Aji(0 <= i <= N-1 && 0 <= j <= N-1)，
 *则矩阵A是对称矩阵。以矩阵的对角线为分隔，分为上三角和下三角。
 *《对称矩阵压缩存储》：只需要存储上三角/下三角的数据，所以最多存储n(n+1)/2个数据。
 *对称矩阵和压缩存储的对应关系：下三角存储  
 *结论：i>=j,  SymmetricMatrix[i][j] == Array[i*(i+1)/2+j]，即对称矩阵中的元素对应存储数组的元素
***************************************************************************************************/

//{0,1,2,3,4}
//{1,0,1,2,3}
//{2,1,0,1,2}
//{3,2,1,0,1}
//{4,3,2,1,0}

#include <iostream>
using namespace std;

#define N 3

template<class T>
class SymmetricMatrix
{
public:
	SymmetricMatrix(T* matrixarray, int L) //T*array 也可以用 T array[][N]
		:_arrayLen(L*(L+1)/2)       
		, _array(new T[_arrayLen])
		, _mtrixLen(L*L)
	{
		int index = 0;
		for (int i = 0; i < L; i++)
		{
			for (int j = 0; j < L; j++)
			{
				if (i >= j)
				{
					_array[index++] = matrixarray[i*L+ j] ;//以一维数组存储
				}
			}
		}
	}
	const T& GetVal(int row,int col)
	{
		if (row < col)
		{
			swap(row,col);
		}
		return _array[row*(row + 1) / 2 + col];//由上结论可得
	}
private:
	T* _array;		 //存储矩阵的数组
	size_t _arrayLen;//存储矩阵的数组大小
	size_t _mtrixLen;//矩阵大小
};

//int main()
//{
//	int matrixarray[N][N] = {
//								{0,1,2},
//								{1,0,1},
//								{2,1,0},
//							};
//	SymmetricMatrix<int> M((int*)matrixarray, N);
//	cout<<M.GetVal(1,2)<<endl;
//	system("pause");
//	return 0;
//}

/*****************************************************************************************************
 *《稀疏矩阵》：M*N的矩阵，矩阵中有效值的个数远小于无效值的个数，且这些数据的分布没有规律。
 *《稀疏矩阵的压缩存储》：存储极少数的有效数据，
 * 即使用{row,col,value}三元组存储每一个有效数据，三元组按原矩阵中的位置，以行优先级先后顺序依次存放。
 *****************************************************************************************************/

//{1,0,3,0,5}
//{0,0,0,0,0}
//{0,0,0,0,0}
//{1,0,3,0,5}
//{0,0,0,0,0}
//{0,0,0,0,0}

/****************************************************
 *稀疏矩阵的逆置
 {1,0,3,0,5}			  {1,0,0,1,0,0}
 {0,0,0,0,0}			  {0,0,0,0,0,0}
 {0,0,0,0,0}  -------->   {3,0,0,3,0,0}
 {1,0,3,0,5}			  {0,0,0,0,0,0}
 {0,0,0,0,0}			  {5,0,0,5,0,0}
 {0,0,0,0,0}
*****************************************************/

#include <vector>

#define ROW 3
#define COL 4

template<class T>
struct Tuple   //三元组
{
	int Trow;   //行
	int Tcol;   //列
	int Tvalue; //数据
};

template<class T>
class SparseMatrix
{
public:
	SparseMatrix(T* _sparsearray, int row ,int col)//以一维数组（强转）存储二维数组
		:_row(row)
		, _col(col)
	{
		for (int i = 0; i < _row; i++)
		{
			for (int j = 0; j < _col; j++)
			{
				if (_sparsearray[i*_col + j] != 0)
				{
					Tuple<T> t;
					t.Trow = i;
					t.Tcol = j;
					t.Tvalue = _sparsearray[i*_col + j];
					_array.push_back(t);//将三元组插入数组中
				}
			}
		}
	}
	SparseMatrix()   //在转置的时候定义了一个SparseMatrix<T>的对象，要调用没有参数的构造函数
	{}
	void Print()    //借助压缩存储的数组打印矩阵
	{
		int index = 0;
		for (int i = 0; i < _row; i++)
		{
			for (int j = 0; j < _col; j++)
			{
				if (index < _array.size()				//防止越界访问
					&& i == _array[index].Trow
					&& j == _array[index].Tcol)
				{
					cout << _array[index++].Tvalue<<" ";//以行优先存储
				}
				else
				{
					cout << '0'<<" ";
				}
			}
			cout << endl;
		}
	}
	SparseMatrix<T> TSmatrix()             //一般转置，返回值不能用引用,因为返回的是临时变量
	{
		SparseMatrix<T> tmp;			    //创建对象为了创建辅助数组，调用没有参数的构造函数
		tmp._row = _col;
		tmp._col = _row;
		int index = 0;
		for (int i = 0; i < _col; i++)      //矩阵按列遍历，矩阵中的第一列是存储矩阵数组的第一行
		{
			index = 0;
			while (index < _array.size())
			{
				if (i == _array[index].Tcol)//如果矩阵中的列等于存储矩阵数组的行
				{
					Tuple<T> t;
					t.Trow = _array[index].Tcol;
					t.Tcol = _array[index].Trow;
					t.Tvalue = _array[index].Tvalue;
					tmp._array.push_back(t);//把该元素的行列交换再插入辅助数组，
											//这样保证了辅助数组也是按行优先存储
				}
				index++;
			}
		}
		return tmp;
	}
	SparseMatrix<T> Quick_TSmatrix()       //快速转置
	{
		SparseMatrix<T> tmp;			  //存储快速转置后矩阵
		tmp._row = _col;
		tmp._col = _row;
		int size = _array.size();
		int* count_col = new int[_col];   //辅助数组，统计矩阵每一列有效数据的个数
		int* colIndex = new int[_col];    //辅助数组，统计矩阵每一列第一个有效数据在colIndex数组存放的下标
		memset(count_col, 0, sizeof(int)*_col);
		memset(colIndex, 0, sizeof(int)*_col);
		for (int i = 0; i < size; i++)
		{
			count_col[_array[i].Tcol]++;   //统计每列有效数据的个数
		}
		colIndex[0] = 0;				 //第一列非零数字的下标肯定是零（假设第一列有数据）
		for (int i = 1; i < size; i++)  //统计每一列第一个非零数字的下标
		{
			colIndex[i] = colIndex[i - 1] + count_col[i - 1];
		}
		for (int i = 0; i < size; i++)
		{
			tmp._array.push_back(Tuple<T>());    //插入默认的三元组，相当于直接创建三元组
		}
		for (int i = 0; i < size; i++)
		{
			int index = colIndex[_array[i].Tcol];//取得元素相对应在转置后的下标
			tmp._array[index].Tvalue = _array[i].Tvalue;
			tmp._array[index].Trow = _array[i].Tcol;
			tmp._array[index].Tcol = _array[i].Trow;
			colIndex[_array[i].Tcol]++;
		}
		return tmp;
	}
private:
	vector<Tuple<T> > _array;//存储稀疏矩阵的数组
	int _row;				 //矩阵的行（在Print函数中使用，就可以不用从类外传参）
	int _col;				 //矩阵的列
};
int main()
{
	int Sparsearray[ROW][COL] = {
								{0,1,0,0},
								{1,0,1,0},
								{0,1,0,1},
							};
	SparseMatrix<int> s((int*)Sparsearray, ROW, COL);
	s.Print();
	cout <<"*********"<< endl;
	//SparseMatrix<int> sm = s.TSmatrix();    //一般转置
	//sm.Print();
	SparseMatrix<int> sm = s.Quick_TSmatrix();//快速转置
	sm.Print();
	system("pause");
	return 0;
}
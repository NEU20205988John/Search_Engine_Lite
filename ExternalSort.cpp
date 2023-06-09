#include <iostream>
#include <limits>
#include <fstream> //处理文件的头文件
#include "QuickSort.h"
#include <ctime>
#include <cstdio>
#include <string>
#include <vector>
#include <cstring>
using namespace std;
//const int N = 75376000;
const int N = 4487550;
const int K = 10;

int treeNode[K];//LoserTree//定义非叶子结点数组，非叶子结点的值表示该结点记录的是哪一个数组来源的值
Tp2 node[K];//辅助数组，指向败者树的第K~2*K-1的叶子节点
//定义叶子结点数组，对应的数组小标表示是那个数据源，值表示数据源中的数据
//定义数据源的偏移指向数据，因为每次从数据源中读取了一个数据，对应的索引就要偏移到下一位
int Iterator[K] = {0};  //初始化全为0，因为每个数源都是要从第0个数据开始取
ifstream infile[K];//输入流
ofstream outfile[K];//输出流
string f_in[K];

void f_in_init() {
	for (int i = 0; i < K; i++) {
		f_in[i] += to_string(i);
		f_in[i] += ".txt";
	}
}

void init() {
	for (int i = 0; i < K; i++) {
		infile[i].close();
		infile[i].open(f_in[i]);
	}
}

//定义获取叶子结点的值的函数
Tp2 get(int index) {
	return node[index];
}

//定义设置叶子结点的值的函数
void set(int index) {
	if (Iterator[index] == N / K) {
		node[index].tid = numeric_limits<int>::max();
		return;
	}
	infile[index] >> node[index].tid >> node[index].did;
	Iterator[index] += 1;
}

void adjust(int index) { //败者树从node结点index开始向上调整
	//找到index对应的父结点
	int father = (index + K) >> 1;
	while (father > 0) {
		if ( (get(treeNode[father]).tid < get(index).tid) || ((get(treeNode[father]).tid == get(index).tid)
		        && (get(treeNode[father]).did <= get(index).did)) ) {
			int tmp = treeNode[father];//和父结点比较，败者的索引更新父结点
			treeNode[father] = index;
			index = tmp;  //向上继续比较，更新index
		}
		father /= 2;
	}
	treeNode[0] = index;   //最终胜者
}

//设置摆着树的初始化工作
void initTree() {
	//败者树要找出最小的指定个数的数，先用数据源数组中首个元素最大的数据源索引来初始化非叶子结点
	//然后开始调整
	memset(Iterator, 0, sizeof(Iterator));
	//1、设置node
	for (int idx = 0; idx != K; ++idx) {
		set(idx);
	}
	int MIN = 0; //初始最小元素来自数组源0
	for (int idx = 0; idx != K; ++idx) {
		if (node[idx].tid < node[MIN].tid)
			MIN = idx;
	}
	//2、初始化treeNode
	for (int idx = 0; idx != K; ++idx) {
		treeNode[idx] = MIN;
	}
	//3、开始从最后一个非叶子结点调整
	for (int idx = K - 1; idx >= 0; --idx) {
		adjust(idx);
	}
}

void merge() { //要输出cnt多少个值
	int cnt = N;
	ofstream finalfile("sorted_temp_index.txt");//最终文件输出流
	while (cnt) {
		finalfile << node[treeNode[0]].tid << ' ' << node[treeNode[0]].did << endl;
		set(treeNode[0]);  //输出一个，要更新对应的叶子结点的值
		adjust(treeNode[0]);  //调整
		--cnt;
	}
	finalfile.close();
}


Tp2 vc[N / K];

void divide() {
	clock_t startTime, endTime;
	int t, d;
	//ifstream totalfile("temp_index.txt");
	ifstream totalfile("small_temp_index.txt");
	for (int i = 0; i < K; i++) {
		ofstream fout(f_in[i]);
		startTime = clock();	///get program run time
		for (int j = 0; j < N / K; j++) {
			totalfile >> vc[j].tid >> vc[j].did;
		}
		endTime = clock();//计时结束
		cout << "第" << i << "路读文件耗时：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" <<
		     endl;
		startTime = clock();	///get program run time
		QuickSort(vc, 0, N / K);
		endTime = clock();//计时结束
		cout << "第" << i << "路快速排序耗时：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s"
		     << endl;
		startTime = clock();	///get program run time
		for (int j = 0; j < N / K; j++) {
			fout << vc[j].tid << ' ' << vc[j].did << endl;
		}
		endTime = clock();//计时结束
		cout << "第" << i << "路写文件耗时：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" <<
		     endl;
		fout.close();
	}
	totalfile.close();
}

int main() {
	//ios::sync_with_stdio(false);
	clock_t startTime, endTime;
	srand(time(NULL));
	f_in_init();
	init();
	startTime = clock();
	divide();
	endTime = clock();//计时结束
	cout << "k-路分治排序耗时：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	init();
	initTree();
	init();
	startTime = clock();	///get program run time
	merge();
	endTime = clock();//计时结束
	cout << "败者树归并耗时：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

	startTime = clock();
	ifstream temp_index("sorted_temp_index.txt");
	ofstream inverted_index("inverted_index.txt");
	if (!temp_index.is_open()) {
		cout << "Opening temp_index.txt failed";
		return 0;
	}
	if (!inverted_index.is_open()) {
		cout << "Opening inverted_index.txt failed";
		return 0;
	}
	int tid, did;
	int last_tid = -1, last_did = -1; //临时索引上一行的tid和did
	while (true) {
		temp_index >> tid >> did;
		if (!temp_index.eof()) {
			if (tid > last_tid) { //发现新单词
				inverted_index << endl << tid << '\t'; //给倒排索引列表添加新行
				last_tid = tid; //更新last_tid
				last_did = -1; //发现新单词，重新计数出现过的网页编号
			}
			if (did > last_did) { //发现新网页
				inverted_index << did << ','; //将此网页编号添加到倒排索引列表当前单词对应行
				last_did = did; //更新last_did
			}
		} else {
			break;
		}
	}
	temp_index.close();
	inverted_index.close();
	endTime = clock();//计时结束
	cout << "生成倒排索引耗时：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	return 0;
}
/*
clock_t startTime, endTime;
startTime = clock();//计时开始
endTime = clock();//计时结束
cout << "The run time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
system("pause");
*/

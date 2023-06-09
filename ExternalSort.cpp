#include <iostream>
#include <limits>
#include <fstream> //�����ļ���ͷ�ļ�
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

int treeNode[K];//LoserTree//�����Ҷ�ӽ�����飬��Ҷ�ӽ���ֵ��ʾ�ý���¼������һ��������Դ��ֵ
Tp2 node[K];//�������飬ָ��������ĵ�K~2*K-1��Ҷ�ӽڵ�
//����Ҷ�ӽ�����飬��Ӧ������С���ʾ���Ǹ�����Դ��ֵ��ʾ����Դ�е�����
//��������Դ��ƫ��ָ�����ݣ���Ϊÿ�δ�����Դ�ж�ȡ��һ�����ݣ���Ӧ��������Ҫƫ�Ƶ���һλ
int Iterator[K] = {0};  //��ʼ��ȫΪ0����Ϊÿ����Դ����Ҫ�ӵ�0�����ݿ�ʼȡ
ifstream infile[K];//������
ofstream outfile[K];//�����
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

//�����ȡҶ�ӽ���ֵ�ĺ���
Tp2 get(int index) {
	return node[index];
}

//��������Ҷ�ӽ���ֵ�ĺ���
void set(int index) {
	if (Iterator[index] == N / K) {
		node[index].tid = numeric_limits<int>::max();
		return;
	}
	infile[index] >> node[index].tid >> node[index].did;
	Iterator[index] += 1;
}

void adjust(int index) { //��������node���index��ʼ���ϵ���
	//�ҵ�index��Ӧ�ĸ����
	int father = (index + K) >> 1;
	while (father > 0) {
		if ( (get(treeNode[father]).tid < get(index).tid) || ((get(treeNode[father]).tid == get(index).tid)
		        && (get(treeNode[father]).did <= get(index).did)) ) {
			int tmp = treeNode[father];//�͸����Ƚϣ����ߵ��������¸����
			treeNode[father] = index;
			index = tmp;  //���ϼ����Ƚϣ�����index
		}
		father /= 2;
	}
	treeNode[0] = index;   //����ʤ��
}

//���ð������ĳ�ʼ������
void initTree() {
	//������Ҫ�ҳ���С��ָ��������������������Դ�������׸�Ԫ����������Դ��������ʼ����Ҷ�ӽ��
	//Ȼ��ʼ����
	memset(Iterator, 0, sizeof(Iterator));
	//1������node
	for (int idx = 0; idx != K; ++idx) {
		set(idx);
	}
	int MIN = 0; //��ʼ��СԪ����������Դ0
	for (int idx = 0; idx != K; ++idx) {
		if (node[idx].tid < node[MIN].tid)
			MIN = idx;
	}
	//2����ʼ��treeNode
	for (int idx = 0; idx != K; ++idx) {
		treeNode[idx] = MIN;
	}
	//3����ʼ�����һ����Ҷ�ӽ�����
	for (int idx = K - 1; idx >= 0; --idx) {
		adjust(idx);
	}
}

void merge() { //Ҫ���cnt���ٸ�ֵ
	int cnt = N;
	ofstream finalfile("sorted_temp_index.txt");//�����ļ������
	while (cnt) {
		finalfile << node[treeNode[0]].tid << ' ' << node[treeNode[0]].did << endl;
		set(treeNode[0]);  //���һ����Ҫ���¶�Ӧ��Ҷ�ӽ���ֵ
		adjust(treeNode[0]);  //����
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
		endTime = clock();//��ʱ����
		cout << "��" << i << "·���ļ���ʱ��" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" <<
		     endl;
		startTime = clock();	///get program run time
		QuickSort(vc, 0, N / K);
		endTime = clock();//��ʱ����
		cout << "��" << i << "·���������ʱ��" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s"
		     << endl;
		startTime = clock();	///get program run time
		for (int j = 0; j < N / K; j++) {
			fout << vc[j].tid << ' ' << vc[j].did << endl;
		}
		endTime = clock();//��ʱ����
		cout << "��" << i << "·д�ļ���ʱ��" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" <<
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
	endTime = clock();//��ʱ����
	cout << "k-·���������ʱ��" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	init();
	initTree();
	init();
	startTime = clock();	///get program run time
	merge();
	endTime = clock();//��ʱ����
	cout << "�������鲢��ʱ��" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

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
	int last_tid = -1, last_did = -1; //��ʱ������һ�е�tid��did
	while (true) {
		temp_index >> tid >> did;
		if (!temp_index.eof()) {
			if (tid > last_tid) { //�����µ���
				inverted_index << endl << tid << '\t'; //�����������б��������
				last_tid = tid; //����last_tid
				last_did = -1; //�����µ��ʣ����¼������ֹ�����ҳ���
			}
			if (did > last_did) { //��������ҳ
				inverted_index << did << ','; //������ҳ�����ӵ����������б�ǰ���ʶ�Ӧ��
				last_did = did; //����last_did
			}
		} else {
			break;
		}
	}
	temp_index.close();
	inverted_index.close();
	endTime = clock();//��ʱ����
	cout << "���ɵ���������ʱ��" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	return 0;
}
/*
clock_t startTime, endTime;
startTime = clock();//��ʱ��ʼ
endTime = clock();//��ʱ����
cout << "The run time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
system("pause");
*/

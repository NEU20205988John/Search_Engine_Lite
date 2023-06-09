struct Tp2 {
	int tid;
	int did;
};

void swap(Tp2 r[], int j, int k) {
	Tp2 temp = r[j];
	r[j] = r[k];
	r[k] = temp;
}

//快排-一次划分
int Partition(Tp2 r[], int low, int high) {
	int i = low, j = high; //初始化
	while (i < j) {//i,j交替指向轴值
		while (i < j && ((r[i].tid < r[j].tid) || ((r[i].tid == r[j].tid) && (r[i].did <= r[j].did)))) {
			j--;//右侧扫描
		}
		if (i < j) {//省去了i==j时的原地交换
			swap(r, i, j);//r[i]←→r[j];
			i++;//将较小记录交换到前面
		}
		while (i < j && ((r[i].tid < r[j].tid) || ((r[i].tid == r[j].tid) && (r[i].did <= r[j].did)))) {
			i++;//左侧扫描
		}
		if (i < j) {//省去了i==j时的原地交换
			swap(r, j, i);//r[j]←→r[i];
			j--;//将较大记录交换到后面
		}
	}
	return i;//i为轴值记录的最终位置
}

//快排-递归主体
void QuickSort(Tp2 r[], int first, int end) {
	//在序列first~end中递归地进行快速排序
	if (first < end) {
		int pivotpos = Partition(r, first, end);//一次划分
		QuickSort(r, first, pivotpos - 1);//对前子序列进行快速排序
		QuickSort(r, pivotpos + 1, end);//对后子序列进行快速排序
	}
}

//int main() {
//	int e[] = {4, 8, 2, 86, 23, 24, 656, 236, 3};
//	int l = sizeof(e) / sizeof(int);
//	cout << l;
//	QuickSort(e, 0, l - 1);
//	for (int i = 0; i < l; i++) {
//		cout << e[i] << ' ';
//	}
//	return 0;
//}
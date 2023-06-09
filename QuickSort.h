struct Tp2 {
	int tid;
	int did;
};

void swap(Tp2 r[], int j, int k) {
	Tp2 temp = r[j];
	r[j] = r[k];
	r[k] = temp;
}

//����-һ�λ���
int Partition(Tp2 r[], int low, int high) {
	int i = low, j = high; //��ʼ��
	while (i < j) {//i,j����ָ����ֵ
		while (i < j && ((r[i].tid < r[j].tid) || ((r[i].tid == r[j].tid) && (r[i].did <= r[j].did)))) {
			j--;//�Ҳ�ɨ��
		}
		if (i < j) {//ʡȥ��i==jʱ��ԭ�ؽ���
			swap(r, i, j);//r[i]����r[j];
			i++;//����С��¼������ǰ��
		}
		while (i < j && ((r[i].tid < r[j].tid) || ((r[i].tid == r[j].tid) && (r[i].did <= r[j].did)))) {
			i++;//���ɨ��
		}
		if (i < j) {//ʡȥ��i==jʱ��ԭ�ؽ���
			swap(r, j, i);//r[j]����r[i];
			j--;//���ϴ��¼����������
		}
	}
	return i;//iΪ��ֵ��¼������λ��
}

//����-�ݹ�����
void QuickSort(Tp2 r[], int first, int end) {
	//������first~end�еݹ�ؽ��п�������
	if (first < end) {
		int pivotpos = Partition(r, first, end);//һ�λ���
		QuickSort(r, first, pivotpos - 1);//��ǰ�����н��п�������
		QuickSort(r, pivotpos + 1, end);//�Ժ������н��п�������
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
r = []  # 待排序数组，使用全局变量进行原地排序以节省栈空间


def Partition(low: int, high: int) -> int:  # 快排-一次划分
	global r
	i = low  # 初始化
	j = high
	while i < j:  # i,j交替指向轴值
		while i < j and r[i].num <= r[j].num:
			j = j - 1  # 右侧扫描
		if i < j:  # 省去了i==j时的原地交换
			r[i], r[j] = r[j], r[i]  # r[i]←→r[j]
			i = i + 1  # 将较小记录交换到前面
		while i < j and r[i].num <= r[j].num:
			i = i + 1  # 左侧扫描
		if i < j:  # 省去了i==j时的原地交换
			r[j], r[i] = r[i], r[j]  # r[j]←→r[i]
			j = j - 1  # 将较大记录交换到后面
	return i  # i为轴值记录的最终位置


def QuickSort(first: int, end: int):  # 快排-递归主体
	global r
	if first < end:  # 一次划分后在序列first~end中递归地进行快速排序
		pivotpos = Partition(first, end)  # 一次划分
		QuickSort(first, pivotpos - 1)  # 对前子序列进行快速排序
		QuickSort(pivotpos + 1, end)  # 对后子序列进行快速排序


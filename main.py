# -*- coding: UTF-8 -*-
from time import time
import jieba
# import QuickSort
# import QuickSort2
import resdid as rsd
import QS2
# import QS
# 输入查询词并分词----查找对应单词编号----根据单词编号挑选出相关网页编号----网页编号整体排序-----统计个数----根据个数从小到大排序（出现最多的20个）----由网页编号查找网页链接并显示
# blanklist = ['\xa0', ' ', '\u3000', '\u2800', '\t', '\n']  # 空白字符列表，若遇到直接丢弃
print('初始化中...')
doc_id = open('doc_id.txt', 'r', encoding='utf-8')  # 网页编号文件 ####
term_id = open('term_id.txt', 'r', encoding='utf-8')  # 单词编号文件
# inv_index = open('inverted_index_bak.txt','r',encoding='utf-8') # 倒排索引文件
inv_index = open('inverted_index.txt','r',encoding='utf-8') # 倒排索引文件

doc_list = list()  # 储存doc_id文件的列表
doc_list_len = 0
term_list = list()  # 储存term_id文件的列表
# term_list_len = 0
index_list = list()  # 储存inverted_index文件的列表
index_list_len = 0

while True:  # 读入doc_id文件
    r_doc = str()
    r_doc = doc_id.readline()
    if r_doc == '':
        break
    spr_doc = r_doc.split()
    doc_list.append(list())
    doc_list[doc_list_len].extend(spr_doc[1:3])
    doc_list_len += 1

while True:  # 读入term_id文件
    r_term = str()
    r_term = term_id.readline()
    if r_term == '':
        break
    spr_term = r_term.split()
    term_list.append(spr_term[0])
term_list_len = len(term_list)

while True:  # 读入inverted_index文件
    r_index = str()
    r_index = inv_index.readline()
    if r_index == '':
        break
    spr_index0 = r_index.split()
    spr_index0 = spr_index0[1].split(',')
    spr_index0.pop()
    spr_index1 = list(map(int, spr_index0))
    index_list.append(spr_index1)
index_list_len = len(index_list)

term_id.close()
doc_id.close()
inv_index.close()

loopflag = True
while loopflag:
    res_list = list()  # 分词后得到的单词的编号列表
    res_word = input('请输入查询序列：')  # 输入查询文本
    start_time = time()
    wdl = jieba.lcut_for_search(res_word)  # 对查询文本进行分词
    res_cnt = len(wdl)  # 分词之后得到的单词编号个数
    print(wdl)
    curr_words = 0  # 当前已发现的单词个数
    for i in range(term_list_len):
        appeared = False  # 检查当前单词是否出现过
        for n in wdl:
            if n == term_list[i]:
                if not appeared:  # 若当前单词已经出现过，则不再加入单词编号列表
                    res_list.append(i)
                    appeared = True
                curr_words += 1
        if curr_words == res_cnt:  # 若已找到全部单词，则立即停止查找
            break

    # 根据单词编号查找网页编号,得到相关网页编号列表
    rel_did = list()  # 与查询词相关的网页编号列表
    for y in res_list:
        rel_did.extend(index_list[y])

    QS2.r = rel_did
    QS2.QuickSort2(0, len(rel_did)-1)  # 得到排序之后相关网页编号列表
    rel_did = QS2.r
    # print(rel_did)

    max_num = [rsd.resdid() for n in range(20)]
    # max_num = [rsd.resdid()]*20
    # 统计相关网页编号出现次数并去重
    did_num = list()  # 按照包含用户输入词个数由大到小顺序排好序的列表（含20个网页）
    for curr in range(len(rel_did)):
        now_resdid = rsd.resdid()
        if curr == 0:
            now_resdid.did = rel_did[0]
            now_resdid.num = 1
            did_num.append(now_resdid)
        else:
            now_did = rel_did[curr]  # 当前网页编号
            for n in range(len(did_num)):  # 对于每个新加入的网页编号
                if now_did == did_num[n].did:  # 相等，说明遇到相同的网页编号，个数加一
                    did_num[n].num += 1  # 个数加一
                    break
            else:  # 相等，说明遇到新的网页编号
                if did_num[-1].num > max_num[-1].num:
                    pos = int()
                    for i in range(len(max_num)):
                        if did_num[-1].num > max_num[i].num:
                            pos = i
                            break
                    for j in range(len(max_num) - 1, pos, -1):
                        max_num[j] = max_num[j-1]
                    max_num[pos] = did_num[-1]

                now_resdid.did = now_did  # now_resdid置为新的网页编号
                now_resdid.num = 1
                did_num.append(now_resdid)

    # 根据出现次数对相关网页编号排序并输出网页编号
    tp_did = list()  # 最终的20个相关网页编号
    for n in range(len(max_num)):
        tp_did.append(max_num[n].did)  # 保存对应网页编号

    resdid_cnt = len(did_num) #统计查询到的相关网页编号个数
    if resdid_cnt == 0:                    #未查询到
        print('未查询到相关结果')
    else:
        while tp_did[-1] == -1:  # 查询到的结果数量少于20个，删除列表中多余的-1
            tp_did.pop()
        print('相关网页链接：（最多20个）')
        print(tp_did)
        for cur in tp_did:
            print('{}\t{}'.format(doc_list[cur][0], doc_list[cur][1]))

    end_time = time()
    t_cost = end_time - start_time
    print('耗时%0.3f秒' % t_cost)

    go = True
    while go:
        flag = input('要继续查询吗？输入y继续，输入n退出程序：')
        if flag == 'y':
            go = False
        elif flag == 'n':
            go = False
            loopflag = False

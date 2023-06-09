class TrieNode:
    chr = str()
    isword = bool()
    term_id = int()
    child = list()  # list of TrieNode

    def __init__(self, c: str = ''):
        self.chr = c
        self.isword = False
        self.term_id = -1
        self.child = []


def findWord(word: str, tree: TrieNode, term_id_count: int) -> (int, bool, TrieNode, int):
    i = 0  # char pointer to word          # returning [term_id, new_word, tree, term_id_count]
    ll = len(word)
    currentnode = tree
    while i < ll:  # word not finished
        xchr = [x.chr for x in currentnode.child]  # 将所有孩子节点对应的字符提出来作成一个新的list
        if word[i] not in xchr:  # there is no corresponding branch
            currentnode.child.append(TrieNode(word[i]))  # 给当前节点增加一个孩子，对应的字符为当前word[i]
            currentnode = currentnode.child[-1]  # 进入这个孩子
        else:  # there is corresponding branch
            currentnode = currentnode.child[xchr.index(word[i])]  # 进入当前word[i]对应的孩子
        i = i + 1  # 指针++

    if not currentnode.isword:
        currentnode.isword = True
        currentnode.term_id = term_id_count
        ret_id = term_id_count
        term_id_count = term_id_count + 1
        new_word = True
    else:
        ret_id = currentnode.term_id
        new_word = False

    return ret_id, new_word, tree, term_id_count

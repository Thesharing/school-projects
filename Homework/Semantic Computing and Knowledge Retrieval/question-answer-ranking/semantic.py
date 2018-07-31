from collections import namedtuple
import re
from nltk.tree import Tree
from typing import Union, List

LabelNode = namedtuple('Node', ['idx', 'node'])

DependencyTriple = namedtuple('DependencyTuple', ['dep', 'governor', 'dependent'])


class ParserTree:
    nonsense_attr = ['PU', 'DEG', 'DEC']

    def __init__(self, tree: Union[Tree, str]) -> None:
        if isinstance(tree, Tree):
            self.tree = tree
        elif isinstance(tree, str):
            self.tree = Tree.fromstring(tree)
        else:
            raise TypeError('ParserTree: Tree or str required.')
        self._flatten = None

    @property
    def flatten(self):
        if self._flatten is None:
            res = list(self.tree)

            def rec(node: Tree):
                for sub_node in node:
                    if isinstance(sub_node, Tree):
                        res.append(sub_node)
                        rec(sub_node)

            rec(self.tree)
            self._flatten = res
        return self._flatten

    def find_label(self, label: str) -> List[LabelNode]:
        return list(LabelNode(idx, node) for idx, node in enumerate(self.flatten) if node.label() == label)

    def find_nearest_label(self, node: LabelNode, label: str) -> Union[LabelNode, None]:
        index = node.idx
        for idx in range(min(index, len(self.flatten)), 0, -1):
            if self.flatten[idx].label() == label:
                return LabelNode(idx, self.flatten[idx])
        return None

    def get_leaf_word(self, label_node: LabelNode) -> List[str]:
        res = list()

        def rec(node: Tree):
            for sub_node in node:
                if isinstance(sub_node, Tree):
                    rec(sub_node)
                elif isinstance(sub_node, str):
                    if node.label() not in self.nonsense_attr:
                        if sub_node not in res:
                            res.append(sub_node)

        if label_node is not None:
            rec(label_node.node)
        return res

    def __str__(self):
        return re.sub(r'\s+', ' ', str(self.tree).replace('\n', ' '))

    def __repr__(self):
        return self.__str__()


if __name__ == '__main__':
    t = Tree.fromstring(
        '(ROOT (IP (NP (NP (NR 张旭) (PU ( ) (NR 明代)) (DNP (NP (NR 知县)) (PU )) (DEG 的)) (NP (NN 代表作))) (VP (VC 是) (NP (PN 什么))) (PU ？)))')
    t = ParserTree(t)
    vp = t.find_label('DNP')
    print(vp)
    nps = t.find_nearest_label(vp[0], 'NP')
    print(nps)
    leaves = t.get_leaf_word(nps)
    print(leaves)

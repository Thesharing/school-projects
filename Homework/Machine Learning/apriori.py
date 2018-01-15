"""
Description     : Simple Python implementation of the Apriori Algorithm

Usage:
    $python apriori.py -f DATASET.csv -s minSupport  -c minConfidence

    $python apriori.py -f DATASET.csv -s 0.15 -c 0.6
"""

import sys

from itertools import chain, combinations
from collections import defaultdict
from optparse import OptionParser


def subsets(arr):
    """ Returns non empty subsets of arr"""
    return chain(*[combinations(arr, i + 1) for i, a in enumerate(arr)])


def return_items_with_min_support(item_set, transaction_list, min_support, freq_set):
    """calculates the support for items in the itemSet and returns a subset
   of the itemSet each of whose elements satisfies the minimum support"""
    _itemSet = set()
    localSet = defaultdict(int)

    for item in item_set:
        for transaction in transaction_list:
            if item.issubset(transaction):
                freq_set[item] += 1
                localSet[item] += 1

    for item, count in list(localSet.items()):
        support = int(count)

        if support >= min_support:
            _itemSet.add(item)

    return _itemSet


def join_set(item_set, length):
    """Join a set with itself and returns the n-element itemsets"""
    return set([i.union(j) for i in item_set for j in item_set if len(i.union(j)) == length])


def get_item_set_transaction_list(data):
    transactionList = list()
    itemSet = set()
    for record in data:
        transaction = frozenset(record)
        transactionList.append(transaction)
        for item in transaction:
            itemSet.add(frozenset([item]))  # Generate 1-itemSets
    return itemSet, transactionList


def run_apriori(data, min_support, minConfidence):
    """
    run the apriori algorithm. data_iter is a record iterator
    Return both:
     - items (tuple, support)
     - rules ((pretuple, posttuple), confidence)
    """
    itemSet, transactionList = get_item_set_transaction_list(data)

    freqSet = defaultdict(int)
    largeSet = dict()
    # Global dictionary which stores (key=n-itemSets,value=support)
    # which satisfy minSupport

    assocRules = dict()
    # Dictionary which stores Association Rules

    oneCSet = return_items_with_min_support(itemSet,
                                            transactionList,
                                            min_support,
                                            freqSet)

    currentLSet = oneCSet
    k = 2
    while (currentLSet != set([])):
        largeSet[k - 1] = currentLSet
        currentLSet = join_set(currentLSet, k)
        currentCSet = return_items_with_min_support(currentLSet,
                                                    transactionList,
                                                    min_support,
                                                    freqSet)
        currentLSet = currentCSet
        k = k + 1

    def get_support(item):
        """local function which Returns the support of an item"""
        return int(freqSet[item])

    toRetItems = []
    for key, value in list(largeSet.items()):
        toRetItems.extend([(tuple(item), get_support(item))
                           for item in value])

    toRetRules = []
    for key, value in list(largeSet.items())[1:]:
        for item in value:
            _subsets = list(map(frozenset, [x for x in subsets(item)]))
            for element in _subsets:
                remain = item.difference(element)
                if len(remain) > 0:
                    confidence = get_support(item) / get_support(element)
                    if confidence >= minConfidence:
                        toRetRules.append(((tuple(element), tuple(remain)),
                                           confidence))
    return toRetItems, toRetRules


def print_results(items, rules):
    """prints the generated itemsets sorted by support and the confidence rules sorted by confidence"""
    for item, support in sorted(items, key=lambda item_support: item_support[1]):
        print("item: %s , %.3f" % (str(item), support))
    print("\n------------------------ RULES:")
    for rule, confidence in sorted(rules, key=lambda rule_confidence: rule_confidence[1]):
        pre, post = rule
        print("Rule: %s ==> %s , %.3f" % (str(pre), str(post), confidence))


def data_from_file(fname):
    """Function which reads from the file and yields a generator"""
    result = []
    file_iter = open(fname, 'rU')
    for line in file_iter:
        line = line.strip().rstrip(',')  # Remove trailing comma
        record = frozenset(line.split(','))
        result.append(record)
    return result


class Apriori:

    def __init__(self):
        self.items = []
        self.rules = []

    def fit(self, transaction_list, min_support: int = 2, min_size: int = 1, min_confidence: float = 0.6):
        itemset, self.rules = run_apriori(transaction_list, min_support, min_confidence)
        self.items.clear()
        for item, support in sorted(items, key=lambda i: i[1]):
            if len(item) >= min_size:
                self.items.append((item, support))

    def print(self):
        for itemset, support in sorted(self.items, key=lambda i: i[1]):
            print(str(itemset) + ' ' + str(support))


if __name__ == "__main__":

    optparser = OptionParser()
    optparser.add_option('-f', '--inputFile',
                         dest='input',
                         help='filename containing csv',
                         default=None)
    optparser.add_option('-s', '--minSupport',
                         dest='minS',
                         help='minimum support value',
                         default=2,
                         type='float')
    optparser.add_option('-c', '--minConfidence',
                         dest='minC',
                         help='minimum confidence value',
                         default=0.6,
                         type='float')

    (options, args) = optparser.parse_args()

    inFile = None
    if options.input is None:
        print("Input file path required.")
        exit(0)
    elif options.input is not None:
        data = data_from_file(options.input)
    else:
        print('No dataset filename specified, system with exit\n')
        sys.exit('System will exit')

    minSupport = options.minS
    minConfidence = options.minC

    items, rules = run_apriori(data, minSupport, minConfidence)

    print_results(items, rules)

# -*- coding=utf-8 -*-

__author__ = 'Thesharing'

import re
import copy
from lda import LdaModel

START_YEAR = 2007
END_YEAR = 2017


class DBLP:
    """
    Contains original data.
    self.data:
    {
        'conference': xxx,
        'title': xxx,
        'year': 20xx,
        'author': [
            xxx,
            xxx,
            xxx
        ]
    }
    """

    def __init__(self, data_file: str, set_level='error'):
        self.conference_list = ['IJCAI', 'AAAI', 'COLT', 'CVPR', 'NIPS', 'KR', 'SIGIR', 'KDD']
        self.start_year = START_YEAR
        self.end_year = END_YEAR
        self.data = []
        self.pattern = re.compile(r'\d{4}')
        node = {}
        for line in open(data_file, 'r', encoding='utf-8'):
            line = line.strip('\n')
            if len(line) <= 0:
                continue
            if line == '#########':
                if len(node.keys()) > 1:
                    if len(node.keys()) < 4:
                        if set_level == 'warning':
                            print('Key missing: %s' % str(node))
                    elif len(node['author']) <= 0:
                        if set_level == 'warning':
                            print('Author missing: %s' % str(node))
                    elif 'Preface' in node['title']:
                        if set_level == 'warning':
                            print('Preface found: %s' % str(node))
                    elif node['conference'] in self.conference_list:
                        self.data.append(node)
                node = {'author': []}
            else:
                word_list = line.split('\t')
                if word_list[0] == 'author':
                    node['author'].append(self._deal_with_author(word_list[1]))
                elif word_list[0] == 'Conference':
                    node['conference'] = self._deal_with_conference(word_list[1])
                elif word_list[0] == 'year':
                    node['year'] = int(word_list[1])
                else:
                    node[word_list[0]] = word_list[1]

    def _deal_with_conference(self, conference: str):
        for c in self.conference_list:
            if c in conference:
                return c
        return conference

    def _deal_with_author(self, author):
        match = self.pattern.search(author)
        if match is None:
            return author
        else:
            return author[:match.start()]

    def get_by_conference(self, conference: str):
        """
        Get all data by conference
        :param conference: Name of conference.
        :return: A list of original data which conference is specified.
        """
        return list(x for x in self.data if x['conference'] == conference)

    def get_author_group_list(self, start_year: int = START_YEAR, end_year: int = END_YEAR):
        return list(x['author'] for x in self.data if start_year <= x['year'] <= end_year)

    def find_occurrence_of_group(self, group):
        result = []
        for d in self.data:
            if set(group).issubset(set(d['author'])):
                result.append(d['author'])
        return result

    def get_title_list(self, start_year: int = START_YEAR, end_year: int = END_YEAR):
        return list(x['title'].strip('.').replace(':', '') for x in self.data if start_year <= x['year'] <= end_year)

    def get_title_by_conference(self, conference):
        """
        Get a list of title group by conference.
        :param conference: Name of conference.
        :return: A list of title.
        """
        return list(x['title'].strip('.').replace(':', '') for x in self.data if x['conference'] == conference)


class DBLPAuthorList:
    """
    Contains a list of authors group by the conference and the year of their latest paper.
    self.data:
    {
        'conference': xxx,
        'year': 20xx,
        'author' (author in this conference and in this year): [
            xxx,
            xxx,
            xxx
        ]
    }
    """

    def __init__(self, dataset: DBLP):
        self.data = {}
        self.start_year = dataset.start_year
        self.end_year = dataset.end_year
        for c in dataset.conference_list:
            self.data[c] = {}
            d = dataset.get_by_conference(c)
            author_year_list = {}
            for data in d:
                author_list = data['author']
                for author in author_list:
                    if author in author_year_list.keys():
                        if author_year_list[author] < data['year']:
                            author_year_list[author] = data['year']
                    else:
                        author_year_list[author] = data['year']
            for i in range(self.start_year, self.end_year + 1):
                self.data[c][i] = []
                for k in author_year_list.keys():
                    if author_year_list[k] == i:
                        self.data[c][i].append(k)

    def get(self, conference: str, year: int):
        return self.data[conference][year]

    def get_author_list_by_conference(self, conference: str):
        result = []
        for i in range(self.start_year, self.end_year):
            for author in self.get(conference, i):
                if author not in result:
                    result.append(author)
        return result

    def get_by_conference(self, conference: str):
        return self.data[conference]

    def get_after_year(self, conference: str, year: int):
        result = {}
        for i in range(year, self.end_year + 1):
            result[i] = self.get(conference, i)
        return result

    def get_before_year(self, conference: str, year: int):
        result = {}
        for i in range(self.start_year, year + 1):
            result[i] = self.get(conference, i)
        return result


class DBLPAuthorPaper:
    """
    Data:
    {
        Author: {
            Paper: Year,
        },
    }
    """

    def __init__(self, dataset: DBLP, start_year: int = START_YEAR, end_year: int = END_YEAR):
        self._list = {}
        for d in dataset.data:
            for author in d['author']:
                # author - paper
                if start_year <= d['year'] <= end_year:
                    if author in self._list.keys():
                        self._list[author][d['title']] = d['year']
                    else:
                        self._list[author] = {d['title']: d['year']}

    def get_by_author(self, author: str):
        return self._list[author]

    def get_by_frequent_itemset(self, itemset: list):
        if len(itemset) <= 0:
            return []
        paper_list = copy.deepcopy(self._list[itemset[0]])
        for i in range(1, len(itemset)):
            for paper in copy.deepcopy(list(paper_list.keys())):
                if paper not in self._list[itemset[i]].keys():
                    paper_list.pop(paper)
        return paper_list


class DBLPGroup:
    class Node:
        def __init__(self, group):
            self.group = group
            self.papers = None
            self.latest_year = None
            self.subgroup = []

        def __repr__(self):
            return 'Group: ' + str(self.group) + '\nSubgroup: ' + str(self.subgroup) + '\nYear: ' + str(
                self.latest_year)

        def __str__(self):
            return self.__repr__()

    class Tree(Node):

        def __init__(self, group):
            DBLPGroup.Node.__init__(self, group)
            self.author_list = copy.deepcopy(group)

        def __repr__(self):
            return 'Group: ' + str(self.group) + ' Author list: ' + str(self.author_list) + ' Subgroup: ' + str(
                self.subgroup)

        def __str__(self):
            return self.__repr__()

    class Group:

        def __init__(self, tree):
            self.author_list = tree.author_list
            self.paper_list = {}
            self.topic_list = {}
            self.forest = [tree]

        def __repr__(self):
            def output(node, layer: int):
                out_str = '\t' * layer + 'Group: ' + str(node.group) + '\n' + \
                          '\t' * layer + 'Paper: ' + str(node.papers) + '\n' + \
                          '\t' * layer + 'Year: ' + str(node.latest_year) + '\n' + \
                          '\t' * layer + 'Subgroup:[ \n'
                for i in node.subgroup:
                    out_str += output(i, layer + 1)
                out_str += '\t' * layer + '] \n'
                return out_str

            out_str = 'Group Member: ' + str(self.author_list) + '\nPapers: ' + self._output_dict(
                self.paper_list) + '\nTopics: ' + self._output_dict(self.topic_list) + '\nContains: \n'
            for i in self.forest:
                out_str = out_str + '\tAuthor List:' + str(i.author_list) + '\n' + output(i, 1)
            return out_str

        def __str__(self):
            return self.__repr__()

        def _output_dict(self, d: dict):
            s = '{\n'
            for k, v in sorted(d.items(), key=lambda i: i[1], reverse=True):
                s = s + '\t' + str(k) + ' : ' + str(v) + '\n'
            s = s + '}'
            return s

        def calculate_similarity(self, group):
            """
            Calculate the similarity of a group vs another group
            :param group: destination group
            :return: similarity (float)
            """
            similar_member = 0
            for member in self.author_list:
                if member in group.author_list:
                    similar_member += 1
            return similar_member / len(self.author_list)

        def find_similar_group(self, dblg_group):
            """
            Find the most similar group in another DBLPGroup
            :param group:
            :return: the most similar group (DBLPGroup.Group)
            """
            similar_group = None
            max_similarity = 0
            for g in dblg_group.groups:
                similarity = self.calculate_similarity(g)
                if similarity > max_similarity:
                    max_similarity = similarity
                    similar_group = g
            return similar_group, max_similarity

    def __init__(self, itemset, author_paper: DBLPAuthorPaper = None, topic_model: LdaModel = None):
        self.itemset = itemset
        self.frequent_items = []
        self.forest = []
        self.groups = []
        # generate frequent_item
        for itemset, support in sorted(itemset, key=lambda i: i[1], reverse=True):
            self.frequent_items.append(itemset)
        # generate forest
        self._generate_forest()
        # generate group
        self._generate_group()
        # generate paper
        if author_paper is not None:
            self._generate_papers(author_paper)
        if topic_model is not None:
            self._generate_topic(topic_model)

    def _is_subset(self, parent: list, subset: list):
        for i in subset:
            if i not in parent:
                return False
        return True

    def _find_parent(self, ancestor: Node, parent: Node, group):
        is_grandchild = False
        for i in parent.subgroup:
            if self._is_subset(group, i.group):
                is_grandchild = True
                self._find_parent(ancestor, i, group)
        if not is_grandchild:
            for i in group:
                if i not in ancestor.author_list:
                    ancestor.author_list.append(i)
            parent.subgroup.append(self.Node(group))

    def _generate_forest(self):
        for i in self.frequent_items:
            is_child = False
            for tree in self.forest:
                if self._is_subset(i, tree.group):
                    self._find_parent(tree, tree, i)
            if not is_child:
                self.forest.append(self.Tree(i))

    # def output_forest(self):
    #
    #     def output(node: self.Node, layer: int):
    #         out_str = '\t' * layer + 'Group: ' + str(node.group) + '\n' + \
    #                   '\t' * layer + 'Paper: ' + str(node.papers) + '\n' + \
    #                   '\t' * layer + 'Year: ' + str(node.latest_year) + '\n' + \
    #                   '\t' * layer + 'Subgroup:[ \n'
    #         for i in node.subgroup:
    #             out_str += output(i, layer + 1)
    #         out_str += '\t' * layer + '] \n'
    #         return out_str
    #
    #     out_str = ''
    #     for i in self.forest:
    #         out_str = out_str + 'Author List:' + str(i.author_list) + '\n' + output(i, 0)
    #     return out_str

    def _generate_papers(self, author_paper: DBLPAuthorPaper):

        def iterate(node: self.Node, paper_list: dict):
            node.papers = author_paper.get_by_frequent_itemset(node.group)
            for p in node.papers.keys():
                if node.latest_year is None or node.latest_year < node.papers[p]:
                    node.latest_year = node.papers[p]
                if p not in paper_list.keys():
                    paper_list[p] = node.papers[p]
            for i in node.subgroup:
                iterate(i, paper_list)

        for i in self.groups:
            for j in i.forest:
                iterate(j, i.paper_list)

    # Join all subsets and prune the subset and the same
    def _generate_group(self):
        for i in self.forest:
            i_is_already_in_list = False
            for j in self.groups:
                i_is_subset = self._is_subset(j.author_list, i.author_list)
                j_is_subset = self._is_subset(i.author_list, j.author_list)
                if i_is_subset:
                    i_is_already_in_list = True
                    j.forest.append(i)
                    break
                elif j_is_subset:
                    i_is_already_in_list = True
                    j.author_list = i.author_list
                    j.forest.append(i)
                    break
            if not i_is_already_in_list:
                self.groups.append(self.Group(i))

    def get_group_member(self):
        return list(i.author_list for i in self.groups)

    def _generate_topic(self, topic_model: LdaModel):
        for i in self.groups:
            if len(i.paper_list.keys()) > 0:
                for p in i.paper_list.keys():
                    result = topic_model.predict(p)
                    for t in result.prob_list:
                        topic = t[0]
                        prob = t[1]
                        if topic not in i.topic_list:
                            i.topic_list[topic] = prob
                        else:
                            i.topic_list[topic] += prob


class DBLPConferenceTopicModel:

    def __init__(self, dataset: DBLP):
        self.topic_model = {}
        print("Initializing topic models...")
        for i in dataset.conference_list:
            lda = LdaModel()
            lda.fit(dataset.get_title_by_conference(i))
            self.topic_model[i] = lda
        print("Initialized.")

    def predict(self, title, conference):
        return self.topic_model[conference].predict(title)

    def get_model(self, conference):
        return self.topic_model[conference].model

    def predict_group(self, group: DBLPGroup):
        pass


class DBLPTopicModel:

    def __init__(self, topic_model: LdaModel, author_paper: DBLPAuthorPaper):
        self.topic_model = topic_model
        self.author_paper = author_paper

    # def predict_group(self, group: DBLPGroup):
    #     papers = []
    #     for i in group:
    #         for

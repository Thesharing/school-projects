from dataset import Data
from typing import List, Union


class MRR:  # Mean Reciprocal Rank

    def __init__(self):
        self.score = 0.0

    @staticmethod
    def _find_answers(scores, target) -> List[int]:
        return list(idx for idx, score in enumerate(scores) if score == target)

    def calculate(self, data: Data, result: Union[str, List[float]]) -> float:
        if isinstance(result, str):
            result_list = []
            with open(result, 'r', encoding='utf-8') as f:
                for line in f.readlines():
                    if len(line.strip()) > 0:
                        result_list.append(float(line.strip()))
            result = result_list
        if isinstance(result, list):
            mrr_list = []
            index = 0

            for question in data.questions:
                find_result = False
                scores = result[index: index + len(question.answers)]
                ranking = sorted(scores, reverse=True)

                for ranking_idx in range(len(ranking)):
                    if find_result:
                        break
                    for idx in self._find_answers(scores, ranking[ranking_idx]):
                        if question.answers[idx].label:
                            mrr_list.append(1 / (ranking_idx + 1))
                            find_result = True
                            break

                index += len(question.answers)

            assert len(mrr_list) == len(data.questions), 'Length not equal: MRR List: %s, Question List: %s' % (
                len(mrr_list), len(data.questions))
            self.score = sum(mrr_list) / len(mrr_list)
            return self.score


class MAP:  # Mean Average Precision

    def __init__(self):
        self.score = 0.0

    @staticmethod
    def _find_answers(scores, target, visited) -> List[int]:
        return list(idx for idx, score in enumerate(scores) if not visited[idx] and score == target)

    def calculate(self, data: Data, result: Union[str, List[float]]) -> float:
        if isinstance(result, str):
            result_list = []
            with open(result, 'r', encoding='utf-8') as f:
                for line in f.readlines():
                    if len(line.strip()) > 0:
                        result_list.append(float(line.strip()))
            result = result_list
        if isinstance(result, list):
            map_list = []
            index = 0

            for question in data.questions:

                ap_list = []
                scores = result[index: index + len(question.answers)]
                ranking = sorted(scores, reverse=True)

                right_count = 1
                visited = [False] * len(scores)

                for ranking_idx in range(len(ranking)):
                    for idx in self._find_answers(scores, ranking[ranking_idx], visited):
                        visited[idx] = True
                        if question.answers[idx].label:
                            ap_list.append(right_count / (ranking_idx + 1))
                            right_count += 1

                assert right_count - 1 == len(question.right_answers), 'Leave out some right answers: %s - %s' % (
                    right_count - 1, len(question.right_answers))

                map_list.append(sum(ap_list) / len(ap_list))

                index += len(question.answers)

            assert len(map_list) == len(data.questions), 'Length not equal: MRR List: %s, Question List: %s' % (
                len(map_list), len(data.questions))
            self.score = sum(map_list) / len(map_list)
            return self.score


if __name__ == '__main__':
    d = Data('./data/nlpcc-iccpol-2016.dbqa.testing-data', allow_duplicate=True)
    mrr = MRR()
    s = mrr.calculate(d, './result/res.txt')
    print('MRR: ', s)
    m = MAP()
    s = m.calculate(d, './result/res.txt')
    print('MAP: ', s)

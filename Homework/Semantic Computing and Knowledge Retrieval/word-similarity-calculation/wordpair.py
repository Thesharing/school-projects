import csv


class WordPair:

    def __init__(self, path: str):
        self.pair = []
        with open(path, 'r', encoding='utf-8', newline='') as f:
            reader = csv.reader(f)
            for row in reader:
                self.pair.append((row[0], row[1], float(row[2])))

    def true_score(self):
        return list(i[2] for i in self.pair)


if __name__ == '__main__':
    word_pair = WordPair('./Data/MTURK-771.csv')
    print(word_pair.pair)

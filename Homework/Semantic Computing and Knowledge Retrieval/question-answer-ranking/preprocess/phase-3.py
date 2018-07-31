import json
from semantic import ParserTree, DependencyTriple
from dataset import Data
from persistence import Persistence

if __name__ == '__main__':
    phase = 'test'  # 'train'
    file_num = 60
    data = Data('../data/nlpcc-iccpol-2016.dbqa.' + phase + 'ing-data', segmentation=True, allow_duplicate=True)
    path = '../data/' + phase
    for i in range(file_num):
        print('Processing File: %d' % i)
        with open(path + '/processed/question-' + str(i) + '.txt.json', 'r', encoding='utf-8') as f:
            sentences = json.load(f)['sentences']
            for idx in range(i * 100, min(i * 100 + 100, len(data.questions))):
                question = data.questions[idx]
                sentence = sentences[idx % 100]
                tokens = list(token['word'] for token in sentence['tokens'])
                ners = list(token['word'] for token in sentence['tokens'] if token['ner'] != "O")
                deps = list(DependencyTriple(dep['dep'], dep['governorGloss'], dep['dependentGloss']) for dep in
                            sentence['enhancedPlusPlusDependencies'])
                try:
                    tree = ParserTree(sentence['parse'])
                except ValueError:
                    print(sentence['parse'])
                if ''.join(question.question).strip() != ''.join(tokens).strip():
                    print('Not the same question. %s - %s' % (''.join(question.question), ''.join(tokens)))
                question.question = tokens
                question.dep = deps
                question.parse = tree
                question.ner = ners
    Persistence.save_data(data, path='../' + phase + '.txt')

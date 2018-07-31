import os
import random
import json
import requests
from time import sleep
from math import log
from scipy import stats
from bs4 import BeautifulSoup as bs
from error import handle_error
from wordpair import WordPair


class GoogleSearchSimilarity:
    headers = {
        'user_agent': None,
        'accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
        'accept-encoding': 'gzip, deflate, br',
        'accept-language': 'en-US;q=0.9,en;q=0.8',
    }

    user_agent_list = [
        'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.146 Safari/537.36',
        'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:59.0) Gecko/20100101 Firefox/59.0',
        'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36 Edge/16.16299',
        'Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko'
    ]

    google_site_list = [
        'https://www.google.co.uk/',
        'https://www.google.co.jp/',
        'https://www.google.co.kr/',
        'https://www.google.co.th/',
        'https://www.google.co.il/',
        'https://www.google.co.in/',
        'https://www.google.com.au/',
        'https://www.google.com.br/'
    ]

    google_search_url = 'search?q={word}&btnG=Search&gbv=1'

    data_file_path = './data/google_search_res.txt'

    waiting_list_file_path = './data/waiting_list.txt'

    c = 5

    n = 10e10

    method_list = [
        'jaccard', 'overlap', 'dice', 'pmi', 'ngd'
    ]

    def __init__(self, word_pair: WordPair):
        self.word_pair = word_pair

    def _get_search_url(self, word: str):
        return self.google_site_list[random.randint(0, len(self.google_site_list) - 1)] + \
               self.google_search_url.format(word=word)

    def _get_headers(self):
        self.headers['user_agent'] = self.user_agent_list[random.randint(0, len(self.user_agent_list) - 1)]
        return self.headers

    def crawl(self, word: str):
        res = 0.0
        try:
            r = requests.get(self._get_search_url(word), headers=self._get_headers(), verify=False, timeout=60)
            r.raise_for_status()
            b = bs(r.text, 'lxml')
            l = b.find_all(id='resultStats')
            if len(l) > 0:
                c = l[0].string.split()
                if len(c) > 2:
                    try:
                        res = float(c[1].replace(',', ''))
                    except ValueError as e:
                        handle_error(word + ': ' + str(e))
        except requests.exceptions.RequestException as e:
            handle_error(word + ': ' + str(e))
        return res

    def prepare_data(self):
        requests.packages.urllib3.disable_warnings()
        data = {}
        waiting_list = []
        if os.path.isfile(self.data_file_path):
            with open(self.data_file_path, 'r', encoding='utf-8') as f:
                data = json.load(f)
        for word_pair in self.word_pair.pair:
            for word in [word_pair[0], word_pair[1], '%27' + word_pair[0] + '%27+%27' + word_pair[1] + '%27']:
                if word not in data.keys():
                    try:
                        sleep(random.uniform(5, 15))
                        res = self.crawl(word)
                        print('%s - %f' % (word, res))
                        if res == 0.0:
                            waiting_list.append(word)
                        else:
                            data[word] = res
                    except:
                        waiting_list.append(word)
        with open(self.data_file_path, 'w', encoding='utf-8') as f:
            json.dump(data, f, indent=4)
        with open(self.waiting_list_file_path, 'w', encoding='utf-8') as f:
            json.dump(waiting_list, f)

    def handle_waiting_list(self):
        requests.packages.urllib3.disable_warnings()
        if os.path.isfile(self.waiting_list_file_path):
            with open(self.waiting_list_file_path, 'r', encoding='utf-8') as f:
                word_list = json.load(f)
                wating_list = []
                data = []
                for word in word_list:
                    try:
                        sleep(random.uniform(5, 15))
                        res = self.crawl(word)
                        print('%s - %f' % (word, res))
                        if res == 0.0:
                            wating_list.append(word)
                        else:
                            [word] = res
                    except:
                        wating_list.append(word)
            with open('./data/waiting_list_res.txt', 'w', encoding='utf-8') as f:
                json.dump(data, f, indent=4)
            with open(self.waiting_list_file_path, 'w', encoding='utf-8') as f:
                json.dump(wating_list, f)

    def similarity_score(self, method_name: str):
        with open(self.data_file_path, 'r', encoding='utf-8') as f:
            data = json.load(f)
            score = []
            for word_pair in self.word_pair.pair:
                for word in [word_pair[0], word_pair[1], '%27' + word_pair[0] + '%27+%27' + word_pair[1] + '%27']:
                    if word not in data.keys():
                        handle_error('%s not find in data.' % word)
                        exit(1)
                score.append(getattr(self, method_name)(data[word_pair[0]], data[
                    word_pair[1]], data['%27' + word_pair[0] + '%27+%27' + word_pair[1] + '%27']))
        return score

    def jaccard(self, p, q, p_q):
        return p_q / (p + q - p_q) if p_q > self.c else 0.0

    def overlap(self, p, q, p_q):
        return p_q / min(p, q) if p_q > self.c else 0.0

    def dice(self, p, q, p_q):
        return 2 * p_q / (p + q) if p_q > self.c else 0.0

    def pmi(self, p, q, p_q):
        return (log(p_q * self.n / (p * q), 2)) if p_q > self.c else 0.0

    def ngd(self, p, q, p_q):
        return (max(log(p, 2), log(q, 2)) - log(p_q, 2)) / (log(self.n, 2) - min(log(p, 2), log(q, 2)))


if __name__ == '__main__':
    wp = WordPair('./Data/MTURK-771.csv')
    g = GoogleSearchSimilarity(word_pair=wp)
    print(stats.spearmanr(wp.true_score(), g.similarity_score('pmi')).correlation)

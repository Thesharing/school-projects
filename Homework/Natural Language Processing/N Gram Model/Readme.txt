preparation.py：对训练集进行一定处理
training.py：训练词典
bigram_add_one.py：add-one平滑的bigram模型,δ=0.001
bigram_good_turing.py：Katz平滑的bigram模型
trigram_add_one.py：add-one平滑的trigram模型, 将频次为0的数据置为1/max
trigram_good_turing.py：Katz平滑的trigram模型
four_add_one.py：add-one平滑的four-gram模型
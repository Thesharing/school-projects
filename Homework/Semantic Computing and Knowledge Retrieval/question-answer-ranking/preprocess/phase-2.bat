set PHASE=test
set LIBRARY="C:/Tool/Stanford NLP/Stanford-CoreNLP"
cd %LIBRARY%
set FILEPATH=C:/Users/Thesharing/Git/QA-Ranking/data/%PHASE%
set /a index=47
set /a endindex=47
:while
if %index% leq %endindex% (
    java -Xmx3g -cp "*" edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,pos,lemma,ner,parse -props StanfordCoreNLP-chinese.properties -file %FILEPATH%/question/question-%index%.txt -outputFormat json -outputDirectory %FILEPATH%/processed
    set /a index+=1
    goto :while
)
def listToString(s):    
  str1 = "" 
  for ele in s: 
    str1 += ele  
  return str1 
############################################################
with open('abstracts.json') as f:
  lines = f.readlines()

for index, line in enumerate(lines):
  line = line[:len(line)-1] + ',' + '\n'
  lines[index] = line

lines[len(lines)-1] = lines[len(lines)-1][:-2]

if not lines[0] == '{ \n "abstracts": [ \n':
  lines.insert(0,'{ \n "abstracts": [ \n')
  lines.insert(len(lines),']\n}')

listToStr = listToString(lines)

with open('abstracts_fine.json', 'w') as f:
  f.write(listToStr)

###########################################################
# MANEJANDO EL JSON
import json
abstracts = []

with open('abstracts_fine.json') as d:
  data = json.load(d)

for state in data['abstracts']:
  abstracts.append(state['abstract'])


for index, abstract in enumerate(abstracts):
  abstract = abstract + "tilin"
  abstracts[index] = abstract

#print(abstracts)

################################################################
# MANEJO DE TEXTO

# Importing modules
import pandas as pd
# NLP Processing
import nltk

#nltk.download('popular')

# Tokenization
from nltk.tokenize import word_tokenize
# Lemmatization 
from nltk.stem import WordNetLemmatizer
# Load the regular expression library
import re

from nltk.tokenize import word_tokenize

papers = pd.DataFrame(abstracts)
papers.columns = ["paper_text"]
print(papers.head(10))


# 1 Remove punctuation
papers['paper_text_processed'] = papers['paper_text'].map(lambda x: re.sub('[,\.!?]', '', x))

# 2 Convert the processed abstracts to lowercase
papers['paper_text_processed'] = papers['paper_text_processed'].map(lambda x: x.lower())

# 3 remove all tokens that are not alphabetic
papers['paper_text_processed'] = papers['paper_text_processed'].map(lambda x: ' '.join([word for word in word_tokenize(x) if word.isalpha()]))

# 4 stopwords
# get all stopwords 
sr = nltk.corpus.stopwords.words('english')
papers['paper_text_processed'] = papers['paper_text_processed'].map(lambda x: ' '.join([word for word in word_tokenize(x) if not word in sr]))

# 5 lemmatization
# lemmatizer = WordNetLemmatizer() # generating lemmatizer object

# papers['paper_text_processed'] = papers['paper_text_processed'].map(lambda x: ' '.join([lemmatizer.lemmatize(word) for word in word_tokenize(x)]))

# Print out the first rows of papers
papers.head(20)

original_tokens = []

for index, row in papers.iterrows():
  original_tokens = original_tokens + word_tokenize(row['paper_text_processed'])

print(original_tokens)

##########################################################
# Afinando todas las palabras :)
file1 = open("txt_fine.txt","w")
for t in original_tokens:
  t = t + "\n"
  file1.writelines(t)
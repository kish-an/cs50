from cs50 import get_string
import re

sentence = get_string("Text: ")

# Split sentence by spaces
sentence = sentence.split()

letters = 0
words = 0
sentences = 0

# Loop over setence list
for word in sentence:
    # Count words
    words += 1

    # Count letters
    filtered_word = re.sub('[^A-Za-z0-9]+', '', word)
    letters += len(filtered_word)

    # Count sentences
    sentence_delims = ['!', '?', '.']
    if any(delim in word for delim in sentence_delims):
        sentences += 1
    # Above is the same as:
    # for delim in sentence_delims:
    #     if delim in word:


AVERAGE = 100 / words
# Coleman-Liau Index
index = (0.0588 * letters * AVERAGE) - (0.296 * sentences * AVERAGE) - 15.8

if index < 1:
    print("Before Grade 1")
elif index > 16:
    print("Grade 16+")
else:
    print(f"Grade {round(index)}")
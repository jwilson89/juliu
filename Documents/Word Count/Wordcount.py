import string

def count_words(text):
    words = text.split()
    counts = {}

    for word in words:
        word = word.lower().strip(string.punctuation)
        if word in counts:
            counts[word] = counts[word] + 1
        else:
            counts[word] = 1
    return counts
    

result = count_words("the the the the best best car car car car car on the planet of this very earth")

print(result)
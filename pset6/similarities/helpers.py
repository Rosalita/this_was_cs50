from nltk.tokenize import sent_tokenize

def lines(a, b):
    """Return lines in both a and b"""
    a_lines = a.splitlines()
    b_lines = b.splitlines()

    matches = []

    for line in a_lines:
        if line in b_lines:
            if line not in matches:
                matches.append(line)
    return matches

def sentences(a, b):
    """Return sentences in both a and b"""

    a_sent_text = sent_tokenize(a)
    b_sent_text = sent_tokenize(b)

    matches = []

    for sentence in a_sent_text:
        if sentence in b_sent_text and sentence not in matches:
            matches.append(sentence)
    return matches


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    a_words = a.split()
    b_words = b.split()

    a_substrings = listSubstrings(a_words, n)
    b_substrings = listSubstrings(b_words, n)

    matches = []

    for substring in a_substrings:
        if substring in b_substrings and substring not in matches:
                matches.append(substring)
    return matches

    return []

def listSubstrings(words, n):
    found_substrings = []
    for word in words:
        substrings = getSubstrings(word, n)
        if substrings is not None:
            if isinstance(substrings, list):
                for string in substrings:
                    if string not in found_substrings:
                        found_substrings.append(string)
            else:
                if substrings not in found_substrings:
                    found_substrings.append(substrings)
    return found_substrings

def getSubstrings(string, n):
    if n > len(string):
        return
    if n == len(string):
        return string
    substrings = []
    start = 0
    end = start + n

    while end < len(string) + 1:
        substrings.append(string[start:end])
        start += 1
        end += 1
    return substrings

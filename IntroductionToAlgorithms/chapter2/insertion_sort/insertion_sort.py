#!/usr/bin/python

def insertion_sort(ai):
    i = 0
    # li = list(ai)
    for item in ai:
        if i > 0:
            j = i - 1
            while j >=0 and ai[j] > item:
                ai[j+1] = ai[j]
                j = j -1
            ai[j+1] = item
        i = i + 1
    return ai

if __name__ == '__main__':
    ai = [2, 1, 3, 6,3, 9]
    insertion_sort(ai)
    print(ai)
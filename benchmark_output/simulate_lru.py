#! /usr/bin/env python
import random
from statistics import mean
L2_CACHE_SIZE = 24576 / 6
SET_ASSOZIATE = 8
NUMBER_OF_CACHE_SETS = L2_CACHE_SIZE / SET_ASSOZIATE


class CacheSet():
    
    def __init__(self):
        self.Line = [False] * 8
        self.access_counter = 0
    def access(self):
        #print self.access_counter
        if self.access_counter < 8:
            self.Line[self.access_counter] = True
        self.access_counter += 1
    def getAccsessedCacheLines(self):
        return self.Line.count(True)
    def __repr(self):
        print self.Line
        
        
def simmulate():
    result = 0
    cache_sets = [CacheSet() for _ in range( NUMBER_OF_CACHE_SETS)]
    for _ in range(L2_CACHE_SIZE * 6):
        random.choice(cache_sets).access()
            
    for line in cache_sets:
        result += line.getAccsessedCacheLines()
            
    #print cache_sets
    return result / float(L2_CACHE_SIZE)

print mean([simmulate() for _ in range(100)]) 

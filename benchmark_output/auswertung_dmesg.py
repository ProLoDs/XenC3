#! /usr/bin/env python

import sys

if(len(sys.argv) < 2):
    print "Usage: python auswertung_dmesg.py <Input file>"
    sys.exit(0)
flush_cache = 0
good_path = 0
swap_dom0 = 0
enough_cache_misses = 0
        
raw_datasets = []
with open(sys.argv[1],"r") as dmesg_in:
    raw = dmesg_in.read()
    
for dataset in raw.split("Total"):
    if len(dataset.split("\n")) == 7:
        raw_datasets.append(dataset)
        
for d in raw_datasets:
    splitted = d.split("\n")
    splitted.pop()
    splitted.pop(0)
    for a in splitted:
        if "FLush" in a.split(":")[0]:
            flush_cache += int(a.split(":")[1])
        if "Good" in a.split(":")[0]:
            good_path += int(a.split(":")[1])
        if "Swap" in a.split(":")[0]:
            swap_dom0 +=  int(a.split(":")[1])
        if "Miss" in a.split(":")[0]:
            enough_cache_misses +=int(a.split(":")[1])
            
total = flush_cache + good_path + swap_dom0 + enough_cache_misses
print "Cache flushes: ",flush_cache / float(total)
print "Trusted ->Trusted: ",good_path / float(total)
print "Swaped: ",swap_dom0 / float(total)
print "Enough cache misses: ",enough_cache_misses / float(total)


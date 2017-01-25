#! /usr/bin/env python

# Converts 10m17.684s to 617.684 Seconds

credit = []
cache = []
creditc3 = []

def timestring_to_sec(input_string):
    mins = input_string.split("m")[0]
    secs = input_string.split("m")[1].replace("s","")
    return int(mins) * 60 + float(secs)

# adds the data of a file to a list
def add_to_list(input_string,dest_list):
    with open(input_string,"r") as dom1_cache:
        dom1_cache_data = dom1_cache.read()
    
    for line in dom1_cache_data.split("\n"):
        if "real" in line:
            dest_list.append(timestring_to_sec(line.split("\t")[1]))


add_to_list("DomU2_cache.txt",cache)
add_to_list("Dom1_cache.txt",cache)
add_to_list("DomU2_credit.txt",credit)
add_to_list("Dom1_credit.txt",credit)
#print credit , cache

with open("output.csv","w") as out:
    for c1,c2,c3 in map(None,credit,cache,creditc3):
        #print str(c1) + "," + str(c2) + "," + str(c3)
        out.write(str(c1) + ";" + str(c2) + ";" + str(c3) + "\n")
    

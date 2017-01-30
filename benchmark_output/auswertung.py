#! /usr/bin/env python

# Converts 10m17.684s to 617.684 Seconds

credit = []
cache = []
creditc3_50 = []
creditc3_100 = []
def avg(l):
    return reduce(lambda x, y: x + y, l) / len(l)

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

add_to_list("output_cache_dom1.txt",cache)
add_to_list("output_cache_dom2.txt",cache)
add_to_list("output_credit_dom1.txt",credit)
add_to_list("output_credit_dom2.txt",credit)
add_to_list("output_c3_50_dom1.txt",creditc3_50)
add_to_list("output_c3_50_dom2.txt",creditc3_50)
add_to_list("output_c3_100_dom1.txt",creditc3_100)
add_to_list("output_c3_100_dom2.txt",creditc3_100)
#print credit , cache

with open("output.csv","w") as out:
    out.write("Credit Scheduler;Flush Cache;Credit C3 50 Scheduler;Credit C3 100 Scheduler\n")
    for c1,c2,c3,c4 in map(None,credit,cache,creditc3_50,creditc3_100):
        #print str(c1) + "," + str(c2) + "," + str(c3)
        out.write(str(c1) + ";" + str(c2) + ";" + str(c3) + ";" + str(c4) + "\n")
    out.write(";;;\n")
    out.write(str(avg(credit)) + ";" + str(avg(cache)) + ";" + str(avg(creditc3_50))+ ";" + str(avg(creditc3_100)) + "\n")
    

#! /usr/bin/env python

# Converts 10m17.684s to 617.684 Seconds
from statistics import pstdev
credit = []
cache = []
creditc3_100 = []
creditc3_300 = []
creditc3_600 = []
creditc3_l3 = []
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

add_to_list("output_c3_real_line_dom1.txt",creditc3_100)
add_to_list("output_c3_real_line_dom2.txt",creditc3_100)

add_to_list("output_c3_50_dom1.txt",creditc3_300)
add_to_list("output_c3_50_dom2.txt",creditc3_300)

add_to_list("output_c3_100_line_dom1.txt",creditc3_600)
add_to_list("output_c3_100_line_dom2.txt",creditc3_600)

add_to_list("output_c3_l3_line_dom1.txt",creditc3_l3)
add_to_list("output_c3_l3_line_dom2.txt",creditc3_l3)
#print credit , cache

with open("output.csv","w") as out:
    out.write("Credit Scheduler;Credit C3 100 Scheduler;Credit C3 300 Scheduler;Credit C3 300 Scheduler;Flush Cache;Credit C3 L3 100\n")
    for c1,c2,c3,c4,c5,c6 in map(None,credit,creditc3_100,creditc3_300,creditc3_600,cache,creditc3_l3):
        #print str(c1) + "," + str(c2) + "," + str(c3)
        out.write(str(c1) + ";" + str(c2) + ";" + str(c3) + ";" + str(c4) + ";" + str(c5) + ";" + str(c6) +"\n")
    out.write("Average;;;\n")
    out.write(str(avg(credit)) + ";"  + str(avg(creditc3_100))+ ";" + str(avg(creditc3_300))+ ";"+ str(avg(creditc3_600)) + ";" + str(avg(cache)) + ";"+str(avg(creditc3_l3))+"\n")
    out.write("standartabweichung;;;\n")
    out.write(str(pstdev(credit)) +";" + str(pstdev(creditc3_100))+ ";" + str(pstdev(creditc3_300)) + ";" + str(pstdev(creditc3_600)) + ";" + str(pstdev(cache)) +";"+ str(pstdev(creditc3_l3))+ "\n")

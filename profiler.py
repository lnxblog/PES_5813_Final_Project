#!/bin/python
import subprocess
import sys
import argparse

k=4 #windows size

# indexes to access readelf section dump
TYPE_IDX=3
ADDR_IDX=1
LEN_IDX=2
FUNC_IDX=7

profiler_str_format='function: {:<30} {:.2f}% samples:{:<15}'
def extract_addr(line):

	addr_list=[]
	for i in range(len(line)-k):
	
		# check for delimiter
		if line[i+3]==0xe3:
			addr_list.append(hex(int.from_bytes(line[i:i+3],"little")))
	
	return addr_list


# get ELF file name from cmdline args
if len(sys.argv) < 3:
	print("Please specify ELF file and LOG file in args")
	print("parser.py <ELF file> <LOG file>")
	quit()
	
elf_file = sys.argv[1]
log_file = sys.argv[2]
profile_addr_list=[]
key_addr=[]
func_dict={}


# READ LOG FILE AND EXTRACT PC VALUES
with open(log_file,"rb") as fp:
	while True:
		line = fp.read(512)
		if line==b'':
			break
		profile_addr_list.extend(extract_addr(line))
		

# GET FUNCTION NAMES AND ADDRESSES USING READELF UTILITY

elf_out = subprocess.check_output(['readelf','-s',elf_file])

# split to get lines
elf_out=elf_out.split(b"\n")


# skip first three lines of readelf output
for i in range(3,len(elf_out)):
	line = elf_out[i].decode('utf-8').split()
	if len(line)==0:
		continue
	if line[TYPE_IDX] == "FUNC":	# check for symbols with FUNC tag

		func_dict[int(line[ADDR_IDX],16)]=[line[FUNC_IDX],0]  # (key,value)=(address,function name)
		key_addr.append(int(line[ADDR_IDX],16))		# store addresses also

key_addr.sort()
total_samples=len(profile_addr_list)

# GET COUNT OF FUNCTIONS SAMPLED
for addr in profile_addr_list:
	addr_int = int(addr,16)
	prev=key_addr[0]
	
	#search which function sampled address belongs by going through sorted address list
	for key in key_addr:
		
		if key > addr_int:
			break
		prev=key
		
	func_dict[prev][1] = func_dict[prev][1] + 1 # increment count for the recorded function
	
func_hits_dict={}

print("******PROFILING RESULT*******")

for key,value in func_dict.items():
	
	# filter out functions which have zero sample
	if func_dict[key][1]>0:
		func_hits_dict[func_dict[key][0]]=func_dict[key][1] # store in dictionary key,value = function,count


func_hits_dict = sorted(func_hits_dict.items(),key=lambda x: x[1], reverse=True)
func_hits_dict = dict(func_hits_dict)

#print(func_hits_dict)

for key,value in func_hits_dict.items():
	func_str = key
	count = value
	percent=(count/total_samples)*100

	formatted=profiler_str_format.format(func_str,percent,count)
	print(formatted)

print('TOTAL SAMPLES: '+ str(total_samples))

	
		
		
	
		

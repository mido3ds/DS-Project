#!/usr/bin/env python
import random

class enemy(object):
	def __init__(self, i, TY, T, H, Pow, Prd, Speed, R):
		self.i = i
		self.TY = TY
		self.T = T
		self.H = H
		self.Pow = Pow
		self.Prd = Prd
		self.Speed = Speed
		self.R = R
		
	def get(self):
		if self.Speed is not None:
			return "{0}    {1}     {2}     {3}     {4}     {5}     {6}	{7}\n".format(self.i, self.TY, self.T, self.H, self.Pow, self.Prd, self.Speed, self.R)
		else:
			return "{0}    {1}     {2}     {3}     {4}     {5}     {6}\n".format(self.i, self.TY, self.T, self.H, self.Pow, self.Prd, self.R)
			
def GetType():
    return random.randrange(0, 3)
    
TH = random.randrange(1, 500)
N = random.randrange(1, 20)
TP = random.randrange(1, 300)

# generate enemies
n = int(input("Number of enemies: "))
includeSpeed = int(input("Include Enemies Speeds in File? [bonus] 0/1?"))
list = []

for i in range(n):
	TY = random.randrange(0, 3)
	T = int(random.random() * 10) 
	H = random.randrange(1, TH)
	Pow = random.randrange(1, TP)
	Prd = random.randrange(1, 10)
	Speed = None
	
	if (includeSpeed == 1):
		Speed = random.randrange(1, 30)

	R = random.choice(['A', 'B', 'C', 'D'])
	
	list.append(enemy(None, TY, T, H, Pow, Prd, Speed, R))
	
# sort list depending on arrive time T
list.sort(key=lambda e: e.T)

with open("input.txt", "w") as f:
    # print first two lines 
    f.write("{0}    {1}    {2}\n".format(TH, N, TP))

    for i in range(3):
        c = random.random() * 10
        f.write("{0} ".format(c))
    f.write("\n")
	
    # write enemies
    i = 0
    for e in list:
            e.i = i
            i += 1
            f.write(e.get())
	
    # end file
    f.write("-1")


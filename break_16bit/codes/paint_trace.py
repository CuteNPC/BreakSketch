import math
import random

import matplotlib.pyplot as plt
import numpy as np


no = 1
while no <= 128:
    file = open("./break_16bit/result/traceinfo/trace"+str(no)+".txt")
    xline = file.readline()[0:-2].split(' ')
    yline = file.readline()[0:-2].split(' ')
    #size = ((len(xline)//30000)//4)*4+1
    print(len(xline))
    size=61
    plt.scatter([math.log10(int(num)) for num in xline[::size]], [math.log10(
        int(num)) for num in yline[::size]], s=2, alpha=0.1, edgecolor="none")

    plt.title("TS Memory = "+str(no*512)+" Bucket Memory = "+str(no*4096))
    plt.savefig("./break_16bit/result/traceinfo/trace"+str(no)+".png",
                dpi=1000, bbox_inches='tight')
    plt.close()
    print("./break_16bit/result/traceinfo/trace"+str(no)+".png")
    no = no*2

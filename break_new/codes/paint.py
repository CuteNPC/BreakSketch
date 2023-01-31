#!/usr/bin/env python
# coding=utf-8

import pandas as pd
import matplotlib.pyplot as plt

# 作 相同输入、不同内存大小 下的F1图
def paint_F1_diff_memory():

    strawFf = u'./codes/res/F1_diff_memory/Break_Sketch_straw_F1.txt'
    basicFf = u'./codes/res/F1_diff_memory/Break_Sketch_basic_F1.txt'
    impFf = u'./codes/res/F1_diff_memory/Break_Sketch_improved_F1.txt'
    Op1Ff = u'./codes/res/F1_diff_memory/Break_Sketch_Optimization_1_F1.txt'
    Op2Ff = u'./codes/res/F1_diff_memory/Break_Sketch_Optimization_2_F1.txt'

    try:
        strawF = pd.read_csv(strawFf)
        basicF = pd.read_csv(basicFf)
        impF = pd.read_csv(impFf)
        Op1F = pd.read_csv(Op1Ff)
        Op2F = pd.read_csv(Op2Ff)

    except FileNotFoundError:
        print("paint_F1_diff_memory() error: file not found\n")
    
    else:    
        plt.title("F1score of Sketch")
        plt.grid(axis='y')
        plt.xlabel("log_2(memory/3)", loc='center')
        plt.ylabel("F1", loc='top')

        plt.plot(strawF['X'], strawF['Y'], label='strawman',
                linestyle='-', marker='*', markersize=10)
        plt.plot(basicF['X'], basicF['Y'], label='basic',
                linestyle='-', marker='*', markersize=10)
        plt.plot(impF['X'], impF['Y'], label='improved',
                linestyle='-', marker='*', markersize=10)
        plt.plot(Op1F['X'], Op1F['Y'], label='Op1',
                linestyle='-', marker='*', markersize=10)
        plt.plot(Op2F['X'], Op2F['Y'], label='Op2',
                linestyle='-', marker='*', markersize=10)

        plt.legend()

        plt.savefig('./codes/res/F1_diff_memory/F1score_memory.png')
        plt.show()
        plt.cla()

# 作 调整BF所占内存不同比例 的BFF1图
def paint_BFF1():

    strawFf = u'./codes/res/BFF1/Break_Sketch_straw_BFF1.txt'
    basicFf = u'./codes/res/BFF1/Break_Sketch_basic_BFF1.txt'
    impFf = u'./codes/res/BFF1/Break_Sketch_improved_BFF1.txt'
    Op1Ff = u'./codes/res/BFF1/Break_Sketch_Optimization_1_BFF1.txt'
    Op2Ff = u'./codes/res/BFF1/Break_Sketch_Optimization_2_BFF1.txt'

    try:
        strawF = pd.read_csv(strawFf)
        basicF = pd.read_csv(basicFf)
        impF = pd.read_csv(impFf)
        Op1F = pd.read_csv(Op1Ff)
        Op2F = pd.read_csv(Op2Ff)
    
    except FileNotFoundError:
        print("paint_BFF1() error: file not found\n")
    
    else:
        plt.title("F1score of Sketch")
        plt.grid(axis='y')
        plt.xlabel("16*BFmemory/TOTmemory", loc='center')
        plt.ylabel("F1", loc='top')

        plt.plot(strawF['X'], strawF['Y'], label='strawman',
             linestyle='-', marker='*', markersize=10)
        plt.plot(basicF['X'], basicF['Y'], label='basic',
             linestyle='-', marker='*', markersize=10)
        plt.plot(impF['X'], impF['Y'], label='improved',
            linestyle='-', marker='*', markersize=10)
        plt.plot(Op1F['X'], Op1F['Y'], label='Op1',
            linestyle='-', marker='*', markersize=10)
        plt.plot(Op2F['X'], Op2F['Y'], label='Op2',
            linestyle='-', marker='*', markersize=10)

        plt.legend()

        plt.savefig('./codes/res/BFF1/BFF1score.png')
        plt.show()
        plt.cla()

# 作 输入包数不同（保持内存与输入流总数的比例固定）下的 F1 图
def paint_F1_diff_packets():

    strawFf = u'./codes/res/F1_diff_packets/Break_Sketch_straw_F1.txt'
    basicFf = u'./codes/res/F1_diff_packets/Break_Sketch_basic_F1.txt'
    impFf = u'./codes/res/F1_diff_packets/Break_Sketch_improved_F1.txt'
    Op1Ff = u'./codes/res/F1_diff_packets/Break_Sketch_Optimization_1_F1.txt'
    Op2Ff = u'./codes/res/F1_diff_packets/Break_Sketch_Optimization_2_F1.txt'

    try:
        strawF = pd.read_csv(strawFf)
        basicF = pd.read_csv(basicFf)
        impF = pd.read_csv(impFf)
        Op1F = pd.read_csv(Op1Ff)
        Op2F = pd.read_csv(Op2Ff)

    except FileNotFoundError:
        print("paint_F1_diff_packets() error: file not found\n")
    
    else:    
        plt.title("F1score of Sketch")
        plt.grid(axis='y')
        plt.xlabel("num of packets (10^6)", loc='center')
        plt.ylabel("F1", loc='top')

        plt.plot(strawF['X'], strawF['Y'], label='strawman',
                linestyle='-', marker='*', markersize=10)
        plt.plot(basicF['X'], basicF['Y'], label='basic',
                linestyle='-', marker='*', markersize=10)
        plt.plot(impF['X'], impF['Y'], label='improved',
                linestyle='-', marker='*', markersize=10)
        plt.plot(Op1F['X'], Op1F['Y'], label='Op1',
                linestyle='-', marker='*', markersize=10)
        plt.plot(Op2F['X'], Op2F['Y'], label='Op2',
                linestyle='-', marker='*', markersize=10)

        plt.legend()

        plt.savefig('./codes/res/F1_diff_packets/F1score_packets.png')
        plt.show()
        plt.cla()

# 作 输入包数不同（保持内存与输入流总数的比例固定）下的 Time 图
def paint_Time():

    strawTf = u'./codes/res/time/Break_Sketch_straw_Time.txt'  # 这里python的相对路径是相对于主文件break的
    basicTf = u'./codes/res/time/Break_Sketch_basic_Time.txt'
    impTf = u'./codes/res/time/Break_Sketch_improved_Time.txt'
    Op1Tf = u'./codes/res/time/Break_Sketch_Optimization_1_Time.txt'
    Op2Tf = u'./codes/res/time/Break_Sketch_Optimization_2_Time.txt'
    
    try:
        strawT = pd.read_csv(strawTf)
        basicT = pd.read_csv(basicTf)
        impT = pd.read_csv(impTf)
        Op1T = pd.read_csv(Op1Tf)
        Op2T = pd.read_csv(Op2Tf)
    
    except FileNotFoundError:
        print("paint_Time() error: file not found\n")
    
    else:
        plt.title("Timecost of Sketch")
        plt.grid(axis='y')
        plt.xlabel("num of packet", loc='center')
        plt.ylabel("time(s)", loc='top')

        plt.plot(strawT['X'], strawT['Y'], label='strawman',
                linestyle='-', marker='*', markersize=10)
        plt.plot(basicT['X'], basicT['Y'], label='basic',
                linestyle='-', marker='*', markersize=10)
        plt.plot(impT['X'], impT['Y'], label='improved',
                linestyle='-', marker='*', markersize=10)
        plt.plot(Op1T['X'], Op1T['Y'], label='Op1',
                 linestyle='-', marker='*', markersize=10)
        plt.plot(Op2T['X'], Op2T['Y'], label='Op2',
                linestyle='-', marker='*', markersize=10)

        plt.legend()

        plt.savefig('./codes/res/time/time.png')
        plt.show()
        plt.cla()

def main():
    paint_Time()

main()
#!/usr/bplt.xscale('log')in/env python
# coding=utf-8

import pandas as pd
import matplotlib.pyplot as plt

def calcu_aver(filename, st=0, ed=10):
    """计算 num 个.csv文件的数据的平均值并保存回.csv文件; st,ed 控制计算 '..._[st ~ ed-1].csv' 的平均值"""
    filepath_list = filename.split('.')
    filepath1, filepath2 = filepath_list[1],  filepath_list[2]
    X, sum = pd.DataFrame(), pd.DataFrame()
    for index in range(st, ed):
        tmpfilename = "." + filepath1 + "_" + str(index) + "." + filepath2
        tmpdata = pd.read_csv(tmpfilename, dtype = float, usecols = [i for i in range(1, 7)])
        if index == st:
            X = pd.read_csv(tmpfilename, usecols = [0])
            sum = tmpdata
        else: sum = sum + tmpdata
    sum = sum / (ed - st)
    resdata = pd.concat([X,sum],axis = 1)
    resdata.to_csv(filename)


def paint_module(filepath, filename ,title, xlabelname, xscalename, ylabelname):
    F1csv = pd.read_csv(filepath + filename + ".csv")

    plt.title(title)
    plt.grid(axis='y')
    plt.xlabel(xlabelname, loc='center')
    plt.ylabel(ylabelname, loc='top')
    plt.xscale(xscalename)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_straw'], label='strawman',
             linestyle='-', marker='o', markersize=6)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_basic_1'], label='basic1',
             linestyle='-', marker='s', markersize=6)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_basic_2'], label='basic2',
             linestyle='-', marker='d', markersize=6)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_improved'], label='improved',
             linestyle='-', marker='v', markersize=6)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_Optimization_1'], label='Op1',
             linestyle='-', marker='<', markersize=6)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_Optimization_2'], label='Op2',
             linestyle='-', marker='>', markersize=6)

    plt.legend()
    plt.savefig(filepath + filename+'.png', dpi=300, bbox_inches='tight')
    plt.show()
    plt.cla()



def paint_time(st=0, ed=10):
    """根据time_res中的数据绘制图像"""
    xlabelname = "num of packets"
    xscalename = "linear"
    filepath = u"./break_new/result/time_res/"
    
    calcu_aver(u"./break_new/result/time_res/time.csv", st, ed)
    paint_module(filepath, "time", "TimeCost of Sketch", xlabelname, xscalename, ylabelname = "time(s)")   


def paint_diff_TSmemory(st=0, ed=10):
    """根据TSmem_res中的数据绘制图像"""
    xlabelname = "TS memory/tot memory"
    xscalename = "linear"
    filepath = u"./break_new/result/TSmem_res/"
    
    calcu_aver(u"./break_new/result/TSmem_res/F1.csv", st, ed)
    calcu_aver(u"./break_new/result/TSmem_res/Precision.csv", st, ed)
    calcu_aver(u"./break_new/result/TSmem_res/Recall.csv", st, ed)

    paint_module(filepath, "F1", "F1score of Sketch", xlabelname, xscalename, ylabelname = "F1")
    paint_module(filepath, "Precision", "Precision of Sketch", xlabelname, xscalename, ylabelname = "PR")
    paint_module(filepath, "Recall", "Recall of Sketch", xlabelname, xscalename, ylabelname = "RR")


def paint_diff_Totalmemory(st=0, ed=10):
    """根据Totmem_res中的数据绘制图像"""
    xlabelname = "total memory"
    xscalename = "log"
    filepath = u"./break_new/result/Totmem_res/"
    
    calcu_aver(u"./break_new/result/Totmem_res/F1.csv", st, ed)
    calcu_aver(u"./break_new/result/Totmem_res/Precision.csv", st, ed)
    calcu_aver(u"./break_new/result/Totmem_res/Recall.csv", st, ed)
    
    paint_module(filepath, "F1", "F1score of Sketch", xlabelname, xscalename, ylabelname = "F1")
    paint_module(filepath, "Precision", "Precision of Sketch", xlabelname, xscalename, ylabelname = "PR")
    paint_module(filepath, "Recall", "Recall of Sketch", xlabelname, xscalename, ylabelname = "RR")

def main():
    # paint_diff_TSmemory(0, 6)
    paint_diff_Totalmemory(0, 1)
    # paint_time(0, 10)

main()

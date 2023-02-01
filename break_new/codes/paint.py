#!/usr/bplt.xscale('log')in/env python
# coding=utf-8

import pandas as pd
import matplotlib.pyplot as plt

def calcu_aver(filename, num = 10):
    """计算num个.csv文件的数据的平均值并保存回.csv文件; num 控制计算'..._[0 ~ num-1].csv'的平均值"""
    filepath_list = filename.split('.')
    filepath1, filepath2 = filepath_list[1],  filepath_list[2]
    X, sum = pd.DataFrame(), pd.DataFrame()
    for index in range(0, num):
        tmpfilename = "." + filepath1 + "_" + str(index) + "." + filepath2
        tmpdata = pd.read_csv(tmpfilename, dtype = float, usecols = [i for i in range(1, 7)])
        if index == 0:
            X = pd.read_csv(tmpfilename, usecols = [0])
            sum = tmpdata
        else: sum = sum + tmpdata
    sum = sum / num
    resdata = pd.concat([X,sum],axis = 1)
    resdata.to_csv(filename)

def paint_F1(filepath, xlabelname, xscalename):
    F1csv = pd.read_csv(filepath + u'F1.csv')

    plt.title("F1score of Sketch")
    plt.grid(axis='y')
    plt.xlabel(xlabelname, loc='center')
    plt.ylabel("F1", loc='top')
    plt.xscale(xscalename)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_straw'], label='strawman',
             linestyle='-', marker='.', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_basic_1'], label='basic1',
             linestyle='-', marker='.', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_basic_2'], label='basic2',
             linestyle='-', marker='.', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_improved'], label='improved',
             linestyle='-', marker='.', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_Optimization_1'], label='Op1',
             linestyle='-', marker='.', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_Optimization_2'], label='Op2',
             linestyle='-', marker='.', markersize=10)

    plt.legend()
    plt.savefig(filepath + u'F1score.png', dpi=300, bbox_inches='tight')
    plt.show()
    plt.cla()


def paint_RR(filepath, xlabelname, xscalename):
    F1csv = pd.read_csv(filepath + u'Recall.csv')

    plt.title("Recall of Sketch")
    plt.grid(axis='y')
    plt.xlabel(xlabelname, loc='center')
    plt.ylabel("RR", loc='top')
    plt.xscale(xscalename)

    plt.plot(F1csv['X'], F1csv['Break_Sketch_straw'], label='strawman',
             linestyle='-', marker='.', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_basic_1'], label='basic1',
             linestyle='-', marker='.', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_basic_2'], label='basic2',
             linestyle='-', marker='.', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_improved'], label='improved',
             linestyle='-', marker='.', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_Optimization_1'], label='Op1',
             linestyle='-', marker='.', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_Optimization_2'], label='Op2',
             linestyle='-', marker='.', markersize=10)

    plt.legend()

    plt.savefig(filepath + u'Recall.png', dpi=300, bbox_inches='tight')
    plt.show()
    plt.cla()


def paint_PR(filepath, xlabelname, xscalename):
    F1csv = pd.read_csv(filepath + u'Precision.csv')

    plt.title("Precision of Sketch")
    plt.grid(axis='y')
    plt.xlabel(xlabelname, loc='center')
    plt.ylabel("PR", loc='top')
    plt.xscale(xscalename)

    plt.plot(F1csv['X'], F1csv['Break_Sketch_straw'], label='strawman',
             linestyle='-', marker='.', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_basic_1'], label='basic1',
             linestyle='-', marker='.', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_basic_2'], label='basic2',
             linestyle='-', marker='.', markersize=10)         
    plt.plot(F1csv['X'], F1csv['Break_Sketch_improved'], label='improved',
             linestyle='-', marker='.', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_Optimization_1'], label='Op1',
             linestyle='-', marker='.', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_Optimization_2'], label='Op2',
             linestyle='-', marker='.', markersize=10)

    plt.legend()

    plt.savefig(filepath + u'Precision.png', dpi=300, bbox_inches='tight')
    plt.show()
    plt.cla()


def paint_time(num = 10):
    """根据time_res中的数据绘制图像"""
    calcu_aver(u"./break_new/result/time_res/time.csv", num)

    filepath = u"./break_new/result/time_res/"
    xlabelname = "num of packets"
    xscalename = "linear"
    F1csv = pd.read_csv(filepath + u'time.csv')

    plt.title("TimeCost of Sketch")
    plt.grid(axis='y')
    plt.xlabel(xlabelname, loc='center')
    plt.ylabel("time(s)", loc='top')
    plt.xscale(xscalename)

    plt.plot(F1csv['X'], F1csv['Break_Sketch_straw'], label='strawman',
             linestyle='-', marker='.', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_basic_1'], label='basic1',
             linestyle='-', marker='.', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_basic_2'], label='basic2',
             linestyle='-', marker='.', markersize=10)         
    plt.plot(F1csv['X'], F1csv['Break_Sketch_improved'], label='improved',
             linestyle='-', marker='.', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_Optimization_1'], label='Op1',
             linestyle='-', marker='.', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_Optimization_2'], label='Op2',
             linestyle='-', marker='.', markersize=10)

    plt.legend()

    plt.savefig(filepath + u'time.png', dpi=300, bbox_inches='tight')
    plt.show()
    plt.cla()


def paint_diff_TSmemory(num = 10):
    """根据TSmem_res中的数据绘制图像"""
    xlabelname = "TS memory/tot memory"
    xscalename = "linear"
    filepath = u"./break_new/result/TSmem_res/"
    
    calcu_aver(u"./break_new/result/TSmem_res/F1.csv", num)
    calcu_aver(u"./break_new/result/TSmem_res/Precision.csv", num)
    calcu_aver(u"./break_new/result/TSmem_res/Recall.csv", num)

    paint_F1(filepath, xlabelname, xscalename)
    paint_PR(filepath, xlabelname, xscalename)
    paint_RR(filepath, xlabelname, xscalename)


def paint_diff_Totalmemory(num = 10):
    """根据Totmem_res中的数据绘制图像"""
    xlabelname = "total memory"
    xscalename = "log"
    filepath = u"./break_new/result/Totmem_res/"
    
    calcu_aver(u"./break_new/result/Totmem_res/F1.csv", num)
    calcu_aver(u"./break_new/result/Totmem_res/Precision.csv", num)
    calcu_aver(u"./break_new/result/Totmem_res/Recall.csv", num)
    
    paint_F1(filepath, xlabelname, xscalename)
    paint_PR(filepath, xlabelname, xscalename)
    paint_RR(filepath, xlabelname, xscalename)

def main():
    # paint_diff_TSmemory(3)
    paint_diff_Totalmemory(3)
    # paint_time(10)

main()

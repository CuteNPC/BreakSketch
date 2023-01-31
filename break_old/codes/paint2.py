#!/usr/bplt.xscale('log')in/env python
# coding=utf-8

import pandas as pd
import matplotlib.pyplot as plt

xlabelname = "BF memory/tot memory"
xscalename = "linear"
# 作 相同输入、不同内存大小 下的F1图


def paint_F1():
    F1csv = pd.read_csv(u'./codes/res2/F1.csv')

    plt.title("F1score of Sketch")
    plt.grid(axis='y')
    plt.xlabel(xlabelname, loc='center')
    plt.ylabel("F1", loc='top')
    plt.xscale(xscalename)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_straw'], label='strawman',
             linestyle='-', marker='*', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_basic'], label='basic',
             linestyle='-', marker='*', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_improved'], label='improved',
             linestyle='-', marker='*', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_Optimization_1'], label='Op1',
             linestyle='-', marker='*', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_Optimization_2'], label='Op2',
             linestyle='-', marker='*', markersize=10)

    plt.legend()
    plt.savefig('./codes/res2/F1score.png', dpi=300, bbox_inches='tight')
    plt.show()
    plt.cla()


def paint_RR():
    F1csv = pd.read_csv(u'./codes/res2/Recall.csv')

    plt.title("Recall of Sketch")
    plt.grid(axis='y')
    plt.xlabel(xlabelname, loc='center')
    plt.ylabel("RR", loc='top')
    plt.xscale(xscalename)

    plt.plot(F1csv['X'], F1csv['Break_Sketch_straw'], label='strawman',
             linestyle='-', marker='*', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_basic'], label='basic',
             linestyle='-', marker='*', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_improved'], label='improved',
             linestyle='-', marker='*', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_Optimization_1'], label='Op1',
             linestyle='-', marker='*', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_Optimization_2'], label='Op2',
             linestyle='-', marker='*', markersize=10)

    plt.legend()

    plt.savefig('./codes/res2/Recall.png', dpi=300, bbox_inches='tight')
    plt.show()
    plt.cla()


def paint_PR():
    F1csv = pd.read_csv(u'./codes/res2/Precision.csv')

    plt.title("Precision of Sketch")
    plt.grid(axis='y')
    plt.xlabel(xlabelname, loc='center')
    plt.ylabel("PR", loc='top')
    plt.xscale(xscalename)

    plt.plot(F1csv['X'], F1csv['Break_Sketch_straw'], label='strawman',
             linestyle='-', marker='*', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_basic'], label='basic',
             linestyle='-', marker='*', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_improved'], label='improved',
             linestyle='-', marker='*', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_Optimization_1'], label='Op1',
             linestyle='-', marker='*', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_Optimization_2'], label='Op2',
             linestyle='-', marker='*', markersize=10)

    plt.legend()

    plt.savefig('./codes/res2/Precision.png', dpi=300, bbox_inches='tight')
    plt.show()
    plt.cla()


def main():
    paint_F1()
    paint_PR()
    paint_RR()


main()

#!/usr/bin/env python
# coding=utf-8

import pandas as pd
import matplotlib.pyplot as plt


def paint_init():
    '''绘图函数框架'''
    F1csv = pd.read_csv(u"./break_16bit/data/analysize/flowData.csv")

    plt.title("")
    plt.grid(axis='y')
    plt.xlabel("flow size(total packets)", loc='center')
    #plt.ylabel()
    plt.xscale("log")
    plt.yscale("linear")
    # plt.plot(F1csv['size'], F1csv['flownum'], label='num of flow',
    #          linestyle='-', marker='.', markersize=5)
    # plt.plot(F1csv['size'], F1csv['initpnum'], label='init_num of packets',
    #          linestyle='-', marker='.', markersize=5)
    # plt.plot(F1csv['size'], F1csv['nowpnum'], label='now_num of packets',
    #          linestyle='-', marker='.', markersize=5)
    # plt.plot(F1csv['size'], F1csv['lossnum'], label='num of lost packets',
    #          linestyle='-', marker='.', markersize=5)
    plt.plot(F1csv['size'], F1csv['stdbreak'], label='standard break',
             linestyle='-', marker='.', markersize=5)
    plt.plot(F1csv['size'], F1csv['repbreak'], label='reported break(improved)',
             linestyle='-', marker='.', markersize=5)
    plt.plot(F1csv['size'], F1csv['lossrep'], label='loss reported break',
             linestyle='-', marker='.', markersize=5)
    plt.plot(F1csv['size'], F1csv['wrongrep'], label='wrong reported break',
             linestyle='-', marker='.', markersize=5)

    plt.legend()
    plt.savefig(u"./break_16bit/data/analysize/flowData.png", dpi=300, bbox_inches='tight')
    plt.show()
    plt.cla()

def paint_seq():
    '''绘图函数框架'''
    F1csv = pd.read_csv(u"./break_16bit/data/analysize/initSeq.csv")

    plt.title("")
    plt.grid(axis='y')
    plt.xlabel("seq", loc='center')
    plt.xscale("linear")
    plt.yscale("linear")
    plt.scatter(F1csv['seq'], F1csv['num'])

    plt.savefig(u"./break_16bit/data/analysize/initSeq.png", dpi=300, bbox_inches='tight')
    plt.show()
    plt.cla()


def main():
    paint_seq()

main()

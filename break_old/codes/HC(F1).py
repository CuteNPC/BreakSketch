#!/usr/bin/env python
# coding=utf-8


import pandas as pd
import matplotlib.pyplot as plt

datafile = u'./HC(F1).xlsx'
data = pd.read_excel(datafile)


plt.rc('font', family='Arial')

plt.figure(figsize=(6,4.5))
plt.yscale('linear')
plt.tick_params(labelsize = 19)


#plt.title('Examples of line chart',fontsize=20)#标题，并设定字号大小
plt.xlabel(u'Memory Usage (MB)',fontweight='bold',fontsize=24)#设置x轴，并设定字号大小
plt.ylabel(u'F1 Score',fontweight='bold',fontsize=24)#设置y轴，并设定字号大小

ax = plt.gca()


#plt.tick_params(axis='x', which='minor')
#ax.set_xticks([16,64,256,1024])
ax.set_xticks([0.2, 0.25, 0.3, 0.35, 0.4])
ax.set_yticks([0.00, 0.25, 0.50, 0.75, 1.00])

#plt.xscale('log', basex=2)
#plt.xlim(13,1300)

plt.ylim(-0.1, 1.1)


#plt.plot(data['bucketPerFlow'],data['FP,1Kflows'], label='FP, 1K flows', linestyle='-', marker='^', markersize=8, alpha = 0.7, color='k')
#plt.plot(data['bucketPerFlow'],data['NoFP,1Kflows'], label='No FP, 1K flows', linestyle='-', marker='v', markersize=8, alpha = 0.7, color='r')
#
#
#plt.plot(data['bucketPerFlow'],data['FP,10Kflows'], label='FP, 10K flows', linestyle='-', marker='d', markersize=8, alpha = 0.7, color='b')
#plt.plot(data['bucketPerFlow'],data['NoFP,10Kflows'], label='No FP, 10K flows', linestyle='-', marker='H', markersize=8, alpha = 0.7, color='g')
#
#plt.plot(data['bucketPerFlow'],data['FP,100Kflows'], label='FP, 100K flows', linestyle='-', marker='o', markersize=8, alpha = 0.7, color='darkviolet')
#plt.plot(data['bucketPerFlow'],data['NoFP,100Kflows'], label = 'No FP, 100Kflows', linestyle='-', marker='p', markersize=8, alpha = 0.7, color='chocolate')

plt.plot(data['Memory (MB)']/10,data['Waving (8,16)'], label='WavingSketch', linestyle='-', marker='D', markersize=8.8, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
plt.plot(data['Memory (MB)']/10,data['FR'], label='FR', linestyle='-', marker='s', markersize=9, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
plt.plot(data['Memory (MB)']/10,data['FR+CF'], label='FR+CF', linestyle='-', marker='v', markersize=10.5, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)


plt.legend(loc = 'best', ncol=1, handlelength=3)
leg = plt.gca().get_legend() #或leg=ax.get_legend()
ltext = leg.get_texts()
plt.setp(ltext,fontweight='bold',fontsize = 20)
#plt.setp(ltext,fontsize = 22)

#plt.legend(fontsize = 16)
plt.grid(True, linestyle='--', axis='y')
plt.grid(True, linestyle='--', axis='x')
plt.tight_layout()

plt.savefig('HC(F1).pdf')
plt.show()

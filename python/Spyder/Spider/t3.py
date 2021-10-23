# -*- coding: utf-8 -*-
"""
Created on Mon Oct 14 10:48:27 2019

@author: zLimbo
"""

import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.ticker
from matplotlib import rcParams
import warnings
warnings.filterwarnings('ignore')

sns.set()
sns.set_context(
    "notebook",
    font_scale=1.5,
    rc={
        "figure.figsize": (11, 8),
        "axes.titlesize": 18
    }
)

rcParams['figure.figsize'] = 11, 8

df = pd.read_csv(
    'https://labfile.oss.aliyuncs.com/courses/1283/mlbootcamp5_train.csv', sep=';')


df['bmi'] = df['weight'] / (df['height']/100)**2
print('bmi median: ', df['bmi'].median())
print('female mean bmi: ', df[df['gender']==1]['bmi'].mean())
print('male mean bmi: ', df[df['gender']==2]['bmi'].mean())
print('healthy person mean bmi: ', df[df['cardio']==False]['bmi'].mean())
print('cardio person mean bmi: ', df[df['cardio']==True]['bmi'].mean())
print('healthy no-drink male mean bmi: ', 
      df[(df['cardio']==True) & (df['alco']==False) & (df['gender']==2)]['bmi'].mean())
print('healthy no-drink female mean bmi: ', 
      df[(df['cardio']==True) & (df['alco']==False) & (df['gender']==1)]['bmi'].mean())
print('So, A is False, B is True, C is False, D is True')

height_up = df['height'].quantile(0.975)
height_down = df['height'].quantile(0.025)
weight_up = df['weight'].quantile(0.975)
weight_down = df['weight'].quantile(0.025)

df_clear = df[(df['ap_lo'] <= df['ap_hi']) &
    (df['height'] >= height_down) &
    (df['height'] <= height_up) &
    (df['weight'] >= weight_down) &
    (df['weight'] <= weight_up)]
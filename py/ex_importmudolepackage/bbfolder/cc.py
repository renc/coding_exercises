# -*- coding: utf-8 -*-
"""
Created on Wed Sep 12 09:25:06 2018

"""

# this . is the current path, which is relative path (always the current folder)
# 
# import bb # this will fail
from . import bb

def printCC():
    bb.printBB()
    print("cc")
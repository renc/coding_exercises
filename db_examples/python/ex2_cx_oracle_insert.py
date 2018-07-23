#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Jul 18 09:16:20 2018

@author: oracle

www.oracle.com/technetwork/articles/dsl/python-091105.html 

"""


import numpy as np
import pandas as pd
import cx_Oracle




ip = 'localhost'
port = 1521
sid = "orcl"
dsn = cx_Oracle.makedsn(ip, port, service_name = sid)
connect = cx_Oracle.connect('hr', 'password',  dsn)
version = connect.version
print("Oracle DB version is: {}".format(version))


def dropTableMyTab():
    cursor = connect.cursor()
    cursor.execute("Drop table mytab")
    connect.commit()
    cursor.close()
    
def createTableMyTab():
    cursor = connect.cursor()
    cursor.execute('create table mytab(id number, data varchar2(20))')
    connect.commit()
    cursor.close()



cursor = connect.cursor()
rows = [ (1, "First" ),
         (2, "Second" ),
         (3, "Third" ),
         (4, "Fourth" ),
         (5, "Fifth" ),
         (6, "Sixth" ),
         (7, "Seventh" ) ]
cursor.bindarraysize = 7
cursor.setinputsizes(int, 20)
cursor.executemany("insert into mytab(id, data) values (:1, :2)", rows)
#connect.commit()
# Now query the results back
cursor2 = connect.cursor()
cursor2.execute('select * from mytab')
res = cursor2.fetchall()
print (res)
               
cursor.close()
cursor2.close()
connect.close()

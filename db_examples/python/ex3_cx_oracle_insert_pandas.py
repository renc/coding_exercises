#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Jul 18 10:02:34 2018

@author: oracle
"""



import numpy as np
import pandas as pd
import cx_Oracle
import datetime 


def createTable(connect):
    cursor = connect.cursor();
    cursor.execute("""
                   CREATE TABLE "HR"."T03_FUTURE_CLOSING_QUOTATION" 
                       (	
                     "TRADEDATE" DATE, 
                    	"FUTURESCONTRACTID" VARCHAR2(50 BYTE), 
                    	"EXCHANGEID" VARCHAR2(10 BYTE), 
                    	"NEWESTPRICE" NUMBER(12,3), 
                    	"LASTCLEARINGPRICE" NUMBER(12,3), 
                    	"LASTCLOSINGPRICE" NUMBER(12,3), 
                    	"LASTPOSITIONVOLUME" NUMBER(16,0), 
                    	"OPENINGPRICE" NUMBER(12,3), 
                    	"HIGHESTTRADEPRICE" NUMBER(12,3), 
                    	"LOWESTTRADEPRICE" NUMBER(12,3), 
                    	"TRADEVOLUME" NUMBER(16,0), 
                    	"TRADEAMOUNT" NUMBER(16,0), 
                    	"POSITIONVOLUME" NUMBER(16,0), 
                    	"CLOSINGPRICE" NUMBER(12,3), 
                    	"CLEARINGPRICE" NUMBER(12,3), 
                    	"HIGHLIMITSPREAD" NUMBER(12,3), 
                    	"LOWLIMITSPREAD" NUMBER(12,3), 
                    	"CONTRACTMULTIPLIER" NUMBER(16,0), 
                    	"ETLTABLE" VARCHAR2(100 BYTE), 
                    	"ETLDATE" DATE
                       )
                   """)
    connect.commit()
    cursor.close()
    
def insertIntoTableALlDate(df, connect):
    newRows = [ tuple(x) for x in df.values ] 
    
    #h5 = df.head(1)
    #h5 = h5.iloc[:, 0:2] # get column subset
    #newRows = [ tuple(x) for x in h5.values ] 
    
    cursor = connect.cursor()
    cursor.bindarraysize = len(newRows)
    cursor.executemany("""
                       insert into t03_future_closing_quotation 
                       (tradedate, futurescontractid, exchangeid, 
                       newestprice, lastclearingprice, lastclosingprice, 
                       lastpositionvolume, openingprice, 
                       highesttradeprice, lowesttradeprice,
                       tradevolume, tradeamount, 
                       positionvolume, closingprice, clearingprice, 
                       highlimitspread, lowlimitspread, contractmultiplier, 
                       etltable, etldate) 
                       values (TO_DATE(:1, 'YYYY-MM-DD'), :2, :3, :4, :5, :6, :7, :8, :9, :10, 
                       :11, :12, :13, :14, :15, :16, :17, :18, :19, TO_DATE(:20, 'YYYY-MM-DD')) """, 
                       newRows
                       )
    connect.commit()
    cursor.close()
 
def dropTable(tableName, connect):
    cursor = connect.cursor()
    cursor.execute("Drop Table {}".format(tableName))
    connect.commit()
    cursor.close()

def deleteTable(tableName, connect):
    cursor = connect.cursor()
    cursor.execute("delete from {}".format(tableName))
    connect.commit()
    cursor.close()
    
ip = 'localhost'
port = 1521
sid = "orcl"
dsn = cx_Oracle.makedsn(ip, port, service_name = sid)
connect = cx_Oracle.connect('hr', 'password',  dsn)

version = connect.version
print("Oracle DB version is: {}".format(version))

dfHis = pd.read_csv("./closing_quotation.csv", header=None)
dfHis.iloc[[0], :]
type(dfHis.iloc[[0], :])

tableName = "T03_FUTURE_CLOSING_QUOTATION" 
dropTable(tableName, connect)

createTable(connect)

deleteTable(tableName, connect)

insertIntoTableALlDate(dfHis, connect)

cursor = connect.cursor()
cursor.execute("select distinct tradedate from T03_FUTURE_CLOSING_QUOTATION")
result = sorted(cursor.fetchall()) # list 
if len(result) > 0:
    dMaxDate = result[len(result)-1][0]
    dMaxDate = datetime.date(dMaxDate.year, dMaxDate.month, dMaxDate.day)
    dNextDate = dMaxDate + datetime.timedelta(days=1)
    dToday = datetime.date.today()
    
    iDeltaDays = (dToday - dNextDate).days
    for e in np.arange(0, iDeltaDays):
        d = dNextDate + datetime.timedelta(days=int(e))
    
else:
    # insert all date
    pass
connect.close()


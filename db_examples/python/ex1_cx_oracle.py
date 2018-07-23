# -*- coding: utf-8 -*-
"""

pre-requirement:
    oracle (11g2 at least) is installed,
    orcl instance is startup

"""

import numpy as np
import pandas as pd
import cx_Oracle

#connect = cx_Oracle.connect('hr/password@localhost/orcl')
ip = 'localhost'
port = 1521
sid = "orcl"
dsn = cx_Oracle.makedsn(ip, port, service_name = sid)
connect = cx_Oracle.connect('hr', 'password',  dsn)
cursor = connect.cursor()
version = connect.version
print("Oracle DB version is: {}".format(version))

sql = "select * from employees"
cursor.execute(sql)

#for result in cursor:
#    print(result)

result = cursor.fetchall()    
print(result)    
    
cursor.close()
connect.close()



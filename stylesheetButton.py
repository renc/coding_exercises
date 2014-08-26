""" 
Example:
    a window with a button, you can define the style sheet of this button; 
    
Environment: 
    + pySide (install the qt 4.8.5 or above, and pyside);
    + in cmd > python thisfile.py 
    
2014/08/25 
""" 

# Import PySide classes
import sys
from PySide.QtCore import *
from PySide.QtGui import *

BUTTON_SYTLE = '''
                 QPushButton {
                     border: 2px solid #8f8f91;
                     border-radius: 20px;
                     background-color: rgb(176, 189, 194);
                     color: rgb(255, 0, 0); 
                     font: 10pt "MS Shell Dlg 2";
                     min-width: 80px;
                     min-height: 80px;  
                 }

                 QPushButton:pressed {
                     background-color: rgb(98, 105, 108);
                 }
                '''  
                
#######################################
# Create a Qt application
app = QApplication(sys.argv)

widget = QWidget() 

button = QPushButton()
button.setText("buttonX") 
button.setStyleSheet(BUTTON_SYTLE) 

layout = QHBoxLayout(widget)
layout.addWidget(button) 
widget.setLayout(layout)
widget.show() 

# Enter Qt application main loop
app.exec_()
sys.exit() 


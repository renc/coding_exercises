""" 
Example:
    a window with some buttons, you can drag and drop the buttons; 
    
Environment: 
    + pySide (install the qt 4.8.5 or above, and pyside);
    + in cmd > python thisfile.py 
    
2014/08/25 
""" 

# Import PySide classes
import sys
from PySide.QtCore import *
from PySide.QtGui import *

####################################### 
# to define the widget UI 

class Button(QPushButton):
    def __init__(self, title, parent):
        super(Button, self).__init__(title, parent)
    
    def mouseMoveEvent(self, event):
        if event.buttons() != Qt.MiddleButton:
            return

        mimeData = QMimeData()
        
        drag = QDrag(self)
        drag.setMimeData(mimeData)
        drag.setHotSpot(event.pos() - self.rect().topLeft())
        
        dropAction = drag.exec_(Qt.MoveAction) 
        print ("moving\n")

class Tray(QDockWidget):
    def __init__(self):
        QDockWidget.__init__(self) 
        
        self.widget = QWidget()
        self.layout = QHBoxLayout()
        self.layout.setSpacing(2)
        self.widget.setLayout(self.layout)
        self.setWidget(self.widget) 
        
        self.widgetList = [] 
        for it in range(4): 
            b1 = Button("button " + str(it), self)
            self.layout.addWidget(b1);  
            self.widgetList.append(b1) 
           
        self.setAcceptDrops(True)  
     
    def dragMoveEvent(self, event):
        pass
        print ("dragMoveEvent begin") 
        source = event.source()        
        # to move the item to index 0. if you want to be able to move the button
        # to the middle of other buttons (insert), u need to calc the new index 
        # depending on the new event.position.
        index = 0
        self.layout.removeWidget(source)
        self.layout.insertWidget(index,source,0)
        event.accept()
        print ("dragMoveEvent end")
        
    def dragEnterEvent(self, event):
        pass
        print ("dragEnterEvent begin")
        event.accept()
        print ("dragEnterEvent end")

    def dropEvent(self, event):
        print ("dropEvent begin")
        source = event.source()
        event.setDropAction(Qt.MoveAction)
        event.accept()
        print ("dropEvent end")
        

#######################################
# Create a Qt application
app = QApplication(sys.argv)
# Create a Label and show it
widget = Tray() 
widget.show()
# Enter Qt application main loop
app.exec_()
sys.exit() 


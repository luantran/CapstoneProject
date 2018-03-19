import sys
from PyQt5.QtWidgets import QDialog, QApplication
from PyQt5.QtWidgets import QApplication, QMainWindow

from src.interface import MainWindow
from src.registration import registration
from src.registration import wrlReader
class AppWindow(QDialog):
    def __init__(self):
        super().__init__()
        self.window = QMainWindow()
        self.ui = MainWindow.Ui_MainWindow()
        self.ui.setupUi(self.window)
        self.registrationObj = registration.Registration(self.ui.frame)
        self.ui.setRegistrationObject(self.registrationObj)
        # Set up VTK render in UI
        # Render wrl files
        # wrlReader.WRLTest(self.ui.frame)
        self.window.show()

app = QApplication(sys.argv)
# window = QMainWindow()
# ui = MainWindow.Ui_MainWindow()
# ui.setupUi(window)
w = AppWindow()
sys.exit(app.exec_())
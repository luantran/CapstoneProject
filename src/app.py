import sys
from PyQt5.QtWidgets import QDialog, QApplication
from PyQt5.QtWidgets import QApplication, QMainWindow

from src.interface import MainWindow
from src.registration import registration

class AppWindow(QDialog):
    def __init__(self, registrationObj):
        super().__init__()
        self.window = QMainWindow()
        self.ui = MainWindow.Ui_MainWindow()
        self.ui.setupUi(self.window, registrationObj)
        self.window.show()

app = QApplication(sys.argv)
# window = QMainWindow()
# ui = MainWindow.Ui_MainWindow()
# ui.setupUi(window)
registrationObj = registration.Registration()
w = AppWindow(registrationObj)
sys.exit(app.exec_())
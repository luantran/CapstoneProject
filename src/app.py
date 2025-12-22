import os
import sys

from PyQt5.QtGui import QIcon
from PyQt5.QtWidgets import QDialog
from PyQt5.QtWidgets import QApplication, QMainWindow

from src.interface import MainWindow
from src.controller import controller

class AppWindow(QDialog):
    def __init__(self):
        super().__init__()
        self.window = QMainWindow()
        self.ui = MainWindow.Ui_MainWindow()
        self.ui.setupUi(self.window)
        self.controller = controller.Controller(self.ui)
        self.window.show()


if __name__ == '__main__':
    # ===== WINDOWS TASKBAR FIX =====
    # Prevents Windows from grouping with python.exe
    try:
        from ctypes import windll

        myappid = 'medical.spineviz.multimodal.1.0'
        windll.shell32.SetCurrentProcessExplicitAppUserModelID(myappid)
    except:
        pass
    # ===============================

    app = QApplication(sys.argv)

    # ===== SET APPLICATION ICON =====
    # Try ICO first, fallback to PNG
    root_dir = os.path.dirname(os.path.abspath(__file__))

    for icon_name in ['app_icon.ico', 'app_icon.png']:
        icon_path = os.path.join(root_dir, 'assets', 'icons', icon_name)
        if os.path.exists(icon_path):
            app.setWindowIcon(QIcon(icon_path))
            print(f"Icon loaded: {icon_name}")
            break

    w = AppWindow()
    sys.exit(app.exec_())
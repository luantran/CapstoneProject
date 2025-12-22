# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'help.ui'
#
# Created by: PyQt4 UI code generator 4.12.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets
from os import path

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(800, 800)

        # remove question mark button
        Dialog.setWindowFlags(Dialog.windowFlags() & ~QtCore.Qt.WindowContextHelpButtonHint)

        # set dialog icon
        current_dir = path.dirname(path.abspath(__file__))
        root_dir = path.dirname(path.dirname(current_dir))
        icon_path = path.join(root_dir, 'assets', 'icons', 'documentation-icon.png')

        if path.exists(icon_path):
            Dialog.setWindowIcon(QtGui.QIcon(icon_path))

        self.gridLayout = QtWidgets.QGridLayout(Dialog)
        self.gridLayout.setObjectName("gridLayout")
        self.textBrowser = QtWidgets.QTextBrowser(Dialog)
        self.textBrowser.setObjectName("textBrowser")
        self.textBrowser.setOpenExternalLinks(True)

        dir_path = path.dirname(path.realpath(__file__))
        help_file_path = path.join(dir_path, "..", "..", "help.html")
        help_file_absolute = path.abspath(help_file_path)

        if path.exists(help_file_absolute):
            file_url = QtCore.QUrl.fromLocalFile(help_file_absolute)
            self.textBrowser.setSource(file_url)
        else:
            self.textBrowser.setHtml(
                f"<h1>Help File Not Found</h1>"
                f"<p>Expected location: <code>{help_file_absolute}</code></p>"
                f"<p>Please ensure help.html exists in the project root directory.</p>"
            )
        self.gridLayout.addWidget(self.textBrowser, 0, 0, 1, 1)

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        Dialog.setWindowTitle("Documentation")


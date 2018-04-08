# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'main.ui'
#
# Created by: PyQt5 UI code generator 5.9
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QFileDialog
from PyQt5.QtWidgets import QMainWindow
from src.registration import registration
from src.registration import wrlReader
from vtk.qt.QVTKRenderWindowInteractor import QVTKRenderWindowInteractor
import vtk

class Ui_MainWindow(QMainWindow):

    def setupUi(self, MainWindow):
        #Define Bold Font
        font = QtGui.QFont()
        font.setPointSize(14)
        font.setBold(True)
        font.setWeight(75)

        #Main Window
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1096, 761)
        self.centralwidget = QtWidgets.QWidget(MainWindow)

        #Central Widget
        self.centralwidget.setObjectName("centralwidget")

        ################  Load Grid  ################

        self.loadGridWidget = QtWidgets.QWidget(self.centralwidget)
        self.loadGridWidget.setGeometry(QtCore.QRect(10, 10, 241, 131))
        self.loadGridWidget.setObjectName("loadGridWidget")
        self.loadGrid = QtWidgets.QGridLayout(self.loadGridWidget)
        self.loadGrid.setContentsMargins(0, 0, 0, 0)
        self.loadGrid.setObjectName("loadGrid")

        #Buttons

        #Load XRay Button
        self.loadXRayButton = QtWidgets.QPushButton(self.loadGridWidget)
        self.loadXRayButton.setEnabled(True)
        self.loadXRayButton.setObjectName("loadXRayButton")
        self.loadGrid.addWidget(self.loadXRayButton, 2, 1, 1, 1)
        self.loadXRayButton.clicked.connect(self.selectXRayFile)

        #Load MRI Button
        self.loadMRIButton = QtWidgets.QPushButton(self.loadGridWidget)
        self.loadMRIButton.setEnabled(True)
        self.loadMRIButton.setObjectName("loadMRIButton")
        self.loadGrid.addWidget(self.loadMRIButton, 1, 1, 1, 1)
        self.loadMRIButton.clicked.connect(self.selectMRIDirectory)

        #Load Surface Topography Button
        self.loadSTButton = QtWidgets.QPushButton(self.loadGridWidget)
        self.loadSTButton.setEnabled(True)
        self.loadSTButton.setObjectName("loadSTButton")
        self.loadGrid.addWidget(self.loadSTButton, 4, 1, 1, 1)
        self.loadSTButton.clicked.connect(self.selectSurfaceTopography)

        #Load Label
        self.loadLabel = QtWidgets.QLabel(self.loadGridWidget)
        self.loadLabel.setFont(font)
        self.loadLabel.setObjectName("loadLabel")
        self.loadGrid.addWidget(self.loadLabel, 0, 1, 1, 1)


        ################  Registration Grid  ################

        self.registrationGridWidget = QtWidgets.QWidget(self.centralwidget)
        self.registrationGridWidget.setGeometry(QtCore.QRect(10, 150, 241, 90))
        self.registrationGridWidget.setObjectName("registrationGridWidget")
        self.registrationGrid = QtWidgets.QGridLayout(self.registrationGridWidget)
        self.registrationGrid.setContentsMargins(0, 0, 0, 0)
        self.registrationGrid.setObjectName("registrationGrid")

        #Rigid Registration Button
        self.rigidRegistrationButton = QtWidgets.QPushButton(self.registrationGridWidget)
        self.rigidRegistrationButton.setEnabled(True)
        self.rigidRegistrationButton.setObjectName("rigidRegistrationButton")
        self.registrationGrid.addWidget(self.rigidRegistrationButton, 2, 1, 1, 1)
        self.rigidRegistrationButton.clicked.connect(self.register)

        #Articulated Registration Button
        self.articulatedRegistrationButton = QtWidgets.QPushButton(self.registrationGridWidget)
        self.articulatedRegistrationButton.setEnabled(True)
        self.articulatedRegistrationButton.setObjectName("articulatedRegistrationButton")
        self.registrationGrid.addWidget(self.articulatedRegistrationButton, 3, 1, 1, 1)

        #Registration Label
        self.regitsrationLabel = QtWidgets.QLabel(self.registrationGridWidget)
        self.regitsrationLabel.setFont(font)
        self.regitsrationLabel.setObjectName("regitsrationLabel")
        self.registrationGrid.addWidget(self.regitsrationLabel, 1, 1, 1, 1)


        ################  View Grid  ################

        self.viewGridWidget = QtWidgets.QWidget(self.centralwidget)
        self.viewGridWidget.setGeometry(QtCore.QRect(10, 250, 241, 366))
        self.viewGridWidget.setObjectName("viewGridWidget")
        self.viewGrid = QtWidgets.QGridLayout(self.viewGridWidget)
        self.viewGrid.setContentsMargins(0, 0, 0, 0)
        self.viewGrid.setObjectName("viewGrid")

        #Checkboxes

        #X-Ray Checkbox
        self.xRayCheckBox = QtWidgets.QCheckBox(self.viewGridWidget)
        self.xRayCheckBox.setObjectName("xRayCheckBox")
        self.viewGrid.addWidget(self.xRayCheckBox, 1, 1, 1, 1)

        #MRI Vertebrae Checkbox
        self.mriVertebraeCheckBox = QtWidgets.QCheckBox(self.viewGridWidget)
        self.mriVertebraeCheckBox.setObjectName("mriVertebraeCheckBox")
        self.viewGrid.addWidget(self.mriVertebraeCheckBox, 8, 1, 1, 1)

        #Vertebral Landmarks Checkbox
        self.vertebralLMCheckBox = QtWidgets.QCheckBox(self.viewGridWidget)
        self.vertebralLMCheckBox.setObjectName("vertebralLMCheckBox")
        self.viewGrid.addWidget(self.vertebralLMCheckBox, 4, 1, 1, 1)

        # Saggital MRI Checkbox
        self.sMRICheckBox = QtWidgets.QCheckBox(self.viewGridWidget)
        self.sMRICheckBox.setObjectName("sMRICheckBox")
        self.viewGrid.addWidget(self.sMRICheckBox, 7, 1, 1, 1)

        # MRI Checkbox
        self.mriCheckBox = QtWidgets.QCheckBox(self.viewGridWidget)
        self.mriCheckBox.setObjectName("mriCheckBox")
        self.viewGrid.addWidget(self.mriCheckBox, 6, 1, 1, 1)

        # External Landmarks Checkbox
        self.externalLMCheckBox = QtWidgets.QCheckBox(self.viewGridWidget)
        self.externalLMCheckBox.setObjectName("externalLMCheckBox")
        self.viewGrid.addWidget(self.externalLMCheckBox, 3, 1, 1, 1)

        # Surface Tography Checkbox
        self.stCheckBox = QtWidgets.QCheckBox(self.viewGridWidget)
        self.stCheckBox.setObjectName("stCheckBox")
        self.viewGrid.addWidget(self.stCheckBox, 2, 1, 1, 1)

        #First Spacer
        spacerItem1 = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.viewGrid.addItem(spacerItem1, 9, 1, 1, 1)

        #Second Spacer
        spacerItem2 = QtWidgets.QSpacerItem(40, 10, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.viewGrid.addItem(spacerItem2, 5, 1, 1, 1)

        # Slice Label
        self.sliceLabel = QtWidgets.QLabel(self.viewGridWidget)
        self.sliceLabel.setObjectName("sliceLabel")
        self.viewGrid.addWidget(self.sliceLabel, 10, 1, 1, 1)

        #View Label
        self.viewLabel = QtWidgets.QLabel(self.viewGridWidget)
        self.viewLabel.setFont(font)
        self.viewLabel.setObjectName("viewLabel")
        self.viewGrid.addWidget(self.viewLabel, 0, 1, 1, 1)

        #SpinBox
        self.sliceSpinBox = QtWidgets.QSpinBox(self.viewGridWidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Minimum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.sliceSpinBox.sizePolicy().hasHeightForWidth())
        self.sliceSpinBox.setSizePolicy(sizePolicy)
        self.sliceSpinBox.setObjectName("sliceSpinBox")
        self.viewGrid.addWidget(self.sliceSpinBox, 11, 1, 1, 1)



        ################  Save Grid  ################

        self.saveGridWidget = QtWidgets.QWidget(self.centralwidget)
        self.saveGridWidget.setGeometry(QtCore.QRect(10, 630, 241, 71))
        self.saveGridWidget.setObjectName("saveGridWidget")
        self.saveGrid = QtWidgets.QGridLayout(self.saveGridWidget)
        self.saveGrid.setContentsMargins(0, 0, 0, 0)
        self.saveGrid.setObjectName("saveGrid")

        #Save Button
        self.saveButton = QtWidgets.QPushButton(self.saveGridWidget)
        self.saveButton.setEnabled(True)
        self.saveButton.setObjectName("saveButton")
        self.saveGrid.addWidget(self.saveButton, 2, 1, 1, 1)
        #Save Label
        self.saveLabel = QtWidgets.QLabel(self.saveGridWidget)
        self.saveLabel.setFont(font)
        self.saveLabel.setObjectName("saveLabel")
        self.saveGrid.addWidget(self.saveLabel, 1, 1, 1, 1)

        #vtk grid
        self.vtkGridWidget = QtWidgets.QWidget(self.centralwidget)
        self.vtkGridWidget.setGeometry(QtCore.QRect(270, 10, 811, 691))
        self.vtkGridWidget.setObjectName("vtkGridWidget")
        self.vtkGrid = QtWidgets.QGridLayout(self.vtkGridWidget)
        self.vtkGrid.setContentsMargins(0, 0, 0, 0)
        self.vtkGrid.setObjectName("vtkGrid")

        #Create VTK window widget
        self.vtkWidget = QVTKRenderWindowInteractor(self.vtkGridWidget)
        # set size policy
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.vtkWidget.sizePolicy().hasHeightForWidth())
        self.vtkWidget.setSizePolicy(sizePolicy)
        self.vtkWidget.setObjectName("vtkWidget")

        # Create Renderer
        self.ren = vtk.vtkRenderer()

        # Render Window
        self.renWin = self.vtkWidget.GetRenderWindow()
        # Add Renderer to Render Window
        self.renWin.AddRenderer(self.ren)
        # Interactor
        self.iren = self.vtkWidget.GetRenderWindow().GetInteractor()

        self.iren.Initialize()
        self.iren.Start()
        #add vtk widget to vtk grid
        self.vtkGrid.addWidget(self.vtkWidget, 0, 0, 1, 1)


        MainWindow.setCentralWidget(self.centralwidget)

        #Menu
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1096, 25))
        self.menubar.setObjectName("menubar")
        self.menuFile = QtWidgets.QMenu(self.menubar)
        self.menuFile.setObjectName("menuFile")
        self.menuHelp = QtWidgets.QMenu(self.menubar)
        self.menuHelp.setObjectName("menuHelp")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuHelp.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    #Add text to all objects
    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "3DSpineVizualization"))
        self.loadXRayButton.setText(_translate("MainWindow", "Load X-Ray"))
        self.loadMRIButton.setText(_translate("MainWindow", "Load MRI"))
        self.loadSTButton.setText(_translate("MainWindow", "Load Surface Topography"))
        self.loadLabel.setText(_translate("MainWindow", "Load"))
        self.rigidRegistrationButton.setText(_translate("MainWindow", "Rigid Registration"))
        self.articulatedRegistrationButton.setText(_translate("MainWindow", "Articulated Registration"))

        self.regitsrationLabel.setText(_translate("MainWindow", "Registration"))
        self.xRayCheckBox.setText(_translate("MainWindow", "X-Ray"))
        self.mriVertebraeCheckBox.setText(_translate("MainWindow", "MRI vertebrae"))
        self.vertebralLMCheckBox.setText(_translate("MainWindow", "Vertebral Landmarks"))
        self.sliceLabel.setText(_translate("MainWindow", "View registered slice"))
        self.stCheckBox.setText(_translate("MainWindow", "Surface"))
        self.viewLabel.setText(_translate("MainWindow", "View"))
        self.sMRICheckBox.setText(_translate("MainWindow", "Saggital MRIs"))
        self.mriCheckBox.setText(_translate("MainWindow", "MRI"))
        self.externalLMCheckBox.setText(_translate("MainWindow", "External Landmarks"))
        self.saveButton.setText(_translate("MainWindow", "Save"))
        self.saveLabel.setText(_translate("MainWindow", "Save Registered File"))
        self.menuFile.setTitle(_translate("MainWindow", "File"))
        self.menuHelp.setTitle(_translate("MainWindow", "Help"))

    def setController(self, controller):
        self.controller = controller

    def selectMRIDirectory(self):
        dirName = QFileDialog.getExistingDirectory(self, 'Open MRI Directory', "")
        self.controller.setMRIDirectory(dirName)

    def selectXRayFile(self):
        filename = QFileDialog.getOpenFileName(self, 'Open XRay File', "")
        self.controller.setXRay(filename[0])
        self.controller.executeReader("XRay")


    def selectSurfaceTopography(self):
        filename = QFileDialog.getOpenFileName(self, 'Open Surface Topography', "")
        self.controller.setSurface(filename[0])
        self.controller.executeReader("Surface")

    def register(self):
        # self.controller.executeReader("XRay")
        # self.controller.executeReader("Surface")
        # self.controller.executeReader("MRI")
        self.controller.register()



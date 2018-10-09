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
        self.font = QtGui.QFont()
        self.font.setPointSize(14)
        self.font.setBold(True)
        self.font.setWeight(75)

        #Main Window
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1096, 761)


        #Central Widget
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setMinimumSize(QtCore.QSize(0, 750))
        self.centralwidget.setObjectName("centralwidget")
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)

        #Define VTK Layouts:
        self.gridLayout = QtWidgets.QGridLayout(self.centralwidget)
        self.gridLayout.setObjectName("centralGridLayout")

        self.panelVerticalLayout = QtWidgets.QVBoxLayout()
        self.panelVerticalLayout.setSizeConstraint(QtWidgets.QLayout.SetMinimumSize)
        self.panelVerticalLayout.setObjectName("panelVerticalLayout")

        ################  Load Grid  ################
        self.createLoadLayout()

        ################  Registration Grid  ################
        self.createRegistrationLayout()

        ################  View Grid  ################
        self.createViewLayout()

        ################  Save Grid  ################
        self.createSaveLayout()

        #widget setup
        # self.widget = QtWidgets.QWidget(self.centralwidget)
        # sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        # sizePolicy.setHorizontalStretch(1)
        # sizePolicy.setVerticalStretch(1)
        # sizePolicy.setHeightForWidth(self.widget.sizePolicy().hasHeightForWidth())
        # self.widget.setSizePolicy(sizePolicy)
        # self.widget.setMinimumSize(QtCore.QSize(700, 700))
        # self.widget.setObjectName("widget")
        # self.gridLayout.addWidget(self.widget, 0, 1, 1, 1)

        #vtk grid
        self.vtkGridWidget = QtWidgets.QWidget(self.centralwidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(1)
        sizePolicy.setVerticalStretch(1)
        sizePolicy.setHeightForWidth(self.vtkGridWidget.sizePolicy().hasHeightForWidth())
        self.vtkGridWidget.setSizePolicy(sizePolicy)
        self.vtkGridWidget.setMinimumSize(QtCore.QSize(700, 700))
        self.vtkGridWidget.setObjectName("vtkGridWidget")
        self.gridLayout.addWidget(self.vtkGridWidget, 0, 1, 1, 1)


        self.vtkGrid = QtWidgets.QGridLayout(self.vtkGridWidget)
        # self.vtkGrid.setContentsMargins(0, 0, 0, 0)
        self.vtkGrid.setObjectName("vtkGrid")

        #Create VTK window widget
        self.vtkWidget = QVTKRenderWindowInteractor(self.vtkGridWidget)
        # set size policy
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(1)
        sizePolicy.setVerticalStretch(1)
        # sizePolicy.setHeightForWidth(self.vtkWidget.sizePolicy().hasHeightForWidth())
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

        horizontalSpacer = QtWidgets.QSpacerItem(20, 40, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        verticalSpacer = QtWidgets.QSpacerItem(250, 20, QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Minimum)
        self.panelVerticalLayout.addItem(verticalSpacer)
        self.panelVerticalLayout.addItem(horizontalSpacer)
        self.gridLayout.addLayout(self.panelVerticalLayout, 0, 0, 1, 1)


        MainWindow.setCentralWidget(self.centralwidget)

        self.createNewMenu(MainWindow)

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


    def createLoadLayout(self):
        self.loadLayout = QtWidgets.QVBoxLayout()
        self.loadLayout.setSizeConstraint(QtWidgets.QLayout.SetMinimumSize)
        self.loadLayout.setObjectName("loadLayout")

        #Load Label
        self.loadLabel = QtWidgets.QLabel(self.centralwidget)
        self.loadLabel.setFont(self.font)
        self.loadLabel.setObjectName("loadLabel")
        self.loadLayout.addWidget(self.loadLabel)

        #Load MRI Button
        self.loadMRIButton = QtWidgets.QPushButton(self.centralwidget)
        self.loadMRIButton.setEnabled(True)
        self.loadMRIButton.setObjectName("loadMRIButton")
        self.loadLayout.addWidget(self.loadMRIButton)
        self.loadMRIButton.clicked.connect(self.selectMRIDirectory)

        #Load XRay Button
        self.loadXRayButton = QtWidgets.QPushButton(self.centralwidget)
        self.loadXRayButton.setEnabled(True)
        self.loadXRayButton.setObjectName("loadXRayButton")
        self.loadLayout.addWidget(self.loadXRayButton)
        self.loadXRayButton.clicked.connect(self.selectXRayFile)

        #Load Surface Topography Button
        self.loadSTButton = QtWidgets.QPushButton(self.centralwidget)
        self.loadSTButton.setEnabled(True)
        self.loadSTButton.setObjectName("loadSTButton")
        self.loadLayout.addWidget(self.loadSTButton)
        self.loadSTButton.clicked.connect(self.selectSurfaceTopography)

        self.panelVerticalLayout.addLayout(self.loadLayout)

    def createRegistrationLayout(self):
        self.registrationLayout = QtWidgets.QVBoxLayout()
        self.registrationLayout.setSizeConstraint(QtWidgets.QLayout.SetMinimumSize)
        self.registrationLayout.setObjectName("registrationLayout")

        #Registration Label
        self.regitsrationLabel = QtWidgets.QLabel(self.centralwidget)
        self.regitsrationLabel.setFont(self.font)
        self.regitsrationLabel.setObjectName("regitsrationLabel")
        self.registrationLayout.addWidget(self.regitsrationLabel)

        #Rigid Registration Button
        self.rigidRegistrationButton = QtWidgets.QPushButton(self.centralwidget)
        self.rigidRegistrationButton.setEnabled(True)
        self.rigidRegistrationButton.setObjectName("rigidRegistrationButton")
        self.registrationLayout.addWidget(self.rigidRegistrationButton)
        self.rigidRegistrationButton.clicked.connect(self.register)

        #Articulated Registration Button
        self.articulatedRegistrationButton = QtWidgets.QPushButton(self.centralwidget)
        self.articulatedRegistrationButton.setEnabled(True)
        self.articulatedRegistrationButton.setObjectName("articulatedRegistrationButton")
        self.registrationLayout.addWidget(self.articulatedRegistrationButton)

        self.panelVerticalLayout.addLayout(self.registrationLayout)

    def createViewLayout(self):
        self.viewLayout = QtWidgets.QVBoxLayout()
        self.viewLayout.setSizeConstraint(QtWidgets.QLayout.SetMinimumSize)
        self.viewLayout.setObjectName("viewLayout")

        # View Label
        self.viewLabel = QtWidgets.QLabel(self.centralwidget)
        self.viewLabel.setFont(self.font)
        self.viewLabel.setObjectName("viewLabel")
        self.viewLayout.addWidget(self.viewLabel)

        # Checkboxes

        # X-Ray Checkbox
        self.xRayCheckBox = QtWidgets.QCheckBox(self.centralwidget)
        self.xRayCheckBox.setObjectName("xRayCheckBox")
        self.viewLayout.addWidget(self.xRayCheckBox)

        # Surface Tography Checkbox
        self.stCheckBox = QtWidgets.QCheckBox(self.centralwidget)
        self.stCheckBox.setObjectName("stCheckBox")
        self.viewLayout.addWidget(self.stCheckBox)

        # External Landmarks Checkbox
        self.externalLMCheckBox = QtWidgets.QCheckBox(self.centralwidget)
        self.externalLMCheckBox.setObjectName("externalLMCheckBox")
        self.viewLayout.addWidget(self.externalLMCheckBox)

        # Vertebral Landmarks Checkbox
        self.vertebralLMCheckBox = QtWidgets.QCheckBox(self.centralwidget)
        self.vertebralLMCheckBox.setObjectName("vertebralLMCheckBox")
        self.viewLayout.addWidget(self.vertebralLMCheckBox)

        # First Spacer
        self.spacerItem1 = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.viewLayout.addItem(self.spacerItem1)

        # MRI Checkbox
        self.mriCheckBox = QtWidgets.QCheckBox(self.centralwidget)
        self.mriCheckBox.setObjectName("mriCheckBox")
        self.viewLayout.addWidget(self.mriCheckBox)

        # Saggital MRI Checkbox
        self.sMRICheckBox = QtWidgets.QCheckBox(self.centralwidget)
        self.sMRICheckBox.setObjectName("sMRICheckBox")
        self.viewLayout.addWidget(self.sMRICheckBox)

        # MRI Vertebrae Checkbox
        self.mriVertebraeCheckBox = QtWidgets.QCheckBox(self.centralwidget)
        self.mriVertebraeCheckBox.setObjectName("mriVertebraeCheckBox")
        self.viewLayout.addWidget(self.mriVertebraeCheckBox)

        # Second Spacer
        self.spacerItem2 = QtWidgets.QSpacerItem(40, 10, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.viewLayout.addItem(self.spacerItem2)

        # Slice Label
        self.sliceLabel = QtWidgets.QLabel(self.centralwidget)
        self.sliceLabel.setObjectName("sliceLabel")
        self.viewLayout.addWidget(self.sliceLabel)

        # SpinBox
        self.sliceSpinBox = QtWidgets.QSpinBox(self.centralwidget)
        self.sliceSpinBox.setObjectName("sliceSpinBox")
        self.viewLayout.addWidget(self.sliceSpinBox)

        self.panelVerticalLayout.addLayout(self.viewLayout)

    def createSaveLayout(self):
        self.saveLayout = QtWidgets.QVBoxLayout()
        self.saveLayout.setSizeConstraint(QtWidgets.QLayout.SetMinimumSize)
        self.saveLayout.setObjectName("saveLayout")

        # Save Label
        self.saveLabel = QtWidgets.QLabel(self.centralwidget)
        self.saveLabel.setFont(self.font)
        self.saveLabel.setObjectName("saveLabel")
        self.saveLayout.addWidget(self.saveLabel)

        # Save Button
        self.saveButton = QtWidgets.QPushButton(self.centralwidget)
        self.saveButton.setEnabled(True)
        self.saveButton.setObjectName("saveButton")
        self.saveLayout.addWidget(self.saveButton)


        self.panelVerticalLayout.addLayout(self.saveLayout)

    def createNewMenu(self, MainWindow):
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



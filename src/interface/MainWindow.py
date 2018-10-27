# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'main.ui'
#
# Created by: PyQt5 UI code generator 5.9
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QFileDialog
from PyQt5.QtWidgets import QMainWindow

from src.interface import HelpWindow, LicenseWindow
from os import walk
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
        verticalSpacer = QtWidgets.QSpacerItem(350, 20, QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Minimum)
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
        self.loadXRaylandmarksButton.setText(_translate("MainWindow", "Load X-Ray LandMarks"))
        self.loadMRIButton.setText(_translate("MainWindow", "Load MRI"))
        self.loadMRILandmarksButton.setText(_translate("MainWindow", "Load MRI LandMarks"))
        self.loadSTButton.setText(_translate("MainWindow", "Load \n Surface Topography"))
        self.loadSTLandmarksButton.setText(_translate("MainWindow", "Load Surface Topography\n LandMarks"))
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
        self.actionDocumentation.setText(_translate("MainWindow", "Documentation", None))
        self.actionLicense.setText(_translate("MainWindow", "License", None))


    def createLoadLayout(self):
        self.loadLayout = QtWidgets.QVBoxLayout()
        self.loadLayout.setSizeConstraint(QtWidgets.QLayout.SetMinimumSize)
        self.loadLayout.setObjectName("loadLayout")

        loadFont = QtGui.QFont()
        loadFont.setPointSize(10)
        loadFont.setItalic(True)

        #Load Label
        self.loadLabel = QtWidgets.QLabel(self.centralwidget)
        self.loadLabel.setFont(self.font)
        self.loadLabel.setObjectName("loadLabel")
        self.loadLayout.addWidget(self.loadLabel)

        self.MRIloader = QtWidgets.QHBoxLayout()
        self.MRIloader.setObjectName("MRIloader")

        #Load MRI Button
        self.loadMRIButton = QtWidgets.QPushButton(self.centralwidget)
        self.loadMRIButton.setEnabled(True)
        self.loadMRIButton.setObjectName("loadMRIButton")
        self.MRIloader.addWidget(self.loadMRIButton)
        self.loadMRIButton.clicked.connect(self.selectMRIDirectory)

        # Load MRI Landmark Button
        self.loadMRILandmarksButton = QtWidgets.QPushButton(self.centralwidget)
        self.loadMRILandmarksButton.setEnabled(True)
        self.loadMRILandmarksButton.setObjectName("loadMRILandmarksButton")
        self.MRIloader.addWidget(self.loadMRILandmarksButton)
        self.loadMRILandmarksButton.clicked.connect(self.selectMRILandmarks)

        self.loadLayout.addLayout(self.MRIloader)

        #MRI Text
        self.mriLoadText = QtWidgets.QLabel(self.centralwidget)
        self.mriLoadText.setFont(loadFont)
        # self.mriLoadText = QtWidgets.QLineEdit(self.centralwidget)
        # self.mriLoadText.setEnabled(False)
        self.mriLoadText.setObjectName("mriLoadText")
        self.mriLoadText.setText("No MRI directory selected...")
        self.loadLayout.addWidget(self.mriLoadText)

        self.XRayloader = QtWidgets.QHBoxLayout()
        self.XRayloader.setObjectName("XRayloader")

        #Load XRay Button
        self.loadXRayButton = QtWidgets.QPushButton(self.centralwidget)
        self.loadXRayButton.setEnabled(True)
        self.loadXRayButton.setObjectName("loadXRayButton")
        self.XRayloader.addWidget(self.loadXRayButton)
        self.loadXRayButton.clicked.connect(self.selectXRayFile)

        #Load XRay landmarks Button
        self.loadXRaylandmarksButton = QtWidgets.QPushButton(self.centralwidget)
        self.loadXRaylandmarksButton.setEnabled(True)
        self.loadXRaylandmarksButton.setObjectName("loadXRayButton")
        self.XRayloader.addWidget(self.loadXRaylandmarksButton)
        self.loadXRaylandmarksButton.clicked.connect(self.selectXRayLandmarks)

        self.loadLayout.addLayout(self.XRayloader)

        #XRay Text
        self.xrayLoadText = QtWidgets.QLabel(self.centralwidget)
        self.xrayLoadText.setFont(loadFont)
        # self.xrayLoadText = QtWidgets.QLineEdit(self.centralwidget)
        # self.xrayLoadText.setEnabled(False)
        self.xrayLoadText.setObjectName("xrayLoadText")
        self.xrayLoadText.setText("No X-ray file selected...")
        self.loadLayout.addWidget(self.xrayLoadText)

        self.STloader = QtWidgets.QHBoxLayout()
        self.STloader.setObjectName("STloader")

        #Load Surface Topography Button
        self.loadSTButton = QtWidgets.QPushButton(self.centralwidget)
        self.loadSTButton.setEnabled(True)
        self.loadSTButton.setObjectName("loadSTButton")
        self.STloader.addWidget(self.loadSTButton)
        self.loadSTButton.clicked.connect(self.selectSurfaceTopography)

        #Load Surface Topography Landmarks Button
        self.loadSTLandmarksButton = QtWidgets.QPushButton(self.centralwidget)
        self.loadSTLandmarksButton.setEnabled(True)
        self.loadSTLandmarksButton.setObjectName("loadSTButton")
        self.STloader.addWidget(self.loadSTLandmarksButton)
        self.loadSTLandmarksButton.clicked.connect(self.selectSurfaceTopographyLandmark)

        self.loadLayout.addLayout(self.STloader)

        #ST Text
        self.stLoadText = QtWidgets.QLabel(self.centralwidget)
        self.stLoadText.setFont(loadFont)
        # self.stLoadText = QtWidgets.QLineEdit(self.centralwidget)
        # self.stLoadText.setEnabled(False)
        self.stLoadText.setObjectName("stLoadText")
        self.stLoadText.setText("No ST file selected...")
        self.loadLayout.addWidget(self.stLoadText)

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
        self.rigidRegistrationButton.setEnabled(False)
        self.rigidRegistrationButton.setObjectName("rigidRegistrationButton")
        self.registrationLayout.addWidget(self.rigidRegistrationButton)
        self.rigidRegistrationButton.clicked.connect(self.register)

        #Articulated Registration Button
        self.articulatedRegistrationButton = QtWidgets.QPushButton(self.centralwidget)
        self.articulatedRegistrationButton.setEnabled(False)
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
        self.xRayCheckBox.setEnabled(False)
        self.viewLayout.addWidget(self.xRayCheckBox)

        # Surface Tography Checkbox
        self.stCheckBox = QtWidgets.QCheckBox(self.centralwidget)
        self.stCheckBox.setObjectName("stCheckBox")
        self.stCheckBox.setEnabled(False)
        self.viewLayout.addWidget(self.stCheckBox)

        # External Landmarks Checkbox
        self.externalLMCheckBox = QtWidgets.QCheckBox(self.centralwidget)
        self.externalLMCheckBox.setObjectName("externalLMCheckBox")
        self.externalLMCheckBox.setEnabled(False)
        self.viewLayout.addWidget(self.externalLMCheckBox)

        # Vertebral Landmarks Checkbox
        self.vertebralLMCheckBox = QtWidgets.QCheckBox(self.centralwidget)
        self.vertebralLMCheckBox.setObjectName("vertebralLMCheckBox")
        self.vertebralLMCheckBox.setEnabled(False)
        self.viewLayout.addWidget(self.vertebralLMCheckBox)

        # First Spacer
        self.spacerItem1 = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.viewLayout.addItem(self.spacerItem1)

        # MRI Checkbox
        self.mriCheckBox = QtWidgets.QCheckBox(self.centralwidget)
        self.mriCheckBox.setObjectName("mriCheckBox")
        self.mriCheckBox.setEnabled(False)
        self.viewLayout.addWidget(self.mriCheckBox)

        # Saggital MRI Checkbox
        self.sMRICheckBox = QtWidgets.QCheckBox(self.centralwidget)
        self.sMRICheckBox.setObjectName("sMRICheckBox")
        self.sMRICheckBox.setEnabled(False)
        self.viewLayout.addWidget(self.sMRICheckBox)

        # MRI Vertebrae Checkbox
        self.mriVertebraeCheckBox = QtWidgets.QCheckBox(self.centralwidget)
        self.mriVertebraeCheckBox.setObjectName("mriVertebraeCheckBox")
        self.mriVertebraeCheckBox.setEnabled(False)
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
        self.sliceSpinBox.setEnabled(False)
        self.sliceSpinBox.valueChanged.connect(self.changeSlice)
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
        self.saveButton.setEnabled(False)
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
        self.statusbar.showMessage("Copyright Rola Harmouche, Luan Tran, Adbullah Sumbal")
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.actionDocumentation = QtWidgets.QAction(MainWindow)
        self.actionDocumentation.setObjectName("actionDocumentation")
        self.actionLicense = QtWidgets.QAction(MainWindow)
        self.actionLicense.setObjectName("actionLicense")

        self.menuHelp.addAction(self.actionDocumentation)
        self.actionDocumentation.triggered.connect(self.openDocumentationWindow)
        self.menuHelp.addAction(self.actionLicense)
        self.actionLicense.triggered.connect(self.openLicenseWindow)
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuHelp.menuAction())

    def setController(self, controller):
        self.controller = controller

    def selectMRIDirectory(self):
        dirName = QFileDialog.getExistingDirectory(self, 'Open MRI Directory', "")
        self.controller.setMRIDirectory(dirName)
        self.controller.executeReader("MRI")

        if dirName:
            errorStatus = False
            for root, dirs, files in walk(dirName):
                if dirs:
                    errorStatus = True
                for type in "dcm", "ima":
                    for file in files:
                        if file.rsplit('.', 1)[1] != type:
                            errorStatus = True
                    break

            if errorStatus:
                msgBox = QtWidgets.QMessageBox()
                msgBox.setText("Error: The selected directory contains non-MRI files")
                msgBox.exec()
            else:
                self.mriLoadText.setText(dirName)
                self.controller.setMRIDirectory(dirName)
                self.sliceSpinBox.setEnabled(True)
                self.sliceSpinBox.setProperty("value", 0)
                self.currentSliceValue = self.sliceSpinBox.value()

        else:
            self.mriLoadText.setText("No MRI directory selected...")

    def selectMRILandmarks(self):
        filename = QFileDialog.getOpenFileName(self, 'Open Landmark file', "", ".scp  file (*.scp)")
        if filename[0]:
            self.controller.loadLandmarks("MRI", filename[0])

    def selectXRayLandmarks(self):
        filename = QFileDialog.getOpenFileName(self, 'Open XRay Landmarks file', "", "o3 file (*.o3)")
        if filename[0]:
            self.controller.loadLandmarks("XRay", filename[0])

    def selectXRayFile(self):
        filename = QFileDialog.getOpenFileName(self, 'Open XRay File', "", "WRL files (*.wrl)")
        if filename[0]:
            self.xrayLoadText.setText(filename[0])
            self.controller.setXRay(filename[0])
            self.controller.executeReader("XRay")
        else:
            self.xrayLoadText.setText("No X-ray file selected...")

    def selectSurfaceTopographyLandmark(self):
        filename = QFileDialog.getOpenFileName(self, 'Open Surface Topography Landamrk', "", ".ext files (*.ext)")
        if filename[0]:
            self.controller.loadLandmarks("Surface", filename[0])

    def selectSurfaceTopography(self):
        filename = QFileDialog.getOpenFileName(self, 'Open Surface Topography', "", "SZE files (*.sze)")
        if filename[0]:
            self.stLoadText.setText(filename[0])
            self.controller.setSurface(filename[0])
            self.controller.executeReader("Surface")
        else:
            self.stLoadText.setText("No ST file selected...")

    def checkRigidRegistration(self):
        if self.controller.setWRL and self.controller.setST:
            self.rigidRegistrationButton.setEnabled(True)

    def checkArticulatedRegistration(self):
        if self.controller.setWRL and self.controller.setST and self.controller.setMRI:
            self.articulatedRegistrationButton.setEnabled(True)

    def openDocumentationWindow(self):
        dialog = QtWidgets.QDialog()
        ui = HelpWindow.Ui_Dialog()
        ui.setupUi(dialog)
        dialog.exec()

    def openLicenseWindow(self):
        dialog = QtWidgets.QDialog()
        ui = LicenseWindow.Ui_Dialog()
        ui.setupUi(dialog)
        dialog.exec()

    def changeSlice(self):

        if self.sliceSpinBox.value() > self.currentSliceValue:
            delta = self.sliceSpinBox.value() - self.currentSliceValue
            self.currentSliceValue = self.sliceSpinBox.value()
            self.upSlice(delta)
        else:
            delta = self.currentSliceValue - self.sliceSpinBox.value()
            self.currentSliceValue = self.sliceSpinBox.value()
            self.downSlice(delta)

    def upSlice(self, delta):
        self.controller.mriReader.reslice.Update()
        sliceSpacing = self.controller.mriReader.reslice.GetOutput().GetSpacing()[2]
        matrix = self.controller.mriReader.reslice.GetResliceAxes()
        # move the center point that we are slicing through
        center = matrix.MultiplyPoint((0, 0, delta*sliceSpacing, 1))
        matrix.SetElement(0, 3, center[0])
        matrix.SetElement(1, 3, center[1])
        matrix.SetElement(2, 3, center[2])
        self.vtkWidget.Render()

    def downSlice(self, delta):
        self.controller.mriReader.reslice.Update()
        sliceSpacing = self.controller.mriReader.reslice.GetOutput().GetSpacing()[2]
        matrix = self.controller.mriReader.reslice.GetResliceAxes()
        # move the center point that we are slicing through
        center = matrix.MultiplyPoint((0, 0, -delta*sliceSpacing, 1))
        matrix.SetElement(0, 3, center[0])
        matrix.SetElement(1, 3, center[1])
        matrix.SetElement(2, 3, center[2])
        self.vtkWidget.Render()

    def register(self):
        self.controller.register()



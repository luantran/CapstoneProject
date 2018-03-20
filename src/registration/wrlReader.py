import vtk
from PyQt5 import QtCore, QtGui, QtWidgets, Qt
from vtk.qt.QVTKRenderWindowInteractor import QVTKRenderWindowInteractor

class WRLReader(QtWidgets.QFrame):
    def __init__(self, parent):
        super(WRLReader,self).__init__(parent)
        self.frame = Qt.QFrame()
        self.vl = Qt.QVBoxLayout()
        self.vtkWidget = QVTKRenderWindowInteractor(self.frame)
        self.vl.addWidget(self.vtkWidget)

        #initialize variables
        self.wrlFile = None
        self.ren = None
        self.renWin = None
        self.iren = None

    def setWRLFile(self, filename):
        self.wrlFile = filename

    def initializeRenderer(self):
        # Render
        self.ren = vtk.vtkRenderer()
        # Render Window
        self.renWin = self.vtkWidget.GetRenderWindow()
        # Add Render to Render Window
        self.renWin.AddRenderer(self.ren)
        # Interactor
        self.iren = self.vtkWidget.GetRenderWindow().GetInteractor()

    def importVRMLData(self):
        # VRML importer
        importer = vtk.vtkVRMLImporter()
        # Set the Render Window for the importer
        importer.SetRenderWindow(self.renWin)
        # WRL file to upload
        importer.SetFileName(self.wrlFile)
        # Read data
        importer.Read()
        # config Screen
        importer.GetRenderer().SetBackground(0.1, 0.2, 0.4)
        importer.GetRenderWindow().SetSize(400, 400)

        # the importer created the renderer
        renCollection = self.renWin.GetRenderers()
        renCollection.InitTraversal()
        ren = renCollection.GetNextItem()
        # change view up to +z
        ren.GetActiveCamera().SetPosition(-3.25303, 3.46205, 3.15906)
        ren.GetActiveCamera().SetFocalPoint(0, 0, 0)
        ren.GetActiveCamera().SetViewUp(0.564063, 0.825024, -0.0341876)

        # let the renderer compute good position and focal point
        ren.ResetCamera()
        ren.GetActiveCamera().Dolly(1.75)
        self.ren.ResetCameraClippingRange()

    def renderInFrame(self):
        # Add layout to the frame
        self.setLayout(self.vl)
        self.show()
        self.iren.Initialize()
        self.iren.Start()


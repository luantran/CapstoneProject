from src.registration import wrlReader
from src.registration import szeReader
from vtk.qt.QVTKRenderWindowInteractor import QVTKRenderWindowInteractor
from PyQt5 import QtWidgets
import vtk


class Controller(object):
    def __init__(self, view):
        self.view = view
        self.view.setController(self)

        self.wrlReader = wrlReader.WRLReader()
        self.szeReader = szeReader.szeReader()

    def setMRIDirectory(self, mriDirectory):
        self.mri_directory = mriDirectory

    def setXRay(self, xray):
        self.wrlReader.setWRLFile(xray)
        print("Setting X-Ray path: "+self.wrlReader.wrlFile)

    def setSurface(self, surface):
        self.szeReader.setSZEFile(surface)
        print("Setting X-Ray path: "+self.szeReader.filename)


    def processXray(self):
        print("Gettting X-Ray data...")
        xRayPolyData = self.wrlReader.importVRMLData()
        #mapper
        mapper = vtk.vtkPolyDataMapper()
        mapper.SetInputData(xRayPolyData)
        # actor
        self.xray_actor = vtk.vtkActor()
        self.xray_actor.SetMapper(mapper)
        return self.xray_actor


    def processSurface(self):
        print("Gettting Surface Topography data...")
        szePolyData = self.szeReader.readSZEData()
        # mapper
        mapper = vtk.vtkPolyDataMapper()
        mapper.SetInputData(szePolyData)
        # actor
        self.surface_actor = vtk.vtkActor()
        self.surface_actor.SetMapper(mapper)
        return self.surface_actor

    def register(self):
        print("Registering...")
        self.render(self.xray_actor, self.surface_actor)

    def render(self, xRayActor, surfaceActor):
        print("Rendering...")
        self.vtkWidget = QVTKRenderWindowInteractor(self.view.vtkGridWidget)

        #set size policy
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.vtkWidget.sizePolicy().hasHeightForWidth())
        self.vtkWidget.setSizePolicy(sizePolicy)
        self.vtkWidget.setObjectName("vtkWidget")
        self.view.vtkGrid.addWidget(self.vtkWidget, 0, 0, 1, 1)

        # Render
        ren = vtk.vtkRenderer()
        # Render Window
        renWin = self.vtkWidget.GetRenderWindow()

        # Add Render to Render Window
        renWin.AddRenderer(ren)
        # Interactor
        iren = self.vtkWidget.GetRenderWindow().GetInteractor()

        # Add actor
        ren.AddActor(xRayActor)
        ren.AddActor(surfaceActor)

        iren.Initialize()
        iren.Start()
        print("Done")


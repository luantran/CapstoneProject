from src.registration import wrlReader
from src.registration import szeReader
from src.registration import registration
from vtk.qt.QVTKRenderWindowInteractor import QVTKRenderWindowInteractor
from PyQt5 import QtWidgets
import vtk


class Controller(object):
    def __init__(self, view):
        self.view = view
        self.view.setController(self)

        self.wrlReader = wrlReader.WRLReader()
        self.szeReader = szeReader.szeReader()
        self.registration = registration.Registration()
        self.xray_actor = None
        self.surface_actor = None

    def setMRIDirectory(self, mriDirectory):
        self.mri_directory = mriDirectory

    def setXRay(self, xray):
        self.wrlReader.setFilePath(xray)
        print("Setting X-Ray path: "+self.wrlReader.filepath)

    def setSurface(self, surface):
        self.szeReader.setFilePath(surface)
        print("Setting Surface path: "+self.szeReader.filepath)

    def executeReader(self, type):
        if type is "XRay":
            print("Getting X-Ray data...")
            self.xray_actor = self.wrlReader.getVTKActor()
        elif type is "Surface":
            print("Getting Surface data...")

            self.surface_actor = self.szeReader.getVTKActor()

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

        # Render
        ren = vtk.vtkRenderer()
        # Render Window
        renWin = self.vtkWidget.GetRenderWindow()

        # Add Render to Render Window
        renWin.AddRenderer(ren)
        # Interactor
        iren = self.vtkWidget.GetRenderWindow().GetInteractor()

        # Add actors
        ren.AddActor(xRayActor)
        ren.AddActor(surfaceActor)

        # Add to layout
        self.view.vtkGrid.addWidget(self.vtkWidget, 0, 0, 1, 1)

        iren.Initialize()
        iren.Start()
        print("Done")


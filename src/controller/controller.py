from src.registration import wrlReader
from src.registration import szeReader
from src.registration import mriReader
from src.registration import registration
from vtk.qt.QVTKRenderWindowInteractor import QVTKRenderWindowInteractor
from PyQt5 import QtWidgets
import vtk


class Controller(object):
    def __init__(self, view):
        self.view = view
        self.view.setController(self)

        self.wrlReader = wrlReader.WRLReader()
        self.szeReader = szeReader.SZEReader()
        self.mriReader = mriReader.MRIReader()
        self.setWRL = False
        self.setSurface = False
        self.setMRI = False
        self.registration = registration.Registration()
        self.xray_actor = None
        self.surface_actor = None
        self.mri_actor = None

    def setMRIDirectory(self, mriDirectory):
        self.mriReader.setFilePath(mriDirectory)
        self.setMRI = True

    def setXRay(self, xray):
        self.wrlReader.setFilePath(xray)
        self.setWRL = True

    def setSurface(self, surface):
        self.szeReader.setFilePath(surface)
        self.setSurface = True

    def executeReader(self, type):
        if type is "XRay":
            print("Getting X-Ray data...")
            self.xray_actor = self.wrlReader.getVTKActor()
            self.view.ren.AddActor(self.xray_actor)
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()

        elif type is "Surface":
            print("Getting Surface data...")
            self.surface_actor = self.szeReader.getVTKActor()
            self.view.ren.AddActor(self.surface_actor)
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()

        elif type is "MRI":
            print("Getting MRI data...")
            self.mri_actor = self.mriReader.getVTKActor()
            self.view.ren.AddActor(self.mri_actor)
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()

    def register(self):
        print("Registering...")
        self.render(self.xray_actor, self.surface_actor)



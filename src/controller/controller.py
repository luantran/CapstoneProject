from src.readers import szeReader, wrlReader, mriReader
from src.registration import registration
import vtk


class Controller(object):
    def __init__(self, view):
        self.view = view
        self.view.setController(self)

        self.wrlReader = wrlReader.WRLReader()
        self.szeReader = szeReader.SZEReader()
        self.mriReader = mriReader.MRIReader()
        self.setWRL = False
        self.setST = False
        self.setMRI = False
        self.registration = registration.Registration()
        self.xray_actor = None
        self.surface_actor = None
        self.mri_actor = None
        self.actors = {}

    def setMRIDirectory(self, mriDirectory):
        self.mriReader.setFilePath(mriDirectory)
        self.setMRI = True

    def setMRILandmarks(self, filename):
        self.mriReader.getLandmarks(filename)

    def setXRay(self, xray):
        self.wrlReader.setFilePath(xray)
        self.setWRL = True

    def setSurface(self, surface):
        self.szeReader.setFilePath(surface)
        self.setST = True

    def loadLandmarks(self, type, filename):
        if type is "XRayLM":
            print("Loading MRI landmarks")
            vertebrae, capteurs = self.wrlReader.getLandmarks(filename)
            lm_actors = []
            for landmark in capteurs:
                lm_actors.append(self.create_spheres_landmarks(landmark, "green"))
                self.view.ren.AddActor(lm_actors[-1])
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()
            self.actors[type] = lm_actors

        elif type is "SurfaceLM":
            print("Loading Surface landmarks...")
            ST_landmarks = self.szeReader.getLandmarks(filename)
            lm_actors = []
            for landmark in ST_landmarks:
                lm_actors.append(self.create_spheres_landmarks(landmark, "red"))
                self.view.ren.AddActor(lm_actors[-1])
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()
            self.actors[type] = lm_actors

        elif type is "MRI_LM":
            print("loading MRI landmarks...")
            MRI_landmarks = self.mriReader.getLandmarks(filename)
            lm_actors = []
            for landmark in MRI_landmarks:
                lm_actors.append(self.create_spheres_landmarks(landmark, "blue"))
                self.view.ren.AddActor(lm_actors[-1])
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()
            self.actors[type] = lm_actors

    def executeReader(self, type):
        if type is "XRay":
            print("Getting X-Ray data...")
            self.xray_actor = self.wrlReader.getVTKActor()
            self.view.ren.AddActor(self.xray_actor)
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()
            self.actors[type] = self.xray_actor

        elif type is "Surface":
            print("Getting Surface data...")
            self.surface_actor = self.szeReader.getVTKActor()
            self.view.ren.AddActor(self.surface_actor)
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()
            self.actors[type] = self.surface_actor

        elif type is "MRI":
            print("Getting MRI data...")
            self.mri_actor = self.mriReader.getVTKActor()
            self.view.ren.AddActor(self.mri_actor)
            self.view.sliceSpinBox.setMaximum(self.mriReader.max_number_of_slices)
            self.view.sliceSpinBox.setSuffix(" of " + str(self.mriReader.max_number_of_slices))
            self.view.sliceSpinBox.setPrefix("Slice ")
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()
            self.actors[type] = self.mri_actor

    def checkboxUpdate(self, type, isChecked):
        # Checkbox update for modalities
        if type in self.actors and type[-2:] != 'LM':
            if isChecked:
                self.actors[type].VisibilityOn()
            else:
                self.actors[type].VisibilityOff()
        # Checkbox update for landmarks. Type which ends with LM are landmarks
        elif type in self.actors and type[-2:] == 'LM':
            if isChecked:
                for actor in self.actors[type]:
                    actor.VisibilityOn()
            else:
                for actor in self.actors[type]:
                    actor.VisibilityOff()
        self.view.vtkWidget.Render()

    def register(self):
        print("Registering...")
        self.render(self.xray_actor, self.surface_actor)

    def create_spheres_landmarks(self, landmark, color):
        sphere_source = vtk.vtkSphereSource()
        sphere_source.SetThetaResolution(64)
        sphere_source.SetPhiResolution(64)
        sphere_source.SetCenter(landmark['x'], landmark['y'], landmark['z'])
        sphere_source.SetRadius(10.0)
        sphere_mapper = vtk.vtkPolyDataMapper()
        sphere_mapper.SetInputConnection(sphere_source.GetOutputPort())
        actor = vtk.vtkActor()
        actor.SetMapper(sphere_mapper)
        if color == "red":
            actor.GetProperty().SetColor(1.0, 0.0, 0.0)
        elif color == "green":
            actor.GetProperty().SetColor(0.0, 1.0, 0.0)
        elif color == "blue":
            actor.GetProperty().SetColor(0.0, 0.0, 1.0)
        return actor



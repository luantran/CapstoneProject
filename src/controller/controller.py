from src.readers import szeReader, wrlReader, mriReader
from src.registration import rigidRegistration
import vtk


class Controller(object):
    def __init__(self, view):
        self.view = view
        self.view.setController(self)

        self.wrlReader = wrlReader.WRLReader()
        self.szeReader = szeReader.SZEReader()
        self.mriReader = mriReader.MRIReader()
        self.rigid = rigidRegistration.RigidRegistration()
        self.xray_actor = None
        self.surface_actor = None
        self.mri_actor = None
        self.landmarkPoints = {}
        self.actors = {}
        self.check = self.initializeChecks()

    def initializeChecks(self):
        dictionary = {}
        dictionary['XRay'] = False
        dictionary['XRay_LM'] = False
        dictionary['MRI'] = False
        dictionary['MRI_LM'] = False
        dictionary['ST'] = False
        dictionary['ST_LM'] = False
        return dictionary

    def setMRIDirectory(self, mriDirectory):
        self.mriReader.setFilePath(mriDirectory)

    def setMRILandmarks(self, filename):
        self.mriReader.getLandmarks(filename)

    def setXRay(self, xray):
        self.wrlReader.setFilePath(xray)

    def setSurface(self, surface):
        self.szeReader.setFilePath(surface)

    def loadLandmarks(self, type, filename):
        if type in self.actors.keys():
            self.removeActors(self.actors[type], True)
        if type is "XRayLM":
            print("Loading XRay landmarks")
            vertebrae, capteurs = self.wrlReader.getLandmarks(filename)
            ext_lm_actors = []
            self.landmarkPoints['XRay'] = capteurs

            for landmark in capteurs:
                ext_lm_actors.append(self.create_spheres_landmarks(landmark, "green", 10.0))
                self.view.ren.AddActor(ext_lm_actors[-1])
            vertebrae_lm_actors = []
            for vertebra in vertebrae:
                for point in vertebra['points']:
                    vertebrae_lm_actors.append(self.create_spheres_landmarks(point, "white", 5.0))
                    self.view.ren.AddActor(vertebrae_lm_actors[-1])
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()

            self.actors[type] = ext_lm_actors
            self.actors["vertebrae_"+type] = vertebrae_lm_actors

        elif type is "SurfaceLM":
            print("Loading Surface landmarks...")
            ST_landmarks = self.szeReader.getLandmarks(filename)
            lm_actors = []
            self.landmarkPoints['Surface'] = ST_landmarks
            for landmark in ST_landmarks:
                lm_actors.append(self.create_spheres_landmarks(landmark, "violet", 10.0))
                self.view.ren.AddActor(lm_actors[-1])
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()
            self.actors[type] = lm_actors

        elif type is "MRI_LM":
            print("loading MRI landmarks...")
            MRI_landmarks = self.mriReader.getLandmarks(filename)
            lm_actors = []
            for landmark in MRI_landmarks:
                lm_actors.append(self.create_spheres_landmarks(landmark, "blue", 5.0))
                self.view.ren.AddActor(lm_actors[-1])
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()
            self.actors[type] = lm_actors

    def executeReader(self, type):
        if type in self.actors.keys():
            self.removeActors(self.actors[type])
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

    def performRigitRegistration(self):

        # check if we have all the actors needed for the registration:
        if ('SurfaceLM' in self.actors and 'Surface' in self.actors
                and 'XRayLM' in self.actors):
            # Trandfrom Surface onto XRay
            # Get the Translation matrix
            Transrigid = self.rigid.SurfaceXRayRegistration(self.landmarkPoints['Surface'], self.landmarkPoints['XRay'])
            self.actors['Surface'].SetUserTransform(Transrigid)

            # Transform Surface landmarks onto Xray landmarks
            for actor in self.actors['SurfaceLM']:
                actor.SetUserTransform(Transrigid)

        self.view.vtkWidget.Render()

    def create_spheres_landmarks(self, landmark, color, size):
        sphere_source = vtk.vtkSphereSource()
        sphere_source.SetThetaResolution(64)
        sphere_source.SetPhiResolution(64)
        sphere_source.SetCenter(landmark['x'], landmark['y'], landmark['z'])
        sphere_source.SetRadius(size)
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
        elif color == "white":
            actor.GetProperty().SetColor(1.0, 1.0, 1.0)
        elif color == "violet":
            actor.GetProperty().SetColor(0.58, 0.0, 0.82)
        return actor

    def changeSlice(self, spinBoxValue):
        transform = vtk.vtkTransform()
        sliceSpacing = self.mriReader.thickness
        transform.Translate(0, 0, self.view.currentSliceValue*sliceSpacing)
        self.mriReader.reslice.SetResliceTransform(transform)

        if spinBoxValue > self.view.currentSliceValue:
            delta = spinBoxValue - self.view.currentSliceValue
            self.view.currentSliceValue = spinBoxValue
            self.upSlice(delta)
        else:
            delta = self.view.currentSliceValue - spinBoxValue
            self.view.currentSliceValue = spinBoxValue
            self.downSlice(delta)

    def upSlice(self, delta):
        self.mriReader.reslice.Update()
        sliceSpacing = self.mriReader.thickness
        matrix = self.mriReader.reslice.GetResliceAxes()

        # move the center point that we are slicing through
        center = matrix.MultiplyPoint((0, 0, delta * sliceSpacing, 1))
        matrix.SetElement(0, 3, center[0])
        matrix.SetElement(1, 3, center[1])
        matrix.SetElement(2, 3, center[2])

        self.view.vtkWidget.Render()

    def downSlice(self, delta):
        self.mriReader.reslice.Update()
        sliceSpacing = self.mriReader.reslice.GetOutput().GetSpacing()[2]
        matrix = self.mriReader.reslice.GetResliceAxes()

        # move the center point that we are slicing through
        center = matrix.MultiplyPoint((0, 0, -delta * sliceSpacing, 1))
        matrix.SetElement(0, 3, center[0])
        matrix.SetElement(1, 3, center[1])
        matrix.SetElement(2, 3, center[2])

        self.view.vtkWidget.Render()

    def checkRequirementsForRigidRegistration(self):
        for key, value in self.check.items():
            if value == False:
                return False
        return True

    def removeActors(self, actors, multiple=False):
        if multiple:
            for actor in actors:
                self.view.ren.RemoveActor(actor)
        else:
            self.view.ren.RemoveActor(actors)




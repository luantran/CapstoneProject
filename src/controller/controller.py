from src.readers import szeReader, wrlReader, mriReader
from src.landmarks_readers import szeLandmarksReader, wrlLandmarksReader, mriLandmarksReader
from src.registration import rigidRegistration
from src.registration import articulatedRegistration
import operator

import vtk

class Controller(object):
    def __init__(self, view):
        self.view = view
        self.view.setController(self)

        self.wrlReader = wrlReader.WRLReader()
        self.szeReader = szeReader.SZEReader()
        self.mriReader = mriReader.MRIReader()
        self.wrlLMReader = wrlLandmarksReader.WRLReaderLM()
        self.szeLMReader = szeLandmarksReader.SZEReaderLM()
        self.mriLMReader = mriLandmarksReader.MRIReaderLM()
        self.readers = {}
        self.readers['XRay'] = wrlReader
        self.readers['MRI'] = mriReader
        self.readers['Surface'] = szeReader

        self.rigid = rigidRegistration.RigidRegistration()
        self.articulated = articulatedRegistration.ArticulatedRegistration()

        self.landmarkPoints = {}
        self.actors = {}
        self.matchingSTXRayPoints = None

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
        self.mriLMReader.setFilePath(filename)

    def setXRay(self, xray):
        self.wrlReader.setFilePath(xray)

    def setXRayLandmarks(self, filename):
        self.wrlLMReader.setFilePath(filename)

    def setSurface(self, surface):
        self.szeReader.setFilePath(surface)

    def setSurfaceLandmarks(self, filename):
        self.szeLMReader.setFilePath(filename)

    def loadLandmarks(self, type, filename):
        self.view.changeStatusMessage("Loading " + type[:-3] + " landmarks...")

        if type in self.actors.keys():
            self.removeActors(self.actors[type])

        if type is "XRay_LM":
            vert_actor, capt_actor = self.wrlLMReader.getVTKActor()
            self.view.ren.AddActor(capt_actor)
            self.actors[type] = capt_actor
            self.view.ren.AddActor(vert_actor)
            self.actors["vertebrae_"+type] = vert_actor
        elif type is "Surface_LM":
            surface_lm_actor = self.szeLMReader.getVTKActor()
            self.view.ren.AddActor(surface_lm_actor)
            self.actors[type] = surface_lm_actor
        elif type is "MRI_LM":
            mri_lm_actor = self.mriLMReader.getVTKActor()
            self.view.ren.AddActor(mri_lm_actor)
            self.actors[type] = mri_lm_actor
        self.view.vtkWidget.Render()
        self.view.changeStatusMessage("Finished Loading " + type[:-3] + " landmarks!")

    def executeReader(self, type):
        self.view.changeStatusMessage("Loading " + type + " data...")
        if type in self.actors.keys():
            self.removeActors(self.actors[type])
        if type is "XRay":
            xray_actor = self.wrlReader.getVTKActor()
            self.view.ren.AddActor(xray_actor)
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()
            self.actors[type] = xray_actor

        elif type is "Surface":
            surface_actor = self.szeReader.getVTKActor()
            self.view.ren.AddActor(surface_actor)
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()
            self.actors[type] = surface_actor

        elif type is "MRI":
            mri_actor = self.mriReader.getVTKActor()
            self.view.ren.AddActor(mri_actor)
            self.view.sliceSpinBox.setMaximum(self.mriReader.max_number_of_slices)
            self.view.sliceSpinBox.setSuffix(" of " + str(self.mriReader.max_number_of_slices))
            self.view.sliceSpinBox.setPrefix("Slice ")
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()
            self.actors[type] = mri_actor
        self.view.changeStatusMessage("Finished loading " + type + " data!")

    def checkboxUpdate(self, type, isChecked):
        # Checkbox update for modalities
        if type in self.actors:
            if isChecked:
                self.actors[type].VisibilityOn()
            else:
                self.actors[type].VisibilityOff()
        self.view.vtkWidget.Render()

    def performRegistration(self, type):
        # check if we have all the actors needed for the registration:
        self.process_mri_xray_landmarks()
        self.process_st_xray_landmarks()
        if ('MRI_LM' in self.actors and 'MRI' in self.actors and
                'Surface_LM' in self.actors and 'Surface' in self.actors
                and 'XRay_LM' in self.actors and 'XRay' in self.actors):
            self.view.changeStatusMessage("Starting " + type + " registration...")
            # Trandfrom Surface onto XRay
            # Get the Translation matrix
            if type == 'articulated':
                registered_actors = self.articulated.SurfaceXRayRegistration(self.szeLMReader, self.wrlLMReader,
                                                                       self.szeReader)
                self.actors['MRI'] = registered_actors[0]
                self.actors['MRI_LM'] = registered_actors[1]
            else:
                registered_actors = self.rigid.SurfaceXRayRegistration(self.szeLMReader, self.wrlLMReader, self.szeReader)
                self.actors['Surface'] = registered_actors[0]
                self.actors['Surface_LM'] = registered_actors[1]

            registered_actors = self.rigid.MRIXRayRegistration(self.mriLMReader, self.wrlLMReader, self.mriReader)
            # Trandfrom MRI onto XRay
            self.actors['MRI'] = registered_actors[0]
            self.actors['MRI_LM'] = registered_actors[1]

            self.view.vtkWidget.Render()
            self.view.changeStatusMessage("Finished " + type + " registration!")

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

    def checkRequirementsForRegistration(self):
        for key, value in self.check.items():
            if value == False:
                return False
        return True

    def removeActors(self, actors):
        self.view.ren.RemoveActor(actors)

    def reload(self):
        self.view.changeStatusMessage("Reloading all modalities...")
        for type in self.actors:
            self.removeActors(self.actors[type])
        if self.mriReader.filepath:
            self.executeReader('MRI')
        if self.szeReader.filepath:
            self.executeReader('Surface')
        if self.wrlReader.filepath:
            self.executeReader('XRay')

        if self.mriLMReader.filepath:
            self.loadLandmarks('MRI_LM', self.mriLMReader.filepath)
        if self.szeLMReader.filepath:
            self.loadLandmarks('Surface_LM', self.szeLMReader.filepath)
        if self.wrlReader.filepath:
            self.loadLandmarks('XRay_LM', self.wrlLMReader.filepath)
        self.view.vtkWidget.Render()
        self.view.changeStatusMessage("All modalities reloaded!")

    def getMatchingPoints(self, first, second):
        # Sort Data by name
        first.sort(key=operator.itemgetter('name'))
        second.sort(key=operator.itemgetter('name'))

        # matching points extraction
        first_set = set()
        second_set = set()

        for data in first:
            first_set.add(data['name'])

        for data in second:
            second_set.add(data['name'])

        matching_points = first_set.intersection(second_set)
        return matching_points



    def process_st_xray_landmarks(self):
        matchingPosition = self.getMatchingPoints(self.szeLMReader.landmarks, self.wrlLMReader.landmarks[1])

        new_landmarks = []
        for data in self.szeLMReader.landmarks:
            if data['name'] in matchingPosition:
                new_landmarks.append(data)
        self.szeLMReader.landmarks = new_landmarks

        new_actor = self.szeLMReader.updateVTKActor()
        self.actors['Surface_LM'] = new_actor

        new_landmarks = []
        for data in self.wrlLMReader.landmarks[1]:
            if data['name'] in matchingPosition:
                new_landmarks.append(data)
        self.wrlLMReader.landmarks[1] = new_landmarks

        new_actor = self.wrlLMReader.updateExtVTKActor()
        self.actors['XRay_LM'] = new_actor
        self.view.vtkWidget.Render()

    def process_mri_xray_landmarks(self):
        matchingPosition = self.getMatchingPoints(self.mriLMReader.landmarks, self.wrlLMReader.landmarks[0])

        new_landmarks = []
        for data in self.mriLMReader.landmarks:
            if data['name'] in matchingPosition:
                new_landmarks.append(data)
        self.mriLMReader.landmarks = new_landmarks

        # self.view.ren.RemoveActor(self.actors['MRI_LM'])
        new_actor = self.mriLMReader.updateVTKActor()
        # self.view.ren.AddActor(new_actor)
        self.actors['MRI_LM'] = new_actor


        new_landmarks = []
        for data in self.wrlLMReader.landmarks[0]:
            if data['name'] in matchingPosition:
                new_landmarks.append(data)
        self.wrlLMReader.landmarks[0] = new_landmarks

        # self.view.ren.RemoveActor(self.actors['vertebrae_XRay_LM'])
        new_actor = self.wrlLMReader.updateVertVTKActor()
        # self.view.ren.AddActor(new_actor)
        self.actors['vertebrae_XRay_LM'] = new_actor
        self.view.vtkWidget.Render()





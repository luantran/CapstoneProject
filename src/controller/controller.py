from src.readers import szeReader, wrlReader, mriReader, szeLandmarksReader, mriLandmarksReader, wrlLandmarksReader
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
        if type is "XRay_LM":
            print("Loading XRay landmarks")
            # vertebrae, capteurs = self.wrlReader.getLandmarks(filename)
            # ext_lm_actors = []
            # self.landmarkPoints['XRay_Ext'] = capteurs
            # self.landmarkPoints['XRay_Vert'] = vertebrae
            #
            # for landmark in capteurs:
            #     ext_lm_actors.append(self.create_spheres_landmarks(landmark, "green", 10.0))
            #     self.view.ren.AddActor(ext_lm_actors[-1])
            # vertebrae_lm_actors = []
            # for point in vertebrae:
            #     vertebrae_lm_actors.append(self.create_spheres_landmarks(point, "white", 5.0))
            #     self.view.ren.AddActor(vertebrae_lm_actors[-1])
            # self.view.ren.ResetCamera()
            # self.view.vtkWidget.Render()
            #
            # self.actors[type] = ext_lm_actors
            # self.actors["vertebrae_"+type] = vertebrae_lm_actors

            self.wrlLMReader.setFilePath(filename)
            vert_actor, capt_actor = self.wrlLMReader.getVTKActor()
            self.view.ren.AddActor(capt_actor)
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()
            self.actors[type] = capt_actor

            self.view.ren.AddActor(vert_actor)
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()
            self.actors["vertebrae_"+type] = vert_actor

        elif type is "Surface_LM":
            print("Loading Surface landmarks...")
            self.szeLMReader.setFilePath(filename)
            surface_lm_actor = self.szeLMReader.getVTKActor()
            print(surface_lm_actor)
            self.view.ren.AddActor(surface_lm_actor)
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()
            self.actors[type] = surface_lm_actor

            # ST_landmarks = self.szeReader.getLandmarks(filename)
            # lm_actors = []
            # self.landmarkPoints['Surface'] = ST_landmarks
            # for landmark in ST_landmarks:
            #     lm_actors.append(self.create_spheres_landmarks(landmark, "violet", 10.0))
            #     self.view.ren.AddActor(lm_actors[-1])
            # self.view.ren.ResetCamera()
            # self.view.vtkWidget.Render()
            # self.actors[type] = lm_actors

        elif type is "MRI_LM":
            print("loading MRI landmarks...")
            self.mriLMReader.setFilePath(filename)
            mri_lm_actor = self.mriLMReader.getVTKActor()
            self.view.ren.AddActor(mri_lm_actor)
            self.view.ren.ResetCamera()
            self.view.vtkWidget.Render()
            self.actors[type] = mri_lm_actor
            # MRI_landmarks = self.mriReader.getLandmarks(filename)
            # lm_actors = []
            # self.landmarkPoints['MRI'] = MRI_landmarks
            # for landmark in MRI_landmarks:
            #     lm_actors.append(self.create_spheres_landmarks(landmark, "blue", 5.0))
            #     self.view.ren.AddActor(lm_actors[-1])
            # self.view.ren.ResetCamera()
            # self.view.vtkWidget.Render()
            # self.actors[type] = lm_actors

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
        if type in self.actors:
            if isChecked:
                self.actors[type].VisibilityOn()
            else:
                self.actors[type].VisibilityOff()
        self.view.vtkWidget.Render()

    def performRegistration(self, type):
        # check if we have all the actors needed for the registration:
        if ('MRI_LM' in self.actors and 'MRI' in self.actors and
                'Surface_LM' in self.actors and 'Surface' in self.actors
                and 'XRay_LM' in self.actors and 'XRay' in self.actors):

            # Trandfrom Surface onto XRay
            # Get the Translation matrix
            if type == 'articulated':
                ST_Xray_Trans = self.articulated.SurfaceXRayRegistration(self.landmarkPoints['Surface'], self.landmarkPoints['XRay_Ext'])
                ftps_sze = vtk.vtkTransformPolyDataFilter()
                ftps_sze.SetInputData(self.szeReader.getPolyData())
                ftps_sze.SetTransform(ST_Xray_Trans)

                actor = vtk.vtkActor()
                # Rotate actor (adopted from Rola's code)
                actor.RotateZ(-90)
                actor.RotateX(90)

                mapper = vtk.vtkPolyDataMapper()
                mapper.SetInputConnection(ftps_sze.GetOutputPort())

                actor.SetMapper(mapper)

                actor.GetProperty().SetInterpolationToFlat()
                actor.GetProperty().SetEdgeColor(1.0, 0.0, 0.0)
                actor.GetProperty().SetOpacity(0.25)
                actor.GetProperty().EdgeVisibilityOn()

                self.view.ren.AddActor(actor)
                self.view.ren.ResetCamera()
                self.view.vtkWidget.Render()

                ftps_sze.Update()

            else:
                ST_Xray_Trans = self.rigid.SurfaceXRayRegistration(self.landmarkPoints['Surface'], self.landmarkPoints['XRay_Ext'])
                self.actors['Surface'].SetUserTransform(ST_Xray_Trans)
                # Transform Surface landmarks onto Xray landmarks
                for actor in self.actors['Surface_LM']:
                    actor.SetUserTransform(ST_Xray_Trans)

            MRI_XRay_Trans = self.rigid.MRIXRayRegistration(self.landmarkPoints['MRI'], self.landmarkPoints['XRay_Vert'])



            # Trandfrom MRI onto XRay
            self.actors['MRI'].SetUserTransform(MRI_XRay_Trans)
            # Transform Surface landmarks onto Xray landmarks
            for actor in self.actors['MRI_LM']:
                actor.SetUserTransform(MRI_XRay_Trans)

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

    def checkRequirementsForRegistration(self):
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

    def reload(self):
        print("Reloading")
        for key in self.actors.keys():
            if key[-2:] == 'LM':
                self.removeActors(self.actors[key], True)
            else:
                self.removeActors(self.actors[key])
        self.view.vtkWidget.Render()

        if self.mriReader.filepath:
            self.executeReader('MRI')
        if self.szeReader.filepath:
            self.executeReader('Surface')
        if self.wrlReader.filepath:
            self.executeReader('XRay')

        if self.mriReader.landmarks_filepath:
            self.loadLandmarks('MRI_LM', self.mriReader.landmarks_filepath)
        if self.szeReader.landmarks_filepath:
            self.loadLandmarks('Surface_LM', self.szeReader.landmarks_filepath)
        if self.wrlReader.landmarks_filepath:
            self.loadLandmarks('XRay_LM', self.wrlReader.landmarks_filepath)


    # def process_st_xray_landmarks(self):
    #     # Sort Data by name
    #     self.landmarkPoints['Surface'].sort(key=operator.itemgetter('name'))
    #     self.landmarkPoints['XRay_Ext'].sort(key=operator.itemgetter('name'))
    #
    #     # matching points extraction
    #     landmarkXrayPositions = set()
    #     landmarkSurfacePositions = set()
    #
    #     for data in self.landmarkPoints['XRay_Ext']:
    #         landmarkXrayPositions.add(data['name'])
    #
    #     for data in self.landmarkPoints['Surface']:
    #         landmarkSurfacePositions.add(data['name'])
    #
    #     matchingPosition = landmarkSurfacePositions.intersection(landmarkXrayPositions)
    #     st_landmarks_to_remove = []
    #     for data in self.landmarkPoints['Surface']:rigid
    #         if data['name'] not in matchingPosition:
    #             st_landmarks_to_remove.append(data)
    #     for el in st_landmarks_to_remove:
    #         self.landmarkPoints['Surface']
    #
    #     for data in self.landmarkPoints['XRay_Ext']:
    #         if data['name'] in matchingPosition:
    #             xrayLMPoints.InsertNextPoint(data['x'], data['y'], data['z'])




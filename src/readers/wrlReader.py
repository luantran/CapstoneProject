import vtk
from src.readers import reader


class WRLReader(reader.Reader):

    ########## Overriding abstract methods ##########

    def setFilePath(self, filepath):
        super().setFilePath(filepath)

    def getPolyData(self):
        # VRML importer
        importer = vtk.vtkVRMLImporter()
        # WRL file to upload
        importer.SetFileName(self.filepath)
        # Read data
        importer.Read()

        self.polydata = vtk.vtkPolyData()
        append = vtk.vtkAppendPolyData()
        renActors = importer.GetRenderer().GetActors()
        renActors.InitTraversal()
        actor = renActors.GetNextActor()

        i = 0
        while (actor != None):
            actor.GetProperty().SetAmbientColor(i, 1 - i, 0.0)
            actor.GetProperty().SetAmbient(1)
            append.AddInputData(actor.GetMapper().GetInput())
            actor = renActors.GetNextActor()
            i += 0.05

        append.Update()
        self.polydata.DeepCopy(append.GetOutput())

        return self.polydata

    def getVTKActor(self):
        self.actor = vtk.vtkActor()
        mapper = vtk.vtkPolyDataMapper()
        mapper.SetInputData(self.getPolyData())
        self.actor.SetMapper(mapper)
        return self.actor

    def getLandmarks(self, filename):
        #filepath = '/home/luantran/EncryptedCapstoneData/2353729.o3'
        vertebrae = []
        capteurs = []
        with open(filename, 'r') as f:
            landmark = {}
            landmarks = f.read().split('# ---- ---- ---- ---- ---- ---- DATA 3D  ---- ---- ---- ---- ---- ----')[
                1].rstrip()
            actual_landmarks = landmarks.split("Objet:")[1:]
            for landmark in actual_landmarks:
                type, data = self._createLandmark(landmark)
                if type == "capteurs":
                    capteurs = data
                else:
                    vertebrae.append(data)
        return vertebrae, capteurs

    #### Helper Methods ####

    def _createLandmark(self, data):
        datalines = data.rstrip().split('\n')
        name = datalines.pop(0).strip()
        datalines = datalines[1:]
        if name == "Capteurs":
            return "capteurs", self._getXrayExternalLandmarks(datalines)
        else:
            vertebra = {}
            vertebra['name'] = name
            vertebra['points'] = []
            for el in datalines:
                coordinates = el.split()
                point = {}
                point['name'] = coordinates[0].strip()
                point['x'] = float(coordinates[1].strip())
                point['y'] = float(coordinates[2].strip())
                point['z'] = float(coordinates[3].strip())
                if len(coordinates) > 4:
                    point['t'] = coordinates[3].strip()
                vertebra['points'].append(point)
            return "vertebra", vertebra

    def _getXrayExternalLandmarks(self, datalines):
        list_external_landmarks = []
        for el in datalines:
            landmark = {}
            key_value = el.split("\t")
            landmark['name'] = key_value[0].strip()
            coordinates = key_value[1].split()
            landmark['x'] = float(coordinates[0].strip())
            landmark['y'] = float(coordinates[1].strip())
            landmark['z'] = float(coordinates[2].strip())
            landmark['t'] = float(coordinates[3].strip())
            list_external_landmarks.append(landmark)
        return list_external_landmarks
import vtk
from src.landmarks_readers import landmarksReader


class WRLReaderLM(landmarksReader.LandmarksReader):

    ########## Overriding abstract methods ##########

    def setFilePath(self, filepath):
        super().setFilePath(filepath)


    def getPolyData(self):
        # Some of the function are comment becasue they are not used
        self.vertebrae_points, self.capteurs_points = self.getVTKPoints()
        # poly data
        self.vertebrae_polydata = vtk.vtkPolyData()
        self.vertebrae_polydata.SetPoints(self.vertebrae_points)

        self.capteurs_polydata = vtk.vtkPolyData()
        self.capteurs_polydata.SetPoints(self.capteurs_points)
        return self.vertebrae_polydata, self.capteurs_polydata

    def getVTKActor(self):
        self.actor = []
        self.mapper = []
        polydatas = self.getPolyData()
        vert_actor, vert_mapper = super().getVTKActor(5, "white", polydatas[0])
        capt_actor, capt_mapper = super().getVTKActor(10, "violet", polydatas[1])
        self.actor.append(vert_actor)
        self.actor.append(capt_actor)
        self.mapper.append(vert_mapper)
        self.mapper.append(capt_mapper)
        return self.actor

    def getVTKPoints(self):
        landmarks = self.getLandmarks(self.filepath)
        vertebrae_points = super().getVTKPoints(landmarks[0])
        capteurs_points = super().getVTKPoints(landmarks[1])
        return vertebrae_points, capteurs_points

    def getLandmarks(self, filename):
        vertebrae = []
        capteurs = []
        with open(filename, 'r') as f:
            landmarks = f.read().split('# ---- ---- ---- ---- ---- ---- DATA 3D  ---- ---- ---- ---- ---- ----')[
                1].rstrip()
            actual_landmarks = landmarks.split("Objet:")[1:]
            for landmark in actual_landmarks:
                type, data = self._createLandmark(landmark)
                if type == "capteurs":
                    capteurs = data
                else:
                    vertebrae.append(data)
        vertebrae = self.process(vertebrae)
        self.landmarks = [vertebrae, capteurs]
        return self.landmarks

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

    def process(self, vertebrae):
        processed_landmarks = []
        for vertebra in vertebrae:
            if "Vertebre" in vertebra['name']:
                id = vertebra['name'][-2:]
                for point in vertebra['points']:
                    # point['name'] = vertebra['name']+"-"+point['name']
                    point['name'] = id+"-"+point['name']
                    processed_landmarks.append(point)
        return processed_landmarks

    def updateVertVTKActor(self):
        self.vertebrae_points = vtk.vtkPoints()
        for point in self.landmarks[0]:
            self.vertebrae_points.InsertNextPoint(point['x'], point['y'], point['z'])

        self.vertebrae_polydata.SetPoints(self.vertebrae_points)
        self.vertebrae_polydata.GetPoints().Modified()
        return self.actor[0]

    def updateExtVTKActor(self):
        self.capteurs_points = vtk.vtkPoints()
        for point in self.landmarks[1]:
            self.capteurs_points.InsertNextPoint(point['x'], point['y'], point['z'])

        self.capteurs_polydata.SetPoints(self.capteurs_points)
        self.capteurs_polydata.GetPoints().Modified()
        return self.actor[1]
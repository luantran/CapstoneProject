import vtk
from src.readers import landmarksReader


class WRLReaderLM(landmarksReader.LandmarksReader):

    ########## Overriding abstract methods ##########

    def setFilePath(self, filepath):
        super().setFilePath(filepath)

    def getLandmarks(self, filename):
        pass

    def getPolyData(self):
        # Some of the function are comment becasue they are not used
        vertebrae_points, capteurs_points = self.getVTKPoints()
        # poly data
        vertebrae_polydata = vtk.vtkPolyData()
        vertebrae_polydata.SetPoints(vertebrae_points)

        capteurs_polydata = vtk.vtkPolyData()
        capteurs_polydata.SetPoints(capteurs_points)
        return vertebrae_polydata, capteurs_polydata

    def getVTKActor(self):
        self.actor = []
        polydatas = self.getPolyData()
        vert_actor = super().getVTKActor(5, "white", polydatas[0])
        capt_actor = super().getVTKActor(10, "violet", polydatas[1])
        self.actor.append(vert_actor)
        self.actor.append(capt_actor)
        return self.actor

    def getVTKPoints(self):
        landmarks = self.getLandmarks(self.filepath)
        self.vertebrae_points = super().getVTKPoints(landmarks[0])
        self.capteurs_points = super().getVTKPoints(landmarks[1])
        return self.vertebrae_points, self.capteurs_points

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
import vtk
from src.readers import landmarksReader


class SZEReaderLM(landmarksReader.LandmarksReader):

    ########## Overriding abstract methods ##########

    def setFilePath(self, filepath):
        super().setFilePath(filepath)

    def getLandmarks(self, filename):
        pass

    def getPolyData(self):
        # Some of the function are comment becasue they are not used
        points = self.getVTKPoints()
        # poly data
        polydata = vtk.vtkPolyData()
        polydata.SetPoints(points)
        return polydata

    def getVTKActor(self):
        self.actor = super().getVTKActor(10, "blue", self.getPolyData())
        return self.actor

    def getVTKPoints(self):
        return super().getVTKPoints(self.getLandmarks(self.filepath))

    def getLandmarks(self, filename):
        with open(filename, 'r') as f:
            data = f.read()
            datalines = data.rstrip().split("Objet:")[1]
            datalines = datalines.split("\n")
            datalines = datalines[2:]
            st_capteurs = self._getSTExternalLandmarks(datalines)
        self.landmarks = st_capteurs
        return self.landmarks

    ########## Helper Methods ##########

    def _getSTExternalLandmarks(self,datalines):
        list_external_landmarks = []
        for el in datalines:
            landmark = {}
            coordinates = el.split()
            landmark['name'] = coordinates[0].strip()
            landmark['x'] = float(coordinates[1].strip())
            landmark['y'] = float(coordinates[2].strip())
            landmark['z'] = float(coordinates[3].strip())
            if len(coordinates) > 4:
                landmark['t'] = coordinates[4].strip()
            list_external_landmarks.append(landmark)
        return list_external_landmarks





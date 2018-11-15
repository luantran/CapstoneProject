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
        self.polydata = vtk.vtkPolyData()
        self.polydata.SetPoints(points)
        return self.polydata

    def getVTKActor(self):
        self.actor, self.mapper = super().getVTKActor(10, "blue", self.getPolyData())
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

    def updateVTKActor(self):
        points = vtk.vtkPoints()
        for point in self.landmarks:
            points.InsertNextPoint(point['x'], point['y'], point['z'])

        self.polydata.SetPoints(points)
        self.polydata.GetPoints().Modified()
        return self.actor





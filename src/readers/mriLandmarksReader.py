import vtk
from src.readers import landmarksReader


class MRIReaderLM(landmarksReader.LandmarksReader):

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
        self.actor, self.mapper = super().getVTKActor(5, "green", self.getPolyData())
        return self.actor

    def getVTKPoints(self):
        return super().getVTKPoints(self.getLandmarks(self.filepath))

    def getLandmarks(self, filename):
        self.landmarks = []
        with open(filename, 'r') as f:
            lines = f.read()
            data_objects = lines.split("\n\n")
            data_objects = data_objects[1:]
            for object in data_objects:
                landmark = {}
                raw_data = object.split("\n")[0].split("Point:")[1]
                actual_data = raw_data.strip().split("create")

                landmark['name'] = actual_data[0].strip().replace('"', '')
                coordinates = actual_data[1].strip().split()
                landmark['x'] = float(coordinates[0].strip())
                landmark['y'] = float(coordinates[1].strip())
                landmark['z'] = float(coordinates[2].strip())
                self.landmarks.append(landmark)
        return self.landmarks

    def updateVTKActor(self):
        points = vtk.vtkPoints()
        for point in self.landmarks:
            points.InsertNextPoint(point['x'], point['y'], point['z'])

        self.polydata.SetPoints(points)
        self.polydata.GetPoints().Modified()
        return self.actor

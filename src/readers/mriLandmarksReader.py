import vtk
from src.readers import reader


class MRIReaderLM(reader.Reader):

    ########## Overriding abstract methods ##########

    def setFilePath(self, filepath):
        super().setFilePath(filepath)

    def getLandmarks(self, filename):
        pass

    def getPolyData(self):
        # Some of the function are comment becasue they are not used
        points = self.getVTKPoints(self.filepath)
        # poly data
        polydata = vtk.vtkPolyData()
        polydata.SetPoints(points)
        return polydata

    def getVTKActor(self):
        sphere_source = vtk.vtkSphereSource()
        glyph3D = vtk.vtkGlyph3D()
        glyph3D.SetSourceConnection(sphere_source.GetOutputPort())
        glyph3D.SetInputData(self.getPolyData())
        glyph3D.Update()
        sphere_source.SetThetaResolution(64)
        sphere_source.SetPhiResolution(64)
        sphere_source.SetRadius(10)

        mapper = vtk.vtkPolyDataMapper()
        mapper.SetInputConnection(glyph3D.GetOutputPort())
        mapper.Update()

        self.actor = vtk.vtkActor()
        self.actor.SetMapper(mapper)
        self.actor.GetProperty().SetColor(1.0, 0.0, 0.0)

        return self.actor

    def getVTKPoints(self, filename):
        landmarks = []
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
                landmarks.append(landmark)

        points = vtk.vtkPoints()
        for point in landmarks:
            points.InsertNextPoint(point['x'], point['y'], point['z'])

        return points

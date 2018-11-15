from abc import ABC, abstractmethod
import vtk
class LandmarksReader(ABC):
    def __init__(self):
        self.filepath = ''
        self.landmarks = None
        super().__init__()

    @abstractmethod
    def setFilePath(self, filepath):
        self.filepath = filepath

    @abstractmethod
    def getPolyData(self):
        pass

    def getVTKActor(self, size, color, polydata):
        sphere_source = vtk.vtkSphereSource()
        glyph3D = vtk.vtkGlyph3D()
        glyph3D.SetSourceConnection(sphere_source.GetOutputPort())
        glyph3D.SetInputData(polydata)
        glyph3D.Update()
        sphere_source.SetThetaResolution(64)
        sphere_source.SetPhiResolution(64)
        sphere_source.SetRadius(size)

        mapper = vtk.vtkPolyDataMapper()
        mapper.SetInputConnection(glyph3D.GetOutputPort())
        mapper.Update()

        actor = vtk.vtkActor()
        actor.SetMapper(mapper)
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
        elif color == "yellow":
            actor.GetProperty().SetColor(1.0, 1.0, 0.0)
        return actor, mapper

    def getVTKPoints(self, landmarks):
        points = vtk.vtkPoints()
        for point in landmarks:
            points.InsertNextPoint(point['x'], point['y'], point['z'])
        return points

    @abstractmethod
    def getLandmarks(self, filepath):
        pass

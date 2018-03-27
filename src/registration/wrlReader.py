import vtk
from src.registration import reader

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
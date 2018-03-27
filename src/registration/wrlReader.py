import vtk

class WRLReader():


    def setWRLFile(self, filename):
        self.wrlFile = filename


    def importVRMLData(self):
        # VRML importer
        importer = vtk.vtkVRMLImporter()
        # WRL file to upload
        importer.SetFileName(self.wrlFile)
        # Read data
        importer.Read()

        polydata = vtk.vtkPolyData()
        append = vtk.vtkAppendPolyData()
        renActors = importer.GetRenderer().GetActors()
        renActors.InitTraversal()
        actor = renActors.GetNextActor()

        i = 0
        while(actor != None):
            actor.GetProperty().SetAmbientColor(i, 1 - i, 0.0)
            actor.GetProperty().SetAmbient(1)
            append.AddInputData(actor.GetMapper().GetInput())
            actor = renActors.GetNextActor()
            i += 0.05

        append.Update()
        polydata.DeepCopy(append.GetOutput())

        return polydata
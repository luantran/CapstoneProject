import vtk
from PyQt5 import QtCore, QtGui, QtWidgets, Qt
from vtk.qt.QVTKRenderWindowInteractor import QVTKRenderWindowInteractor


class QTorsoViewer(QtWidgets.QFrame):
    def __init__(self, parent):
        super(QTorsoViewer,self).__init__(parent)

        self.frame = Qt.QFrame()
        self.vl = Qt.QVBoxLayout()
        self.vtkWidget = QVTKRenderWindowInteractor(self.frame)
        self.vl.addWidget(self.vtkWidget)

        self.ren = vtk.vtkRenderer()
        self.vtkWidget.GetRenderWindow().AddRenderer(self.ren)
        self.iren = self.vtkWidget.GetRenderWindow().GetInteractor()

        # Create source
        source = vtk.vtkSphereSource()
        source.SetCenter(0, 0, 0)
        source.SetRadius(5.0)

        # Create a mapper
        mapper = vtk.vtkPolyDataMapper()
        mapper.SetInputConnection(source.GetOutputPort())

        # Create an actor
        actor = vtk.vtkActor()
        actor.SetMapper(mapper)

        self.ren.AddActor(actor)

        self.ren.ResetCamera()

        self.setLayout(self.vl)
        # self.setCentralWidget(self.frame)

        self.show()
        self.iren.Initialize()
        self.iren.Start()


class WRLTest(QtWidgets.QFrame):
    def __init__(self, parent):
        super(WRLTest,self).__init__(parent)
        self.frame = Qt.QFrame()
        self.vl = Qt.QVBoxLayout()
        self.vtkWidget = QVTKRenderWindowInteractor(self.frame)
        self.vl.addWidget(self.vtkWidget)

        # Render
        self.ren = vtk.vtkRenderer()
        # Render Window
        self.renWin = self.vtkWidget.GetRenderWindow()
        # Add Render to Render Window
        self.renWin.AddRenderer(self.ren)
        # Interactor
        self.iren = self.vtkWidget.GetRenderWindow().GetInteractor()

        # VRML importer
        importer = vtk.vtkVRMLImporter()
        # Set the Render Window for the importer
        importer.SetRenderWindow(self.renWin)
        # WRL file to upload
        importer.SetFileName("/media/sumbal/C814-5638/2353729.wrl")
        # Read data
        importer.Read()
        # config Screen
        importer.GetRenderer().SetBackground(0.1, 0.2, 0.4)
        importer.GetRenderWindow().SetSize(300, 300)

        # the importer created the renderer
        renCollection = self.renWin.GetRenderers()
        renCollection.InitTraversal()
        ren = renCollection.GetNextItem()

        # change view up to +z
        #
        ren.GetActiveCamera().SetPosition(-3.25303, 3.46205, 3.15906)
        ren.GetActiveCamera().SetFocalPoint(0, 0, 0)
        ren.GetActiveCamera().SetViewUp(0.564063, 0.825024, -0.0341876)

        # let the renderer compute good position and focal point
        #
        ren.ResetCamera()
        ren.GetActiveCamera().Dolly(1.75)
        self.ren.ResetCameraClippingRange()

        # Add layout to the frame
        self.setLayout(self.vl)
        self.show()
        self.iren.Initialize()
        self.iren.Start()

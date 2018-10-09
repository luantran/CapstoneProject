import vtk
from src.registration import reader
import vtk
from vtk.util import numpy_support
import os
import numpy
from matplotlib import pyplot, cm

class MRIReader(reader.Reader):

    ########## Overriding abstract methods ##########

    def setFilePath(self, filepath):
        super().setFilePath(filepath)

    def getPolyData(self):
        return

    def getVTKActor(self):
        reader = vtk.vtkDICOMImageReader()
        reader.SetDirectoryName(self.filepath)
        reader.Update()

        # Calculate the center of the volume
        xMin, xMax, yMin, yMax, zMin, zMax = reader.GetDataExtent()
        xSpacing, ySpacing, zSpacing = reader.GetOutput().GetSpacing()
        x0, y0, z0 = reader.GetOutput().GetOrigin()
        center = [x0 + xSpacing * 0.5 * (xMin + xMax),
                  y0 + ySpacing * 0.5 * (yMin + yMax),
                  z0 + zSpacing * 0.5 * (zMin + zMax)]

        # Matrices for axial, coronal, sagittal, oblique view orientations
        axial = vtk.vtkMatrix4x4()
        axial.DeepCopy((1, 0, 0, center[0],
                        0, 1, 0, center[1],
                        0, 0, 1, center[2],
                        0, 0, 0, 1))

        coronal = vtk.vtkMatrix4x4()
        coronal.DeepCopy((1, 0, 0, center[0],
                          0, 0, 1, center[1],
                          0, -1, 0, center[2],
                          0, 0, 0, 1))

        sagittal = vtk.vtkMatrix4x4()
        sagittal.DeepCopy((0, 0, -1, center[0],
                           1, 0, 0, center[1],
                           0, -1, 0, center[2],
                           0, 0, 0, 1))

        # Extract a slice in the desired orientation
        self.reslice = vtk.vtkImageReslice()
        self.reslice.SetInputConnection(reader.GetOutputPort())
        self.reslice.SetOutputDimensionality(2)
        self.reslice.SetResliceAxes(axial)  # sagittal)
        self.reslice.SetInterpolationModeToCubic()

        # Create a greyscale lookup table
        table = vtk.vtkLookupTable()
        table.SetRange(0, 2000)  # image intensity range
        table.SetValueRange(0.0, 1.0)  # from black to white
        table.SetSaturationRange(0.0, 0.0)  # no color saturation
        table.SetRampToLinear()
        table.Build()

        # Map the image through the lookup table
        color = vtk.vtkImageMapToColors()
        color.SetLookupTable(table)
        color.SetInputConnection(self.reslice.GetOutputPort())

        # Display the image
        actor = vtk.vtkImageActor()
        actor.GetMapper().SetInputConnection(color.GetOutputPort())

        return actor

    def setInteractor(self, win, iren):
        # Set up the interaction
        interactorStyle = vtk.vtkInteractorStyleImage()

        iren.SetInteractorStyle(interactorStyle)
        win.Render()

        # Create callbacks for slicing the image
        actions = {}
        actions["Slicing"] = 0

        def ButtonCallback(obj, event):
            if event == "LeftButtonPressEvent":
                actions["Slicing"] = 1
            else:
                actions["Slicing"] = 0

        def MouseMoveCallback(obj, event):
            (lastX, lastY) = iren.GetLastEventPosition()
            (mouseX, mouseY) = iren.GetEventPosition()
            if actions["Slicing"] == 1:
                deltaY = mouseY - lastY
                self.reslice.Update()
                sliceSpacing = self.reslice.GetOutput().GetSpacing()[2]
                matrix = self.reslice.GetResliceAxes()
                # move the center point that we are slicing through
                center = matrix.MultiplyPoint((0, 0, sliceSpacing * deltaY, 1))
                matrix.SetElement(0, 3, center[0])
                matrix.SetElement(1, 3, center[1])
                matrix.SetElement(2, 3, center[2])
                win.Render()
            else:
                interactorStyle.OnMouseMove()

        interactorStyle.AddObserver("MouseMoveEvent", MouseMoveCallback)
        interactorStyle.AddObserver("LeftButtonPressEvent", ButtonCallback)
        interactorStyle.AddObserver("LeftButtonReleaseEvent", ButtonCallback)

        # Start interaction
        iren.Start()
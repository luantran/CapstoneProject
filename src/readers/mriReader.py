from src.readers import reader
import vtk
import pydicom as dicom
import os


class MRIReader(reader.Reader):

    ########## Overriding abstract methods ##########

    def setFilePath(self, filepath):
        super().setFilePath(filepath)

    def getPolyData(self):
        return

    def getLandmarks(self, filename):
        pass

    def getVTKActor(self):
        reader = vtk.vtkDICOMImageReader()
        reader.SetDirectoryName(self.filepath)
        reader.Update()

        # Calculate the center of the volume
        xMin, xMax, yMin, yMax, zMin, zMax = reader.GetDataExtent()

        self.max_number_of_slices = zMax

        xSpacing, ySpacing, zSpacing = reader.GetOutput().GetSpacing()
        if zSpacing == 0:
            self.thickness = self.get_spacing(self.filepath)
        else:
            self.thickness = zSpacing
        x0, y0, z0 = reader.GetOutput().GetOrigin()
        center = [x0 + xSpacing * 0.5 * (xMin + xMax),
                  y0 + ySpacing * 0.5 * (yMin + yMax),
                  z0 - zMax * zSpacing]
        if x0 == 0:
            x0, y0 = self.get_origin(self.filepath)
            """
            x0 = -(x0 + xSpacing * 0.5 * (xMin + xMax)) #447
            y0 = (y0 + ySpacing * 0.5 * (yMin + yMax)) #311 is ok
            """

        center = [-x0,
                  y0 + ySpacing * (yMin + yMax),
                  z0 - (zMax) * zSpacing]

        """x0 + xSpacing * 0.5 * (xMin + xMax),
                  y0 + ySpacing * 0.5 * (yMin + yMax),"""
        # this should be the center: [255.85492420197, 206.88938069343115, -324.0]


        # Matrices for axial, coronal, sagittal, oblique view orientations
        axial = vtk.vtkMatrix4x4()
        axial.DeepCopy((1, 0, 0, center[0],
                        0, -1, 0, center[1],
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
        self.reslice.SetOutputDimensionality(3)
        self.reslice.SetResliceAxes(axial)
        self.reslice.SetInterpolationModeToCubic()

        # Create a greyscale lookup table
        table = vtk.vtkLookupTable()
        table.SetRange(0, 2000)  # image intensity range
        table.SetValueRange(0.0, 5.0)  # from black to white
        table.SetSaturationRange(0.0, 0.0)  # no color saturation
        table.SetRampToLinear()
        table.Build()

        # Map the image through the lookup table
        color = vtk.vtkImageMapToColors()
        color.SetLookupTable(table)
        color.SetInputConnection(self.reslice.GetOutputPort())

        # Display the image
        self.actor = vtk.vtkImageActor()
        self.actor.GetProperty().SetOpacity(0.90)
        self.actor.GetMapper().SetInputConnection(color.GetOutputPort())

        return self.actor

    def get_spacing(self, filepath):
        lstFilesDICOM = []
        for dirName, subdirList, fileList in os.walk(filepath):
            for filename in fileList:
                if ".dcm" in filename.lower() or ".ima" in filename.lower():  # check whether the file's DICOM
                    lstFilesDICOM.append(os.path.join(dirName, filename))
        RefDs = dicom.read_file(lstFilesDICOM[0])
        spacing = float(RefDs.SliceThickness)
        return spacing

    def get_origin(self, filepath):
        lstFilesDICOM = []
        for dirName, subdirList, fileList in os.walk(filepath):
            for filename in fileList:
                if ".dcm" in filename.lower() or ".ima" in filename.lower():  # check whether the file's DICOM
                    lstFilesDICOM.append(os.path.join(dirName, filename))
        RefDs = dicom.read_file(lstFilesDICOM[0])
        origin = [float(RefDs[('0020', '0032')].value[0]), float(RefDs[('0020', '0032')].value[1])]
        return origin

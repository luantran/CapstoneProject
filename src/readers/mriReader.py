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
                  z0]

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
        self.reslice.SetOutputDimensionality(3)
        self.reslice.SetResliceAxes(axial)
        self.reslice.SetInterpolationModeToCubic()
        # self.reslice.SetOutputExtent(xMin, xMax, yMin, yMax, zMin, zMax)
        # self.reslice.SetOutputSpacing(xSpacing, ySpacing, zSpacing)

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
        actor = vtk.vtkImageActor()
        actor.GetProperty().SetOpacity(0.5)
        actor.GetMapper().SetInputConnection(color.GetOutputPort())

        return actor

    def getLandmarks(self, filepath):
        #filepath = '/home/luantran/EncryptedCapstoneData/2353729points_all.scp'
        landmarks = []
        with open(filepath, 'r') as f:
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
        return landmarks

    def get_spacing(self, filepath):
        lstFilesDICOM = []
        for dirName, subdirList, fileList in os.walk(filepath):
            for filename in fileList:
                if ".dcm" in filename.lower() or ".ima" in filename.lower():  # check whether the file's DICOM
                    lstFilesDICOM.append(os.path.join(dirName, filename))
        RefDs = dicom.read_file(lstFilesDICOM[0])
        spacing = float(RefDs.SliceThickness)
        print(spacing)
        return spacing
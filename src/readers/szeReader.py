# % function sze = ReadSZE(varargin)
# % Read the selected .sze file
# %
# %   Input:
# %       - if none, open a window to select a .sze file
# %       - if one argument: name of a .sze file located in the current directory
# %       - if two arguments: pathname and filename
# %
# %   Output:
# %       sze.Name: filename
# %       sze.Coord: nx3 matrix, Coordinates x,y,z
# %       sze.Connect: mx3 matrix, connectivity
# %       sze.RGB: nx3 matrix, RGB texture information
# %
# % Created by Valerie Pazos
# % Modified by Philippe Debanné
# %
# % Laboratoire d'Imagerie et de Vision 4D (LIV4D)
# % Département de Génie Informatique et Génie Logiciel
# % École Polytechnique de Montréal
# %
import re
import numpy as np
import vtk
from src.readers import reader


class SZEReader(reader.Reader):

    ########## Overriding abstract methods ##########

    def setFilePath(self, filepath):
        super().setFilePath(filepath)
        self._extractLinesFromFile()

    def getPolyData(self):

        # Some of the function are comment becasue they are not used
        self._getNBPolygon()
        self._getConnectivityMatrix()
        self._getCoordinatesMatrix()
        self._addingVTKCoords()
        self._addingVTKPolygonCells()

        # poly data
        polydata = vtk.vtkPolyData()
        polydata.SetPoints(self.points)
        polydata.SetPolys(self.polys)
        return polydata

    def getVTKActor(self):
        self.actor = vtk.vtkActor()
        # Rotate actor (adopted from Rola's code)
        self.actor.RotateZ(-90)
        self.actor.RotateX(90)
        mapper = vtk.vtkPolyDataMapper()
        mapper.SetInputData(self.getPolyData())
        self.actor.SetMapper(mapper)
        return self.actor

    def getLandmarks(self, filename):
        #filepath = '/home/luantran/EncryptedCapstoneData/2353729_debout.ext'
        st_capteurs = []
        with open(filename, 'r') as f:
            data = f.read()
            datalines = data.rstrip().split("Objet:")[1]
            datalines = datalines.split("\n")
            datalines = datalines[2:]
            st_capteurs = self._getSTExternalLandmarks(datalines)
        return st_capteurs

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

    def _mkVtkIdList(self, it):
        '''
            Helper function
        :param it: polygon list
        :return: vtkIdList
        '''
        vil = vtk.vtkIdList()
        for i in it:
            vil.InsertNextId(int(i))
        return vil

    def _extractLinesFromFile(self):
        with open(self.filepath) as file:
            self.text = file.read()
            self.lines = file.readlines()

    # Get the number of polygons (triangles)
    def _getNBPolygon(self):
        self.nbpolygon = int(re.search('\d+', re.search('NbPolygon=\d*', self.text).group(0)).group(0))

    # Read index table to 3D coordinates
    def _getConnectivityMatrix(self):
        self.connect = np.array([int(i) for i in re.search('SizePolygon={1}[\d\s]*END{1}', self.text).group(0).split()[1:-1]])
        self.connect = self.connect.reshape(self.nbpolygon, 4)
        self.connect = self.connect[:, 1:4]

    # Read the 3D Coordinates Table
    def _getCoordinatesMatrix(self):
        tmp = re.search('XPos.*\s[+-\.\d\s]*END', self.text).group(0)
        first_line_break = tmp.find('\n')
        self.coord = np.array([float(i) for i in re.search('XPos.*\s[+-\.\d\s]*END', self.text).group(0)[first_line_break:].split()[:-1]])
        self.nbvertices = int(self.coord.size / 3)
        self.coord = self.coord.reshape(self.nbvertices, 3)

    def _addingVTKCoords(self):
        # adding coordinates
        vtk_coords = list(self.coord)
        self.points = vtk.vtkPoints()
        for vtk_coord in vtk_coords:
            self.points.InsertNextPoint(vtk_coord)

    def _addingVTKPolygonCells(self):
        # Adding polygon cells
        self.polys = vtk.vtkCellArray()
        vtk_connects = list(self.connect)
        for vtk_connect in vtk_connects:
            self.polys.InsertNextCell(self._mkVtkIdList(vtk_connect))

if __name__ == "__main__":
    sze = SZEReader()
    sze.setFilePath('/home/luantran/Downloads/2353729.sze')
    sze.getPolyData()



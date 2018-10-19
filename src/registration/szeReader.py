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
from src.registration import reader
import json

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
        mapper = vtk.vtkPolyDataMapper()
        mapper.SetInputData(self.getPolyData())
        self.actor.SetMapper(mapper)
        return self.actor

    def getLandmarks(self):
        filepath = '/home/luantran/EncryptedCapstoneData/2353729_debout.ext'
        st_capteurs = []
        with open(filepath, 'r') as f:
            data = f.read()
            datalines = data.rstrip().split("Objet:")[1]
            datalines = datalines.split("\n")
            datalines = datalines[2:]
            st_capteurs = self._getSTExternalLandmarks(datalines)
        with open('result_st.json', 'w') as fp:
            json.dump(st_capteurs, fp)
        return st_capteurs

    ########## Helper Methods ##########

    def _getSTExternalLandmarks(self,datalines):
        list_external_landmarks = []
        for el in datalines:
            landmark = {}
            coordinates = el.split()
            landmark['name'] = coordinates[0].strip()
            landmark['x'] = coordinates[1].strip()
            landmark['y'] = coordinates[2].strip()
            landmark['z'] = coordinates[3].strip()
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
        file = open(self.filepath)
        self.text = file.read()
        file.close()
        file = open(self.filepath)
        self.lines = file.readlines()

    # Get the number of polygons (triangles)
    def _getNBPolygon(self):
        self.nbpolygon = nbpolygon = int(re.search('\d+', re.search('NbPolygon=\d*', self.text).group(0)).group(0))

    # Read index table to 3D coordinates
    def _getConnectivityMatrix(self):
        self.connect = np.array([int(i) for i in re.search('SizePolygon={1}[\d\s]*END{1}', self.text).group(0).split()[1:-1]])
        self.connect = self.connect.reshape(self.nbpolygon, 4)
        self.connect = self.connect[:, 1:4]

    # Read the indexing table to the texture coordinates
    def _getMeshTextureMatrix(self):
        self.meshtex = np.array([int(i) for i in re.search('BEGIN MESH_TEX{1}[\d\s]*END MESH_TEX{1}', self.text).group(0).split()[2:-2]])
        self.meshtex = self.meshtex.reshape(self.nbpolygon, 4)
        self.meshtex = self.meshtex[:, 1:4] + 1

    # Read the table of texture coordinates
    def _getUVMatrix(self):
        nbuv = int(re.search('\d+', re.search('NbTexture=\d*', self.text).group(0)).group(0))
        self.uv = np.array([float(i) for i in re.search('NbTexture=[\.\d\s]*END', self.text).group(0).split()[1:-1]])
        self.uv = self.uv.reshape(nbuv, 2)

    # Read the 3D Coordinates Table
    def _getCoordinatesMatrix(self):
        tmp = re.search('XPos.*\s[+-\.\d\s]*END', self.text).group(0)
        first_line_break = tmp.find('\n')
        self.coord = np.array([float(i) for i in re.search('XPos.*\s[+-\.\d\s]*END', self.text).group(0)[first_line_break:].split()[:-1]])
        self.nbvertices = int(self.coord.size / 3)
        self.coord = self.coord.reshape(self.nbvertices, 3)

    def _getRGBMatrixData(self):
        self.RGB = None
        self.sizeTex = []
        # Main Loop
        for line_num, line in enumerate(self.lines):

            # Find the BEGIN IMAGE string in found in file
            # Retain the component of RGB and Ignore the alpha component
            if 'BEGIN IMAGE ' in line:
                # print("BEGIN IMAGE")
                self.sizeTex = [int(i) for i in self.lines[line_num + 1].split()]
                # Convert the hex values of triplets directly (R, G, B)
                RGB_shape = (0, 3)
                self.RGB = np.zeros(RGB_shape, dtype=np.int)
                base = line_num + 2
                # Iterate each line
                pass_check = base + self.sizeTex[1]
                for line in self.lines[base:pass_check]:
                    my_rgb = np.array([])
                    count = 0

                    hex = line.split()
                    # # Iterate within line
                    for hex_string in hex:
                        my_rgb = np.append(my_rgb, [int(hex_string[2:4], 16), int(hex_string[4:6], 16),
                                                    int(hex_string[6:8], 16)])
                    my_rgb = my_rgb.reshape(self.sizeTex[0], 3)
                    self.RGB = np.r_[self.RGB, my_rgb]
                break

    def convertTextureCoords(self):
        # Convert texture coordinates (normalized in [0,1] to)
        # coordinates in the texture image
        self.u = np.round_((self.uv[:, 0]) * (self.sizeTex[0] - 1)) + 1
        self.v = np.round_((1 - self.uv[:, 1]) * (self.sizeTex[1] - 1)) + 1

    def _extractRGB(self):
        # For each polygon (triangle) of the surface, extract the values ​​of the
        #  triplets (R, G, B) associated with vertices and normalize them in [0,1].
        #  We only keep a triplet (R, G, B) per 3D vertex.
        sze_RGB_shape = (self.nbvertices, 3)
        self.sze_RGB = np.zeros(sze_RGB_shape, dtype=np.float)

        for j in range(self.connect.shape[0]):
            # print((u[meshtex[j, 0] - 1] + (v[meshtex[j, 0] - 1]-1) * sizeTex[0]))
            # print(RGB[int(u[meshtex[j, 2] - 1] + (v[meshtex[j, 2] - 1]-1) * sizeTex[0]) - 1, :] / 255.0)
            if self.connect[j, 2] >= 72971:
                # print(j, self.connect[j, 2])
                continue
            self.sze_RGB[self.connect[j, 0], :] = self.RGB[int(self.u[self.meshtex[j, 0] - 1] + (self.v[self.meshtex[j, 0] - 1]-1) * self.sizeTex[0]) - 1, :] / 255.0
            self.sze_RGB[self.connect[j, 1], :] = self.RGB[int(self.u[self.meshtex[j, 1] - 1] + (self.v[self.meshtex[j, 1] - 1]-1) * self.sizeTex[0]) - 1, :] / 255.0
            self.sze_RGB[self.connect[j, 2], :] = self.RGB[int(self.u[self.meshtex[j, 2] - 1] + (self.v[self.meshtex[j, 2] - 1]-1) * self.sizeTex[0]) - 1, :] / 255.0

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



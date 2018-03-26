#   Input:
#       - if none, open a window to select a .sze file
#       - if one argument: name of a .sze file located in the current directory
#       - if two arguments: pathname and filename
#
#   Output:
#       sze.Name: filename
#       sze.Coord: nx3 matrix, Coordinates x,y,z
#       sze.Connect: mx3 matrix, connectivity
#       sze.RGB: nx3 matrix, RGB texture information
#
# Created by Muhammad Abdullah Sumbal
#
# To run this file, add the following two lines in app.py
# from misc.szeReader import SurfaceTopographyViewer
# SurfaceTopographyViewer(self.ui.frame)

import re
import numpy as np
import vtk
from PyQt5 import QtWidgets, Qt
from vtk.qt.QVTKRenderWindowInteractor import QVTKRenderWindowInteractor


class SurfaceTopographyViewer(QtWidgets.QFrame):

    def mkVtkIdList(self, it):
        """
            Helper function
        :param it: polygon list
        :return: vtkIdList
        """
        vil = vtk.vtkIdList()
        for i in it:
            vil.InsertNextId(int(i))
        return vil

    def __init__(self, parent):
        super(SurfaceTopographyViewer,self).__init__(parent)

        # open sze file
        file = open('/media/sumbal/C814-5638/2353729.sze')
        text = file.read()
        file.close()
        file = open('/media/sumbal/C814-5638/2353729.sze')
        lines = file.readlines()

        # Get the number of polygons (triangles)
        nbpolygon = int(re.search('\d+', re.search('NbPolygon=\d*', text).group(0)).group(0))

        # Read index table to 3D coordinates
        Connect = np.array([int(i) for i in re.search('SizePolygon={1}[\d\s]*END{1}', text).group(0).split()[1:-1]])
        Connect = Connect.reshape(nbpolygon, 4)
        Connect = Connect[:, 1:4] + 1

        # Read the indexing table to the texture coordinates
        meshtex = np.array([int(i) for i in re.search('BEGIN MESH_TEX{1}[\d\s]*END MESH_TEX{1}', text).group(0).split()[2:-2]])
        meshtex = meshtex.reshape(nbpolygon, 4)
        meshtex = meshtex[:, 1:4] + 1

        # Read the table of texture coordinates
        nbuv = int(re.search('\d+', re.search('NbTexture=\d*', text).group(0)).group(0))
        uv = np.array([float(i) for i in re.search('NbTexture=[\.\d\s]*END', text).group(0).split()[1:-1]])
        uv = uv.reshape(nbuv, 2)

        # Read the 3D Coordinates Table
        tmp = re.search('XPos.*\s[+-\.\d\s]*END', text).group(0)
        first_line_break = tmp.find('\n')
        Coord = np.array([float(i) for i in re.search('XPos.*\s[+-\.\d\s]*END', text).group(0)[first_line_break:].split()[:-1]])
        nbvertices = int(Coord.size / 3)
        Coord = Coord.reshape(nbvertices, 3)

        RGB = None
        sizeTex = []
        # Main Loop
        for line_num, line in enumerate(lines):

            # Find the BEGIN IMAGE string in found in file
            # Retain the component of RGB and Ignore the alpha component
            if 'BEGIN IMAGE ' in line:
                print("BEGIN IMAGE")
                sizeTex = [int(i) for i in lines[line_num + 1].split()]
                # Convert the hex values of triplets directly (R, G, B)
                RGB_shape = (0, 3)
                RGB = np.zeros(RGB_shape, dtype=np.int)
                base = line_num + 2
                # Iterate each line
                pass_check = base + sizeTex[1]
                for line in lines[base:pass_check]:
                    my_rgb = np.array([])
                    count = 0

                    hex = line.split()
                    # # Iterate within line
                    for hex_string in hex:
                        my_rgb = np.append(my_rgb, [int(hex_string[2:4], 16),int(hex_string[4:6], 16),int(hex_string[6:8], 16)])
                    my_rgb = my_rgb.reshape(sizeTex[0], 3)
                    RGB = np.r_[RGB, my_rgb]
                break

        # Convert texture coordinates (normalized in [0,1] to)
        # coordinates in the texture image
        u = np.round_((uv[:, 0]) * (sizeTex[0]-1))+1
        v = np.round_((1 - uv[:, 1]) * (sizeTex[1]-1))+1

        # For each polygon (triangle) of the surface, extract the values ​​of the
        #  triplets (R, G, B) associated with vertices and normalize them in [0,1].
        #  We only keep a triplet (R, G, B) per 3D vertex.
        sze_RGB_shape = (nbvertices, 3)
        sze_RGB = np.zeros(sze_RGB_shape, dtype=np.float)

        for j in range(Connect.shape[0]):
            # print((u[meshtex[j, 0] - 1] + (v[meshtex[j, 0] - 1]-1) * sizeTex[0]))
            # print(RGB[int(u[meshtex[j, 2] - 1] + (v[meshtex[j, 2] - 1]-1) * sizeTex[0]) - 1, :] / 255.0)
            if Connect[j, 2] >= 72971:
                print(j, Connect[j, 2])
                continue
            sze_RGB[Connect[j, 0], :] = RGB[int(u[meshtex[j, 0] - 1] + (v[meshtex[j, 0] - 1]-1) * sizeTex[0]) - 1, :] / 255.0
            sze_RGB[Connect[j, 1], :] = RGB[int(u[meshtex[j, 1] - 1] + (v[meshtex[j, 1] - 1]-1) * sizeTex[0]) - 1, :] / 255.0
            sze_RGB[Connect[j, 2], :] = RGB[int(u[meshtex[j, 2] - 1] + (v[meshtex[j, 2] - 1]-1) * sizeTex[0]) - 1, :] / 255.0

        # adding coordinates
        vtk_coords = list(Coord)
        points = vtk.vtkPoints()
        for vtk_coord in vtk_coords:
            points.InsertNextPoint(vtk_coord)

        # Adding polygon cells
        polys = vtk.vtkCellArray()
        vtk_connects = list(Connect)
        for vtk_connect in vtk_connects:
            polys.InsertNextCell(self.mkVtkIdList(vtk_connect))

        # ploy data
        self.polydata = vtk.vtkPolyData()
        self.polydata.SetPoints(points)
        self.polydata.SetPolys(polys)

        # mapper
        self.mapper = vtk.vtkPolyDataMapper()
        self.mapper.SetInputData(self.polydata)

        # actor
        self.actor = vtk.vtkActor()
        self.actor.SetMapper(self.mapper)

        # adding the render to frame
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
        # Add actor
        self.ren.AddActor(self.actor)

        # setting the window size
        self.renWin.SetSize(600, 600)
        # Add layout to the frame
        self.setLayout(self.vl)
        self.show()
        self.iren.Initialize()
        self.iren.Start()
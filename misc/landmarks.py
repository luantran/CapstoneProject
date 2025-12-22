import re
import json

#Xray
def getXrayExternalLandmarks(datalines):
    list_external_landmarks=[]
    for el in datalines:
        landmark = {}
        key_value = el.split("\t")
        landmark['name']=key_value[0].strip()
        coordinates=key_value[1].split()
        landmark['x']=coordinates[0].strip()
        landmark['y']=coordinates[1].strip()
        landmark['z']=coordinates[2].strip()
        landmark['t']=coordinates[3].strip()
        list_external_landmarks.append(landmark)
    return list_external_landmarks

def getSTExternalLandmarks(datalines):
    list_external_landmarks=[]
    for el in datalines:
        landmark = {}
        coordinates = el.split()
        landmark['name']=coordinates[0].strip()
        landmark['x']=coordinates[1].strip()
        landmark['y']=coordinates[2].strip()
        landmark['z']=coordinates[3].strip()
        if len(coordinates) > 4:
            landmark['t']=coordinates[4].strip()
        list_external_landmarks.append(landmark)
    return list_external_landmarks

def createLandmark(data):
    datalines=data.rstrip().split('\n')
    name=datalines.pop(0).strip()
    datalines=datalines[1:]
    if name == "Capteurs":
        return "capteurs", getXrayExternalLandmarks(datalines)
    else:
        vertebra = {}
        vertebra['name'] = name
        vertebra['points'] = []
        for el in datalines:
            coordinates = el.split()
            point={}
            point['name']= coordinates[0].strip()
            point['x']=coordinates[1].strip()
            point['y']=coordinates[2].strip()
            point['z']=coordinates[3].strip()
            if len(coordinates) > 4:
                point['t']=coordinates[3].strip()
            vertebra['points'].append(point)
        return "vertebra", vertebra
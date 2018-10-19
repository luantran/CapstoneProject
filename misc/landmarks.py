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

def getXRayLandmarks():
    filepath = '/home/luantran/EncryptedCapstoneData/2353729.o3'
    vertebrae=[]
    capteurs=[]
    with open(filepath, 'r') as f:
        landmark={}
        landmarks = f.read().split('# ---- ---- ---- ---- ---- ---- DATA 3D  ---- ---- ---- ---- ---- ----')[1].rstrip()
        actual_landmarks = landmarks.split("Objet:")[1:]
        for landmark in actual_landmarks:
            type, data = createLandmark(landmark)
            if type == "capteurs":
                capteurs = data
            else:
                vertebrae.append(data)
        with open('result_xray.json', 'w') as fp:
            json.dump(vertebrae, fp)
        with open('result_xray_ext.json', 'w') as fp:
            json.dump(capteurs, fp)
#ST
def getSTLandmarks():
    filepath = '/home/luantran/EncryptedCapstoneData/2353729_debout.ext'
    st_capteurs=[]
    with open(filepath, 'r') as f:
        data=f.read()
        datalines=data.rstrip().split("Objet:")[1]
        datalines=datalines.split("\n")
        datalines=datalines[2:]
        st_capteurs=getSTExternalLandmarks(datalines)
    with open('result_st.json', 'w') as fp:
        json.dump(st_capteurs, fp)

#MRI
def getMRILandmarks():
    filepath = '/home/luantran/EncryptedCapstoneData/2353729points_all.scp'
    landmarks=[]
    with open(filepath, 'r') as f:
        lines=f.read()
        data_objects=lines.split("\n\n")
        data_objects=data_objects[1:]
        for object in data_objects:
            landmark={}
            raw_data=object.split("\n")[0].split("Point:")[1]
            actual_data=raw_data.strip().split("create")

            landmark['name'] = actual_data[0].strip().replace('"', '')
            coordinates=actual_data[1].strip().split()
            landmark['x']=coordinates[0]
            landmark['y']=coordinates[1]
            landmark['z']=coordinates[2]
            landmarks.append(landmark)
    with open('result_mri.json', 'w') as fp:
        json.dump(landmarks, fp)

if __name__ == "__main__":
    getXRayLandmarks()
    getSTLandmarks()
    getMRILandmarks()
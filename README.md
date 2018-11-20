[![Waffle.io - Columns and their card count](https://badge.waffle.io/luantran/CapstoneProject.png?columns=all)](https://waffle.io/luantran/CapstoneProject?utm_source=badge)
# 3D Image Reconstruction of a Human Torso

This repository contains the source code for the The-Luan Tran's and Abdullah Sumbal's Capstone Project: **Multimodal Image Fusion for the Reconstruction of a Human Torso**

By The-Luan Tran, Abdullah Sumbal

# User Guide

Here follows a user guide to help any user of this software.

## The User Interface

Below is a screenshot of what the User Interface looks like with some annotations.

![alt text](https://github.com/luantran/CapstoneProject/blob/master/assets/images/ui_view.png "User Interface")

1. Load Section: Where the users loads each modality and its respective landmarks
2. Registration Section: Buttons that allow the user to registaer the modalities
3. View Section:
4. Save Section:
5. Render Window:

## Loading the modalities and the landmarks

There are three modalities to load, along with 3 landmarks files. If these files are not loaded, then you cannot perform registration. The registration buttons will stay greyed out.

#### MRI 

The MRI modality is a directory consisting of .dcm files or .ima files.
The MRI landmarks are in an .scp file.

#### X-Ray

The X-Ray modality is a .wrl file of the VRML format.
The X-Ray landmarks come in an .o3 file.

#### Surface Topography

The Surface Topography modality is a .sze file.
The X-Ray landmarks come in an .ext file.

### Using the Questionnaire

### Reloading the Modalities

## Registration

## Viewing Options

## Saving and Reloading


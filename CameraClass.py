import os
import cv2
import numpy as np
import mediapipe as mp
import matplotlib.pyplot as plt
import time

class camera:

    cameraLink = ""
    camFeed = None

    def findCamera(self, WebcamID):
        camerafolder = '/dev/v4l/by-id/'
        cameras = os.listdir(camerafolder)
        for camera in cameras:
            if WebcamID in camera:
                WebcamNumber = camerafolder + camera
        print("Finding Camera...")

    def linkCamera(self):
        self.camFeed = cv2.VideoCapture('/dev/v4l/by-id/usb-OmniVision_Technologies__Inc._USB_Camera-B4.09.24.1-video-index0')
        ret, frame = self.camFeed.read()
        ret, frame = self.camFeed.read()
        ret, frame = self.camFeed.read()
        ret, frame = self.camFeed.read()

        print("Initializing")
        time.sleep(1)

    def readCamera(self):
        ret, frame = self.camFeed.read()
        if ret == True:
            return frame
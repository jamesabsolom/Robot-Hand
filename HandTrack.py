import os
import cv2
import numpy as np
import mediapipe as mp
import matplotlib.pyplot as plt
import CameraClass
import math
import serial
import time
from FingerClass import finger

#Initiates the hand class and stores it in a variable
mpHands = mp.solutions.hands
hands = mpHands.Hands()
mpDraw = mp.solutions.drawing_utils
Debug = True

Cam = CameraClass.camera()
Cam.findCamera("OmniVision")
Cam.linkCamera()
success, image = Cam.camFeed.read()
success, image = Cam.camFeed.read()
success, image = Cam.camFeed.read()
arduinoCom = serial.Serial("/dev/ttyACM0", 9600, timeout= 0.01)
arduinoCom.setDTR(False)
dim = (640, 480)

pinky = finger()
ring = finger()
middle = finger()
index = finger()
thumb = finger()

while True:
    arduinoCom.flushInput()
    arduinoCom.flushOutput()
    success, image = Cam.camFeed.read()
    if Debug:
        print("Got image from camera")
    image = cv2.resize(image, dim, interpolation = cv2.INTER_AREA)
    if Debug:
        print("Resized image")
    imageRGB = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    if Debug:
        print("Converted image to RGB")

    results = hands.process(imageRGB)
    if Debug:
        print("Found hands")
    # checking whether a hand is detected
    if results.multi_hand_landmarks:
        for handLms in results.multi_hand_landmarks: # working with each hand
            for id, lm in enumerate(handLms.landmark):
                h, w, c = image.shape
                cx, cy = int(lm.x * w), int(lm.y * h)
                if id == 0:
                    palmPoint = (cx, cy)
                elif id == 17:
                    pinkyBase = (cx, cy)
                elif id == 20:
                    pinkyPoint = (cx, cy)
                elif id == 16:
                    ringPoint = (cx, cy)
                elif id == 12:
                    middlePoint = (cx, cy)
                elif id == 8:
                    indexPoint = (cx, cy)        
                elif id == 4:
                    thumbPoint = (cx, cy)             
                elif id == 9:
                    cv2.circle(image, (cx, cy), 25, (255, 0, 255), cv2.FILLED)

            mpDraw.draw_landmarks(image, handLms, mpHands.HAND_CONNECTIONS)
            if Debug:
                print("Drawn landmarks")

        try:
            pinkyDistance = pinky.calculatedistance(pinkyPoint, palmPoint)
            ringDistance = ring.calculatedistance(ringPoint, palmPoint)
            middleDistance = middle.calculatedistance(middlePoint, palmPoint)
            indexDistance = index.calculatedistance(indexPoint, palmPoint)
            thumbDistance = thumb.calculatedistance(thumbPoint, pinkyBase)
            if Debug:
                print("Calculated distances")

            output = (str(pinkyDistance).encode() + " " .encode() +
                      str(ringDistance).encode() + " ".encode() +
                      str(middleDistance).encode() + " ".encode() +
                      str(indexDistance).encode() + " ".encode() +
                      str(thumbDistance).encode() + '\r\n'.encode())
            
            # print(output)
            arduinoCom.write(output)
            if Debug:
                print("Sent data to arduino")
            cv2.imshow("Output", image)
            cv2.waitKey(1)
            time.sleep(1./120)

        except Exception as e:
            print(e)
    
    else:
        pinky.reset()
        ring.reset()
        middle.reset()
        index.reset()
        thumb.reset()
        output = (str(0).encode() + " " .encode() +
                    str(0).encode() + " ".encode() + 
                    str(0).encode() + " ".encode() +
                    str(0).encode() + " ".encode() +
                    str(0).encode() + '\r\n'.encode())

        # print(output)
        arduinoCom.write(output)
        if Debug:
            print("Sent data to arduino")
        cv2.imshow("Output", image)
        cv2.waitKey(1)
        time.sleep(1./120)




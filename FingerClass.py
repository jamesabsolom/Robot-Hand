import os
import cv2
import numpy as np
import mediapipe as mp
import matplotlib.pyplot as plt
import CameraClass
import math
import serial
import time

class finger:

    def __init__(self):
        self.Max = 0
        self.Min = 1000
        self.distance = 0

    def map_range(self, x, in_min, in_max, out_min, out_max):
        return (x - in_min) * (out_max - out_min) // (in_max - in_min) + out_min
    
    def calculatedistance(self, p1, p2):
        self.distance = int(math.sqrt(((p1[0] - p2[0])**2) + ((p1[1] - p2[1])**2)))

        if self.distance > self.Max:
            self.Max = self.distance

        if self.distance < self.Min:
            self.Min = self.distance

        self.distance = self.map_range(self.distance, self.Min, self.Max, 180, 0)

        return self.distance
    
    def reset(self):
        self.Max = 0
        self.Min = 1000


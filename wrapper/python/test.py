#import sys
import time
#import ctypes

from Hub import *

import numpy as np
from matplotlib import pyplot as plt

width = 512
depth = 512
height = 100

resolution = [width, depth, height], Format.DENSITY
print(resolution)

metadata = { "realWidth" : 100.0, "realDepth" : 100.0, "realHeight" : 50.0 }
print(metadata)

sensorSpec = SensorSpec("sensorName", [resolution], metadata)
print(sensorSpec.to_string())


streamer = Streamer("127.0.0.1", 4042)
streamer.addStream("streamName", sensorSpec)


data = np.zeros((width, depth, height, 4), dtype=np.uint8)
#data[0:width,0:height] = [255, 0, 0]
#data = np.zeros((width, depth, height, 4))
data[0:width,0:depth,0:height,0:4] = 127
print(data.shape)


vector = data.flatten()
print(vector.shape)

acq = Acquisition(1, 1)

measure = Measure(vector, resolution)

acq.addMeasure(measure)

streamer.newAcq("streamName", acq)


time.sleep(1)
iAcq = 0

while streamer.isConnected():

    vector = data.flatten()

    acq = Acquisition(1, 1)

    measure = Measure(vector, resolution)

    acq.addMeasure(measure)

    streamer.newAcq("streamName", acq)

    print("sended acq " + str(iAcq))

    try:
        time.sleep(1)
    except KeyboardInterrupt:
        print("The program is terminated manually!")
        raise SystemExit

    iAcq += 1

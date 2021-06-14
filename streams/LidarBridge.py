import pyrealsense2 as rs
import matplotlib.pyplot as plt
import numpy as np
import socket

# Configure depth and color streams
pipeline = rs.pipeline()
config = rs.config()
config.enable_stream(rs.stream.color, 640, 480, rs.format.rgb8, 6)

print("Starting pipeline...")
# Start streaming
try :
    pipeline.start(config)
except Exception as e:
    print(e)
    exit(2)

print("Camera connected")

while True :
    print("\nStarting server...")
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(("127.0.0.1", 4043))
    server.listen(1)
    print("Server is listening")

    conn, addr = server.accept()

    print("Client connected, begining streaming")

    try:
        while True:

            a = conn.recv(1)
            frames = pipeline.wait_for_frames()
            frame = frames.get_color_frame()
            #image = np.asanyarray(frame.get_data())

            size = frame.get_data_size()

            conn.send(size.to_bytes(4, byteorder='little', signed=False))
            conn.send(frame.get_data())

    except Exception as e:
        print(e)


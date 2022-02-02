import win32event
import win32api
import win32gui
import os
import sys
import numpy as np
import socket

Type = 0
Slice = 0
TotalSize = 786432
NBlocks = 192
BlockLength = 512
BlockSize = 4096
FirstBlock = 0

nlinesperframe = 192

BlocksToDiscard = (FirstBlock % nlinesperframe)
if(BlocksToDiscard != 0):
    BlocksToDiscard = nlinesperframe-BlocksToDiscard

BlocksToLoad = NBlocks - BlocksToDiscard
BlocksToLoad = BlocksToLoad - (BlocksToLoad % nlinesperframe)

class ModulaConnect :
    def __init__(self):
        self.initEvent = win32event.CreateEvent(None,True,False,"UO2ML_INIT_EVENT")
        win32event.ResetEvent(self.initEvent)

    def launch(self):
        if win32api.ShellExecute(None, None, os.getcwd() + "/ModulaLauncher.exe", "", ".", 1) <= 32 :
            print("Could not launch Modula", file=sys.stderr)
            exit(-1)

        if win32event.WaitForSingleObject(self.initEvent, 5000):
            print("Modula launch timmed out", file=sys.stderr)
            exit(-2)

        self.win = win32gui.FindWindow(None, "ULA-OP Modula - RealTime Software")
        self.msg = win32gui.RegisterWindowMessage("ML2UOCMD")
        win32gui.PostMessage(self.win, self.msg, 2, 65536)

    def next_acq(self):
        win32gui.PostMessage(self.win, self.msg, 2, 65536)
        win32gui.PostMessage(self.win, self.msg, 0, 0)
        hEvent = win32event.OpenEvent(win32event.SYNCHRONIZE | win32event.EVENT_MODIFY_STATE, 0, "UO2ML_SAVE_EVENT")
        win32event.ResetEvent(hEvent)

        if win32event.WaitForSingleObject(hEvent, 5000):
            print("Save event timed out", file=sys.stderr)
            return

        win32api.CloseHandle(hEvent)

    def get_b_mode(self):
        try :
            with open("C:/Users/fgaits/ULA-OP2/MatLink_000_SliceIQ.uob", "rb") as f:
                f.seek(BlockSize * BlocksToDiscard, 0)
                data = np.fromfile(f, count=BlocksToLoad * BlockLength * 2, dtype=np.int32)

            a = []
            for i in range(0, len(data), 2):
                a.append((data[i]) + 1j * data[i + 1])

            c = np.array(a)
            d = np.abs(np.squeeze(np.reshape(c, (BlockLength, nlinesperframe, -1), order='F')))
            m = (np.max(d))
            b = 20 * np.log10(d / m)
            b[b < -50] = -50
            b = (b + 50) * 255 / 50
            b = b.astype('B')
            b = np.stack((b,) * 3, axis=-1)

            return b

        except Exception as e:
            print(e)
            return np.zeros((512,192,3))

    def close(self):
        win32gui.PostMessage(self.win, self.msg, 10, 0)


print("Connecting to Modula...")
mc = ModulaConnect()
print("Lauching app...")
mc.launch()
print("App Launched")

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
            print(a)
            mc.next_acq()
            frame = mc.get_b_mode().tobytes()

            size = len(frame)
            print(size)

            conn.send(size.to_bytes(4, byteorder='little', signed=False))
            conn.send(frame)

    except Exception as e:
        print(e)

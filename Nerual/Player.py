import math
import time

def Vector3Add(v1, v2):
    return [v1[0]+v2[0], v1[1]+v2[1], v1[2]+v2[2]]


def Vector3Scale(v1, arg):
    return [v1[0] * arg, v1[1] * arg, v1[2] * arg]



def LEFT(Angle):
    Angle += 1 * time.process_time()
    return Angle

def RIGHT(Angle):
    Angle -= 1 * time.process_time()
    return Angle

def UP(Position,Vel,Ax,Angle,isrunning):
    isrunning = bool(not isunning)
    if isrunning:
        Ax[0] = 5 * math.sin(Angle)
        Ax[2] = 5 * math.cos(Angle)
    else:
        Ax[0] = -Vel[0] * 3
        Ax[2] = -Vel[2] * 3
        if abs(Vel[0]) < 0.1:
            Vel[0] = 0
            Ax[0] = 0
        if abs(Vel[2]) < 0.1:
            Vel[2] = 0
            Ax[2] = 0
    Vel = Vector3Add(Vel, Vector3Scale(Ax, time.process_time()))
    Position = Vector3Add(Position, Vector3Scale(Vel, time.process_time()))
    
         





    
    
        

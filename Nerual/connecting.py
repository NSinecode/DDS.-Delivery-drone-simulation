import zmq

context = zmq.Context()
print("Connecting to hello world server…")
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5555")
points=[]
    
def Received():
    points.append(recMessage)
    return points
        
def SendMess(sendMessage):
    socket.send(sendMessage)

for request in range(100000000):
    socket.send(b"")
    recMessage = socket.recv()
    print(recMessage)
    Received()


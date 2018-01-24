import socket
import socketserver
import struct
import threading

# A thread which spawns a TCP server. This thread will receive trajectories on TCP port 1234
# until the application exits. 
class ReceiveThread(threading.Thread):

    def __init__(self, parent_window):
        super(ReceiveThread, self).__init__()

        self.parent_window = parent_window

        # Ensure this thead dies when the application exits
        self.setDaemon(True)

    def run(self):

        host = ''
        port = 1234

        # internally this sets socket.SO_REUSEADDR, preventing errors from
        # half-closed TCP sockets
        socketserver.TCPServer.allow_reuse_address = True

        server = socketserver.TCPServer((host, port), ReceiveHandler)

        # Attach parent_window to the server object, so the ReceiveHandler can find it
        server.parent_window = self.parent_window

        print('Listening on port {}...'.format(port))

        # serve requests forever
        server.serve_forever()

# Request handler for new TCP connections. Receives a single trajectory packet,
# posts it to the parent Qt window, then dies.
class ReceiveHandler(socketserver.BaseRequestHandler):

    def handle(self):

        # Read the length int (number of points in this trajectory)
        len = struct.unpack('!I', self.request.recv(4, socket.MSG_WAITALL))[0]

        # Unpack the trajectory into groups of points
        x = []
        y = []

        # Unpack the points. Note that doubles are interpreted in machine-dependent form.
        # (unfortunately there is not a reliable way to encode floating-point values in a known form in C)
        for i in range(len): 
            point = struct.unpack('dd', self.request.recv(16, socket.MSG_WAITALL))
            x.append(point[0])
            y.append(point[1])

        # Post the received trajectory to the main Qt window
        self.server.parent_window.on_receive_trajectory(x,y)

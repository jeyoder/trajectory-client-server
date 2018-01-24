#!/usr/bin/python3

import sys
from PyQt5 import QtCore, QtWidgets

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure

import receive_thread

# Main Qt widget, a window that displays received trajectories in an embedded matplotlib plot.
# This class uses the Qt signals and slots mechanism to synchronize events coming from external threads
class Window(QtWidgets.QWidget):

    receive_trajectory = QtCore.pyqtSignal(list, list)
    
    def __init__(self, parent=None):
        super(Window, self).__init__(parent)

        self.figure = Figure()
        self.canvas = FigureCanvas(self.figure)

        layout = QtWidgets.QVBoxLayout()
        layout.addWidget(self.canvas)

        self.setLayout(layout)
        self.setWindowTitle("Trajectory Receiver")

        self.receive_trajectory.connect(self._on_receive_trajectory)
    
    # Update the plot with a new trajectory. Callable from an external thread. 
    # x : list of x coordinates for the new trajectory
    # y : list of y coordinates for the new trajectory
    #
    # This method is a stub that emits a Qt signal, which is routed to _on_receive_trajectory. 
    def on_receive_trajectory(self, x, y):
        self.receive_trajectory.emit(x,y)

    # A PyQt slot which receives the internal receive_trajectory signal.
    # Qt ensures that this method is called on the UI thread.
    @QtCore.pyqtSlot(list, list)
    def _on_receive_trajectory(self, x, y):
        print('Received a trajectory with {} points'.format(len(x)))
        
        self.figure.clear()

        ax = self.figure.add_subplot(111)
        ax.plot(x, y)

        self.canvas.draw()

if __name__ == '__main__':

    app = QtWidgets.QApplication(sys.argv)
    w = Window()
    w.show()

    thread = receive_thread.ReceiveThread(w)
    thread.start()

    sys.exit(app.exec_())

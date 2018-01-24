# trajectory-client-server

## Interface definition

The receiver ("server") and generator ("client") communicate using the TCP protocol.

The server listens and accepts connections on port 1234. The client will send trajectory information serialized to the following format (integer values are big-endian, doubles are machine-dependent):

struct trajectory {

    uint32_t num_points;
    double coords [num_points * 2];

}

The client can send multiple trajectories, and multiple clients can be connected at once.

## Client (Trajectory generator)

The trajectory sender is a linux C application.

To build: 

~~~~
cd generator
make
~~~~

To use:

~~~~
cd generator
./trajectory_generator [hostname]
~~~~

## Server (Trajectory receiver)

The trajectory receiver is a Python 3 GUI app using PyQt5 and matplotlib. It listens on port 1234, and displays new trajectories as they are received. 

To install dependencies:

~~~~
cd receiver
pip3 install -r requirements.txt
~~~~

To run:

~~~~
cd receiver
python3 trajectory-receiver.py
~~~~

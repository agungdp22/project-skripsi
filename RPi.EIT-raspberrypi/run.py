#!/usr/bin/env python3
from config import *
from konekArduino.I2C import I2C
from socketIO_client import SocketIO, LoggingNamespace

socketIO = SocketIO(host, port, LoggingNamespace)
arduino = I2C(address)

# main function
if __name__ == '__main__':
	socketIO.emit('raspiConnect', {'status': True})
	print ("Listening server...")
	socketIO.wait()
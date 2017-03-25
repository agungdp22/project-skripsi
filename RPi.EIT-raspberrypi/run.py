#!/usr/bin/env python3
from config import *
# from konekArduino.I2C import I2C
from socketIO_client import SocketIO, LoggingNamespace

import numpy as np
from random import uniform

socketIO = SocketIO(host, port, LoggingNamespace)
# arduino = I2C(address)

def getData(*args):
	dataVolt = []
	for i in range(208):
		dataVolt.append(uniform(0.001,0.05))
	print(dataVolt)
	socketIO.emit('postDataVoltage',dataVolt)

# main function
if __name__ == '__main__':
	socketIO.emit('raspiConnect', {'status': True})
	print ("Listening server...")
	socketIO.on('getDataVoltage', getData)
	socketIO.wait()
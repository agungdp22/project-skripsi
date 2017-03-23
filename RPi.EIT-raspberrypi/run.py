#!/usr/bin/env python3
from config import *
from socketIO_client import SocketIO, LoggingNamespace

socketIO = SocketIO(host, port, LoggingNamespace)

# main function
if __name__ == '__main__':
	socketIO.emit('raspiConnect', {'status': True})
	print ("Listening server...")
	socketIO.wait()
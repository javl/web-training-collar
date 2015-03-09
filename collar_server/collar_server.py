#! /usr/bin/env python
import os, sys, serial, commands, time

from flask import Flask
app = Flask(__name__)
app.debug  = True
ser = None

@app.route('/cmd/<cmd_id>/')
def page(cmd_id):
	if cmd_id == "0":
		return "Ignore zero"
	else:
		print "Send out number %s" % str(cmd_id)
		# Commands are formatted like this: <10>
		# inclusing the < and > characters
		ser.write("<%s>" % str(cmd_id))
		# Output the command sent to the browser window
		return "Cmd %s sent" % cmd_id

def main():
	global ser
	try:
		# Find Arduino on OSX
		status, address = commands.getstatusoutput('ls /dev | grep tty.usbmodem')
		if address == "":
			# Find Arduino on Linux
			status, address = commands.getstatusoutput('ls /dev | grep ttyUSB')
			if address == "":
		 		exit("No Arduino found...")
		ser = serial.Serial("/dev/"+address, 4800)
		print ser
		app.run(host='0.0.0.0', port=8080, use_reloader=False)

	except KeyboardInterrupt:
		ser.close()
	except Exception:
		ser.close()
		sys.exit(0)

if __name__ == "__main__":
	main()

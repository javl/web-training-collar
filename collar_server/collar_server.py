#! /usr/bin/env python
import os, sys, serial, commands

from flask import Flask
app = Flask(__name__)
app.debug = True

# Create a global object for the serial connection so
# all functions can access it
ser = None

# Catch calls to 0.0.0.0:8080/cmd/<number>
@app.route('/cmd/<cmd_id>/')
def page(cmd_id):
	global ser
	# Had some trouble with weird zeros coming in, so ignore those
	# commands start at 1
	if cmd_id == "0":
		return "Ignored zero"
	else:
		print "Send out number %s" % str(cmd_id)
		# Commands are formatted like this: <10>
		# including the < and > characters
		ser.write("<%s>" % str(cmd_id))
		# Also output the command to the browser window
		return "Cmd %s sent" % cmd_id

# The main function of the script
def main():
	global ser
	try:
		# Find Arduino on OSX
		status, address = commands.getstatusoutput('ls /dev | grep tty.usbmodem')
		if address == "":
			# Else, find Arduino on Linux
			status, address = commands.getstatusoutput('ls /dev | grep ttyUSB')
			if address == "":
		 		exit("No Arduino found...")
		# Open a serial connection to the Arduino with a baudrate of 4800
		ser = serial.Serial("/dev/"+address, 4800)
		print ser
		# Start the local server
		app.run(host='0.0.0.0', port=8080, use_reloader=False)
	
	# Close the serial connection before quiting when receiving
	# ctrl+c or on an uncaught exception.
	except KeyboardInterrupt:
		ser.close()
	except Exception:
		ser.close()
		sys.exit(0)

# Start the main function if this script was called directly
if __name__ == "__main__":
	main()

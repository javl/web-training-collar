#! /usr/bin/env python
# The Flask server for the Web Training Collar
# More info at http://github.com/javl/webUserTrainingCollar

import os, sys, serial, commands, argparse
from argparse import RawTextHelpFormatter
from flask import Flask
import sqlite3 as lite
import time, struct

#===========================================================
# Handle arguments
#===========================================================
PARSER = argparse.ArgumentParser(prog='collar', description='''''',
formatter_class=RawTextHelpFormatter)
# If --no-arduino is found, set use_arduino to false
PARSER.add_argument('-n', '--no-arduino', dest='use_arduino', action='store_false', help="Run server without \
	Arduino (for testing purposes)")
PARSER.add_argument('-v', '--verbose', dest="verbose", action='count', help="Verbose")
PARSER.add_argument('-s', '--silent', dest='silent', action='store_true', help="Don't use the beep sound")
PARSER.add_argument('-f', '--friendly', dest='friendly', action='store_true', help="If set, you always get a \
	mild shock instead of more painful ones with every visit.")
PARSER.add_argument('-d', '--drop-database', dest="drop_database", action='store_true',\
help="Drop the database.")
PARSER.add_argument('--version', action='version', version='%(prog)s version 1.0',\
help="Show program's version number and exit")
ARGS = PARSER.parse_args()

# Global objects
ser = None # Serial connection
last_shock = 0; # Keep track of last shock time
app = Flask(__name__) # Server object
app.debug = False
if ARGS.verbose > 0: app.debug = True

#=======================================================
# Initialize database
#=======================================================
def init_db():
  if ARGS.verbose > 0: print "Checking database sites.db"

  con = lite.connect('sites.db')
  with con:
    cur = con.cursor()
    # Delete database if requested
    if ARGS.drop_database:
      if raw_input("Drop the existing database? (y/n) [n] ") == 'y':
        cur.execute('DROP TABLE IF EXISTS entries')

    #=======================================================
    # Create the database if it doesn't exist yet
    #=======================================================
    cur.execute('CREATE TABLE IF NOT EXISTS "main"."entries" \
      ("id" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , \
      "hashed_hostname" TEXT, \
      "visits" INT)')

#===========================================================
# Catch requests to 127.0.0.1:3001/shock
#===========================================================
@app.route('/shock')
def simpleShock():
	if ARGS.verbose > 1: print "Incoming shock"
	global ser

	if not ARGS.friendly:
		if ARGS.verbose > 0:
			print "The /shock endpoint only offers a friendly shock"

	if ARGS.use_arduino:
		if ARGS.silent == False:
			ser.write(struct.pack('B',0)) # Send beep (unless in silent mode)
		ser.write(struct.pack('B', 8)) # Shock 2
	return 'success'

#===========================================================
# Catch requests to 127.0.0.1:3001/host/<hashedhostname>/
#===========================================================
@app.route('/host/<int:hashed_hostname>/')
def extendedShock(hashed_hostname):
	global ser
	hashed_hostname = str(hashed_hostname)
	if ARGS.verbose > 0: print "Incoming hashed hostname: %s" % hashed_hostname

	# Open the database connection and see if the hashed_hostname is already in there
	# If not, vibrate as a first-visit warning and add it to the database
	# If already there, check the number of previous visits and shock the user accordingly
	con = lite.connect('sites.db')
	with con:
		cur = con.cursor()
		cur.execute("SELECT * from entries WHERE hashed_hostname=?", (hashed_hostname,))
		result = cur.fetchone()
		if not result:
			cur.execute("INSERT INTO entries ('hashed_hostname', 'visits') VALUES(?, ?)", (hashed_hostname, 1))
			con.commit()

			if ARGS.verbose > 0: print "New entry, warning only"
			if ARGS.use_arduino:
				ser.write(struct.pack('B',4)) # Send vibrate
				if ARGS.silent == False:
					ser.write(struct.pack('B',0)) # Send beep (unless in silent mode)
		else:
			# Limit the amount of shock commands to one every ten seconds
			# prevents getting a lot of shocks when a page loads multiple objects
			global last_shock
			current_time = int(time.time())
			if current_time - last_shock > 10:
				last_shock = current_time
				if ARGS.friendly:
					ser.write(struct.pack('B', 8)) # Shock 2
				else:
					visits_before = result[2]
					if ARGS.use_arduino:
						if visits_before == 1:
							ser.write(struct.pack('B', 7)) # Shock 1
						elif visits_before == 2:
							ser.write(struct.pack('B', 11)) # Shock 5
						elif visits_before == 3:
							ser.write(struct.pack('B', 16)) # Shock 10
						elif visits_before == 4:
							ser.write(struct.pack('B', 20)) # Shock 50
						elif visits_before >= 5:
							ser.write(struct.pack('B', 25)) # Shock 100

				cur.execute("UPDATE entries SET visits=visits+1 WHERE hashed_hostname=?", (hashed_hostname,))
				con.commit()

	return "success" # Send some output to the browser

#===========================================================
# Main function
#===========================================================
def main():
	global ser

	if not ARGS.friendly:
		init_db()

	try:
		# Try some different Arduino paths
		if ARGS.use_arduino:
			# Find Arduino on Linux, name type 1
			status, address = commands.getstatusoutput('ls /dev | grep ttyUSB')
			if address == "":
				# Find Arduino on Linux, name type 2
				status, address = commands.getstatusoutput('ls /dev | grep ttyACM0')
				if address == "":
					# Find Arduino on OSX, name type 1
					status, address = commands.getstatusoutput('ls /dev | grep tty.usbserial-')
					if address == "":
						# Find Arduino on OSX, name type 2
						status, address = commands.getstatusoutput('ls /dev | grep tty.usbmodem')
						if address == "":
							exit("No Arduino found. Use the -n parameter to run without, or -h for help");

			# Open a serial connection to the Arduino with a baudrate of 4800
			if ARGS.verbose > 0: print "Arduino found at %s" % address
			ser = serial.Serial("/dev/"+address, 4800)
			if ARGS.verbose > 0: print ser

		# Start the local server
		if ARGS.verbose > 0: print "Starting server, press ctrl+c to quit"
		app.run(host='127.0.0.1', port=3001, use_reloader=False)

	# Close the serial connection before quiting
	except (KeyboardInterrupt, Exception) as e:
		print e.message
		if ARGS.use_arduino: ser.close()
		sys.exit(0)

#===========================================================
# Start the main function if this script was called directly
#===========================================================
if __name__ == "__main__":
	main()

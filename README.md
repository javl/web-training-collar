# Collar

1: Install the Firefox extension: in Firefox use 'file > open' 
and select the collar.xpi file found in collar/browser_plugin/firefox/
Restart Firefox when the installation is done.

2: Plug the transmitter into a USB port

3: Start the Flask Python server. Open a terminal window (OSX users;
use Spotlight to open the Terminal application) and navigate to the
collar folder. If you've put it in your downloads folder, this would
be something like:
    cd ~/Downloads/collar/collar_server
And start the server with:
    python collar.py

4: Put the two AAA batteries into the collar module. You might
want to wait closing it up until you've tested the connection

5: To test the connection, open up any browser and navigate to:
    http://0.0.0.0:8080/cmd/2/
If the connection works you will hear a short beep from the collar,
meaning the transmitter and receiver have been paired. Reloading the
page should result in an annoying beep from the transmitter. Visiting
    http://0.0.0.0:8080/cmd/3/
should make the collar vibrate.

6: You are now ready to use the collar. Close the collar and screw
the metal end terminals back on if not already done so.
Now, put the collar around your arm (or where ever you want) and try
visiting some websites to find out which websites have a valid SSL 
certificate and which don't.

# System
The installed browser plugin checks if the connection to websites you
visit is made over a protected HTTPS, or unprotected HTTP connection.
If a HTTP connection is used, it tells the local server by requesting
a URL including a number that tells the server what command to send
to the collar (the numbers 2 and 3 in step 5 above).
The server than sends this command over a Serial connection (via the
USB cable) to an Arduino Nano which in turn sends the signal to a
small 433Mhz radio transmitter - the same type of radio transmitter
used by the official remote control. The collar receives this signal
and will beep, vibrate or send out a shock. 

Browser plugin -> GET request to server
server -> sends serial command to collar


# Privacy
The plugin checks every website you visit to see if it's using a
HTTP or HTTPS connection. It does not look at the contents of any
website and nothing is stored, neither online nor offline.

# Health risks
The collar will give the wearer an electrostatic shock. Seeing as
it doesn't kill dogs, I think it should be harmless for humans
as well - especially on the lowest setting - but I could be wrong.
So please do be carefull when using one of these collars.

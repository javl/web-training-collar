# Web Training Collar

Voor Nederlands, zie LEESMIJ.md

See [jaspervanloenen.com](http://www.jaspervanloenen.com) for more info.

## Introduction


A lot of websites still only offer an unencrypted (HTTP) connection to their visitors. The communication between the computer of the visitor and the server hosting the website is open and can easily be intercepted by others. Possible attackers can see anything the user is seeing: text, images, links clicked, etc. Especially on open or public WIFI networks there is always the change of someone looking at your internet usage.

The Web Training Collar was made to solve this problem in a four easy steps:

1. Connect the Web Training Collar Control Unit to one of your computer's available USB ports.
2. Install the browser extension and the (free) piece of software that comes with the Web Training Collar.
3. Put the Web Training Collar around your arm, or anywhere else on your body where it can sit comfortably for longer periods of time.
4. You can now safely browse the web.

## How It Works
The Web Training Collar follows one simple principle: if website owners don't offer an encrypted (HTTPS) connection to their websites, visitors should refrain from visiting those websites. This behavior can easily be achieved by means of conditioning. This is where the Web Training collar shines: every time you visit a website over an unencrypted connection, you will get a warning. The Web Training Collar will vibrate and you will hear a beep (optional: the sound can be disabled for more discrete use in public places). The next time you visit the website, the Web Training Collar will administer an electrostatic shock. The intensity of this shock will increase with every consecutive visit of the website.

Optionally you can enable a more friendly mode where the power of the shock will not increase, but in this mode the warning vibration on a first visit will also be replaced by a shock.

## Project Background
The Web Training Collar was built for medialab [Setup](http://www.setup.nl)'s "Controlegroep" (control group) project. The 25 participants of the Controlegroep have set up experiments to see if and how their behavior can be monitored or altered with the help of apps and gadgets.

## Technical Information
Every time you visit a website the browser extension checks the protocol used. If the protocol is not HTTPS or ABOUT (used by somebrowsers), it sends a hashed version of the website's hostname to a local Flask server running in Python.

This server stores a reference to all visited websites by saving the hashes in a Sqlite3 database, together with a counter to keep track of the number of times a specific website has been visited. Depending on this number, it will send a signal to an attached Arduino Nano over a serial connection. The Arduino code holds a list of all of the radio commands for the collar and sends the appropriate command to the collar using a small 433Mhz transmitter.

# Privacy
The browser extension translates every website you visit over an HTTP connection into a numeric hash which gets stored in a Sqlite3 database on your local machine. Nothing is saved on or transmitted to external servers.

# Health risks
The collar used was meant for dogs of 18+ kilos. Seeing as it doesn't kill them I think it should be harmless to humans as well - especially on the lowest setting - but I might be wrong.

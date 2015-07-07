# Web trainings halsband

For English, see README.md

## Introductie

Zie [jaspervanloenen.com](http://www.jaspervanloenen.com) voor meer info.

Veel websites bieden hun gebruikers nog steeds alleen een onbeveiligde (HTTP) verbinding, wat betekent dat de communicatie tussen de computer van de gebruiker en de server van de website gemakkelijk af te luisteren is door derden. Kwaadwillenden kunnen met gemak de inhoud van websites zien: tekst, afbeeldingen, aangeklikte links, etc. Vooral op openbare WIFI netwerken (zoals 'Wifi in de trein') is er een reële kans dat er iemand met je meekijkt.

De Web trainings halsband is gemaakt om dit probleem op te lossen in vier simpele stappen:

1. Plug de Web trainings halsband controle eenheid in vrije USB poort van je computer met behulp van de bijgeleverde USB kabel.

2. Installeer de bijgeleverde browser extensie en de (gratis) software op je PC.

3. Doe de Web trainings halsband eenheid rond je arm of een ander lichaamsdeel waar deze voor langere tijd comfortabel kan blijven zitten..

4. Je kan nu veilig het web op.

## Hoe het werk

De Web trainings halsband werkt volgens een simpel principe: als website eigenaren geen beveiligde (HTTPS) verbinding aanbieden, dan moet de gebruiker zich- of haarzelf aanleren deze website te vermijden. Dit gedrag kan men zich makkelijk aanleren via conditionering, het gebied waar de Web gebruiker trainings halsband in uitblinkt.

Elke keer dat je een website bezoekt via een onveilige verbinding krijg je een waarschuwing. De halsband zal trillen en er klinkt een pieptoon (optioneel: het geluid kan worden uitgeschakeld voor discreet gebruik in de openbare ruimte). De volgende keer dat de gebruiker weer op de website komt krijgt deze een elektrostatische schok. De intensiteit van deze schok wordt hoger bij elk volgend bezoek aan de website.
Optioneel is er ook een meer vriendelijke modus aanwezig. In de vriendelijke modus zal de kracht van de schok niet hoger worden, maar de trilfunctie als waarschuwing bij een eerste bezoek zal ook vervangen worden door een schok.

## Project achtergrond

De Web trainings halsband is gemaakt voor medialab [Setup](http://www.setup.nl)'s "Controlegroep" project. De 25 deelnemers van de Controlegroep hebben allemaal een experiment opgezet om te onderzoeken in hoeverre ons gedrag gemonitord en bijgestuurd kan worden met behulp van apps en gadgets.

## Technische informatie

Elke keer dat je een website bezoekt controlleerd de browser extensie het gebruikte protocol. Wanneer dit niet HTTPS of ABOUT is (wordt door sommige browsers gebruikt) wordt er een hash van de website hostname verzonden naar een lokale Flask server die in Python draait.

Deze server bewaard een lijst van bezochte websites door de hash op te slaan in een Sqlite3 database, samen met een teller die aangeeft hoe vaak de site bezocht is. Afhankelijk van de hoogte van deze teller stuurt de server over een seriele verbinding een signaal naar een Arduino Nano. Deze Arduino heeft een lijst met alle mogelijke commando's die de halsband begrijpt en stuurt het benodigde commando naar de halsband met behulp van een kleine 433Mhz radiozender.

# Privacy

De browser extensie vertaald elke website die bezocht wordt over een HTTP verbinding naar een hash die lokaal opgeslagen wordt in een Sqlite3 database. Er wordt niets verzonden naar of opgeslagen op externe servers.

# Gezondheidsrisico's

De gebruikte halsband is bedoelt voor honden van 18+ kilo. Aangezien deze niet dood gaan van de schokken leek het mij ook veilig voor een mensen - zeker op de laagste stand - maar ik kan het fout hebben.

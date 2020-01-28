# Challenge-D3D

## Hoe gebruik je het project
Er zijn meerdere branches:<br/>
master -> Permanente branch, hierin staat alleen werkende code<br/>
2-ontwerp-site -> Deze branch is voor het designen van de webapp<br/>
10-API -> Hierin is de API ontwikkeld <i>Merged</i><br/>
Database -> Hier is de creatiecode van de db geschreven <i>Merged</i><br/>
ESP -> Hier wordt gewerkt aan de LED-BaroMeter, de main code die op de esp staat is ESP_BaroMeter<br/>


## Raspberry Pi opzetten 
### Raspbian lite
- Download raspbian lite van de officiele raspberry pi site
- pak het bestand uit zodat je een .img bestand hebt ipv .zip
- installeer raspbian op de micro-sd kaart met behulp van een progamma als win32diskimager
- stel de wifi in door de installatie hiervoor het config bestand in de boot schijf te plaatsen
- voeg een leeg bestand genaamd "ssh" op de boot schijf toe om ssh aan te zetten

### MariaDB
- $ sudo apt-get install mariadb-server
- $ sudo mysql
- grant toegang voor een andere gebruiker dan root op localhost  
  - // TODO: command toevoegen

### Apache2
Door de onderstaande commando's uit te voeren op de pi configureer je de apache2 server
- $ sudo apt-get install apache2
- $ sudo apt-get install php
- $ sudo a2enmod php7.3
- $ sudo vim etc/php/7.3/apache2/php.ini
  - /mysqli
    - Dit commando zoekt mysql in het bestand, ga met n naar de volgende tot je de volgende regel tegenkomt:
    - ;extension=mysqli
  - haal de puntkomma weg
  - :wq
- $ sudo service apache2 restart

### Java
- Download de java jdk van de oracle site
  - https://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html
  - Linux ARM 32 Hard Float ABI valt aan te raden
- Plaats het gedownloadde bestand in de /tmp directory op de pi (aangeraden tool is WinSCP)
- Voer het volgende commando uit vanuit /etc/ om de jdk te installeren (aangeraden tool is putty)
  - $ sudo tar -xvzf [bestandsnaam].tar.gz
  - De bestandsnaam is waarschijnlijk het volgende: jdk-8u241-linux-arm32-vfp-hflt
- Als volgende stap gaan we ~/.bashrc aanpassen door deze regel toe te voegen:
  - PATH=$PATH:/etc/[naam jdk directory]/bin
  - De naam van de jdk directory is waarschijnlijk dit: jdk1.8.0_241

## How to setup ESP NodeMCU
1.  Download de ESP_BaroMeter.ino code<br/>
2.  Open File/Preferences/Additional Boards Manager URL (Paste): http://arduino.esp8266.com/stable/package_esp8266com_index.json<br/>
3.  Sketch/Include Library/Manage Libraries/"ESP8266"
4.  Tools/Board/"Adaruit Feather HUZZAH ESP8266"<br/>
5.  Install Moquitto (on raspberry or Windows)<br/>
6.  Open CMD/Terminal<br/>
7.  C:\Program Files\mosquitto<br/>
8.  $mosquitto install    // Mosquitto install command<br/>
9.  $net start mosquitto  // Mosquitto service start command<br/>
10. Open port 1883   // MQTT port<br/>
11. mosquitto_pub -h [IP address] -m "[Message here]" -t esp/slimmemeter // Publish to topic<br/>
12. mosquitto_sub -h [IP address] -t esp/slimmemeter // Subscripe to topic<br/>
13. $cd path/to/your/app<br/>
14. $php -S localhost:8000<br/>

Documentatie:<br/>
- http://www.bytesofgigabytes.com/mqtt/installing-mqtt-broker-on-windows/<br/>
- http://www.steves-internet-guide.com/mosquitto_pub-sub-clients/<br/>

## Hoe gebruik je git
### Git commando's
git clone [ssh download link] -> to clone new repository<br/>
git status -> geeft welke bestanden bewerkt zijn<br/>
git add [bestandsnaam] -> voegt bestanden toe voor de commit<br/>
git commit -> vul commit message in met de aanvulling op de zin: deze commit zal...<br/>
git commit -a -m "[message]" -> commit alles en je kan gelijk een message toevoegen<br/>
git push -> pusht je lokale commits naar het internet<br/>
git checkout [branchnaam] -> verander van branch<br/>
git checkout -b [branchnaam] -> maakt nieuwe branch waar je direct heengaat<br/>
git merge [branchnaam] -> vanuit de branch waar je naartoe wil gaan. ALLEEN MET MASTER MERGEN NA MASTER IN JE EIGEN BRANCH TE MERGEN<br/>
git pull -> pulls current branch from github<br/>

### Documentatie merge conflicten
https://help.github.com/en/github/collaborating-with-issues-and-pull-requests/resolving-a-merge-conflict-using-the-command-line
  
Lees vooral de reacties van git, deze geven meestal het command dat je nodig hebt stel iets werkt niet. Ook kan je met tab regels automatisch aan laten vullen.<br/>

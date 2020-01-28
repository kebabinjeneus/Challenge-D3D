# Challenge-D3D

## Hoe gebruik je het project
Er zijn meerdere branches:<br/>
master -> Permanente branch, product is klaar staat<br/>
2-ontwerp-site -> Hier wordt gewerkt aan de website<br/>
10-API -> Hier wordt gewerkt aan de API<br/>
Database -> Hier wordt gewerkt aan de database<br/>
ESP -> Hier wordt gewerkt aan de LED-BaroMeter, de main code die op de esp staat is ESP_BaroMeter<br/>

## Hoe gebruik je git
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

## Merge Conflicts Documentatie
https://help.github.com/en/github/collaborating-with-issues-and-pull-requests/resolving-a-merge-conflict-using-the-command-line
  
Lees vooral de reacties van git, deze geven meestal het command dat je nodig hebt stel iets werkt niet. Ook kan je met tab regels automatisch aan laten vullen.<br/>

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

post_req.php file moet geplaatst worden in de folder /var/www/html/php/<br/>>

Documentatie:<br/>
- http://www.bytesofgigabytes.com/mqtt/installing-mqtt-broker-on-windows/<br/>
- http://www.steves-internet-guide.com/mosquitto_pub-sub-clients/<br/>

## How to setup Raspberry Pi with Raspbian lite


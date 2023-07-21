# Computer Physik Blatt 1 | Arto Steffan | 27.04.23

## __Aufgabe 1__
### Teil a
Betrieb System, die ich kenne:
1. Windows : Besitzerprogramm, entw. von Micrasoft
2. Linux : General Public liscence v2
3. MacOS : aus Darwin (heritiert aus UNIX->BSD), für MAC, entwikelt von Apple
4. iOS :   aus MacOs, mobil Betzriebsystem, proprietär, entwikelt von Apple
5. Android : modif. Linux Kern, mobil Betzriebsystem, open-source unter APACHE Liscence

Sie Bieten alle die folgenden Möglichkeiten:
- Speicherung
- Software pakete für 
  - text Dokumenten
  - Bild Visualisierung
  - Audio- und Videowiedergabe

Für Windows, MacOs: Spracherkennung

### Teil b
Linux distribution = ein Betriebsystem, das einen Linux Kern besitzt.
Bekannter Linux Distribution sind:
- Ubuntu
- Redhat
- Debian
- CentOS

Note: **Quelle**: https://fr.wikipedia.org/wiki/Linux#Utilisations

### Teil c
Möglichen Hilfsquellen:
- -h oder --help in der Shell
- Wikipedia
- Chat GPT (info verifizieren)
  
### Teil d 
Über meinen Rechner:
- Betriebsystem : Windows 10
- CPU : Intel i7 @2.8Ghz 8 Kern
- GPU : Intel Iris Xe Graphics
- RAM : 8Go
- SDD 512 Gb
  
### Teil e
Vorteile von Shell statt GUI:
- Schnellere Bearbeitung der Kommande
- Schellere Laden wg weniger Elemente (keienen Bild, ...)
- Scheller um kommande zu tippen, als eine Tast auf die UI suchen
- Bieten mehr möglichkeiten im gleichen Ort als UI. z.B:
  - Servers und Ferncomputer zugriff
  - Umgebung Konfiguration

## __Aufgabe 2__

### Teil a | Befehle
List der wichtigsten Befehle für einen UNIX-artig Betriebsysteme
 - ```ls``` : **Inhalt** einer Ordner anzeigen ("list"). Optionen:
   - ```ls -l``` : Zusätzliche Infos zur Dateien
 - ```cd (+ pfad)``` : In einer **andere Ordner wechseln** ("Change directory"). Optionen:
   - ```cd ..``` : die Hierarchie um eine Ebene nach oben wechseln 
   - ```cd ~ ```: Zurück zur pers. Hierachie navigieren
- ```pwd``` : Zeigt die Namme des laufendes Ordner ("Print Working Directory")
- ```mkdir (+name)```: Neues **Ordner erstellen** ("Make Directory"). Optionen:
  - ```mkdir [path]/name``` : in spec. Ordner
  - ```mkdir -p [path/name_1] [path/name_2]``` ... : mehrere Ordner erstellen
- ```cp [source] [destination]``` : Den Inhalt einer Ordner **kopieren**
- ```mv [source] [destination]``` : Den Ihalt einer Ordner zu **verschieben**. **Unebenen** falls gleichen Pfad
- ```rm [target] ```: Einen Datei zu **löschen**
- ```grep [text] [target]``` : Text in einer Datei **suchen**
- ```ps``` : **Laufenden Prozess** zu zeigen
- ``` cat [target]``` : Zeigt der Inhalt eines Text-Datei an ("Concatenate").
- ``` cat [targetA] [targetB]``` : Fast den inhalt von zwei text Datei zusammen.
  - ``` cat [targetA] [targetB] > [InTargetC]```

### Teil b | Operatoren
Es existiert dazu folgenden Operatoren:
- ``` > ``` Schreibt den Ergebnis einer Befehle in einer Text Datei. Ersetzt den Inhalt des Targets 
- ``` >> ``` Schreibt den Ergebnis einer Befehle in einer Text Datei.
- ``` | ``` "Pipe" : Output von der Befehle links ist als input der Befehle rechts behandelt.


#### Anwendungsmöglichkeiten:
- ```[Befehle] > [target]``` Bsp: ```ls > text.txt``` schreibt den Inhalt des lauenden Orderns in der Datei ```text.txt``` (!!überschreibt den Inhalt!!). ```text.txt``` wird ergestellt falls es nicht existiert.
- ```[Befehle] >> [target]``` Bsp: ```ls >> text.txt``` ist analog aber schreibt am Ende von (oder erstellt) ```text.txt```.
- ```[Befehle A] | [Befehle B]```. Bsp: ``` ls | sort > text.txt ``` Den Inhalt des laufenen Orders wird alphabetisch sortiert ``` sort ``` und in ``` text.tx ``` gespeichert. 

### Teil c | Weiteren Text Kommando
Man kann die folgenden Befehle benutzen
-``` wc text.txt``` Ergibt die Anzahl an Wörter, Linien und Charackter von einer ```Text.txt```. Z.B. für den Datei Inhal
```
-
-Le petit oiseau gazouille dans la prairie mais pas trop sinon une vache va le manger.
```
ergibt es sich 2 16 87. Also 2 Linien, 16 Wörter und 87 Charakter.

Nun benutzen wir den Befehl ```file [filename]``` um das Datentyp eins Daten zu bestimmen.
```
text.txt: ASCII text
```
Unsere ```text.txt``` ist eine text Datei mit ASCII Charakter.


### Teil d | Prozessen 
Zehn wichtigen Elementen der Befehl ```top``` :
- *n* user: Anzahl von Benutzer, die eingeloggt sind. 
- Task: Alle Prozessen, die auf den CPU laufen.
  - ```running```: Prozessen, die den CPU aktiv nutzend
  - ```sleeping```: Prozessen, die registieret sind aber die gerade nicht de CPU benutzen.
  - ```stopped```: Prozessen, die ausgeschlatet waren.
  - ```zombie```: gestorben Prozessen mit falsche Ameldung
- PID: ID des Prozess, um den zu tracken
- USER: Auf welche Benutzer läuft den Prozess
- PR: Die Priorität des Prozess
- NI: Die Güte der Kernel-Behandlung des Prozess 
- S: Der Status des Prozess:
  - D: Uniterruptible Sleep
  - R: Running
  - S: Sleeping
  - T: Traced =^ Stopped
  - Z: Zombie
- VIRT: Anzahl an benutzte virtuelle Memory
- RES: Anzahl an verbleibende Memory (resident)
- SHR: Anzahl an geteilte Memory (shared)
- %CPU: den vom Prozess gebrauchte Teil von der CPU Leistung
- %MEM: die Anteil der RAM, die vom Prozess benutzt ist
- TIME+: Gebrauchte Zeit vom CPU für dieses Prozess in (1/100) von Sekunde
- COMMAND: Namme der Befehl

### Teil e | scp
Die Befehl ```scp [option] [source] [target]``` (secured copy) wird benutzt um ein Datei zwischen zwei Rechner zu kopieren. Haupt. mittels ein SSH.
Beispiele zur Optionen:
- -p: port number. Portnummer für den SSH. Standart 22.
- -r: recurlsively. Kopiert die die Kinderordner rekursiv
- -q: Fortschrittsinformationen
- -v: verbose. Detaillierte Log vom Prozess

Ich habe es auf BART geflaufen lassen: ```scp main.c a.c``` hat den Datei ```main.c``` in den gleichen Ordner als ```a.c``` kopiert.

### Teil f | ssh -x
Die Option ```-X``` ist mit der ```ssh``` nützlich, um sich mit einer graphische Oberfläche ein zuloggen.
Wir haben: ```-ssh -X asteffan@login.mittelerde.physik.uni-konstanz.de ```. Es gibt leider keine native graphische Oberfläche mit BART

### Teil g | gnuplott
Um Funktionen zu plotten kann man den folgende Befehle benutzen
```
gnuplot
plot [-5:5] sin(x)
```
Lädt leider unendlich lang mit BART.

### Teil h | Berechtigungen
Das Befehl ```chmod [option] [rights] [target] ``` kann man Benutzer erlauben zu lesen, schreiben oder den Datei ausführen.
Rights: 
- Kominbation von Ziffern:  xyz
  - 4: lesen
  - 2: schreiben
  - 1: ausführen
- *oder* Expliziter:
  - r: lesen
  - w: schreiben
  - x: ausführen
>Full rights: rwx , Nor rights: ---
  
Ziele:
- u: den benutzer
- g: den Gruppe
- o: anderen Benutzer
- a: alle Benutzer

Optionen:
- -R: rekursiv auf alle Kindordner
- -v, -c, -f: Verbose
  
Beispiel:
```
chmod u=rw,g=r,o=r text.txt 
```
Hier kann den Benutzer lesen und schreiben, der Gruppe und die anderebn können nur schreiben.

*Zu finden*: ```chmod``` mit ```ls -l```

### Teil i | df und du
Nutzung der Partitionen ```df [options] [partition]```. Ohne Eingabe in ```[partition]``` werden alle detektierte Partition angezeigen.
Optionen:
- h: lesbare Größeneinheiten (Mb, kb, ..)
- a: zeigt auch Dateisysteme an, die nicht eingehängt sind 

Größe des Odrners mit ```df [options] [folder]``` Ohne Eingabe in ```[folder]``` wird die Größe des aktuelen Folder gezeigen.
- h: lesbare Größeneinheiten (Mb, kb, ..)
- a: auch verschtecktes Ordner betrachten 
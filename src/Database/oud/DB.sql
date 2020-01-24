CREATE SCHEMA Kosten;

CREATE TABLE Energiemaatschappij (
    kvknummer INT(8) NOT NULL,
    naam VARCHAR(64) NOT NULL,
    PRIMARY KEY (kvknummer)
); 

CREATE TABLE Pakket (
    pakketnummer INT(4) NOT NULL,
    naam VARCHAR(64) NOT NULL,
    energiemaatschappij INT(8) NOT NULL,
    maandduur INT(2) NOT NULL,
    dagtarief DOUBLE(1 , 2 ) NOT NULL,
    nachttarief DOUBLE(1 , 2 ),
    gastarief DOUBLE(2 , 2 ),
    vastrecht DOUBLE(3 , 2 ) NOT NULL,
    PRIMARY KEY (pakketnummer),
    FOREIGN KEY (energiemaatschappij)
        REFERENCES energiemaatschappij (kvknummer)
        ON DELETE NO ACTION ON UPDATE CASCADE
); 
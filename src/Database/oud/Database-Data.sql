Create  schema Gegevens ;

Create table Huidigverbruik 

(Tijd       timestamp     not null,           

Huidigverbruik double(2,3)    not null, 

Primary key (tijd) 

);

CREATE TABLE Uurverbruik (
    Tijd TIMESTAMP NOT NULL,
    MeterstandDagtarief DOUBLE(6 , 3 ) NOT NULL,
    MeterstandNachttarief DOUBLE(6 , 3 ) NOT NULL,
    PRIMARY KEY (Datum , Tijd)
);

CREATE TABLE Dagverbruik (
    Tijd TIMESTAMP NOT NULL,
    MeterstandDagtarief DOUBLE(6 , 3 ) NOT NULL,
    MeterstandNachttarief DOUBLE(6 , 3 ) NOT NULL,
    PRIMARY KEY (Datum)
);

CREATE TABLE Gas (
    Tijd TIMESTAMP NOT NULL,
    Gasverbruik DOUBLE(4 , 2 ) NOT NULL,
    PRIMARY KEY (Datum , Tijd)
); 

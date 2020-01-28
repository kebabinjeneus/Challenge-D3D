CREATE SCHEMA BewustE;

CREATE TABLE verbruik (
	tijd TIMESTAMP NOT NULL,
	huidigVerbruik double(5, 3) NOT NULL,
	totVerbruikLaag double(9, 3) NOT NULL,
	totVerbruikHoog double(9,3) NOT NULL,
	gasVerbruik double(8, 3) NOT NULL,
	PRIMARY KEY (tijd)
);

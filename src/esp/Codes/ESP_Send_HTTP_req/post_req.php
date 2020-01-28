<?php
/*
 * php post request in json format
 * Auteur: Appie Strorman
 * Project: The Challenge
 * Group:   D3D
 * Name:    ESP BaroMeter
 * Date:    1/28/2020
 * Version: V1.2
*/

// Credentials for db
$dbhost = "localhost";
$dbuser = "bewust";
$dbpass = "yoloww";
$dbname = "meterData";

// Create and check connection
$conn = new mysqli($dbhost, $dbuser, $dbpass, $dbname) or die("ERROR: could not connect to the database!");


// Run query that will select all from table verbruik, only get the latest data
$query = "SELECT * FROM verbruik ORDER BY tijd DESC LIMIT 1;";
$result = $conn->query($query);

// Fetch all data from json table in associative array format and store in $result variable
$array = $result->fetch_assoc();

// Now encode PHP array in JSON string 
$json = json_encode($array, true);

//test the json string
//var_dump($json);
echo $json;

// //create file if not exists
// $fo=fopen("myjson.json","w");

// //write the json string in file
// $fr=fwrite($fo,$json);

// Close connection
$conn->close();
?>

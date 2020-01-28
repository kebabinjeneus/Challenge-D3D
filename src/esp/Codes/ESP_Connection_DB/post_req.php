<?php
    // Credentials for db
    $servername = "192.168.2.6/php/";
    $dbusername = "bewust";
    $dbpassword = "yoloww";
    $dbname = "meterData";

    if($_SERVER["REQUEST_METHOD"] == "POST"){
        $tijd = test_input($_POST["tijd"]);
        $huidigVerbruik = test_input($_POST["huidigVerbruik"]);
        $totVerbruikLaag = test_input($_POST["totVerbruikLaag"]);
        $totVerbruikHoog = test_input($_POST["totVerbruikHoog"]);
        $gasVerbruik = test_input($_POST["gasVerbruik"]);
            
        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        
        // Check connection
        if($conn->connect_error){
            die("Connection failed: " . $conn->connect_error);
        }
        $conn->close();
    }
    else{
        echo "No data posted with HTTP POST.";
    }
?>
<?php
session_start(); // Starting Session
$error=''; // Variable To Store Error Message
if (isset($_POST['submit'])) {
if (empty($_POST['gebruikersnaam']) || empty($_POST['wachtwoord'])) {
	$error = "Username or Password is invalid";
}
else {
	// Define $username and $password
	$username=$_POST['gebruikersnaam'];
	$password=$_POST['wachtwoord'];

	if ($username == 'admin') {
		if($password == 'admin') {
			$_SESSION['login_user'] = $username; // Initializing Session
			header("location: index.php"); // Redirecting To Other Page
		} else {
			$error = "Username or Password is invalid";
		}
	}
}
?>

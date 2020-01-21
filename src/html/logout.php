<?php
session_start();
if(session_destroy()) // Destroying All Sessions
{
header("Location: loginForm.php"); // Redirecting To Home Page
}
?>
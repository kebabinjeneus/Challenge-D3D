<?php 
	if(!isset$_SESSION['login_user']){
		header("Location: loginForm.php");
	} else {
		include("index.htm");
	}
?>
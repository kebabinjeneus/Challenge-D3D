<?php
include('login.php'); // Includes Login Script

if(isset($_SESSION['login_user'])){
header("location: profile.php");
}
?>
<!DOCTYPE html>
<html>
    <head>
        <title>Bewust-E</title>
        <link rel="stylesheet" type="text/css" href="css/signin.css">
    </head>

    <body>
        <div class= "signup";>
            <h2> Login </h2>
            <hr>
            
			<span><?php echo $error; ?></span>
            <label for="gebruikersnaam"><b>Gebruikersnaam: </b></label>
            <input id="gebruikersnaam" type="text" placeholder="Gebruikersnaam" name="gebruikersnaam" required>

            <label for="wachtwoord"><b>Wachtwoord</b></label>
            <input id="wachtwoord" type="password" placeholder="********" name="wachtwoord" required>
            
            <label>
            <!--<input type="checkbox" checked="checked" name="remember" style="margin-bottom:15px"> Remember me-->
            </label>
            
            <div class="options">
                <!--<button type="button" class="cancelbtn" onClick="parent.location='index.htm'">Cancel</button>-->
                <!--<button type="submit" class="loginbtn" >Inloggen</button>-->
                <!--<button type="button" class="signupbtnLogin" onClick="parent.location='signUpForm.htm'">Sign Up</button>-->
				<input name="submit" type="submit" value=" Login ">
            </div>
        </div>
    </body>
</html>
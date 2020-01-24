<?php
$server="localhost";
$username="root";   //THE DEFAULT USERNAME OF THE DATABASE
$password="";
$dbname="kosten";
$con=mysqli_connect($server,$username,$password,$dbname) or die("unable to connect");
$rank=$_GET["RANK"];
$sql="SELECT NAME from class where RANK=$rank"; //WE ARE TRYING TO GET THE NAME OF THE STUDENT BY ENTERING THE RANK
$result=mysqli_query($con,$sql);
if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {    
      echo  $row["NAME"];
    }
}
?>
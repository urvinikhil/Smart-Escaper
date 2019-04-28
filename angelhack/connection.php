<?php 

$connection = mysqli_connect("localhost","root","","codefiler"); //host,username,password

/*if($connection){
		mysql_select_db("codefiler",$connection);
}else{
	echo mysql_error($connection);die;
}*/
if (mysqli_connect_errno())
  {
  echo "Failed to connect to MySQL: " . mysqli_connect_error();
  }
?>

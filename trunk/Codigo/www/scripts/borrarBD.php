<?php
$username="UMixUser";
		$password="umixpass";
		$database="UMix";
		$myFile = "../conf/database";
		$fh = fopen($myFile, 'r');
		$url = fread($fh, filesize($myFile));
		fclose($fh);

		mysql_connect($url,$username,$password);
		@mysql_select_db($database) or die( "Unable to select database");
		$query="DELETE FROM Puntos";
		mysql_query($query);
header('Location: ../aplicacion.php');

?>

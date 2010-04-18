<?
$username="UMixUser";
$password="umixpass";
$database="UMix";

mysql_connect(localhost,$username,$password);
@mysql_select_db($database) or die( "Unable to select database");
$query="DELETE FROM Puntos";

mysql_query($query);

/*
$num=mysql_num_rows($result);
$i=0;
while ($i < $num) {

	$latitud=mysql_result($result,$i,"Latitud");
	$longitud=mysql_result($result,$i,"Longitud");
	$time=mysql_result($result,$i,"Timestamp");
	$encontro=mysql_result($result,$i,"Encontro");
	echo "<b> $latitud $longitud $time $encontro </b>";


$i++;
}
*/
?>

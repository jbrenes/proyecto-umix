<?php

$convertir="/home/ubuntu/UMix/GPSConvert ";
$username="UMixUser";
$password="umixpass";
$database="UMix";
$myFile = "conf/database";
$fh = fopen($myFile, 'r');
$url = fread($fh, filesize($myFile));
fclose($fh);

function parseToXML($htmlStr) 
{ 
$xmlStr=str_replace('<','&lt;',$htmlStr); 
$xmlStr=str_replace('>','&gt;',$xmlStr); 
$xmlStr=str_replace('"','&quot;',$xmlStr); 
$xmlStr=str_replace("'",'&#39;',$xmlStr); 
$xmlStr=str_replace("&",'&amp;',$xmlStr); 
return $xmlStr; 
} 


// Opens a connection to a mySQL server
$connection=mysql_connect ($url, $username, $password);
if (!$connection) {
  die('Not connected : ' . mysql_error());
}

// Set the active mySQL database
$db_selected = mysql_select_db($database, $connection);
if (!$db_selected) {
  die ('Can\'t use db : ' . mysql_error());
}

// Select all the rows in the markers table
$query = "SELECT * FROM Puntos WHERE 1";
$result = mysql_query($query);
if (!$result) {
  die('Invalid query: ' . mysql_error());
}

header("Content-type: text/xml");

// Start XML file, echo parent node
echo '<markers>';

// Iterate through the rows, printing XML nodes for each
while ($row = @mysql_fetch_assoc($result)){
  // ADD TO XML DOCUMENT NODE
  echo '<marker ';
 // echo 'name="' . parseToXML($row['name']) . '" ';
 // echo 'address="' . parseToXML($row['address']) . '" ';
  echo 'lat="' . $row['Latitud'] . '" ';
  echo 'lng="' . $row['Longitud'] . '" ';
  $latlongcon = shell_exec($convertir.$row['Latitud']." ".$row['Longitud']);
  echo 'conv="' . $latlongcon . '" ';
  echo 'encontro="' . $row['Encontro'] . '" ';
  echo '/>';
}

// End XML file
echo '</markers>';

?>

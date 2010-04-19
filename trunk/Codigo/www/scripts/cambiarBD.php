<?php

$myFile = "../conf/database";

$fh = fopen($myFile, 'w') or die("can't open file");
$stringData = $_GET['baseDatos'].'\n';
fwrite($fh, $stringData);
fclose($fh);


header('Location: ../configuracion.php');

?>

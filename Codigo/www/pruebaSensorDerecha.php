<?php
$myFile = "/sys/class/gpio/gpio139/value";
$fh = fopen($myFile, 'r');
$theData = fread($fh, filesize($myFile));
fclose($fh);
echo $theData;

?>

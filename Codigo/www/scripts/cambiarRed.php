
<?php
$file = "/etc/network/interfaces";
$fh = fopen($file, 'w');

$ipadd = $_GET['dirIP'];
$mascara = $_GET['mask'];
$gateway = $_GET['gateway'];
$interfaz = $_GET['interfaz'];
echo "int ".$interfaz;
fwrite($fh , 'auto ' .$interfaz ."\n");
echo "ip ".$ipadd;
fwrite($fh , 'iface ' .$interfaz ." inet static \n");
fwrite($fh, 'address ' .$ipadd ."\n");
echo "mascara ".$mascara;
fwrite($fh, 'netmask ' .$mascara ."\n");
echo "gw ".$gateway;
fwrite($fh, 'gateway ' .$gateway ."\n");
fclose($fh);
echo shell_exec('sudo /etc/init.d/networking restart');
//header('Location: http://' .$ip );

?>
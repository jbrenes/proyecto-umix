<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html><head><meta content="text/html; charset=ISO-8859-1" http-equiv="content-type"><title>apagar.php</title></head><body>&lt;?php<br>

<br>
;<br>

$salida = shell_exec('./app/apagar')
echo $salida;<br>

<br>

?&gt;</body></html>
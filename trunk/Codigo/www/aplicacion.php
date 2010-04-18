<?php
 	require ('xajax_core/xajax.inc.php'); 
	$xajax = new xajax(); 
	$xajax->registerFunction("adelante"); 
	$xajax->registerFunction("atras"); 
	$xajax->registerFunction("derecha"); 
	$xajax->registerFunction("izquierda"); 
	
	$xajax->processRequest();
	function adelante(){
		#$salida = "Adelante";
		$obstaculoDer = "sudo cat /sys/class/gpio/gpio133/value";
		$obstaculoIzq = "sudo cat /sys/class/gpio/gpio183/value";
		$comando = "sudo /var/www/scripts/adelante";	
		shell_exec($comando);
		$resObsDer = shell_exec($obstaculoDer);
		$resObsIzq = shell_exec($obstaculoIzq);	
		$salidaDer="";
		if($resObsDer == "1"){
				$salidaDer='<img src="images/on.jpg">';
		}else{
			$salidaDer='<img src="images/off.jpg">';
		}
		$salidaIzq="";
		if($resObsIzq == "1"){
				$salidaIzq='<img src="images/on.jpg">';
		}else{
			$salidaIzq='<img src="images/off.jpg">';
		}
				
		$salida = $salidaDer.$salidaIzq;
   		$respuesta = new xajaxResponse();
   		$respuesta->assign("respuesta","innerHTML",$salida);
   		return $respuesta;
	}
	function atras(){
		$comando = "sudo /var/www/scripts/atras";	
		$obstaculoDer = "sudo cat /sys/class/gpio/gpio133/value";
		$obstaculoIzq = "sudo cat /sys/class/gpio/gpio183/value";
		#$salida = "Atras";
		
		
		shell_exec($comando);
		$resObsDer = shell_exec($obstaculoDer);
		$resObsIzq = shell_exec($obstaculoIzq);		
		$salidaDer="";
		if($resObsDer == "1"){
				$salidaDer='<img src="images/on.jpg">';
		}else{
			$salidaDer='<img src="images/off.jpg">';
		}
		$salidaIzq="";
		if($resObsIzq == "1"){
				$salidaIzq='<img src="images/on.jpg">';
		}else{
			$salidaIzq='<img src="images/off.jpg">';
		}
				
		$salida = $salidaDer.$salidaIzq;
		$respuesta = new xajaxResponse();
   		$respuesta->assign("respuesta","innerHTML",$salida);
   		return $respuesta;
	}
	function derecha(){
		#$salida = "Derecha";
		$comando = "sudo /var/www/scripts/derecha";	
		$obstaculoDer = "sudo cat /sys/class/gpio/gpio133/value";
		$obstaculoIzq = "sudo cat /sys/class/gpio/gpio183/value";
		#$salida = "Atras";
		shell_exec($comando);
		$resObsDer = shell_exec($obstaculoDer);
		$resObsIzq = shell_exec($obstaculoIzq);		
		$salidaDer="";
		if($resObsDer == "1"){
				$salidaDer='<img src="images/on.jpg">';
		}else{
			$salidaDer='<img src="images/off.jpg">';
		}
		$salidaIzq="";
		if($resObsIzq == "1"){
				$salidaIzq='<img src="images/on.jpg">';
		}else{
			$salidaIzq='<img src="images/off.jpg">';
		}
				
		$salida = $salidaDer.$salidaIzq;
   		$respuesta = new xajaxResponse();
   		$respuesta->assign("respuesta","innerHTML",$salida);
   		return $respuesta;
	}
	function izquierda(){
		#$salida = "Izquierda";
		$comando = "sudo /var/www/scripts/izquierda";	
		$obstaculoDer = "sudo cat /sys/class/gpio/gpio133/value";
		$obstaculoIzq = "sudo cat /sys/class/gpio/gpio183/value";
		#$salida = "Atras";
		shell_exec($comando);
		$resObsDer = shell_exec($obstaculoDer);
		$resObsIzq = shell_exec($obstaculoIzq);		
		$salidaDer="";
		if($resObsDer == "1"){
				$salidaDer='<img src="images/on.jpg">';
		}else{
			$salidaDer='<img src="images/off.jpg">';
		}
		$salidaIzq="";
		if($resObsIzq == "1"){
				$salidaIzq='<img src="images/on.jpg">';
		}else{
			$salidaIzq='<img src="images/off.jpg">';
		}
				
		$salida = $salidaDer.$salidaIzq;
   		$respuesta = new xajaxResponse();
   		$respuesta->assign("respuesta","innerHTML",$salida);
   		return $respuesta;
	}
	
	$xajax->processRequest();
	$xajax_js = $xajax->getJavascript();



?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<?php

$xajax->printJavascript(); 
?>
<meta content="text/html; charset=windows-1250" http-equiv="Content-Type">
<meta content="Microsoft FrontPage 4.0" name="GENERATOR">
<meta content="FrontPage.Editor.Document" name="ProgId">
<title>UMIX</title>
<script type='text/javascript' src="functions.js"></script>
<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAyrdeV_NswcK9b4JhAiG8-hQmGsXG_ntmPmO65ulpWnbiaP5ibxSJkGkNYs5I8TQhcKG7aEQgTumBQQ" type="text/javascript">
</script>

<style type="text/css" media="screen">
@import url(css/bloques.css);
</style>
<link rel="stylesheet" type="text/css" href="style.css">
<style>
BODY {
	scrollbar-arrow-color:094588;
	scrollbar-shadow-color:f0f0f0;
	scrollbar-face-color:f0f0f0;
	scrollbar-highlight-color:094588;
	scrollbar-darkshadow-color:094588;
}
</style>
</head>
<body topmargin="0" leftmargin="0" bottommargin="0" rightmargin="0" onLoad="mostrarMapa()">
<table style="width: 100%;" border="0" cellpadding="0" cellspacing="0">
  <tbody>
    <tr>
      <td style="width: 100%; text-align: left; vertical-align: middle;"><img src="header.JPG" width="972" height="193"> </td>
    </tr>
  </tbody>
</table>
<table background="pozadinatop.gif" bgcolor="#172886" border="0" cellpadding="0" cellspacing="0" width="100%">
  <tbody>
    <tr>
      <td width="100%">&nbsp;</td>
    </tr>
  </tbody>
</table>
<table border="0" cellpadding="0" cellspacing="0" width="118%">
  <tbody>
    <tr>
      <td width="100%"><img src="lijevotop.gif" border="0" height="123" width="400"></td>
    </tr>
  </tbody>
</table>
<table border="0" cellpadding="0" cellspacing="0" width="118%">
  <tbody>
    <tr>
      <td valign="top" width="20%"><table style="background-color: rgb(16, 27, 89); width: 198px;" border="0" cellpadding="0" cellspacing="0">
          <tbody>
            <tr>
              <td colspan="2" width="100%"><img src="lijevo.gif" border="0" height="15" width="198"></td>
            </tr>
            <tr>
              <td width="5%">&nbsp;</td>
              <td width="95%">&nbsp;</td>
            </tr>
            <tr>
              <td align="left" width="5%"><p style="margin-left: 25px;"><img src="tocka.gif" border="0" height="15" width="15"></p></td>
              <td onMouseOver="this.bgColor = '#9FA3BC';" onMouseOut="this.bgColor = '#101B59';" bgcolor=			 										"#101b59" width="95%"><font color="#ffffff" size="2" face="Verdana"><a href="acercade.htm" 						 												class="lijevi">Inicio</a></font></td>
            </tr>
            <tr>
              <td align="left" width="5%">&nbsp;</td>
              <td width="95%">&nbsp;</td>
            </tr>
            <tr>
              <td align="left" width="5%"><p style="margin-left: 25px;"><img src="tocka.gif" border="0" height="15" width="15"></p></td>
              <td onMouseOver="this.bgColor = '#9FA3BC';" onMouseOut="this.bgColor = '#101B59';" bgcolor="#101b59" width="95%"><font color="#ffffff" face="Verdana" size="2"><a href="acercade.htm" class="lijevi">Acerca de UMix</a></font></td>
            </tr>
            <tr>
              <td align="left" width="5%">&nbsp;</td>
              <td width="95%">&nbsp;</td>
            </tr>
            <tr>
              <td align="left" width="5%"><p style="margin-left: 25px;"><img src="tocka.gif" border="0" height="15" width="15"></p></td>
              <td onMouseOver="this.bgColor = '#9FA3BC';" onMouseOut="this.bgColor = '#101B59';" bgcolor="#101b59" width="95%"><font color="#ffffff" face="Verdana" size="2"><a href="servicios.htm" class="lijevi">Servicios</a></font></td>
            </tr>
            <tr>
              <td align="left" width="5%">&nbsp;</td>
              <td width="95%">&nbsp;</td>
            </tr>
            <tr>
              <td align="left" width="5%"><p style="margin-left: 25px;"><img src="tocka.gif" border="0" height="15" width="15"></p></td>
              <td onMouseOver="this.bgColor = '#9FA3BC';" onMouseOut="this.bgColor = '#101B59';" bgcolor="#101b59" width="95%"><font color="#ffffff" face="Verdana" size="2"><a href="soporte.htm" class="lijevi">Soporte</a></font></td>
            </tr>
            <tr>
              <td align="left" width="5%">&nbsp;</td>
              <td width="95%">&nbsp;</td>
            </tr>
            <tr>
              <td align="left" width="5%"><p style="margin-left: 25px;"><img src="tocka.gif" border="0" height="15" width="15"></p></td>
              <td onMouseOver="this.bgColor = '#9FA3BC';" onMouseOut="this.bgColor = '#101B59';" bgcolor="#101b59" width="95%"><font color="#ffffff" face="Verdana" size="2"><a href="faq.htm" class="lijevi">Faq</a></font></td>
            </tr>
            <tr>
              <td align="left" width="5%">&nbsp;</td>
              <td width="95%">&nbsp;</td>
            </tr>
            <tr>
              <td align="left" width="5%"><p style="margin-left: 25px;"><img src="tocka.gif" border="0" height="15" width="15"></p></td>
              <td onMouseOver="this.bgColor = '#9FA3BC';" onMouseOut="this.bgColor = '#101B59';" bgcolor="#101b59" width="95%"><font color="#ffffff" face="Verdana" size="2"><a href="contacto.htm" class="lijevi">Contacto</a></font></td>
            </tr>
            <tr>
              <td width="5%">&nbsp;</td>
              <td width="95%">&nbsp;</td>
            </tr>
            <tr>
              <td style="height: 18px;" align="left" width="5%"><p style="margin-left: 25px;"><img src="tocka.gif" border="0" height="15" width="15"></p></td>
              <td onMouseOver="this.bgColor = '#9FA3BC';" onMouseOut="this.bgColor = '#101B59';" bgcolor="#101b59" width="95%"><font color="#ffffff" face="Verdana" size="2"><a href="aplicacion.php" class="lijevi">Aplicaci&#242;n</a></font></td>
            </tr>
            <tr>
              <td style="height: 15px;" width="5%">&nbsp;</td>
              <td style="height: 15px;" width="95%">&nbsp;</td>
            </tr>
            <tr>
              <td style="height: 18px;" align="left" width="5%"><p style="margin-left: 25px;"><img src="tocka.gif" border="0" height="15" width="15"></p></td>
              <td onMouseOver="this.bgColor = '#9FA3BC';" onMouseOut="this.bgColor = '#101B59';" bgcolor="#101b59" width="95%"><font color="#ffffff" face="Verdana" size="2"><a href="debug.php" class="lijevi">Debug</a></font></td>
            </tr>
            <tr>
              <td style="height: 15px;" width="5%">&nbsp;</td>
              <td style="height: 15px;" width="95%">&nbsp;</td>
            </tr>
            <tr>
              <td style="height: 18px;" align="left" width="5%"><p style="margin-left: 25px;"><img src="tocka.gif" border="0" height="15" width="15"></p></td>
              <td onMouseOver="this.bgColor = '#9FA3BC';" onMouseOut="this.bgColor = '#101B59';" bgcolor="#101b59" width="95%"><font color="#ffffff" size="2" face="Verdana">Links</font></td>
            </tr>
            <tr>
              <td style="height: 15px;" width="5%">&nbsp;</td>
              <td style="height: 15px;" width="95%">&nbsp;</td>
            </tr>
            <tr>
              <td style="height: 18px;" align="left" width="5%"><p style="margin-left: 25px;"><img src="tocka.gif" border="0" height="15" width="15"></p></td>
              <td onMouseOver="this.bgColor = '#9FA3BC';" onMouseOut="this.bgColor = '#101B59';" bgcolor="#101b59" width="95%"><font color="#ffffff" size="2" face="Verdana"><a href="configuracion.php" class="lijevi">Configuración</a></font></td>
            </tr>
            <tr>
              <td style="height: 78px;" width="5%">&nbsp;</td>
              <td style="height: 78px;" width="95%">&nbsp;</td>
            </tr>
            <tr>
              <td width="30%">&nbsp;
                <p>&nbsp;</p>
                <p>&nbsp;</p></td>
              <td width="80%">&nbsp;
                <p>&nbsp;</p>
                <p>&nbsp;</p>
                <p>&nbsp;</p>
                <p>&nbsp;</p></td>
            </tr>
            <tr>
              <td colspan="2" width="100%"><img src="lijevo.gif" border="0" height="15" width="198"></td>
            </tr>
          </tbody>
        </table></td>
      <td valign="top" width="80%"><table width="682" height="338" border="0" cellpadding="0" cellspacing="0">
          <tbody>
            <tr>
              <td width="100%" height="338"><p style="margin-left: 30px;">
                <div id="map2"> </div>
                
                <div id="botonesMovimiento">
                  <table width="140" >
                    <tr>
                      <td colspan="2" align="center"> Controles del móvil </td>
                    </tr>
                    <tr>
                      <td  align="center" colspan="2"><form method="post" action="">
                          <img src="images/adelante.JPG" onClick="xajax_adelante()" >
                        </form></td>
                    </tr>
                    <tr>
                      <td width="64" align="right"><form method="post" action="">
                          <img src="images/izquierda.JPG" onClick="xajax_izquierda()" >
                        </form></td>
                      <td width="64" align="left"><form method="post" action="">
                          <img src="images/derecha.JPG" onClick="xajax_derecha()" >
                        </form></td>
                    </tr>
                    <tr>
                      <td align="center" colspan="2"><form method="post" action="">
                          <img src="images/atras.JPG" onClick="xajax_atras()" >
                        </form></td>
                    </tr>
                  </table>
                </div>
                <div id="botonesBD">
                  <table width="140" >
                    <tr>
                      <td colspan="2" align="center"><p>Controles de la Base de Datos</p></td>
                    </tr>
                    <tr>
                      <td  align="center" ><form method="post" action="scripts/borrarBD.php">
                          <input type="image" src="images/trash.jpg"  >
                        </form></td>
                    </tr>
                  </table>
                </div>
                <p style="margin-left: 30px; margin-right: 30px; margin-top: 0pt;"><br>
                </p>
                <p>&nbsp;</p>
                <p>&nbsp;</p>
                <p>
                <div id="respuesta"> </div>
 
              </p></td>
            </tr>
          </tbody>
        </table>
        <p>&nbsp;</p>
        <p style="margin-left: 10px;" align="right"><a href="http://www.rickyswebtemplates.com"><img src="rickysanimationsmall.gif" border="0" height="56" width="140"></a></p>
        <p style="margin-left: 10px;" align="right">Design
          by<a href="http://www.rickyswebtemplates.com">www.rickyswebtemplates.com</a></p></td>
    </tr>
  </tbody>
</table>
<center>
  <span style="width: 100%; font-family: helvetica; font-size: 6px;">Design downloaded from Zeroweb.org<br>
  <a href="http://www.zeroweb.org" style="font-family: helvetica; font-size: 6px;">Web
  templates, layouts, and website tools for FREE!</a><br>
  <a href="http://urlsnip.com" style="font-family: helvetica; font-size: 6px;">Shorten URL
  services</a><br>
  </span>
</center>
<br>
<br>
</body>
</html>

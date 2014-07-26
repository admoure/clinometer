<?php
	$us = $_POST['usuario'];
	$cl = $_POST['clave'];
	$mius = "svvcog";
	$miclave = "svv-teide";
	if ($us==$mius && $cl==$miclave)
	{
		echo "estupendo todo";
		header ("Location: index2.php");
	}
	else
	{
		echo "nanai";
	}
?>

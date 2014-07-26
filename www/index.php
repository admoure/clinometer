<http>
<head>

</head>
<body>


<form action="index.php" method="post">
	Usuario:<input type="text" name="usuario">
	Pass: <input type="password" name="clave">
	<input type="submit" name="enviao" value="envio">
</form>

<?php
	global $clave_ok;

?>



<?php
	//funcion para descarga de fichero
	function download_file($archiv)
	{
		header('Content-Type: application/octet-stream');
		header('Content-Disposition: attachment; filename="'.basename($archiv).'"');
		header('Content-Length: ' . filesize($archiv));
		header('Content-Transfer-Encoding: binary');
		header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
		header('Pragma: public');

		ob_clean();
		flush();
		readfile($archiv);
		exit;
	}
?>
<?php
	function lee_config_file()
	{
		global $ch1,$ch2,$ch3,$muest,$names,$gain,$filter,$lat,$lon,$alt,$nserie;
		global $usar_servidor,$server,$port,$folder;
		$BASE_PI = "/home/pi/soft_inclinometro/";
		$cf = fopen($BASE_PI."configura.txt","r");
		fscanf($cf,"NAME=%s",$names);
		fscanf($cf,"NSERIAL=%s",$nserie);
		fscanf($cf,"CHANNEL1=%s",$ch1);
		fscanf($cf,"CHANNEL2=%s",$ch2);
		fscanf($cf,"CHANNEL3=%s",$ch3);
		fscanf($cf,"MUESTREO=%s",$muest);
		fscanf($cf,"HIGHGAIN=%s",$gain);
		fscanf($cf,"FILTER=%s",$filter);

		fscanf($cf,"LAT=%s",$lat);
		fscanf($cf,"LON=%s",$lon);
		fscanf($cf,"ALT=%s",$alt);

		fscanf($cf,"SERVERCHECK=%s",$usar_servidor);
		fscanf($cf,"SERVER=%s",$server);
		fscanf($cf,"PORT=%s",$port);
		fscanf($cf,"FOLDER=%s",$folder);

		fclose($cf);

	}
?>

<?
	global $Xmax,$Xmin,$Ymax,$Ymin;
	
	if (isset($_POST['rango']))
	{
		$scale = fopen("/home/pi/soft_inclinometro/XYscale.txt","w");
		$Xmax=$_POST['xmax'];
		$Xmin=$_POST['xmin'];
		$Ymax=$_POST['ymax'];
		$Ymin=$_POST['ymin'];
		$tdia=$_POST['mdeys'];
		fwrite($scale,"Xmax=$Xmax\n");
		fwrite($scale,"Xmin=$Xmin\n");
		fwrite($scale,"Ymax=$Ymax\n");
		fwrite($scale,"Ymin=$Ymin\n");
		fwrite($scale,"Tdia=$tdia\n");
		fclose($scale);
		shell_exec("sudo /var/www/incli_graph_local.sh");
		
	}
	
	$scale = fopen("/home/pi/soft_inclinometro/XYscale.txt","r");
	
	$Xmaxi=fgets($scale);
	$Xmini=fgets($scale);
	$Ymaxi=fgets($scale);
	$Ymini=fgets($scale);
	$Tdey=fgets($scale);
	fclose($scale);
	
	$tok = strtok($Xmaxi,"=");
	$Xmax=strtok("=");
	$tok = strtok($Xmini,"=");
	$Xmin=strtok("=");
	$tok = strtok($Ymaxi,"=");
	$Ymax=strtok("=");
	$tok = strtok($Ymini,"=");
	$Ymin=strtok("=");
	$tok = strtok($Tdey,"=");
	$tdia = strtok("=");
	
	
?>


<?php
	//Mira todos los ficheros de un directorio y los mete en un vector

	global $minx,$miny,$mint,$maxx,$maxy,$maxt,$currentx,$currenty,$currentt,$fecha;
	global $anio,$mes,$dia,$hora,$minuto,$segundo;
	global $difx,$dify,$dift;
	global $anno,$mese;


	if (isset($_POST['selecciona']))
	{
		$anno = $_POST['anios'];
		$mese = $_POST['meses'];
		$mese = str_pad($mese,2,"0",STR_PAD_LEFT);
		//$mese = strtolower($mese);
		//echo $mese;
		$aux = fopen("/var/www/auxiliar.txt","w");
		fwrite($aux, "$anno\n");
		fwrite($aux, "$mese\n");
		fclose($cf);
	}
	/*else
	{
		$anno = date("Y");
		$mese = date("M");
		$mese = strtolower($mese);
		$aux = fopen("/var/www/auxiliar.txt","w");
		fwrite($aux, "$anno\n");
		fwrite($aux, "$mese\n");
		fclose($cf);
	}*/
	$aux = fopen("/var/www/auxiliar.txt","r");
	$anno = trim(fgets($aux));
	$mese = trim(fgets($aux));
	fclose($aux);


	$BASE = "/media/CAM/datalogger/inclinometro/$anno/$mese/";
	//echo $BASE;
	$directorio = opendir($BASE);
	while ($archivo = readdir($directorio))
	{
		if (!is_dir($archivo))
		{
			$lista[] = $archivo;
		}
	}
	sort($lista);

	//cuando se pulsa el boton de envio se coge el nombre del fichero y se llama a la 
	//funcion de descarga

	if (isset($_POST['envio']))
	{
		$name = $_POST['ficheros'];
		$fich=$BASE.$name;
		download_file($fich);
	}
	//AHORA LEE EL FICHERO DE CONFIGURACIÓN PARA VER QUE ES LO QUE ESTA CHECKED O NO
	lee_config_file();


if (isset($_POST['config']))
{
	$cf = fopen("/home/pi/soft_inclinometro/configura.txt","w");

	if (!empty($_POST['check1']))
	{
		$ch1="ON";
	}
	else
	{
		$ch1="0FF";
	}
	if (!empty($_POST['check2']))
	{
		$ch2="ON";
	}
	else
	{
		$ch2="OFF";
	}
	if (!empty($_POST['check3']))
	{
		$ch3="ON";
	}
	else
	{
		$ch3="OFF";
	}
	if (!empty($_POST['check4']))
	{
		$gain="ON";
	}
	else
	{
		$gain="OFF";
	}
	if (!empty($_POST['check5']))
	{
		$filter="ON";
	}
	else
	{
		$filter="OFF";
	}
	if (!empty($_POST['server_check']))
	{
		$usar_servidor="ON";
	}
	else
	{
		$usar_servidor="OFF";
	}
	$muest=$_POST['muestreo'];
	$names=$_POST['nombre'];
	$nserie=$_POST['serial'];
	$lat=$_POST['latitud'];
	$lon=$_POST['longitud'];
	$alt=$_POST['altitud'];
	$server=$_POST['server'];
	$port=$_POST['port'];
	$folder=$_POST['folder'];
	
	fwrite($cf, "NAME=$names\nNSERIAL=$nserie\nCHANNEL1=$ch1\nCHANNEL2=$ch2\nCHANNEL3=$ch3\nMUESTREO=$muest\n");
	fwrite($cf, "HIGHGAIN=$gain\nFILTER=$filter\nLAT=$lat\nLON=$lon\nALT=$alt\n");
	fwrite($cf, "SERVERCHECK=$usar_servidor\nSERVER=$server\nPORT=$port\nFOLDER=$folder\n");
	fclose($cf);
}

	date_default_timezone_set('UTC');
	$today = "c".date("ymd").".txt";
	$yearg = date("Y");
	
	$monthl = date("m");
	$monthl = str_pad($monthl,2,"0",STR_PAD_LEFT);
	//$monthl=strtolower($monthl);
	
	$file_curso = fopen("/media/CAM/datalogger/inclinometro/$yearg/$monthl/".$today,"r");

	$current_line = fgets($file_curso);

	$tok = strtok($current_line," ");
	$anio = $tok;
	$tok = strtok(" ");
	$mes = $tok;
	$tok = strtok(" ");
	$dia = $tok;
	$tok = strtok(" ");
	$hora = $tok;
	$tok = strtok(" ");
	$minuto = $tok;
	$tok = strtok(" ");
	$segundo = $tok;
	$tok = strtok(" ");
	$minx = $tok;
	$tok = strtok(" ");
	$miny = $tok;
	$tok = strtok(" ");
	$mint = $tok;


	$maxx = $minx;
	$maxy = $miny;
	$maxt = $mint;

	while($current_line = fgets($file_curso))
	{
		$tok = strtok($current_line," ");
		$anio = $tok;
		$tok = strtok(" ");
		$mes = $tok;
		$tok = strtok(" ");
		$dia = $tok;
		$tok = strtok(" ");
		$hora = $tok;
		$tok = strtok(" ");
		$minuto = $tok;
		$tok = strtok(" ");
		$segundo = $tok;
		$tok = strtok(" ");
		$currentx = $tok;
		$tok = strtok(" ");
		$currenty = $tok;
		$tok = strtok(" ");
		$currentt = $tok;
		//echo "$equis $ys $temp a las $hora:$minuto:$segundo<br>";
		if($currentx < $minx)
		{
			$minx = $currentx;
		}
		if($currentx > $maxx)
		{
			$maxx = $currentx;
		}
		if($currenty < $miny)
		{
			$miny = $currenty;
		}
		if($currenty > $maxy)
		{
			$maxy = $currenty;
		}
		if($currentt < $mint)
		{
			$mint = $currentt;
		}
		if($currentt > $maxt)
		{
			$maxt = $currentt;
		}



	}
	fclose($file_curso);

	$dffx=abs($maxx-$minx);
	$dffy=abs($maxy-$miny);
	$dfft=abs($maxt-$mint);
	$difx=number_format($dffx,2);
	$dify=number_format($dffy,2);
	$dift=number_format($dfft,2);
	
	if(isset($_POST['apaga']))
	{
		if(isset($_POST['apaga_check']))
		{
			shell_exec("sudo /sbin/shutdown -h now");
			
		}
	}
?>



<?	if(isset($_POST['enviao']))
	{
		$us = $_POST['usuario'];
		$cl = $_POST['clave'];
		$mius = "svvogc";
		$miclave = "svv-teide";
		if($us == $mius && $cl == $miclave)
		{
?>





<form method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">

	<center>
	<?php
	echo "<h1>INCLIN&Oacute;METRO (".$names.")</h1>";
	?>
	<div style="float:left;width:100%">
	<iframe src="./xy.html" width=1024 height=500 frameborder=0 scrolling=no>
	</iframe>
	</div>
	
	<div style="float:left;width:100%">
		<div style="float:left;width:20%">
			<p> </p>
		</div>
		<div style="float:left;width:48%">
			<div style="float:left;width:45%">
				<h3 align=right>Rango</h3>
				<p align=right><input type=submit name=rango value=Cambiar></p>
			</div>
			<div style="float:right;width:48%">
				<p align="left">X max <input type=text size=4 name=xmax value=<?echo $Xmax;?>>  Y max <input type=text size=4 name=ymax value=<?echo $Ymax;?>></p>
				<p align="left">X min <input type=text size=4 name=xmin value=<?echo $Xmin;?>>  Y min <input type=text size=4 name=ymin value=<?echo $Ymin;?>></p>
				<p align="left">Dias <input type=text size=4 name=mdeys value=<?echo $tdia;?>></p>
			</div>
			
		</div>
	</div>
	

	<div style="float:left;width:100%">
	<iframe src="./temp.html" width=1024 height=500 frameborder=0 scrolling=no>
	</iframe>
	</div>
<?	
/*
	<div style="loat:left;width:100%">
		<div style="float:left;width:20%">
		<p> </p>
		</div>
		<div style="float:left;width:48%">
			<div style="float:left;width:45%">
				<h3 align=right>D&iacute;as</h3>
				<p align=right><input type=submit name=deys value=Cambiar></p>
			</div>
			<div style="float:right;width:45%">
				<p align="left">&Uacute;ltimos <input type=text size=4 name=mdeys value=<?echo $diaas;?>>dias</p>
				
			</div>
			
		</div>
	</div>
	*/
?>
	
	<div style="float:left;width:100%">
	<iframe src="./multi.html" width=1024 height=300 frameborder=0 scrolling=no>
	</iframe>
	</div>

	<div style="float:left;width:100%">
	<h2>Archivos diarios</h2>


	<select size="1" name="anios">

		<?php
			$paranios=date("Y") - 1;
			for($j=0;$j<3;$j++)
			{
				if ($anno==$paranios+$j)
				{
					echo "<option selected>".($paranios+$j)."</option><br>";
				}
				else
				{
					echo "<option>".($paranios+$j)."</option><br>";
				}
			}

		?>
	</select>

	<select size="1" name="meses">
	<?php

		for($j=1;$j<13;$j++)
		{
			if ($mese == $j)
			{
				echo "<option selected>".$j."</option><br>";

			}
			else
			{
				echo "<option>".$j."</option><br>";
			}
		}

	?>
	</select>

	<input type="submit" name="selecciona" value="Select">

	<br>
	

	
	<select size="1" name="ficheros">

	<?php
		for ($i=0;$i<count($lista);$i++)
		{
			echo "<option>".$lista[$i]."</option><br>";
		}

	?>

	</select>
	<input type="submit" name="envio" value="Descarga">
	
	</center>
	</div>
	<div style="float:left;width:100%">
		<div style="float:left;width:48%">
			<h2 align="center">Configuraci&oacute;n</h2>
		</div>
		<div style="float:right:width:48%">
			<h2 align="center">Informaci&oacute;n</h2>
		</div>
	</div>
	<div style="width:100%">
		<div style="float:left; width:8%">
			<p> </p>
		</div>
		<div style="border-style:solid; border-color:#CCCCCC; border-width:thin; width:35%;float:left">
			<div style="width:100%; float:left">
				<div style="width:45%;float:left">
					<p align="right">Nombre de la estaci&oacute;n:</p>
				</div>
				<div style="width:45%;float:right">
					<p aling="left">
						<?php
							echo "<input type="."text"." size=15 name=nombre value=$names align=left>"
						?>
					</p>
				</div>
			</div>
			<div style="width:100%; float:left">
				<div style="width:45%;float:left">
					<p align="right">Sensor S/N:</p>
				</div>
				<div style="width:45%;float:right">
					<p aling="left">
						<?php
							echo "<input type="."text"." size=15 name=serial value=$nserie align=left>"
						?>
					</p>
				</div>
			</div>
			<br>
			<div style="width:100%; float:left">
				<div style="width:45%;float:left">
					<br>
					<p align="right">Selecci&oacute;n de canales: </p>
				</div>
				<div style="width:45%;float:right">
					<p aling="left">
						<?php

							if ($ch1=="ON")
							{
								echo "<input type="."checkbox"." name="."check1"." checked>CANAL 1<br>";
							}
							else
							{
								echo "<input type="."checkbox"." name="."check1"." >CANAL 1<br>";

							}
							if ($ch2=="ON")
							{
								echo "<input type="."checkbox"." name="."check2"." checked>CANAL 2<br>";
							}
							else
							{
								echo "<input type="."checkbox"." name="."check2"." >CANAL 2<br>";

							}
							if ($ch3=="ON")
							{
								echo "<input type="."checkbox"." name="."check3"." checked>CANAL 3<br>";
							}
							else
							{
								echo "<input type="."checkbox"." name="."check3"." >CANAL 3<br>";

							}
						?>
					</p>
				</div>
			</div>
			<div style="width:100%; float:left">

				<div style="float:left;width:45%">
					<p align="right">Muestreo (en minutos):</p>
				</div>
				<div style="float:right;width:45%">
					<p align="left">
						<?php
							echo "<input type=text size=15 name=muestreo value=$muest><br>";
						?>
					</p>
				</div>
			</div>
			<br>
			<div style="float:left;width:45%">
				<p align="center">
					<?php
						if($gain=="ON")
						{
							echo "<input type=checkbox name=check4 checked>HIGH GAIN<br>";
						}
						else
						{
							echo "<input type=checkbox name=check4>HIGH GAIN<br>";
						}
					?>
				</p>
			</div>
			<div style="float:right;width:45%">
				<p align="center">
					<?php
						if($filter=="ON")
						{
							echo "<input type=checkbox name=check5 checked>FILTER<br>";
						}
						else
						{
							echo "<input type=checkbox name=check5>FILTER<br>";
						}
					?>
				</p>
			</div>
			<div style="float:left;width:100%;height:30px">
				<div style="float:left;width:45%">
					<p align="center"><b>Localizaci&oacute;n</b></p>
				</div>
			</div>
			<div style="float:left;width:100%;height:30px">
				<div style="float:left;width:45%">
					<p align="center">LAT</p>
				</div>
				<div style="float:right;width:45%">
					<p align="left">
					<?php
						echo "<input type=text size=15 name=latitud value=$lat>";
					?>
					</p>
				</div>
			</div>
			<div style="float:left;width:100%;height:30px">

				<div style="float:left;width:45%">
					<p align="center">LON</p>
				</div>
				<div style="float:right;width:45%">
					<p align="left">
					<?php
						echo "<input type=text size=15 name=longitud value=$lon>";
					?>
					</p>
				</div>
			</div>
			<div style="float:left;width:100%;height:50px">

				<div style="float:left;width:45%">
					<p align="center">ALT</p>
				</div>
				<div style="float:right;width:45%">
					<p align="left">
					<?php
						echo "<input type=text size=15 name=altitud value=$alt>";
					?>
					</p>
				</div>
			</div>

			<div style="float:left;width:100%;height:30px">
				<div style="float:left;width:45%">
					<p align="center"><b>Servidor remoto   </b>
						<?
							if ($usar_servidor=="ON")
							{
								echo "<input type=checkbox name=server_check checked>";
							}
							else	
							{
								echo "<input type=checkbox name=server_check>";
							}
						?>
					</p>
				</div>
				
			</div>
			
			<div style="float:left;width:100%;height:30px">
				<div style="float:left;width:45%">
					<p align="center">Servidor</p>
				</div>
				<div style="float:right;width:45%">
					<p align="left">
					<?php
						echo "<input type=text size=15 name=server value=$server>";
					?>
					</p>
				</div>
			</div>
			<div style="float:left;width:100%;height:30px">

				<div style="float:left;width:45%">
					<p align="center">Puerto</p>
				</div>
				<div style="float:right;width:45%">
					<p align="left">
					<?php
						echo "<input type=text size=15 name=port value=$port>";
					?>
					</p>
				</div>
			</div>
			<div style="float:left;width:100%;height:50px">

				<div style="float:left;width:45%">
					<p align="center">Carpeta</p>
				</div>
				<div style="float:right;width:45%">
					<p align="left">
					<?php
						echo "<input type=text size=15 name=folder value=$folder>";
					?>
					</p>
				</div>
			</div>
			
			
			<div style="float:left;width:100%">
				<p align="center">
				<input type="submit" name="config" value="Enviar"><br>
				</p>
			</div>
		</div>
		<div style="float:left; width:14%">
			<p> </p>
		</div>

		<div style="border-style:solid; border-color:#CCCCCC; border-width:thin; width:35%;float:left">
			<div style="width:100%">
				<p align="center"><b>Datos actuales</b></p>
			</div>
			<div style="float:left;width:45%">
				<p align="center"><b>Fecha</b></p>
				<p align="center"><b>X (&microrad)</b></p>
				<p align="center"><b>Y (&microrad)</b></p>
				<p align="center"><b>T (&degC)</b></p>
			</div>
			<div style="float:right;width:45%">
				<?php
				echo "<p align=center style=color:green><b>$dia/$mes/$anio $hora:$minuto:$segundo</p>";
				echo "<p align=center style=color:green>$currentx</p>";
				echo "<p align=center style=color:green>$currenty</p>";
				echo "<p align=center style=color:green>$currentt</b></p>";
				?>
			</div>
			<div style="float:left;width:100%">
				<p align="center"><b>Estad&iacute;sticas</b></p>
			</div>

			<div style="float:left;width:100%">
			<div style="float:left;width:22%;">
				<p align="center"> </p>
			</div>
			<div style="float:left;width:23%;">
				<p align="center"><b>Min</b></p>
			</div>
			<div style="float:left;width:23%;">
				<p align="center"><b>Max</b></p>
			</div>
			<div style="float:left;width:23%;">
				<p align="center"><b>Dif</b></p>
			</div>
			</div>
			<div style="float:left;width:100%">
			<div style="float:left;width:22%;">
				<p align="center"><b>X (&microrad)</b></p>
				<p align="center"><b>Y (&microrad)</b></p>
				<p align="center"><b>T (&degC)</b></p>
			</div>
			<div style="float:left;width:23%;">
				<?php
				echo "<p align=center style=color:green><b>$minx</b></p>";
				echo "<p align=center style=color:green><b>$miny</b></p>";
				echo "<p align=center style=color:green><b>$mint</b></p>";
				?>
			</div>
			<div style="float:left;width:23%">
				<?php
				echo "<p align=center style=color:green><b>$maxx</b></p>";
				echo "<p align=center style=color:green><b>$maxy</b></p>";
				echo "<p align=center style=color:green><b>$maxt</b></p>";
				?>
			</div>
			<div style="float:left;width:23%;">
				<?php
				echo "<p align=center style=color:green><b>$difx</b></p>";
				echo "<p align=center style=color:green><b>$dify</b></p>";
				echo "<p align=center style=color:green><b>$dift</b></p>";
				?>
			</div>
			</div>

			<div style="float:left;width:100%">
				<p align="center"><b>Almacenamiento</b></p>
			</div>

			<?
				exec("df -h /media/CAM",$salida);
							
				$tok = strtok($salida[1]," ");
				$m=0;
				while($tok != false)
				{
					$varios[$m]=$tok;
					//echo "<br>".$varios[$m]."<br>";
					$tok = strtok(" ");
					$m++;
				}
			?>
			

			<div style="float:right;width:100%">
			<div style="border-width:thin;float:left;width:45%">
				<p align=center><strong>Capacidad:</strong></p>
			</div>
			<div style="border-width:thin;float:left;width:45%">
				<p align=center><?echo $varios[1];?></p>
			</div>
		</div>
			
		<div style="float:right;width:100%">
			<div style="border-width:thin;float:left;width:45%">
				<p align=center><strong>Usado:</strong></p>
			</div>
			<div style="border-width:thin;float:left;width:45%">
				<p align=center><?echo $varios[4];?></p>
			</div>
		</div>
		
		
		</div>
	</div>
	<div style="float:left;width:100%;">
		<div style="float:left;width:55%;">
			<p align=right>Copyleft IGN 2014 </p>
		</div>
		<div style="float:left;width:40%;">
			<p align=right><input type="checkbox" name="apaga_check"><input type="submit" name="apaga" value="Apagar"></p>
		</div>
		
	</div>
</form>
<?	}else{echo "incorrecto";}}?>
</body>
</head>

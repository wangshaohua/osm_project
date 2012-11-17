<?php
	echo shell_exec('../bin/osm_parser_online ' . $_POST['arg']);
	/*
	//for debugging purpose only: 
	$f = fopen('/tmp/arg', 'w');
	fwrite($f, $_POST['arg']);
	fclose($f);
	*/
?>

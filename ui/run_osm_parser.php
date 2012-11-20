<!--
	In order to handle the large file size of the OSM output, the following properties in php.ini need to be set:
	
	php_value upload_max_filesize 20M
	php_value post_max_size 20M
	php_value max_input_time 300
	php_value max_execution_time 300
	
	** The value should be change according to experiment.
-->
<?php	
	echo "Loading..." . $_FILES['osmfile']['name'] . "<br/>";
	
	if ($_FILES['osmfile']['error'] > 0)
		echo "Error: " . $_FILES['osmfile']['error'] . "<br />";
		
	if(!file_exists($_FILES['osmfile']['tmp_name']) || !is_uploaded_file($_FILES['osmfile']['tmp_name'])) {
	    echo 'No upload' . "<br/>";
	} else {
		//echo "File is in temporary location: " . $_FILES['osmfile']['tmp_name'] . "<br/>";
		//move_uploaded_file($_FILES['osmfile']['tmp_name'], "input.osm");
		//echo "File saved to input.osm.<br/>";
	}
	
	//$tmp_file = $_FILES['osmfile']['tmp_name'];
	//echo "File is in location: " . $tmp_file . "<br />";
	
	$pwd = shell_exec("pwd");
	echo shell_exec('../bin/osm_parser ' . $_FILES['osmfile']['tmp_name']) . "<br/>";
	sleep(3);
	
	// Move WA_* files to the web directory.
	//echo shell_exec('sudo cp -v /tmp/WA_* .'); // for some reason this won't execute!
	
	echo substr($pwd, 0, -1)."/WA_Vertices.txt<br/>";
	echo rename("/tmp/WA_Vertices.txt", substr($pwd, 0, -1) . "/WA_Vertices.txt") . "<br/>";
	echo rename("/tmp/WA_Edges.txt", substr($pwd, 0, -1) . "/WA_Edges.txt") . "<br/>";
	
	echo "<br/><a href='viz.html'>Go back to visualize page</a>";
	
	/*
	//for debugging purpose only: 
	$f = fopen('/tmp/arg', 'w');
	fwrite($f, $_POST['arg']);
	fclose($f);
	*/
?>

<?php
	$min_lat = $_POST['min_lat']; 
	$max_lat = $_POST['max_lat']; 
	$min_lon = $_POST['min_lon']; 
	$max_lon = $_POST['max_lon']; 
	$c = curl_init("http://api.openstreetmap.org/api/0.6/map?bbox=$min_lon,$min_lat,$max_lon,$max_lat");
	$fn = "$min_lon^$min_lat^$max_lon^$max_lat.osm";
	$fp = fopen($fn, 'w');
	curl_setopt($c, CURLOPT_FILE, $fp);
	curl_setopt($c, CURLOPT_HEADER, false);
	curl_setopt($c, CURLOPT_FAILONERROR, true);
	$rc = curl_exec($c);
	$http_code = curl_getinfo($c, CURLINFO_HTTP_CODE);
	fclose($fp);
	if ($http_code != 200){
		echo '<p>' . curl_error($c) . '</p>';
	}else{
		echo '<h>Query Result:</h><br/>';
		echo '<textarea readonly="readonly" rows=10 cols=40>' . file_get_contents($fn) . "</textarea><br/>";
		echo '<a href="' . $fn . '">Download Result</a><br/>';
		echo '<a href="generate_result.php?fn=' . $fn . '">Generate Vertex and Edge Representation</a>';
	}
	curl_close($c);
?>

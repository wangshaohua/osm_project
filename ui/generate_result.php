<?php
	$fn = $_GET['fn'];
	shell_exec('./osm_parser ' . $fn);
	$zip = new ZipArchive();
	if($zip -> open($fn . '.zip', ZIPARCHIVE::OVERWRITE)) {
		$zip->addFile($fn . '_Nodes.txt');
		$zip->addFile($fn . '_Vertices.txt');
		$zip->addFile($fn . '_Edges.txt');
		$zip->addFile($fn . '_EdgeGeometry.txt');
	}
	$zip->close();
	unlink($fn . '_Nodes.txt');
	unlink($fn . '_Vertices.txt');
	unlink($fn . '_Edges.txt');
	unlink($fn . '_EdgeGeometry.txt');
	echo '<a href="' . $fn . '.zip' . '">Download Result</a>';
?>

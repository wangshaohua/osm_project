<html>
	<head>
		<title>Visualization of Edges and Vertices   -- Author: Yitao Li</title>
		<script src="js/d3.v2.js"></script>
		<script src="js/jquery-1.8.2.min.js"></script>
		<script>
			$(document).ready(function(){
				var width = 800, height = 600, viz = d3.select("#viz").append("svg:svg").attr("width", width).attr("height", height), v_str = $("#WA_Vertices").html().split(/[ \n]+/), e_str = $("#WA_Edges").html().split(/[ \n]+/), v_lat = new Array(), v_lon = new Array(), max_lat = -90, min_lat = 90, max_lon = -180, min_lon = 180, w, h, i;
				for (i = 0; i < v_str.length;){
					var id = parseInt(v_str[i++]), lat = parseFloat(v_str[i++]), lon =  parseFloat(v_str[i++]);
					v_lat[id] = lat; v_lon[id] = lon;	
					if (lat < min_lat){
						min_lat = lat;
					}else if (lat > max_lat){
						max_lat = lat;
					}
					if (lon < min_lon){
						min_lon = lon;
					}else if (lon > max_lon){
						max_lon = lon;
					}
				}
				h = max_lat - min_lat;
				w = max_lon - min_lon;
				for (i = 1; i < e_str.length;){
					var a = parseInt(e_str[i]);   //skipping edge id
					++i;
					var b = parseInt(e_str[i]);
					++i;
					var cost = parseFloat(e_str[i]);
					i += 2;
					viz.append("svg:line").attr("y1", (v_lat[a] - min_lat) / h * height).attr("y2", (v_lat[b] - min_lat) / h * height).attr("x1", (v_lon[a] - min_lon) / w * width).attr("x2", (v_lon[b] - min_lon) / w * width).style("stroke", "rgb(6,120,155)");

				}
				//var myLine = viz.append("svg:line").attr("x1", 40).attr("y1", 50).attr("x2", 450).attr("y2", 150).style("stroke", "rgb(6,120,155)");
			});
		</script>
	</head> 
	<body>   
		<div id="viz"></div>
		<div id="WA_Vertices" style="display:none"><?php require_once("/tmp/WA_Vertices.txt"); ?></div>
		<div id="WA_Edges" style="display:none"><?php require_once("/tmp/WA_Edges.txt"); ?></div>
	</body>
</html>

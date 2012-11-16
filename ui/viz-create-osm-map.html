<meta http-equiv="Content-type" content="text/html;charset=UTF-8"> 
<html>
	<head>
		<title>Visualization of Edges and Vertices (Powered by d3.js)  -- Author: Yitao Li</title>
		<script src="js/d3.v2.js"></script>
		<script src="js/jquery-1.8.2.min.js"></script>
		<script>
			var MAP_POS, MAP_WIDTH, MAP_HEIGHT, X_OFFSET, Y_OFFSET, map, prev_X = null, prev_Y = null, X, Y, lat, lon, node_num = 0, _node_num, way_num = -1, node_exists = false;	
			$(document).ready(function(){
				map = d3.select("#map").append("svg:svg").attr("width", "100%").attr("height", "100%");
				MAP_POS = $("#map").position();
				MAP_WIDTH = $("#map").width();
				MAP_HEIGHT = $("#map").height();
				X_OFFSET = MAP_POS.left;
				Y_OFFSET = MAP_POS.top;
				$(document).bind("contextmenu",function(e){
					return false;
				}); 
				$("#map").click(function(event){
					if (!node_exists){   //create a new node
						X = event.pageX - X_OFFSET;
						Y = event.pageY - Y_OFFSET;
						lat = Math.round((-Y * 180.0 / MAP_HEIGHT + 90.0) * 1000000) / 1000000.0;
						lon = Math.round((X * 180.0 / MAP_WIDTH - 90.0) * 1000000) / 1000000.0;
						map.append("svg:circle").data([ {node_num: node_num, lat: lat, lon: lon} ]).attr("cx", X).attr("cy", Y).attr("r", 6).style("fill", "red").style("cursor", "pointer").on("click", function(d,i){
							node_exists = true;
							lat = d.lat;
							lon = d.lon;
							X = d3.select(this).attr("cx");
							Y = d3.select(this).attr("cy");
							_node_num = d.node_num;	
						});
						_node_num = node_num
						$("#node-xml").append("&lt;node id=\"" + node_num + "\" lat=\"" + lat + "\" lon=\"" + lon  + "\"/&gt;\n");
						++node_num;
					}else{
						node_exists = false;  //use an existing node
					}
					if (prev_X != null && prev_Y != null){
						map.append("svg:line").attr("x1", prev_X).attr("y1", prev_Y).attr("x2", X).attr("y2", Y).style("stroke", "blue").style("stroke-width", 4); 
						$("#way-xml").append("\n  ");
					}else{
						++way_num;
						$("#way-xml").append("&lt;way id=\"" + way_num + "\"&gt;\n  ");
					}
					$("#way-xml").append("&lt;nd ref=\"" + _node_num + "\"/&gt;")
					prev_X = X;
					prev_Y = Y;
				});
				$("#map").mousedown(function(event){
					if (event.which == 3){
						prev_X = null;
						prev_Y = null;
						$("#way-xml").append("\n&lt/way&gt;\n")
					}
				});
			});
		</script>
	</head> 
	<body>   
		<div id="demo"> 
			<table border="2" cellpadding="0" cellspacing="0" style="width: 100%; height: 100%; float: left; clear: left;">
				<tr style="height:16pt;">
					<th>Map</th>
					<th>Vertex-and-Edge Representation</th>
				</tr>
				<tr style="height:61.8%;">
					<td align="left"><div id="map" style="width:100%;height:100%;"></div></td>
					<td align="left"><div id="graph" style="width:100%;height:100%;"></div></td>
				</tr>
				<tr>
					<td align="top">
						<table border="1" cellpadding="0" cellspacing="0" style="width: 100%; height: 100%; float: left; clear: left;">
							<tr style="height:16pt;">
								<th>OSM Node(s):</th> 
								<th>OSM Way(s):</th> 
							</tr>
							<tr>
								<td>
									<div id="node-div" style="width:100%;height:100%;">
										<textarea id="node-xml" readonly="true" rows="10" cols="50"></textarea>
									</div>
								</td>
								<td>
									<div id="way-div" style="width:100%;height:100%;">
										<textarea id="way-xml" readonly="true" rows="10" cols="35"></textarea>
									</div>
								</td>
							</tr>
						</table>
					</td>
					<td align="top">
						<table border="2" cellpadding="0" cellspacing="0" style="width: 100%; height: 100%; float: left; clear: left;">
							<tr style="height:16pt;">
								<th>Vertice(s):</th> 
								<th>Edge(s):</th> 
							</tr>
							<tr>
								<td>
									<div id="vertex-div" style="width:100%;height:100%;">
										<textarea id="vertex-xml" readonly="true" rows="10" cols="50"></textarea>
									</div>
								</td>
								<td>
									<div id="edge-div" style="width:100%;height:100%;">
										<textarea id="edge-xml" readonly="true" rows="10" cols="35"></textarea>
									</div>
								</td>
							</tr>
						</table>
					</td>
				</tr>
			</table>
		</div>
	</body>
</html>

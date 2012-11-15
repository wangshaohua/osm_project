<meta http-equiv="Content-type" content="text/html;charset=UTF-8"> 
<html>
	<head>
		<title>Visualization of Edges and Vertices (Powered by d3.js)  -- Author: Yitao Li</title>
		<script src="js/d3.v2.js"></script>
		<script src="js/jquery-1.8.2.min.js"></script>
		<script>
			var MAP_POS, X_OFFSET, Y_OFFSET, map, prev_X = null, prev_Y = null, X, Y;	
			$(document).ready(function(){
				map = d3.select("#map").append("svg:svg").attr("width", "100%").attr("height", "100%");
				MAP_POS = $("#map").position();
				X_OFFSET = MAP_POS.left;
				Y_OFFSET = MAP_POS.top;
				$(document).bind("contextmenu",function(e){
					return false;
				}); 
				$("#map").click(function(event){
					X = event.pageX - X_OFFSET;
					Y = event.pageY - Y_OFFSET
					map.append("svg:circle").attr("cx", X).attr("cy", Y).attr("r", 4).style("fill", "red");
					if (prev_X != null && prev_Y != null){
						map.append("svg:line").attr("x1", prev_X).attr("y1", prev_Y).attr("x2", X).attr("y2", Y).style("stroke", "blue").style("stroke-width", 2);  
					}
					prev_X = X;
					prev_Y = Y;
				});
				$("#map").mousedown(function(event){
					if (event.which == 3){
						prev_X = null;
						prev_Y = null;
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
								<th>OSM Road(s):</th> 
							</tr>
							<tr>
								<td>
									<div id="node-div" style="width:100%;height:100%;">
										<textarea id="node-xml" readonly="true" rows="10" cols="50"></textarea>
									</div>
								</td>
								<td>
									<div id="road-div" style="width:100%;height:100%;">
										<textarea id="road-xml" readonly="true" rows="10" cols="35"></textarea>
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

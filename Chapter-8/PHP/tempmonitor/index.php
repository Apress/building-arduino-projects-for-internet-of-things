<html lang="en">

<head>
	<title>Temperature Monitoring System - Dashboard</title>

	<script src="//ajax.googleapis.com/ajax/libs/dojo/1.10.4/dojo/dojo.js"
	            data-dojo-config="async: true"></script>            
	
	<script>
		<?php
		include('util-dbconn.php');
			
		$sql = "SELECT * FROM (SELECT * FROM `TEMPERATURE_MONITORING_DATA` ORDER BY ID DESC LIMIT 30) sub ORDER BY id ASC";
		$result = $mysqli->query($sql);
		$resultCount = $result->num_rows;
		
		if ($resultCount > 0)
		{
			$currentRow = 0;
			echo "var chartData = [";
			// output data of each row
			while($row = $result->fetch_assoc())
			{
			    $currentRow = $currentRow + 1;
			
			    echo $row["TEMPERATURE"];
			            
			    if($currentRow < $resultCount)
			    {
			        echo ",";
			    }
			}
			echo "];";
			}
		else
		{
			echo "0 results";
		}

		$mysqli->close();
		?>
	
		require([
		"dojox/charting/Chart",
		"dojox/charting/themes/PlotKit/orange",
		"dojox/charting/plot2d/Columns",
		"dojox/charting/plot2d/Markers",
		"dojox/charting/axis2d/Default",
		"dojo/domReady!"
		], function(Chart, theme) {
			var chart = new Chart("chartNode");
			
			chart.setTheme(theme);
			
			chart.addPlot("default", {type: "Columns", gap: 5 , labels: true, labelStyle: "outside"});
			
			chart.addAxis("x", {title: "Readings (#)", titleOrientation: "away"});
			chart.addAxis("y", {title: "Temperature (F)", titleOrientation: "axis" , min: 0, max: 270, vertical: true, fixLower: "major", fixUpper: "major" });

			chart.addSeries("TemperatureData",chartData);
		
			chart.render();
		});	
	</script>		
</head>

<body style="background-color: #F5EEE6">
	<div style="align: center;"><font size="5px">Temperature Monitoring System - Dashboard</font></div>
	<div id="chartNode" style="width: 100%; height: 50%; margin-top: 50px;"></div>
			
	<script type="text/javascript">
		init();
	</script>
</body>
</html>

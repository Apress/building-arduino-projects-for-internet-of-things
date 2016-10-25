<?php
    include('util-dbconn.php');

    $temperature = $_GET['temperature'];

    echo "[DEBUG] Temperature Sensor Data: " . $temperature . "\n";

    $sql = "INSERT INTO `TEMPERATURE_MONITORING_DATA`(`TEMPERATURE`) VALUES ($temperature)";
	
    if (!$result = $mysqli->query($sql))
    {
        echo "[Error] " . mysqli_error() . "\n";
        exit();
    }

    $mysqli->close();

    echo "[DEBUG] New Temperature Sensor Data Added Successfully\n";
?>

<?php
    include('util-dbconn.php');

    $parkingUpdate = $_GET['parkingUpdate'];

    echo "[DEBUG] Parking Update: " . $parkingUpdate . "\n";

    $currentParkingCount = 1;	
    
    if($parkingUpdate == "OPEN")
    {	
	$currentParkingCount = 1;
    }
    else
    {
    	$currentParkingCount = 0;
    }

    $sql = "UPDATE `PARKING_SPOTS_DATA` SET PARKING_SPOTS_COUNT = $currentParkingCount";
    
    if (!$result = $mysqli->query($sql))
    {
        echo "[Error] " . mysqli_error() . "\n";
        exit();
    }

    $mysqli->close();

    echo "[DEBUG] Updated Parking Spots Counter Successfully\n";
?>
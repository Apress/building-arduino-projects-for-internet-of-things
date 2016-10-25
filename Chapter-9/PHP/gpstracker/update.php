<?php
    include('util-dbconn.php');

    $clientID = $_GET['clientID'];    
    $latitude = $_GET['latitude'];
    $longitude = $_GET['longitude'];   
    
    $sql = "INSERT INTO `GPS_TRACKER_DATA` (CLIENT_ID, LATITUDE, LONGITUDE) VALUES('$clientID', $latitude, $longitude) ";
    $sql = $sql . "ON DUPLICATE KEY UPDATE CLIENT_ID='$clientID', LATITUDE=$latitude, LONGITUDE=$longitude";

    echo $sql;
   
    if (!$result = $mysqli->query($sql))
    {
        echo "[Error] " . mysqli_error() . "\n";
        exit();
    }

    $mysqli->close();

    echo "[DEBUG] Updated GPS Coordinates Successfully\n";
?>
<?php
    include('util-dbconn.php');

    $sql = "SELECT PARKING_SPOTS_COUNT FROM `PARKING_SPOTS_DATA`";
    $result = $mysqli->query($sql);
    $resultCount = $result->num_rows;
    
    if ($resultCount > 0)
    {
    	$row = $result->fetch_assoc();
    	print(json_encode($row));
    }
    else
    {
        echo "0 results";
    }
    $mysqli->close();    
?>
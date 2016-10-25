<?php
    $servername = "localhost";
    $dbname = "codifyth_iot_book_db";
    $username = "codifyth_admin";
    $password = "Codify20!5";

    //Open a new connection to MySQL server
    $mysqli = new mysqli($servername, $username, $password, $dbname);

    //Output connection errors
    if ($mysqli->connect_error)
    {
        die("[ERROR] Connection Failed: " . $mysqli->connect_error);
    }
?>
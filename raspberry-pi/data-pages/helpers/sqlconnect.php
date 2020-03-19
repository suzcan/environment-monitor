<?php

function getdb() {
	$servername = "localhost";
	$username = "reader";
	$password = "likes_to_read";
	$db = "emd_sensors_data";

	try {
		$conn = mysqli_connect($servername, $username, $password, $db);
	} catch (exception $e) {
		echo "Connection failed: " . $e->getMessage();
	}
	return $conn
}

?>

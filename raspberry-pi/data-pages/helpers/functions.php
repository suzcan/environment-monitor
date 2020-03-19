<?php
	$start = $_POST['start-unix'];
	$stop = $_POST['stop-unix'];
	$con = mysqli_connect("localhost", "reader", "likes_to_read", "emd_sensors_data");

	$date_from = date('d.m.y', $start);
	$date_to = date('d.m.y', $stop);

if(isset($_POST["Export"])) {
	header('Content-Type: text/csv; charset=utf-8');
	header('Content-Disposition: attachment; filename=data(' . $date_from . '-' . $date_to .').csv');
	$output = fopen("php://output", "w");
	fputcsv($output, array('Unix Time', 'Device ID', 'Analogue 0', 'Analogue 1', 'Analogue 2', 'Analogue 3', 'Analogue 4', 'Analogue 5', 'BME680-Temperature (C)', 'BME680-Pressure (hPa)', 'BME680-Humidity (%)', 'BME680-Gas Resistance (KOhms)', 'BME-Altitude (m)', 'PMS5003-PM1.0 Standard', 'PMS5003-PM2.5 Standard', 'PMS5003-PM10.0 Standard', 'PMS5003-PM1.0 Environmental', 'PMS5003-PM2.5 Environmental', 'PMS5003-PM10.0 Environmental', 'PMS5003-Particles > 0.3um/0.1L air', 'PMS5003-Particles > 0.5um/0.1L air', 'PMS5003-Particles > 1.0um/0.1L air', 'PMS5003-Particles > 2.5um/0.1L air', 'PMS5003-Particles > 5.0um/0.1L air', 'PMS5003-Particles > 10.0um/0.1L air', 'SCD30-CO2 (ppm)', 'SCD30-Temperature (C)', 'SCD30-TVOC (ppb\t)', 'SCD30-Raw H2 (\t)', 'SCD30-Raw Ethanol', 'SI1145-Visible Light', 'SI1145-Infrared', 'SI1145-UV Index'));
	$query = "SELECT * from sensor_data WHERE time >= $start AND time <= $stop";
	$result = mysqli_query($con, $query);
	while($row = mysqli_fetch_assoc($result))
	{
		fputcsv($output, $row);
	}
	fclose($output);
}

?>

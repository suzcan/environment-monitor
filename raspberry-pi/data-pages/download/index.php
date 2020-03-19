<!DOCTYPE html>
<html>

<head>
	<meta charset="UTF-8">
	<title>Download Page</title>
	<?php include '../helpers/bootstrap.php'; ?>
</head>
<body>
<div class="container">
<?php

// Get time range
if(empty($_GET)) {
	$start = strtotime("-1 day");
	$stop = strtotime("now");
} else {
	$start = $_GET["start-date"];
	$start .= " ";
	$start .= $_GET['start-time'];
	$start = strtotime($start);

	$stop = $_GET["stop-date"];
	$stop .= " ";
	$stop .= $_GET["stop-time"];
	$stop = strtotime($stop);
}
// Convert time range to milliseconds
$start_mil = $start;
$start_mil .= '000';
$stop_mil = $stop;
$stop_mil .= '000';

// Default values
$def_date_stop = date("Y-m-d", $stop);
$def_time_stop = date("H:i", $stop);
$def_date_start = date("Y-m-d", $start);
$def_time_start = date("H:i", $start);


// URL retrieval
$url_http = "http://" . $_SERVER['HTTP_HOST'];
?>

<header class="blog-header py-3">
	<div class="row flex-nowrap justify-content-between align-items-centre">
		<div class="col-4 pt-1">
			<?php echo '<a href="', $url_http, '">'; ?>
				<img src="https://i.guim.co.uk/img/static/sys-images/Guardian/Pix/pictures/2015/4/24/1429871252768/University-of-Warwick-new-007.jpg?width=300&quality=85&auto=format&fit=max&s=bfe0db5ba5c9a7e5fccaf3a8a9f55e13" alt="University of Warwick" width=120px height=80px>
			</a>
		</div>
		<div class="col-4 text-center">
			<h3 class="display-5 font-weight-bold">Data Download Page</h3>
		</div>
		<div class="col-4 d-flex justify-content-end align-items-center">
			<?php
				$url = $url_http;
				$url .= '/?page_id=15';
				echo '
				<a class="btn btn-sm btn-outline-secondary" href="', $url, '">Back to User Guide</a>';
			?>
		</div>
	</div>
</header>

<!-- Time Period Selection -->
<form class="form-horizontal" method="GET" name="time_range" enctype="mutlipart/form-data">
<div class="row mb-2">
	<div class="col-md-6">
		<div class="card flex-md-row mb-4 box-shadow h-md-250">
			<div class="card-body d-flex flex-column align-items-start">
				<h3 class="mb-0">
					<a class="text-dark">Start Time</a>
				</h3>
				<?php
				echo '
				<input type="date" class="form-control" name="start-date" value="', $def_date_start,'">
				<p></p>
				<input type="time" class="form-control" name="start-time" value="', $def_time_start, '">';
				?>
			</div>
		</div>
	</div>
	<div class="col-md-6">
		<div class="card flex-md-row mb-4 box-shadow h-md-250">
			<div class="card-body d-flex flex-column align-items-start">
				<h3 class="mb-0">
					<a class="text-dark">End Time</a>
				</h3>
				<?php
				echo '
				<input type="date" class="form-control" name="stop-date" value="', $def_date_stop, '">
				<p></p>
				<input type="time" class="form-control" name="stop-time" value="', $def_time_stop,'">';
				?>
			</div>	
		</div>
	</div>

</div>	
<div class="row mb-2">
	<div class="col-1">
		<input type="submit" name="Select" class="btn btn-secondary" value="Select Time Range"/>
	</div>
</div>
</form>

<!-- Download Functionality -->
<div class="row mb-2">
	<div class="col-1">
		<form class="form-horizontal" action="../helpers/functions.php" method="post" name="get_data" enctype="multipart/form-data">
			<div class="form-group">
				<input type="submit" name="Export" class="btn btn-success" value="Download Data"/>
				<?php
				echo ' 
				<input type="hidden" name="start-unix" value="', $start,'"/>
				<input type="hidden" name="stop-unix" value="', $stop, '"/>';
				?>
			</div>
		</form>
	</div>
</div>

<div class="jumbotron text-center">
	<div class="container">
		<h2 class="jumbotron-heading">Data Preview</h2>
<?php

// Embedded table
echo '<iframe src="', $url_http, ':3000/d-solo/q3XzrnWRk/table?orgId=1&from=', $start_mil, '&to=', $stop_mil, '&var-field=bme_temperature&var-field=bme_pressure&var-field=bme_humidity&var-field=bme_gas_resistance&var-field=bme_altitude&var-where_cond=time%20IS%20NOT%20NULL&panelId=2" width="100%" height="800" position="absolute" frameborder="0"></iframe>'

?>
	</div>
</div>

</div>
</body>
</html>

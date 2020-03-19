<!DOCTYPE html>
<html lang="en">
<head>
	<title>View Page Redirect</title>
</head>
<body>
	<p>Being redirected shortly</p>

<?php 
	$url = "http://";
	$url .= $_SERVER['HTTP_HOST'];
	$url .= ":3000";

	header("Location: $url");
?>

</body>
</html>

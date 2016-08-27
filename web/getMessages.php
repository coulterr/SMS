<?php
	$q = $_REQUEST["q"];
	exec("/var/www/html/message_service coulterr.com 5362 GET_ALL_BY_CONTACT \"$q\""  , $output); 
	echo json_encode($output); 
?>

<?php

	exec("/var/www/html/message_service coulterr.com 5362 GET_ALL_CONTACTS", $output); 
	foreach($output as $line){
		unlink("/var/www/html/$line.html"); 
		$file = fopen("/var/www/html/$line.html", "w");
		fwrite($file, "<!DOCTYPE html>");
		fwrite($file, "<html>"); 
		fwrite($file, "<head>");
		fwrite($file, "<link rel=\"stylesheet\" type=\"text/css\" href=\"contact.css\">");
		fwrite($file, "<script type=\"text/javascript\" src=\"functions.js\"></script>");
		fwrite($file, "<h1>$line</h1>");
		fwrite($file, "</head>");
		fwrite($file, "<body onload=\"populateSMS('$line')\">");
		fwrite($file, "<div id=\"content_win\">");
		fwrite($file, "</div>");
		fwrite($file, "<div id=\"entry_win\">");
		fwrite($file, "<textarea id=\"input\" cols=\"100\" rows=\"12\">");
		fwrite($file, "</textarea>");
		fwrite($file, "<button id=\"send_btn\" onclick=\"send_msg('$line')\"> send </button>");
		fwrite($file, "</div>");
		fwrite($file, "</body>");
		fwrite($file, "</html>");
		 
		fclose($file); 
	}
	echo json_encode($output); 
?>

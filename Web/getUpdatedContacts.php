<?php
        exec("/var/www/html/message_service coulterr.com 5362 GET_ALL_CONTACTS", $output); 
        echo json_encode($output); 
?>

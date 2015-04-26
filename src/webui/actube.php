<?php
header('Content-Type: application/json');

class DB extends SQLite3
{
	function __construct()
	{
		$this->open("../ac/ac.sqlite3");
	}

}


$db = new DB();

$results = $db->query("SELECT acid as id, acname as name ,lastseen>datetime('now','-10 second') as active FROM acs;");

$r= array();

while ($row = $results->fetchArray()) {
	array_push($r,$row);

}
$j = json_encode ($r);
echo "$j";


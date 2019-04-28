<html>
	<head>
		<title>
			<script>
			</script>
		</title>
	</head>
	
	<body>
	<form action="">
		<input type="text" id="demo"></input>
		<input type="text" name="latitude"></input>
		<input type="text" name="longitude"></input>
		<input type="button" name="Take me to the nearest warehouse" onclick="button1()"></input>
		<input type="button" name="Show me the way" onclick="button1()"></input>
		<input type="submit" name="submit"/>
	</form>
	</body>
	
	<script type = "text/javascript">
		var x = document.getElementById("demo");
		function getLocation() {
		  //if (navigator.geolocation) {
			navigator.geolocation.getCurrentPosition(showPosition);
		  //} else {
			//x.innerHTML = "Geolocation is not supported by this browser.";
		  //}
		}

		function showPosition(position) {
			var l1=document.getElementById("Latitude");
			l1.innerHTML = position.coords.latitude;
			var l2=document.getElementById("Longitude");
			l2.innerHTML = position.coords.longitude;
			
		  x.innerHTML = "Latitude: " + position.coords.latitude + 
		  "<br>Longitude: " + position.coords.longitude;
		}
	</script>
	<!--displaying nearest safehouse-->
	<?php
	include "connection.php";
/*function vincentyGreatCircleDistance($latitudeFrom, $longitudeFrom, $latitudeTo, $longitudeTo, $earthRadius = 6371000)
{*/
	$sql="SELECT * FROM police_station";
	$resourceQuery = mysqli_query($connection,$sql);
	$n = mysqli_num_rows($resourceQuery);
	$no=0;
	$latFrom=$_REQUEST["latitude"];
	$lonFrom=$_REQUEST["longitude"];
	/*echo $latFrom;
	echo $lonFrom;*/
	//$p = mysqli_fetch_array($resourceQuery);
	
	class station 
	{
		public $lat;
		public $lng;
		public $id;
		public $dist;
		public $area;
		
		public function __construct($lt,$ln,$i,$d,$a)
		{
			$this->lat=$lt;
			$this->lng=$ln;
			$this->id=$i;
			$this->dist=$d;
			$this->area=$a;
		}
	}
	
	function cmp($a,$b)
	{
		if($a->dist==$b->dist) {return 0;}
		
		return ($a->dist < $b->dist)? -1:1;
	}
	
	$StArr=[];
	
	while ($p = mysqli_fetch_array($resourceQuery))
	{
		
		/*<td><?php echo $p["event_id"]; echo $p["participant_id"];?></td>
		<td><?php echo $p["name"];?></td>
		<td><?php echo $p["phnno"];?></td>
		<td><?php echo $p["email"];?></td>*/
		$latTo = $p["latitude"];
		$lonTo = $p["longitude"];
		$earthRadius = 6371000;
		$lonDelta = $lonTo - $lonFrom;
		$a = pow(cos($latTo) * sin($lonDelta), 2) +pow(cos($latFrom) * sin($latTo) - sin($latFrom) * cos($latTo) * cos($lonDelta), 2);
		$b = sin($latFrom) * sin($latTo) + cos($latFrom) * cos($latTo) * cos($lonDelta);
		$angle = atan2(sqrt($a), $b);
		$distance = $angle * $earthRadius;
		//echo $distance;
		
		$StArr[]= new station($latTo,$lonTo,$p["id"],$distance,$p["area"]);
		//echo "<br>";
	}
	
	usort($StArr,'cmp');
	echo "<b>";
	echo "ID \t Latitude\t Longitude\tDistance\tArea";
	echo "</b>";
	echo "<br><br>";
	for($i=0;$i<3;$i++)
	{
		echo $StArr[$i]->id."\t".$StArr[$i]->lat."\t".$StArr[$i]->lng."\t".$StArr[$i]->dist."\t".$StArr[$i]->area."<br>";
	}?>
</html>

Time left to evacuate
00.00
Take me to nearest safehouse
Show me the way
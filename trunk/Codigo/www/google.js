<script src=		"http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAyrdeV_NswcK9b4JhAiG8-hQmGsXG_ntmPmO65ulpWnbiaP5ibxSJkGkNYs5I8TQhcKG7aEQgTumBQQ" type="text/javascript">
</script>

<script type="text/javascript">
function cargar() {
	if (GBrowserIsCompatible()) {
		var map = new GMap2(document.getElementById("map2"));
		map.setCenter(new GLatLng(37.4419, -122.1419), 13);
		GEvent.addListener(map,"click", function(overlay, latlng) {     
  			if (latlng) { 
    			var myHtml = "The GPoint value is: " + map.fromLatLngToDivPixel(latlng) + " at 			 								zoom level " + map.getZoom();
    			map.openInfoWindow(latlng, myHtml);
  			}
		});
		map.addControl(new GSmallMapControl());
		map.addControl(new GMapTypeControl());
	}
}


</script>

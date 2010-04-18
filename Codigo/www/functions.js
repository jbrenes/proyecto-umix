

var map;
function mostrarMapa(){	
		
		if (GBrowserIsCompatible()) {
				//alert("1");
				map = new GMap2(document.getElementById("map2"));
				//map.setCenter(new GLatLng(-34.9, -56.2), 20);
				/*GEvent.addListener(map,"click", function(overlay, latlng) {     
  					if (latlng) { 
    					var myHtml = "The GPoint value is: " + map.fromLatLngToDivPixel(latlng) 				 										+ " at zoom level " + map.getZoom();
    					map.openInfoWindow(latlng, myHtml);
  					}
				});
				*/
				
				map.addControl(new GSmallMapControl());
				map.addControl(new GMapTypeControl());
				//alert("2");
				cargarPuntos();
			
		}
		
	
}

function cargarPuntos(){
			if(map!=null){
					//alert("3");
					map.clearOverlays();
					GDownloadUrl("phpsqlajax_genxml.php", function(data) {
  						var xml = GXml.parse(data);
  						var markers = xml.documentElement.getElementsByTagName("marker");
  						var point = new GLatLng(-34.9, -56.2);
						for (var i = 0; i < markers.length; i++) {
    						
							point = new GLatLng(parseFloat(markers[i].getAttribute("lat")),
                            	parseFloat(markers[i].getAttribute("lng")));
    						var marker;
							var encontro = markers[i].getAttribute("encontro");
							var icon = new GIcon();
								icon.image = 			         		        		 	 	 	 	 									"http://labs.google.com/ridefinder/images/mm_20_red.png";
          						icon.shadow = 	             	 	 	 	 	 	 	 	 	 	 	 	 							"http://labs.google.com/ridefinder/images/mm_20_shadow.png";
          						
          						
		  						icon.iconAnchor = new GPoint(6, 20);
          						icon.infoWindowAnchor = new GPoint(5, 1);  
							if(	encontro == "0"){
								
							   marker = new GMarker(point,{icon:icon});
							}else{
								
														     
								var icongreen = new GIcon(icon,     										 										"http://labs.google.com/ridefinder/images/mm_20_green.png"	 										); 

								marker =  new  GMarker(point, {icon:icongreen});
								
							}
							map.addOverlay(marker);
							marker.bindInfoWindowHtml(markers[i].getAttribute("conv"));

  							
						}
						map.setCenter(point);
					});
					//alert("6");
				
				//alert("7");
				setTimeout("cargarPuntos()", 15000);
			}
}


	
	
				

		
	   		






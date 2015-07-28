var optionen = {};

Pebble.addEventListener("ready",
  function(e) {
    console.log("PebbleKit JS ist bereit!");
  }
);

Pebble.addEventListener("showConfiguration",
  function(e) {
    //Lädt die Konfigurationsseite
    console.log("showConfiguration: An Webseite gesendete Optionen = " + JSON.stringify(optionen));
    
    // Die URL der Konfigurationsseite 
    // Eigene Adresse an Stelle von "MeineSeite" einsetzen !!!
    var URL = "http://www.risingdreams.com/other/pebble/digiTime/settings.html?";
    Pebble.openURL( URL + encodeURIComponent(JSON.stringify(optionen)));
  }
);

Pebble.addEventListener("webviewclosed",
  function(e) {
       
    // Senden der Optionen, wenn die Syntax-Anforderungen erfüllt sind
    if (e.response.charAt(0) == "{" && e.response.slice(-1) == "}" && e.response.length > 5) 
    {
      optionen = JSON.parse(decodeURIComponent(e.response));
      console.log("Von der Webseite gesendete Optionen = " + JSON.stringify(optionen));      
      
      // An Pebble senden
      Pebble.sendAppMessage( {"bgColor": optionen.bgColor},
                          function(e) { console.log("Daten gesendet.");},
                          function(e) { console.log("Senden der Daten fehlgeschlagen!");});   
    } 
    else
    {
      console.log("Cancel");
    }
  }
                        
);
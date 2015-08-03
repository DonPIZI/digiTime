Pebble.addEventListener("showConfiguration",
  function(e) {
    //Load the remote config page
    Pebble.openURL("http://risingdreams.com/other/pebble/digiTime/settings.html");
  }
);



Pebble.addEventListener("webviewclosed",
  function(e) {
    //Get JSON dictionary
    var configuration = JSON.parse(decodeURIComponent(e.response));
    console.log("Configuration window returned: " + JSON.stringify(configuration));
 
    //Send to Pebble, persist there
    Pebble.sendAppMessage(
      {"bgColor": configuration.bgColor,
      "timeColor": configuration.timeColor,
      "dateColor": configuration.dateColor,
      "weekdayColor": configuration.weekdayColor,
      "sundayColor": configuration.sundayColor},
      function(e) {
        console.log("Sending settings data...");
      },
      function(e) {
        console.log("Settings feedback failed!");
      }
    );
  }
);
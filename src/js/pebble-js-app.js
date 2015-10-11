// TODO Save settings in local storage.
// TODO Use this for style: https://github.com/pebble/slate

// DataURI config page roughly based on https://github.com/bertfreudenberg/PebbleONE/
// Run (echo -n configHTML= && sed "s/^/\'/;s/$/\' +/" config.html && echo \'\<\!--.html\'\;) to generate this.
var configHTML='<!DOCTYPE html>' +
'<meta charset=utf-8>' +
'<meta name="viewport" content="width=device-width, height=device-height, user-scalable=no">' +
'' +
'<body>' +
'' +
'<form id="form" class="center" onsubmit="return save()">' +
'<fieldset>' +
'Day correction:' +
'<label><input type="radio" name="DAYCORRECTION" value="-1">-1</label>' +
'<label><input type="radio" name="DAYCORRECTION" value="0">0</label>' +
'<label><input type="radio" name="DAYCORRECTION" value="+1">+1</label>' +
'</fieldset>' +
'' +
'<input type="submit" value="Save">' +
'</form>' +
'' +
'<script>' +
'function save(e) {' +
'	var config = {};' +
'	var inputs = form.getElementsByTagName("input");' +
'	for (var i = 0; i < inputs.length; i++) {' +
'		if (inputs[i].checked) {' +
'			config[inputs[i].name] = parseInt(inputs[i].value);' +
'		}' +
'	}' +
'	document.location = "pebblejs://close#" + encodeURIComponent(JSON.stringify(config));' +
'	return false;' +
'}' +
'</script>' +
'<!--.html';

Pebble.addEventListener('showConfiguration', function(e) {
	Pebble.openURL("data:text/html," + configHTML);
});

Pebble.addEventListener('webviewclosed', function(e) {
	var config = JSON.parse(decodeURIComponent(e.response));
	Pebble.sendAppMessage(config, function(){
		console.log('Sent config data to Pebble');  
	}, function() {
		console.log('Failed to send config data!');
	});
});

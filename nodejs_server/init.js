//require the express nodejs module
var express = require('express'),
	//set an instance of exress
	app = express(),
	//require the body-parser nodejs module
	bodyParser = require('body-parser'),
	//require the path nodejs module
	path = require("path");

//support parsing of application/json type post data
app.use(bodyParser.json());

//support parsing of application/x-www-form-urlencoded post data
app.use(bodyParser.urlencoded({ extended: true }));

//tell express that www is the root of our public web folder
app.use(express.static("../website"));

//tell express what to do when the /form route is requested
app.post('/profiles',function(req, res){
	res.setHeader('Content-Type', 'application/json');

	var fs = require('fs');
	fs.writeFile("../website/profiles.json", JSON.stringify(req.body), function(err) {
	    if(err) {
	        return console.log(err);
	    }
	});

});

//wait for a connection
app.listen(3000, function () {
  console.log('Server is running. Point your browser to: http://localhost:3000');
});


/*var connect = require('connect');
var serveStatic = require('serve-static');

connect().use(serveStatic('../website/')).listen(8080, function(){
    console.log('Server running on 8080...');
});*/

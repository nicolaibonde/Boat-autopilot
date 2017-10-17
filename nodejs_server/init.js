//require the express nodejs module
var express = require('express'),
	//set an instance of exress
	app = express(),
	//require the body-parser nodejs module
	bodyParser = require('body-parser'),
	//require the path nodejs module
	path = require("path");

var fs = require('fs');
//support parsing of application/json type post data
app.use(bodyParser.json());

//app.use(express.errorHandler({ dumpExceptions: true, showStack: true }));
//support parsing of application/x-www-form-urlencoded post data
app.use(bodyParser.urlencoded({ extended: true }));

//tell express that www is the root of our public web folder
app.use(express.static("../website"));

//tell express what to do when the /form route is requested
app.post('/profiles',function(req, res){
	console.log(req.body);
	res.set("Connection", "close");
	res.setHeader('Content-Type', 'application/json');

	fs.writeFile("../website/savedData/profiles.json", JSON.stringify(req.body), function(err) {
	    if(err) {
			res.sendStatus(500);
	        return console.log(err);
	    }
	});
	res.sendStatus(200);

});

app.post('/active',function(req, res){
	console.log(req.body);
	res.set("Connection", "close");
	res.setHeader('Content-Type', 'application/json');

	fs.writeFile("../website/savedData/activeParam.json", JSON.stringify(req.body), function(err) {
	    if(err) {
			res.sendStatus(500);
	        return console.log(err);
	    }
	});
	res.sendStatus(200);

});

app.post('/toNav',function(req, res){
	console.log(req.body);
	res.set("Connection", "close");
	res.setHeader('Content-Type', 'application/json');

	fs.writeFile("../website/savedData/toNav.json", JSON.stringify(req.body), function(err) {
	    if(err) {
			res.sendStatus(500);
	        return console.log(err);
	    }
	});
	res.sendStatus(200);

});

//wait for a connection
app.listen(80, function () {
  console.log('Server is running. Point your browser to: http://localhost:80');
});


/*var connect = require('connect');
var serveStatic = require('serve-static');

connect().use(serveStatic('../website/')).listen(8080, function(){
    console.log('Server running on 8080...');
});*/

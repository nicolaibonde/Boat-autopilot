//require the express nodejs module
var express = require('express'),
	//set an instance of exress
	app = express(),
	//require the body-parser nodejs module
	bodyParser = require('body-parser'),
	//require the path nodejs module
	path = require("path");

var fs = require('fs');
var ip = require('ip');

var website_path = __dirname + '/../website'

//support parsing of application/json type post data
app.use(bodyParser.json());

//app.use(express.errorHandler({ dumpExceptions: true, showStack: true }));
//support parsing of application/x-www-form-urlencoded post data
app.use(bodyParser.urlencoded({ extended: true }));

//tell express that www is the root of our public web folder
app.use(express.static(website_path))
//tell express what to do when the /form route is requested
app.post('/profiles',function(req, res){
	console.log(req.body);
	res.set("Connection", "close");
	res.setHeader('Content-Type', 'application/json');

	fs.writeFile(website_path+'/savedData/profiles.json', JSON.stringify(req.body), function(err) {
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

	fs.writeFile(website_path+'/savedData/activeParam.json', JSON.stringify(req.body), function(err) {
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

	fs.writeFile(website_path+'/savedData/toNav.json', JSON.stringify(req.body), function(err) {
	    if(err) {
			res.sendStatus(500);
	        return console.log(err);
	    }
	});
	res.sendStatus(200);

});

//wait for a connection
app.listen(3000, function () {
  console.log('Server is running. Point your browser to: '+ip.address()+':3000');
});

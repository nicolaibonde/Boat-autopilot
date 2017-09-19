var connect = require('connect');
var serveStatic = require('serve-static');

connect().use(serveStatic('../website/')).listen(8080, function(){
    console.log('Server running on 8080...');
});
var fs = require('fs');
fs.writeFile("test.txt", jsonData, function(err) {
    if(err) {
        return console.log(err);
    }
});

var app = angular.module("myApp", ["ngRoute", "leaflet-directive"]);



app.factory('dataHolder', function() {
    return {};
});

//tell express that unhandled rejection shouldn't throw errors
app.config(['$qProvider', function($qProvider) {
    $qProvider.errorOnUnhandledRejections(false);
}]);

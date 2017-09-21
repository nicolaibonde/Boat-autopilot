var app = angular.module("myApp", ["ngRoute", "leaflet-directive"]);
app.config(function($routeProvider) {
    $routeProvider
        .when("/", {
            templateUrl: "home.htm",
        })
        .when("/p2p", {
            templateUrl: "p2p.htm",
        })
        .when("/coverage", {
            templateUrl: "coverage.htm"
        })
        .when("/status", {
            templateUrl: "status.htm"
        })
        .when("/editparameters", {
            templateUrl: "editparameters.htm"
        })
        .otherwise({
            template: "<h1><i class='glyphicon glyphicon-bullhorn'></i> Ups?</h1><p>I can't find that page, sorry</p><p>Maybe call support for me? <i class='glyphicon glyphicon-earphone'></i></p>"
        });
});


app.factory('dataHolder', function() {
    return {};
});

//tell express that unhandled rejection shouldn't throw errors
app.config(['$qProvider', function($qProvider) {
    $qProvider.errorOnUnhandledRejections(false);
}]);

app.controller('navCtrl', function($scope, $location) {
    $scope.selectedTab = $location.path();
});


var app = angular.module("myApp", ["ngRoute"]);
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
app.controller('navCtrl', function($scope, $location) {
	console.log($location.path())
	$scope.selectedTab = $location.path();
});



app.controller('profileCtrl', function($scope, $http) {
	$http.get("profiles.json").then(function(response) {
        $scope.profiles = response.data.profiles;
    });
});

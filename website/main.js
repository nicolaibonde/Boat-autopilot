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
		$scope.selectedProfile = $scope.profiles[0];
    });

	$scope.newProfile = function(){
		$scope.profiles.push({
			name:"New profile",
			parameterNames:[
				"P","I","D","Tool Width"
			],
			parameters:[
				0,0,0,0
			]
		});

		saveProfiles();
	}

	$scope.deleteProfile = function(){
		if($scope.editingParam != true){
			$scope.profiles.splice($scope.profiles.indexOf($scope.selectedProfile), 1)
			$scope.selectedProfile = $scope.profiles[0];
		}else{
			//console.log($scope.profiles.indexOf($scope.selectedProfile));
			window.alert("Can't delete a profile while editing!");
		}
	}

	$scope.editParameters = function(){
		if($scope.editingParam != true){
			$scope.saveIndex = $scope.profiles.indexOf($scope.selectedProfile);
			$scope.profile = $.extend(true, {},$scope.selectedProfile);
			//console.log(JSON.stringify($scope.profile.parameters['right']));
		}else{
			window.alert("Please save before editing a new profile");
		}
		$scope.editingParam = true;
	}

	$scope.activateProfile = function(){
		$scope.activeProfile = $scope.selectedProfile.name;
	}

	$scope.saveProfile = function(){
		$scope.profiles[$scope.saveIndex] = $.extend(true, {},$scope.profile);
		//delete $scope.profile;
		//$scope.profile = $.extend({},$scope.selectedProfile);
		$scope.editingParam = false;
		$scope.editName = false;
	}

	$scope.revertProfile = function(){
		console.log("revert")
		console.log($scope.profile);
		console.log($scope.profiles[$scope.saveIndex]);
		$scope.profile = $.extend(true, {},$scope.profiles[$scope.saveIndex]);
		console.log($scope.profile);
		console.log($scope.profiles[$scope.saveIndex]);
	}

	function saveProfiles(){
		var fs = require('fs');
	}

});

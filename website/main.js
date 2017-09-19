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

	$scope.selectedTab = $location.path();
});



app.controller('profileCtrl', function($scope, $http) {
	$http.get("profiles.json").then(function(response) {
        $scope.profiles = response.data.profiles;
		$scope.selectedProfile = $scope.profiles[0];
    });

	$scope.newProfile = function(){

		$scope.profiles.push({
			name:"New Profile",
			parameterNames:[
				"P","I","D","Tool Width"
			],
			parameters:[
				0,0,0,0
			],
			creationTimestamp:Date.now()
		});
		$scope.selectedProfile = $scope.profiles[($scope.profiles).length-1];

		saveProfiles();
	}

	$scope.deleteProfile = function(){
		if($scope.editingParam != true){
			$scope.profiles.splice($scope.profiles.indexOf($scope.selectedProfile), 1)
			$scope.selectedProfile = $scope.profiles[0];
		}else{
			window.alert("Can't delete a profile while editing!");
		}
	}

	$scope.editParameters = function(){
		if($scope.editingParam != true){

			$scope.revertingProfile = $.extend(true, {},$scope.selectedProfile);
			$scope.profile = $scope.selectedProfile;

		}else{
			window.alert("Please save before editing a new profile");
		}
		$scope.editingParam = true;
	}

	$scope.activateProfile = function(){
		$scope.activeProfile = $scope.selectedProfile.name;
	}

	$scope.saveProfile = function(){
		$scope.editingParam = false;
		$scope.editName = false;
	}

	$scope.revertProfile = function(){
		$scope.profile = $.extend(true, {},$scope.revertingProfile);
	}

	function saveProfiles(){
	}

});

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

app.factory('dataHolder',function(){
	return{};
});

//tell express that unhandled rejection shouldn't throw errors
app.config(['$qProvider', function($qProvider) {
    $qProvider.errorOnUnhandledRejections(false);
}]);

app.controller('navCtrl', function($scope, $location) {
    $scope.selectedTab = $location.path();
});

app.controller('p2pCtrl', function($scope, $http, dataHolder){
	$scope.cachedData = dataHolder;
	$scope.test = 5;
	$scope.cachedData.currentBoatPose = {latitude:56.1719590,longitude:10.1916530,orientation:0};

});

app.controller('coverageCtrl', function($scope, $http, dataHolder){
	$scope.cachedData = dataHolder;

});

app.controller('profileCtrl', function($scope, $http, dataHolder) {
	$scope.cachedData = dataHolder;

    $http.get("profiles.json").then(function(response) {
        $scope.profiles = response.data.profiles;
        $scope.profileData = response.data;
        $scope.selectedProfile = $scope.profiles[0];
    }).catch(function(data){
		$scope.profileData = {"profiles":[]};
		$scope.profiles = $scope.profileData.profiles;
	});

	$http.get("activeParam.json").then(function(response) {
        $scope.activeProfile = response.data;
    });

    $scope.newProfile = function() {

        $scope.profiles.push({
            name: "New Profile",
            parameterNames: [
                "P", "I", "D", "Tool Width"
            ],
            parameters: [
                0, 0, 0, 0
            ],
            creationTimestamp: Date.now()
        });
        $scope.saveProfilesToFile();
        $scope.selectedProfile = $scope.profiles[($scope.profiles).length - 1];
    }

    $scope.deleteProfile = function() {
        if ($scope.editingParam != true) {
            $scope.profiles.splice($scope.profiles.indexOf($scope.selectedProfile), 1)
            $scope.selectedProfile = $scope.profiles[0];
			$scope.saveProfilesToFile();
        } else {
            //window.alert("Can't delete a profile while editing!");
        }

    }

    $scope.editParameters = function() {
        if ($scope.editingParam != true) {

            $scope.revertingProfile = $.extend(true, {}, $scope.selectedProfile);
            $scope.profile = $scope.selectedProfile;
			$scope.saveProfilesToFile();
        } else {
            //window.alert("Please save before editing a new profile");
        }

        $scope.editingParam = true;
    }

    $scope.activateProfile = function() {
        $scope.activeProfile = $scope.selectedProfile;
		$scope.saveActiveProfileToFile();
    }

    $scope.saveProfile = function() {
        $scope.editingParam = false;
        $scope.editName = false;
        $scope.saveProfilesToFile();
    }

    $scope.revertProfile = function() {
        $scope.profile = $.extend(true, {}, $scope.revertingProfile);
    }

    $scope.hoverIn = function() {
        this.hoverEdit = true;
    };

    $scope.hoverOut = function() {
        this.hoverEdit = false;
    };

    $scope.addParameter = function() {
        if (this.newParamName != undefined) {
            $scope.profile.parameters.push(0);
            $scope.profile.parameterNames.push(this.newParamName);
            this.newParamName = undefined;
        }

    }

    $scope.removeParameter = function() {
        $scope.profile.parameters.splice(this.$index, 1);
        $scope.profile.parameterNames.splice(this.$index, 1);
    }

    $scope.saveProfilesToFile = function() {
        var url = "profiles";
        var parameter = angular.toJson($scope.profileData);
        $http.post(url, parameter).
        then(function(data, status, headers, config) {
        })
    }

    $scope.saveActiveProfileToFile = function() {
		var url = "active";
        var parameter = angular.toJson($scope.activeProfile);
        $http.post(url, parameter).
        then(function(data, status, headers, config) {
        })
    }
});

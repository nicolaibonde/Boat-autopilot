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

app.controller('p2pCtrl', function($scope, $http, dataHolder) {
    $scope.cachedData = dataHolder;

    console.log($scope.cachedData);

    $scope.test = 5;
    $scope.cachedData.currentBoatPose = {
        latitude: 56.1719590,
        longitude: 10.1916530,
        orientation: 0
    };
    if ($scope.cachedData.boatPosition == undefined) {
        $scope.cachedData.boatPosition = {

            lat: $scope.cachedData.currentBoatPose.latitude,
            lng: $scope.cachedData.currentBoatPose.longitude,
            zoom: 18
        };
    }
    $scope.cachedData.events = {
        map: {
            enable: ['click'],
            logic: 'emit'
        }
    };
    $scope.cachedData.targetIcon = {
        iconUrl: 'images/target.png',
        iconSize: [32, 32], // size of the icon
        iconAnchor: [16, 16], // point of the icon which will correspond to marker's location
        //popupAnchor:  [-3, -76] // point from which the popup should open relative to the iconAnchor
    }

    $scope.cachedData.boatIcon = {
        iconUrl: 'images/boat.png',
        iconSize: [32, 32], // size of the icon
        iconAnchor: [16, 16], // point of the icon which will correspond to marker's location
        //popupAnchor:  [-3, -76] // point from which the popup should open relative to the iconAnchor
    }

    if ($scope.cachedData.markers == undefined) {
        $scope.cachedData.markers = new Array();
        $scope.cachedData.markers[1] = {
            lat: 0,
            lng: 0,
            icon: $scope.cachedData.targetIcon,
            opacity: 0
        }
    }
	$scope.cachedData.markers[0] = {
		lat: $scope.cachedData.currentBoatPose.latitude,
		lng: $scope.cachedData.currentBoatPose.longitude,
		icon: $scope.cachedData.boatIcon,
		iconAngle: $scope.cachedData.currentBoatPose.orientation,
		opacity: 1
	}

    if ($scope.cachedData.p2pPaths == undefined) {
        $scope.cachedData.p2pPaths = {
            p1: {
                color: 'lime',
                weight: 2,
                latlngs: [{
                        lat: $scope.cachedData.currentBoatPose.latitude,
                        lng: $scope.cachedData.currentBoatPose.longitude
                    },
                    {
                        lat: $scope.cachedData.markers[1].lat,
                        lng: $scope.cachedData.markers[1].lng
                    }
                ],
                opacity: 0
            }
        }
    }
    console.log($scope.cachedData.p2pPaths.p1)

    $scope.$on('leafletDirectiveMap.click', function(event, args) {
        var leafEvent = args.leafletEvent;

        $scope.cachedData.markers[1] = {
            lat: leafEvent.latlng.lat,
            lng: leafEvent.latlng.lng,
            icon: $scope.cachedData.targetIcon,
            opacity: 1
        }
        console.log($scope.cachedData.p2pPaths.p1)
        $scope.updateLine();
        console.log($scope.cachedData)
        console.log('clicked on map');
        $scope.eventDetected = "Click";
    });

    $scope.updateLine = function() {
        $scope.cachedData.p2pPaths.p1.latlngs[0].lat = $scope.cachedData.currentBoatPose.latitude;
        $scope.cachedData.p2pPaths.p1.latlngs[0].lng = $scope.cachedData.currentBoatPose.longitude;
        $scope.cachedData.p2pPaths.p1.latlngs[1].lat = $scope.cachedData.markers[1].lat;
        $scope.cachedData.p2pPaths.p1.latlngs[1].lng = $scope.cachedData.markers[1].lng;
        $scope.cachedData.p2pPaths.p1.opacity = 1;
    }
});

app.controller('coverageCtrl', function($scope, $http, dataHolder) {
    $scope.cachedData = dataHolder;

});

app.controller('profileCtrl', function($scope, $http, dataHolder) {
    $scope.cachedData = dataHolder;

    $http.get("profiles.json").then(function(response) {
        $scope.profiles = response.data.profiles;
        $scope.profileData = response.data;
        $scope.selectedProfile = $scope.profiles[0];
    }).catch(function(data) {
        $scope.profileData = {
            "profiles": []
        };
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
        then(function(data, status, headers, config) {})
    }

    $scope.saveActiveProfileToFile = function() {
        var url = "active";
        var parameter = angular.toJson($scope.activeProfile);
        $http.post(url, parameter).
        then(function(data, status, headers, config) {})
    }
});

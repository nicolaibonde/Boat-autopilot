app.controller('p2pCtrl', function($scope, $http, dataHolder, leafletMarkerEvents, $interval) {

    initP2PMap = function() {

        //Set up map center
        if ($scope.cachedData.mapCenter == undefined) {
            $scope.cachedData.mapCenter = {

                lat: $scope.cachedData.currentBoatPose.latitude,
                lng: $scope.cachedData.currentBoatPose.longitude,
                zoom: 14,

            };
        }

        $scope.mapControls = {
            scale: true
        }

        //Setup what events are allowed on the map
        $scope.cachedData.events = {
            map: {
                enable: ['click'],
                logic: 'emit'
            }
        };

        //Loading the target icon
        $scope.cachedData.targetIcon = {
            iconUrl: 'images/target.png',
            iconSize: [32, 32],
            iconAnchor: [16, 16],
        }

        //Loading the boat icon
        $scope.cachedData.boatIcon = {
            iconUrl: 'images/boat.png',
            iconSize: [32, 32],
            iconAnchor: [16, 16],
        }

        //Setting up the markers
        if ($scope.cachedData.markers == undefined) {
            $scope.cachedData.markers = new Array();
            //Marker 1 is the target marker, set up to be invisible at 0 0
            $scope.cachedData.markers[1] = {
                lat: 0,
                lng: 0,
                icon: $scope.cachedData.targetIcon,
                opacity: 0,
                message: "target",
                draggable: true
            }
        }
        //Boat marker, set to the boats current position
        $scope.cachedData.markers[0] = {
            lat: $scope.cachedData.currentBoatPose.latitude,
            lng: $scope.cachedData.currentBoatPose.longitude,
            icon: $scope.cachedData.boatIcon,
            iconAngle: $scope.cachedData.currentBoatPose.orientation,
            opacity: 1,
            message: "boat"
        }


        //Setup the path from the boat to the boat, it is transparent at the start
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
                    opacity: 0,
                }
            }
        }

    }


    $scope.$on('leafletDirectiveMap.click', function(event, args) {
        var leafEvent = args.leafletEvent;

        $scope.updateMapData(leafEvent.latlng.lat, leafEvent.latlng.lng);
    });

    $scope.$on("leafletDirectiveMarker.drag", function(event, args) {
        var point = args.leafletEvent.target._leaflet_events.drag[0].context._latlng;
        $scope.updateMapData(point.lat, point.lng);
        //console.log(" lat, lng = " + point.lat + ", " + point.lng);
    });

    updateLine = function() {
        $scope.cachedData.p2pPaths.p1.latlngs[0].lat = $scope.cachedData.currentBoatPose.latitude;
        $scope.cachedData.p2pPaths.p1.latlngs[0].lng = $scope.cachedData.currentBoatPose.longitude;
        $scope.cachedData.p2pPaths.p1.latlngs[1].lat = $scope.cachedData.markers[1].lat;
        $scope.cachedData.p2pPaths.p1.latlngs[1].lng = $scope.cachedData.markers[1].lng;
        $scope.cachedData.p2pPaths.p1.opacity = 1;
    }

    updateTarget = function(lat, lng) {
        if (lat != undefined) {
            $scope.cachedData.markers[1].lat = lat;
        }
        if (lng != undefined) {
            $scope.cachedData.markers[1].lng = lng;
        }
        $scope.cachedData.markers[1].opacity = 1;
    };

	updateBoat = function(){
		$scope.cachedData.markers[0].lat = $scope.cachedData.currentBoatPose.latitude;
		$scope.cachedData.markers[0].lng = $scope.cachedData.currentBoatPose.longitude;
		$scope.cachedData.markers[0].iconAngle = $scope.cachedData.currentBoatPose.orientation;

	}

    $scope.updateMapData = function(lat, lng) {
        updateTarget(lat, lng);
        updateLine();
		updateBoat();
    };

    getDataFromNav = function() {
		//console.log('getting nav data');
		$scope.cachedData.currentBoatPose = {
			//Just a default boat position, mostly for testing,
			//because there should always be a fromNav.json file.
			latitude: 56.1719590,
			longitude: 10.1916530,
			orientation: 0
		};

        return $http.get("../savedData/fromNav.json").then(function(response) {
            //Parse fromNav.json, add what is need to be know
            $scope.cachedData.currentBoatPose = response.data.telemetry //For now this works because telemtry is only the pose
            //console.log($scope.cachedData.currentBoatPose);
			updateBoat();

        }).catch(function(data) {
            //console.log('not found')
            $scope.cachedDatacachedData.currentBoatPose = {
                //Just a default boat position, mostly for testing,
                //because there should always be a fromNav.json file.
                latitude: 56.1719590,
                longitude: 10.1916530,
                orientation: 0
            };
        });
    }

	$scope.cachedData = dataHolder;

    getDataFromNav()
    initP2PMap();

	$interval(getDataFromNav, 5000) //Update frequency for the boat data

});

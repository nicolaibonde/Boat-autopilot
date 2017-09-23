app.controller('p2pCtrl', function($scope, $http, dataHolder, leafletMarkerEvents, $interval, $timeout) {

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


    initP2P = function(){
      if($scope.cachedData.p2pCalcButton == undefined){
    		$scope.cachedData.calcState = 0;
    		$scope.cachedData.p2pCalcButton = {
    	        text: "Calculate Path",
    	        class: "btn-warning",
    	        icon: "glyphicon glyphicon-flash"
    	    }
    		$scope.cachedData.etaProgress = {
    			progress: 0,
    			time: "",
    			class:"active"
    		};
    	}

        getDataFromNav()
        initP2PMap();
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

    updateBoat = function() {
        $scope.cachedData.markers[0].lat = $scope.cachedData.currentBoatPose.latitude;
        $scope.cachedData.markers[0].lng = $scope.cachedData.currentBoatPose.longitude;
        $scope.cachedData.markers[0].iconAngle = $scope.cachedData.currentBoatPose.orientation;

    }

    $scope.updateMapData = function(lat, lng) {
        updateTarget(lat, lng);
        updateLine();
        updateBoat();
    };

    $scope.calculatePathP2P = function() {
        switch ($scope.cachedData.calcState) {
            case 0:
                $scope.cachedData.targetMissing = false;
                if ($scope.cachedData.markers[1].opacity != 0) {
                    var url = "toNav";
                    var calculatePath = {
                        func: "calcP2P",
                        targetPosition: {
                            latitude: $scope.cachedData.markers[1].lat,
                            longitude: $scope.cachedData.markers[1].lng
                        }
                    }
                    var data = angular.toJson(calculatePath);
                    $http.post(url, data).then(function(data, status, headers, config) {
                        //Output data to check if this is successfull
                    })
                    $scope.cachedData.etaProgress.progress = 0;
                    $scope.cachedData.etaProgress.time = "";
      							$scope.cachedData.etaProgress.class= ""
                    $scope.cachedData.p2pCalcButton = {
                        text: "Calculating ",
                        class: "btn-warning disabled",
                        icon: "fa fa-spinner fa-spin"
                    }
					$scope.p2ptimeout = $timeout(function() {
						$scope.cachedData.calcState = 1;
						$scope.calculatePathP2P()
					}, 2000);
        }else{
          $scope.cachedData.targetMissing = true;
        }
                break;
            case 1:
                $scope.cachedData.p2pCalcButton = {
                    text: "Start ",
                    class: "btn-success",
                    icon: "glyphicon glyphicon-play"
                }
                $scope.cachedData.calcState = 2
                break;
            case 2:
                $scope.cachedData.p2pCalcButton = {
                    text: "Running",
                    class: "btn-success disabled",
                    icon: "fa fa-spinner fa-spin"
                }

				$scope.cachedData.etaProgress.time ="Calculating ...";
				//$scope.cachedData.etaProgress.class="active";
				var promise = $interval(function(){

					if($scope.cachedData.calcState !=2){
						$interval.cancel(promise);
					}else{
						if($scope.cachedData.etaProgress.progress >= 100){
							$scope.cachedData.calcState = 100
							$scope.cachedData.etaProgress.time = "Arrived at target";
							$scope.cachedData.etaProgress.class= "progress-bar-success"
							$scope.cachedData.calcState = 3;
							$scope.calculatePathP2P()
							$interval.cancel(promise);

						}else{
							$scope.cachedData.etaProgress.progress +=1;
						}

					}
				}, 100)
                break;

            case 3:
                $scope.cachedData.p2pCalcButton = {
                    text: "Calculate Path",
                    class: "btn-warning",
                    icon: "glyphicon glyphicon-flash"
                }
				$scope.cachedData.etaProgress.class= "progress-bar-success"
                $scope.cachedData.calcState = 0
                break;
        }
    }

    $scope.stopPathP2P = function() {
        $timeout.cancel($scope.p2ptimeout);
        $scope.cachedData.p2pCalcButton = {
            text: "Calculate Path",
            class: "btn-warning",
            icon: "glyphicon glyphicon-flash"
        }
        $scope.cachedData.calcState = 0
		      $scope.cachedData.etaProgress.time = "Canceled";
		        $scope.cachedData.etaProgress.class= "progress-bar-danger"
    }


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



    initP2P();


    $interval(getDataFromNav, 1000); //Update frequency for the boat data

});

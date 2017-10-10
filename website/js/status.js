app.controller('Status', function($scope, $http, dataHolder, $interval, $sce) {

    getDataFromNav = function(path) {
        return $http.get(path).then(function(response) { //Does a http GET call and returns a promise.
            $scope.Boat_pose_ = response.data.Telemetry_; //Reads the Boat_pose_
            updateStatusDisplay(response.data.Status_, response.data.Timestamp_); //Updates the html panels.
        });
    }

    updateStatusDisplay = function(status, timestamp) {
        let html = ""; //Creates a variable containing the html
        status.forEach(function(current_value) {
			//For each status object it creates a panel with the necessary data
            html += `
      <div class="panel panel-default">
      	<div class="panel-heading">
      		<h3 class="panel-title">` + current_value.title_ + `</h3>
      		</div>
      		<div class="panel-body">`;

            current_value.items_.forEach(function(item) {
                html += "<strong>" + item.title_ + ":</strong> " + item.data_ + " " + item.unit_ + "<br />"
                if (item.unit_ == '%') {
                    html += `<div class="progress">
                  			<div class="progress-bar ` + item.color_ + `  " role="progressbar" aria-valuenow="0" aria-valuemin="0" aria-valuemax="100" style="width: ` + item.data_ + `%">
                  			</div>
                  		</div>`;
                }
            });

			//Adds ending html
            html += `
      		</div>
      	</div>
      </div>`
        })
        $scope.Status_panels_ = $sce.trustAsHtml(html); //Sets the panels to the html created and trusts it, maybe not good for security

		//Updates the time size last data panel.
        $scope.Time_since_last_data_.time_ = Math.round((Date.now() - timestamp) / 1000);
        if ($scope.Time_since_last_data_.time_ > 20) {
            $scope.Time_since_last_data_.warning_ = "alert-danger";
        } else if ($scope.Time_since_last_data_.time_ > 10) {
            $scope.Time_since_last_data_.warning_ = "alert-warning";
        } else {
            $scope.Time_since_last_data_.warning_ = "alert-info";
        }


    };

	 //Setting up leaflet map with appropriate settings
    setupMap = function() {

		//Centers the map on the boat
        $scope.mapCenter = {
            lat: $scope.Boat_pose_.latitude_,
            lng: $scope.Boat_pose_.longitude_,
            zoom: 14,
        };

		//Adds scale markers to the map
        $scope.mapControls = {
            scale: true
        }

        //Setup what events are allowed on the map
        $scope.events = {
            map: {
                enable: ['click'],
                logic: 'emit'
            }
        };

        //Loading the boat icon
        boat_icon_ = {
            iconUrl: 'images/boat.png',
            iconSize: [32, 32],
            iconAnchor: [16, 16],
        }

        //Setting up the marker
        if ($scope.markers == undefined) {
            $scope.markers = new Array();
        }
        //Boat marker, set to the boats current position
        $scope.markers[0] = {
            lat: $scope.Boat_pose_.latitude_,
            lng: $scope.Boat_pose_.longitude_,
            icon: boat_icon_,
            iconAngle: $scope.Boat_pose_.orientation_,
        }
    }

	//This updates the map postion on the map, also recenters the map on the boat.
    updateBoatPose = function(pose) {
		//Updates the marker position
        $scope.markers[0] = {
            lat: pose.latitude_,
            lng: pose.longitude_,
            icon: boat_icon_,
            iconAngle: pose.orientation_,
        }

		//Map centering
        $scope.mapCenter = {
            lat: $scope.Boat_pose_.latitude_,
            lng: $scope.Boat_pose_.longitude_,
            zoom: $scope.mapCenter.zoom,
        };
    }

	//Creates the Time_since_last_data_ object with default values
    $scope.Time_since_last_data_ = {
        warning_: "alert-info",
        time_: NaN,
    }
	//The interval of reading data and updating
    let update_status_interval_ = 1000;

    $scope.mapCenter = {
        lat: 0,
        lng: 0,
        zoom: 0,
    };

	//Get data from nav and setup the map when its here
    getDataFromNav("../savedData/fromNav.json").then(function() {
        setupMap();
    });


	//Gets data every interval and updates the map and panels
    let intervalPromise = $interval(function() {
        getDataFromNav("../savedData/fromNav.json");
        updateBoatPose($scope.Boat_pose_);

    }, update_status_interval_);

	$scope.$on('$destroy',function(){
    if(intervalPromise)
        $interval.cancel(intervalPromise);
	});

});

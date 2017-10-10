app.controller("PointToPoint", function($scope, $http, dataHolder, leafletMarkerEvents, $interval, $timeout, $q) {

  setup = function() {
    $scope.Cached_data_.action_state_ = 0;
    $scope.Cached_data_.p2p_Action = {
      text: "Calculate Path",
      class: "btn-warning",
      icon: "glyphicon glyphicon-flash"
    }
    $scope.Cached_data_.ETE_ = {
      progress: 0,
      time: "",
      class: "active"
    };
    setupMap();
  }

  setupMap = function() {
    //Set up map center
    $scope.Cached_data_.map_center_ = {
      lat: $scope.Cached_data_.Boat_pose_.latitude_,
      lng: $scope.Cached_data_.Boat_pose_.longitude_,
      zoom: 14
    };

    //Setup map controls, scale
    $scope.map_controls_ = {
      scale: true
    }

    //Loading the target icon
    $scope.Cached_data_.target_icon_ = {
      iconUrl: "images/target.png",
      iconSize: [32, 32],
      iconAnchor: [16, 16]
    }

    //Loading the boat icon
    $scope.Cached_data_.boat_icon_ = {
      iconUrl: "images/boat.png",
      iconSize: [32, 32],
      iconAnchor: [16, 16]
    }

    //Setting up the markers_p2p_
    if ($scope.Cached_data_.markers_p2p_ == undefined) {
      $scope.Cached_data_.markers_p2p_ = new Array();
      //Marker 1 is the target marker, set up to be invisible at 0 0
      $scope.Cached_data_.markers_p2p_[1] = {
        lat: 0,
        lng: 0,
        icon: $scope.Cached_data_.target_icon_,
        opacity: 0,
        draggable: true
      }
    }
    //Boat marker, set to the boats current position
    $scope.Cached_data_.markers_p2p_[0] = {
      lat: $scope.Cached_data_.Boat_pose_.latitude_,
      lng: $scope.Cached_data_.Boat_pose_.longitude_,
      icon: $scope.Cached_data_.boat_icon_,
      iconAngle: $scope.Cached_data_.Boat_pose_.orientation_,
      opacity: 1,
    }
  }

  $scope.$on("leafletDirectiveMap.click", function(event, args) {
    updateEndCoord(args.leafletEvent.latlng);
  });

  $scope.$on("leafletDirectiveMarker.drag", function(event, args) {
    let point = args.leafletEvent.target._leaflet_events.drag[0].context._latlng;
    updateEndCoord(point);
  });

  updateEndCoord = function(end_coord) {
    if (end_coord.lat != undefined) {
      $scope.Cached_data_.markers_p2p_[1].lat = end_coord.lat;
    }
    if (end_coord.lng != undefined) {
      $scope.Cached_data_.markers_p2p_[1].lng = end_coord.lng;
    }
    $scope.Cached_data_.markers_p2p_[1].opacity = 1;
  };

  updateBoatPose = function(pose) {
    $scope.Cached_data_.Boat_pose_ = pose;
  }

  $scope.Action = function() {
    switch ($scope.Cached_data_.action_state_) {
      case 0: //User presses Calculate path
        //Check if a marker has been placed
        if ($scope.Cached_data_.markers_p2p_[1].opacity != 0) {
          $scope.Cached_data_.target_missing = false;
          $scope.Cached_data_.ETE_.progress = 0;
          $scope.Cached_data_.ETE_.time = "";
          $scope.Cached_data_.ETE_.class = "active";
          $scope.Cached_data_.p2p_Action = {
            text: "Calculating ",
            class: "btn-warning disabled",
            icon: "fa fa-spinner fa-spin"
          };

          calculatePath($scope.Cached_data_.markers_p2p_[1]);

        } else {
          //No marker has been placed
          $scope.Cached_data_.target_missing = true;
        }
        break;
      case 1: //Change icon to indicate that a path has been received
        $scope.Cached_data_.p2p_Action = {
          text: "Start ",
          class: "btn-success",
          icon: "glyphicon glyphicon-play"
        }
        $scope.Cached_data_.action_state_ = 2
        break;
      case 2: //User presses Start, to begin traversing the path
        $scope.Cached_data_.p2p_Action = {
          text: "Running",
          class: "btn-success disabled",
          icon: "fa fa-spinner fa-spin"
        }

        start();

        break;
      case 3: //The path has been traversed successfully
        $scope.Cached_data_.p2p_Action = {
          text: "Calculate Path",
          class: "btn-warning",
          icon: "glyphicon glyphicon-flash"
        }
        $scope.Cached_data_.ETE_.class = "progress-bar-success"
        $scope.Cached_data_.action_state_ = 0
        break;
    }
  }

  calculatePath = function(end_coord) {

    saveToFile("toNav", {
      "func": "calcP2P",
      "targetPosition": {
        latitude_: end_coord.lat,
        longitude_: end_coord.lng
      }
    });

    waitForPath();
    //Return when the Path object has been updated by the controller
  }

  waitForPath = function() {
    let currentTimestamp = $scope.Cached_data_.Path_.timestamp_;
    $scope.waitForPathPromise = $interval(function() {
      getDataFromNav("../savedData/fromNav.json");
      $http.get("../savedData/fromNav.json").then(function(response) {
        $scope.Cached_data_.Path_ = response.data.Path_;
        if (currentTimestamp < $scope.Cached_data_.Path_.timestamp_) {
          $interval.cancel($scope.waitForPathPromise);
          $scope.Cached_data_.action_state_ = 1;

          $scope.Action();
        }
      });
    }, 1000); //Update frequency for the boat data
  }

  start = function() {
    waitForCompletion()
  }

  waitForCompletion = function() {
    $scope.Cached_data_.ETE_.time = "Calculating ...";
    //$scope.Cached_data_.ETE_.class="active";
    let promise = $interval(function() {
      if ($scope.Cached_data_.action_state_ != 2) {
        $interval.cancel(promise);
      } else {
        if ($scope.Cached_data_.ETE_.progress >= 100) {
          $scope.Cached_data_.ETE_.time = "Arrived at target";
          $scope.Cached_data_.ETE_.class = "progress-bar-success"
          $scope.Cached_data_.action_state_ = 3;
          $scope.Action()
          $interval.cancel(promise);
        } else {
          $scope.Cached_data_.ETE_.progress += 1;
        }

      }
    }, 100)
  }

  $scope.Stop = function() {
    if($scope.waitForPathPromise){
        $interval.cancel($scope.waitForPathPromise);
      }
    $timeout.cancel($scope.p2ptimeout);
    $scope.Cached_data_.p2p_Action = {
      text: "Calculate Path",
      class: "btn-warning",
      icon: "glyphicon glyphicon-flash"
    }

    //Update command file
    saveToFile("toNav", {
      "func": "stop"
    });

    $scope.Cached_data_.action_state_ = 0
    $scope.Cached_data_.ETE_.time = "Canceled";
    $scope.Cached_data_.ETE_.class = "progress-bar-danger"
  }

  getDataFromNav = function(path) {
    return $http.get(path).then(function(response) {
      //Parse fromNav.json, add telemetry data
      updateBoatPose(response.data.Telemetry_);
      $scope.Cached_data_.Path_ = response.data.Path_;
    }).catch(function(data) {
      $scope.Cached_data_.Boat_pose_ = {
        //Default boat position, for testing purposes
        latitude_: 0,
        longitude_: 0,
        orientation_: 0
      };
    });
  }

  saveToFile = function(url, content) {
    $http.post(url, angular.toJson(content)). //Build a http POST call with the url and content
    then(function(data, status, headers, config) {}) //No error handling
  }

  //Program start, set dataHolder
  $scope.Cached_data_ = dataHolder;

  //Center map to null island
  $scope.Cached_data_.map_center_ = {
    lat: 0,
    lng: 0,
    zoom: 0,
  };

  //Must be declared before running setup
  //Setup what events are allowed on the map
  $scope.Cached_data_.events_ = {
    map: {
      enable: ["click"],
      logic: "emit"
    }
  };

  //Get data from nav and run setup when it"s loaded
  getDataFromNav("../savedData/fromNav.json").then(function() {
    setup();
  });

  //Main loop polls fromNav.json
  let mainPromise = $interval(function() {
    getDataFromNav("../savedData/fromNav.json");
  }, 1000); //Update frequency for the boat data

  $scope.$on('$destroy',function(){
    if(mainPromise)
        $interval.cancel(mainPromise);
    if($scope.waitForPathPromise)
        $interval.cancel($scope.waitForPathPromise);
  });
});

app.controller('coverageCtrl', function($scope, $http, dataHolder, leafletMapEvents, leafletMarkerEvents, $interval, $timeout) {
    $scope.Cached_data_ = dataHolder;

    $scope.Action = function() {
        switch ($scope.Cached_data_.action_state_) {
            case 0: //User presses Calculate path
                //Check if markers has been placed
                if ($scope.Cached_data_.markers_[1].opacity != 0 && $scope.Cached_data_.markers_[2].opacity != 0) {
                    $scope.target_missing = false;
                    $scope.Cached_data_.ETE_.progress_ = 0;
                    $scope.Cached_data_.ETE_.time_ = "";
                    $scope.Cached_data_.ETE_.class_ = "active";
                    $scope.Cached_data_.coverage_action_ = {
                        text_: "Calculating ",
                        class_: "btn-warning disabled",
                        icon_: "fa fa-spinner fa-spin"
                    };

                    calculatePath($scope.Cached_data_.markers_[1], $scope.Cached_data_.markers_[2]);

                } else {
                    //Missing a maker
                    $scope.target_missing = true;
                }
                break;
            case 1: //Change icon to indicate that a path has been received
                $scope.Cached_data_.coverage_action_ = {
                    text_: "Start ",
                    class_: "btn-success",
                    icon_: "glyphicon glyphicon-play"
                }
                $scope.Cached_data_.action_state_ = 2
                break;
            case 2: //User presses Start, to begin traversing the path
                $scope.Cached_data_.coverage_action_ = {
                    text_: "Running",
                    class_: "btn-success disabled",
                    icon_: "fa fa-spinner fa-spin"
                }

                start();

                break;
            case 3: //The path has been traversed successfully
                $scope.Cached_data_.coverage_action_ = {
                    text_: "Calculate Path",
                    class_: "btn-warning",
                    icon_: "glyphicon glyphicon-flash"
                }
                $scope.Cached_data_.ETE_.class = "progress-bar-success"
                $scope.Cached_data_.action_state_ = 0
                break;
        }
    }

    calculatePath = function(start_coord, end_coord) {

        //Saving to function and coordinates to  a file so the navigation can use it.
        saveToFile("toNav", {
            "func_": "calcCoverage",
            "coverage_rectangle_": {
                start_coord_: {
                    latitude_: start_coord.lat,
                    longitude_: start_coord.lng
                },
                end_coord: {
                    latitude_: end_coord.lat,
                    longitude_: end_coord.lng
                }
            }
        });

        //Wait for response from navigation
        waitForPath();
    };

    waitForPath = function() {

        let current_timestamp = $scope.Cached_data_.Path_.timestamp_; //The time stamp of the path from the last navigation

        $scope.waitForPathPromise = $interval(function() { //At an interval get the nav data and check if there is a new path
            getDataFromNav("../savedData/fromNav.json");
            $http.get("../savedData/fromNav.json").then(function(response) {
                $scope.Cached_data_.Path_ = response.data.Path_;
                if (current_timestamp < $scope.Cached_data_.Path_.timestamp_) {
                    $interval.cancel($scope.waitForPathPromise);
                    $scope.Cached_data_.action_state_ = 1;

                    $scope.Action();
                }
            });
        }, wait_for_path_interval_); //Update frequency for the boat data
    }

    start = function() {
        saveToFile("toNav", {
            "func_": "start"
        });
        waitForCompletion();
    }


    waitForCompletion = function() {
        //Wait untill progress is 100%
        $scope.Cached_data_.ETE_.time = "Calculating ...";
        $scope.waitForCompletionPromise = $interval(function() {
            if ($scope.Cached_data_.ETE_.progress_ >= 100) {
                $scope.Cached_data_.ETE_.time_ = "Arrived at target";
                $scope.Cached_data_.ETE_.class_ = "progress-bar-success"
                $scope.Cached_data_.action_state_ = 3;
                $scope.Action()
                $interval.cancel($scope.waitForCompletionPromise);
            } else {
                getDataFromNav("../savedData/fromNav.json");
                $http.get("../savedData/fromNav.json").then(function(response) {
                    $scope.Cached_data_.ETE_.progress_ = response.data.Progress_
                });
            }
        }, wait_for_completion_interval_)
    }


    $scope.Stop = function() {
        if ($scope.waitForPathPromise) {
            $interval.cancel($scope.waitForPathPromise);
        }
        if ($scope.waitForCompletionPromise) {
            $interval.cancel($scope.waitForCompletionPromise);
        }
        $timeout.cancel($scope.timeout);
        $scope.Cached_data_.coverage_action_ = {
            text_: "Calculate Path",
            class_: "btn-warning",
            icon_: "glyphicon glyphicon-flash"
        }

        //Update command file
        saveToFile("toNav", {
            "func_": "stop"
        });

        $scope.Cached_data_.action_state_ = 0
        $scope.Cached_data_.ETE_.time_ = "Canceled";
        $scope.Cached_data_.ETE_.class_ = "progress-bar-danger"
    }


    //Makes sure that the cursors are updated/added when you change the variables in the input fields
    $scope.UpdateMapData = function() {
        updateStartCoord($scope.Cached_data_.markers_[1].lat, $scope.Cached_data_.markers_[1].lng);
        updateEndCoord($scope.Cached_data_.markers_[2].lat, $scope.Cached_data_.markers_[2].lng);
    }

    //Gets telemetry and path from the navigation
    getDataFromNav = function(path) {
        return $http.get(path).then(function(response) {
            updateBoatPose(response.data.Telemetry_);
            $scope.Cached_data_.Path_ = response.data.Path_;

        }).catch(function(data) {
            let pose = {
                //Just a default boat position, mostly for testing,
                //because there should always be a fromNav.json file.
                latitude_: 0,
                longitude_: 0,
                orientation_: 0
            };
            updateBoatPose(pose);

        });
    }

    //Sets up variables thate does not need the boat pose
    setup = function() {

        $scope.layers_ = {
            baselayers: {
                offline: {
                    name: 'offline_map',
                    options: {
                        attribution: "",
                        maxZoom: 14
                    },
                    url: 'offline_map/{z}/{x}/{y}.png',
                    type: 'xyz',
                    refresh: true
                },
                osm: {
                    name: 'OpenStreetMap',
                    url: 'http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png',
                    type: 'xyz'
                }
            }
        }
        $scope.Cached_data_markers_ = new Array();

        if ($scope.Cached_data_.action_state_ == undefined) {
            $scope.Cached_data_.action_state_ = 0;
        }

        if ($scope.Cached_data_.coverage_action_ == undefined) {
            $scope.Cached_data_.coverage_action_ = {
                text_: "Calculate Path",
                class_: "btn-warning",
                icon_: "glyphicon glyphicon-flash"
            }
        }
        if ($scope.Cached_data_.ETE_ == undefined) {
            $scope.Cached_data_.ETE_ = {
                progress_: 0,
                time_: "",
                class_: "active"
            };
        }

        if ($scope.Cached_data_.map_center_ == undefined) {
            $scope.Cached_data_.map_center_ = {
                lat: 0,
                lng: 0,
                zoom: 0,
            };
        }

        //Loading the target icon
        $scope.Cached_data_.target_icon_ = {
            iconUrl: 'images/target.png',
            iconSize: [32, 32],
            iconAnchor: [16, 16],
        }

        //Loading the boat icon
        $scope.Cached_data_.boat_icon_ = {
            iconUrl: 'images/boat.png',
            iconSize: [32, 32],
            iconAnchor: [16, 16],
        }

        //Setting up the markers
        if ($scope.Cached_data_.markers_ == undefined) {
            $scope.Cached_data_.markers_ = new Array();
            //Marker 1 is the target marker, set up to be invisible at 0 0
            $scope.Cached_data_.markers_[1] = {
                lat: 0,
                lng: 0,
                icon: $scope.Cached_data_.target_icon_,
                opacity: 0,
                message: "target",
                draggable: true
            }

            $scope.Cached_data_.markers_[2] = {
                lat: 0,
                lng: 0,
                icon: $scope.Cached_data_.target_icon_,
                opacity: 0,
                message: "target",
                draggable: true
            }
        }
        //Boat marker, set to the boats current position
        $scope.Cached_data_.markers_[0] = {
            lat: 0,
            lng: 0,
            icon: $scope.Cached_data_.boat_icon_,
            iconAngle: 0,
            opacity: 1,
            message: "boat"
        }

        if ($scope.Cached_data_.events_ == undefined) {
            $scope.Cached_data_.events_ = {
                map: {
                    enable: ['click'],
                    logic: 'emit'
                }
            }
        }



        if ($scope.Cached_data_.paths_ == undefined) {
            $scope.Cached_data_.paths_ = {
                p1: {
                    color: 'orange',
                    weight: 2,
                    latlngs: [{
                            lat: 0,
                            lng: 0
                        },
                        {
                            lat: 10,
                            lng: 10
                        }
                    ],
                    opacity: 0,
                    fillOpacity: 0,
                    type: 'rectangle'

                }
            }
        }
    }

    //Setup for the map with the appropriate settings.
    setupMap = function() {
        //Set up map center
        $scope.Cached_data_.map_center_ = {

            lat: $scope.Cached_data_.Boat_pose_.latitude_,
            lng: $scope.Cached_data_.Boat_pose_.longitude_,
            zoom: 14,

        };
        $scope.map_controls_ = {
            scale: true
        }
        //Setup what events are allowed on the map
        $scope.Cached_data_.events_ = {
            map: {
                enable: ['click'],
                logic: 'emit'
            }
        }


    }



    //Handels clicks on the map, adds up to two cursors.
    $scope.$on("leafletDirectiveMap.click", function(event, args) {

        var leafEvent = args.leafletEvent;
        if ($scope.Cached_data_.markers_[1].opacity == 0) {
            updateStartCoord(leafEvent.latlng);
        } else if ($scope.Cached_data_.markers_[2].opacity == 0) {
            updateEndCoord(leafEvent.latlng);
        }



    });

    //Allows you to drag the cursors
    $scope.$on("leafletDirectiveMarker.drag", function(event, args) {
        let point = args.leafletEvent.target._leaflet_events.drag[0].context._latlng;
        if (args.modelName == 1) {
            updateStartCoord(point);
        } else if (args.modelName == 2) {
            updateEndCoord(point);
        }
    });


    updateBoatPose = function(pose) {
        $scope.Cached_data_.Boat_pose_ = pose;
        //Boat marker, set to the boats current position
        $scope.Cached_data_.markers_[0].lat = pose.latitude_
        $scope.Cached_data_.markers_[0].lng = pose.longitude_
        $scope.Cached_data_.markers_[0].iconAngle = pose.orientation_
    }

    //Updates the start coord and the square to match
    updateStartCoord = function(start_coord) {
        $scope.Cached_data_.paths_.p1.latlngs[0] = $scope.Cached_data_.markers_[1]
        if (start_coord.lat != undefined) {
            $scope.Cached_data_.markers_[1].lat = start_coord.lat;
        }
        if (start_coord.lng != undefined) {
            $scope.Cached_data_.markers_[1].lng = start_coord.lng;
        }
        $scope.Cached_data_.markers_[1].opacity = 1;
    }

    //Updates the end coord and the square to match
    updateEndCoord = function(end_coord) {
        $scope.Cached_data_.paths_.p1.latlngs[1] = $scope.Cached_data_.markers_[2]
        $scope.Cached_data_.paths_.p1.fillOpacity = 0.2;
        $scope.Cached_data_.paths_.p1.opacity = 1;
        if (end_coord.lat != undefined) {
            $scope.Cached_data_.markers_[2].lat = end_coord.lat;
        }
        if (end_coord.lng != undefined) {
            $scope.Cached_data_.markers_[2].lng = end_coord.lng;
        }
        $scope.Cached_data_.markers_[2].opacity = 1;
    }

    //Make sure that the interval stops on page change.
    $scope.$on('$destroy', function() {
        if (intervalPromise)
            $interval.cancel(intervalPromise);
    });

    //Save to file sends a Post call to the server to save some content
    saveToFile = function(url, content) {
        $http.post(url, angular.toJson(content)). //Build a http POST call with the url and content
        then(function(data, status, headers, config) {}) //No error handling
    }

    let update_status_interval_ = 1000; //The interval for getting new navData
    let wait_for_path_interval_ = 1000; //The interval for waitForPath
    let wait_for_completion_interval_ = 100; //The interval for waitForCompletion

    //--- Execution --//

    setup();

    getDataFromNav("../savedData/fromNav.json").then(function() {
        setupMap();
    })

    let intervalPromise = $interval(function() {
        getDataFromNav("../savedData/fromNav.json")
    }, update_status_interval_);;
});

app.controller('EditParameters', function($scope, $http, dataHolder) {
  //TODO should this not be a function?? and a private one ?
  $http.get("../savedData/profiles.json").then(function(response) {
    $scope.Profiles_ = response.data.Profiles_;
    $scope.Profile_data_ = response.data;
    $scope.Selected_profile_ = $scope.Profiles_[0];
  }).catch(function(data) {
    $scope.Profile_data_ = {
      Profiles_: []
    };
    $scope.Profiles_ = $scope.Profile_data_.Profiles_;
  });

  //TODO should this not be a function?? and a private one ?
  $http.get("../savedData/activeParam.json").then(function(response) {
    $scope.Active_profile_ = response.data;
  });





  $scope.NewProfile = function() {
    //Creates a new default profile and appends it to the list of Profiles_
    $scope.Profiles_.push({
      name_: "New Profile",
      parameter_names_: [
        "P", "I", "D", "Tool Width"
      ],
      parameters_: [
        0, 0, 0, 0
      ],
      creation_timestamp_: Date.now()
    });
    saveToFile("profiles", $scope.Profile_data_); //Sending the content to the server to get saved.
    $scope.Selected_profile_ = $scope.Profiles_[($scope.Profiles_).length - 1]; // Selects the new profile in the ui
  }

  $scope.DeleteProfile = function(index) {
    if ($scope.Editing_profile_ != true) { //Checks to make sure you are not deleting the profile that's currently being edited

		if($scope.Profiles_[index] == $scope.Active_profile_ ){
			$scope.Active_profile_ = {
		      name_: "None",
		      creation_timestamp_: Date.now()
		    }
			saveToFile("active", $scope.Active_profile_);
		}

	  $scope.Profiles_.splice(index, 1) //Removes one element from the list at index

      if (index < 1) {
        $scope.Selected_profile_ = $scope.Profiles_[0];
      } else {
        $scope.Selected_profile_ = $scope.Profiles_[index - 1]; //Selects the profile under the one deleted.
      }

      saveToFile("profiles", $scope.Profile_data_); //Sending the content to the server to get saved.
    } else {

    }
  }

  $scope.EditProfile = function(index) {
    if ($scope.Editing_profile_ != true && ($scope.Profiles_).length > 0) { //Makes sure that there is something to edit!
      $scope.Revert_profile_ = {}; //Makes sure a Revet_profile_ exists
      $scope.Profile_ = $scope.Profiles_[index];
      angular.copy($scope.Profile_, $scope.Revert_profile_); //Creates a copy of Profile_ into Revert_profile_ instead of reference
      $scope.Editing_profile_ = true; //turns on the panel in the ui.
    }
  }

  $scope.ActivateProfile = function(index) {
    $scope.Active_profile_ = $scope.Profiles_[index]; //Sets active profile to be the profile at index in the list Profiles_
    saveToFile("active", $scope.Active_profile_); //Sending the content to the server to get saved.
  }

  $scope.SaveProfile = function() {
    $scope.Editing_profile_ = false; //Turning of the panel in the ui for editing
    $scope.Editing_profile_name = false; //makeing sure that the edit name ui change is reset.
    saveToFile("profiles", $scope.Profile_data_); //Sending the content to the server to get saved.
  }

  $scope.RevertProfile = function() {
    //Deep copy of only the things that we want to revert
    $scope.Revert_profile_.parameters_.forEach(function(param, index) {
      $scope.Profile_.parameter_names_[index] = $scope.Revert_profile_.parameter_names_[index];
      $scope.Profile_.parameters_[index] = $scope.Revert_profile_.parameters_[index];
    })
  }

  //On hover in it turns on the ui element button to remove a param
  $scope.HoverIn = function() {
    this.hover_edit = true;
  };

  //When hover out it turn off the ui element button to remove a param
  $scope.HoverOut = function() {
    this.hover_edit = false;
  };


  $scope.AddParameter = function(name) {
    if (this.new_param_name != undefined) { //If user has entered a name
      $scope.Profile_.parameters_.push(0); // add a parameter with the value 0
      $scope.profile.parameter_names_.push(name); // and the corresponding name name
      this.new_param_name = undefined; //Resetting the name in the ui
    }
  }


  $scope.RemoveParameter = function(index) {
    $scope.Profile_.parameters_.splice(index, 1); //removing one parameter at index
    $scope.profile.parameter_names_.splice(index, 1); //removing one parameter_name at index
  }

  saveToFile = function(url, content) {
    $http.post(url, angular.toJson(content)). //Build a http POST call with the url and content
    then(function(data, status, headers, config) {}) //We do not handle errors atm.
  }
});

app.controller('statusCtrl', function($scope, $http, dataHolder, $interval, $sce) {

  $scope.getDataFromNav = function() {
      $http.get("../savedData/fromNav.json").then(function(response) {
          $scope.cachedData.status = response.data.status;
          $scope.cachedData.timeStamp = response.data.timestamp;
          updateStatusDisplay();
      });
  }


  updateStatusDisplay = function(){
    var html = "";
      $scope.cachedData.status.forEach(function(currentValue){
      html += `
      <div class="panel panel-default">
      	<div class="panel-heading">
      		<h3 class="panel-title">` + currentValue.title + `</h3>
      		</div>
      		<div class="panel-body">`;

          currentValue.items.forEach(function (item){
            html +=  "<strong>"+item.title +":</strong> "+item.data+" " + item.unit+"<br />"
            if(item.unit == '%'){
              html += `<div class="progress">
                  			<div class="progress-bar `+ item.accentColor + `  " role="progressbar" aria-valuenow="0" aria-valuemin="0" aria-valuemax="100" style="width: `+item.data+`%">
                  			</div>
                  		</div>`;
            }
          });

            html +=`
      		</div>
      	</div>
      </div>`
      })
      $scope.statusPanels = $sce.trustAsHtml(html);

  };

  $scope.cachedData = dataHolder;

  $scope.timeSinceLastData = {
    warning : "alert-info",
    time : NaN,
  }

  $scope.getDataFromNav();


  $interval(function(){
    $scope.getDataFromNav();
    $scope.timeSinceLastData.time = Math.round((Date.now() - $scope.cachedData.timeStamp)/1000);
    if($scope.timeSinceLastData.time > 20){
      $scope.timeSinceLastData.warning = "alert-danger";
    }else if($scope.timeSinceLastData.time > 10){
      $scope.timeSinceLastData.warning = "alert-warning";
    }else{
      $scope.timeSinceLastData.warning = "alert-info";
    }

    $scope.cachedData.status
  }, 1000);

});

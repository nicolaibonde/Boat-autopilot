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
		if($location.path() == ""){
			$scope.selectedTab = "/";
		}
});


$(document).ready(function() {
	navbarResize();
	$('#sidebarCollapse').on('click', function() {
		$('#sidebar').toggleClass('active');
	});
});

$(window).on('resize', function() {
	navbarResize();
});

function navbarResize() {
	if ($(window).width() < 768) {
		$('#sidebar').addClass('active');
	} else {
		$('#sidebar').removeClass('active');
	}
}

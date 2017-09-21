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

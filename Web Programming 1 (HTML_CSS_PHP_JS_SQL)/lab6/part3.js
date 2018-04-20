
	// Add the click handler for the button
	$(':button').click(function() {
		setText($('#myInput').val());
	})

	var upperElt = $("#allUpper");

	var lowerElt = $("#allLower");

	var redElt = $("#redText");

	var flashyText = $("#flashyText");

	function setText(text) {
		upperElt.text(text.toUpperCase());
		lowerElt.text(text.toLowerCase());
		redElt.text(text);
		redElt.css("color","red");
		flashyText.text(text);
		flashyText.addClass("flashy");
	}

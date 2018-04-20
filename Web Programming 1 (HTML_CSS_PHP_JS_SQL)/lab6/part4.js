(function () {
	var resources = "http://students.engr.scu.edu/~adiaztos/resources/";

	// Load sample1.php
	$("#sample1").load(resources+"sample1.php");
	// Load sample2.php
	$("#sample2").load(resources+"sample2.php");
	// Get sample3.php
	$.get(resources+"sample3.php",loadFriends);

	function loadFriends(data) {
		var friends = JSON.parse(data)['friends'];
		var sample3div = $("#sample3");

			var listNode = $("<ul>")

			for(var i=0; i<friends.length; i++) {
				var node = $("<li>")
				node.text(friends[i]["name"]);
				listNode.append(node);
			}
			
			sample3div.append(listNode);
	}
})();
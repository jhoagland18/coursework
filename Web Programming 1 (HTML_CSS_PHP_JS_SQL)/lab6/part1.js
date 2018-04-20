var req = new XMLHttpRequest();
		req.open("GET", "http://students.engr.scu.edu/~adiaztos/resources/sample1.php");
		req.onreadystatechange = function() {
			if(this.readyState==4) {
				document.getElementById("sample1").innerHTML = req.responseText;	
			}		
		}
		req.send();

		var req2 = new XMLHttpRequest();
		req2.open("GET", "http://students.engr.scu.edu/~adiaztos/resources/sample2.php");
		req2.onreadystatechange = function() {
			if(this.readyState==4) {
				document.getElementById("sample2").innerHTML = req2.responseText;	
			}		
		}
		req2.send();

		var req3 = new XMLHttpRequest();
		req3.open("GET", "http://students.engr.scu.edu/~adiaztos/resources/sample3.php");
		req3.onreadystatechange = function() {
			if(this.readyState==4) {
			var json = JSON.parse(req3.responseText);

			var friends = json['friends'];

			var sample3div = document.getElementById('sample3');

			var listNode = document.createElement("ul");

			for(var i=0; i<friends.length; i++) {
				var node = document.createElement("li");
				node.innerHTML = friends[i]["name"];
				listNode.appendChild(node);
			}

			sample3div.appendChild(listNode);
			}
		}
		req3.send();
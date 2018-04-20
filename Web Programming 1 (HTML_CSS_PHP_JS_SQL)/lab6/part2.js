
	var template = document.getElementById("template");

	// remove template from the page, since it is only a template
	var parent = template.parentNode;
	parent.removeChild(template);

	document.getElementById('searchBtn').onclick = search;

	// Create an XMLHttpRequest object
	var req = new XMLHttpRequest();

	// Set onreadystatechange
	req.onreadystatechange = function() {
		if(req.readyState==4) {
			var data = JSON.parse(req.responseText);
			populateContacts(data);
		}
	}


	
	
	// Open and send requests
	req.open("GET", "http://students.engr.scu.edu/~adiaztos/resources/contacts.php");
	req.send();

	// This function takes the list of contacts and clones a new element from the template with the value of each contact
	function populateContacts(contacts) {

		for(var i=0; i<contacts.length; i++) {
			var node = template.cloneNode(true);

			var id = contacts[i]['id'];
			var name = contacts[i]['name'];
			var email = contacts[i]['email'];

			var index = node.getElementsByClassName('input-group-text')[0];
			index.id = node.id+id;
			index.innerHTML = i+1;

			var nameElt = node.querySelectorAll("input[name='name']");
			nameElt[0].value = name;

			var emailElt = node.querySelectorAll("input[name='email']");
			emailElt[0].value = email;

			parent.appendChild(node);
		}
	}

	// submits a request with the search query for the filtered list of contacts
	function search() {
		// clear the current contacts list
		while (parent.lastChild)
			parent.removeChild(parent.lastChild);

	var keyword = document.getElementById('searchField').value;

	var searchReq = new XMLHttpRequest();

	// Set onreadystatechange
	searchReq.onreadystatechange = function() {
		if(searchReq.readyState==4) {
			var data = JSON.parse(searchReq.responseText);
			populateContacts(data);
		}
	}
	
	// Open and send requests
	searchReq.open("GET", "http://students.engr.scu.edu/~adiaztos/resources/contacts.php?query="+keyword);
	searchReq.send();

			
	}

	// assign the search function as the click handler for search button
	

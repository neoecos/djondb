
function loadResult(post, msg) {
	post.empty();

	$("<h1>Classified saved</h1>").appendTo(post);
}

function loadPost(post, data) {
	post.empty();

	var title = $("<input id='title' type='text'>");
	addRow(post, 'Title:', title);
	var price = $("<input id='price' type='text'>");
	addRow(post, 'Price:', price);
	var place = $("<input id='place' type='text'>");
	addRow(post, 'Place:', place);
	var descrip = $("<textarea id='description' />");
	addRow(post, 'Description:', descrip);

	var rooms = null;
	if (data.category == 'casa' || data.category == 'apartamento') {
		rooms = $("<input id='rooms' type='text' />");
		addRow(post, 'Rooms', rooms);
	}
	var pisos = null;
	if (data.category == 'casa') {
		pisos = $("<input id='floors' type='text' />");
		addRow(post, 'Floors', pisos);
	}
	var area = null;
	if (data.category == 'lote') {
		area = $("<input id='area' type='text' />");
		addRow(post, 'Area', area);
	}

	var button = $("<input type=button value='Submit' />");
	button.bind("click", function() {
		data.title = title.val();
		data.price = price.val();
		data.place = place.val();
		data.description = descrip.val();
		if (rooms != null) data.rooms = rooms.val();
		if (pisos != null) data.pisos = pisos.val();
		if (area != null) data.area = area.val();
		var entry = { data: data };
		$.ajax({
			url: "post.php",
			data: entry,
			type: 'POST',
			dataType: "json",
			context: document.body
		}).done(function(msg) {
			window.location.href = "confirmation.html";
		});

	});
	button.appendTo(post);
}

function loadCategories(post, data) {
	post.empty();
	if (data.type == '1') {
		var sel = $("<select />");
		addOption(sel, 'casa', 'House');
		addOption(sel, 'apartamento', 'Apartment');
		addOption(sel, 'lote', 'Plot');
		addRow(post, 'Type:', sel);

		$("<input type='button' value='Next' />")
			.appendTo(post)
			.bind("click", function() {
			data.category = sel.val();
			loadPost(post, data);
		});
	}	
}

function preparePost() {
	var post = $("#post");


	var sel = $("<select></select>");
	addOption(sel, 1, 'House for sale');
	addRow(post, 'Pick the type of classified', sel);

	var data = {}; // this will hold the post information
	sel.bind("click", function() {
		data.type = sel.val();
		loadCategories(post, data);
	});

}

$(document).ready(function() {
	preparePost();
});

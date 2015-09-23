function dir(obj, space, label) {
	var s = '';

	try{
		//s += space + '$' + label + ':' + obj + '=' + obj.toSource() + '</br>';
		s += color_text('#000000', space + '$') + color_text('#ff0000', label) + color_text('#000000', ':') + color_text('#00ff00', obj) + color_text('#000000', ':') + color_text('#0000ff', obj.toSource()) + color_text('#000000', '</br>');
	} catch(err) {
		//s += space + '$' + label + ':' + obj + '</br>';
		s += color_text('#000000', space + '$') + color_text('#ff0000', label) + color_text('#000000', ':') + color_text('#00ff00', obj) + color_text('#000000', '</br>');
	}

	space += '⊙'

	for (i in obj) {
		var type = '';

		try{
			//s += space + '$' + i + '=' + obj[i] + '=' + obj[i].toSource() + '</br>';
			s += color_text('#000000', space + '$') + color_text('#ff0000', i) + color_text('#000000', '=') + color_text('#00ff00', obj[i]) + color_text('#000000', '=') + color_text('#0000ff', obj[i].toSource()) + color_text('#000000', '</br>');
		} catch(err) {
			//s += space + '$' + i + '=' + obj[i] + '</br>';
			s += color_text('#000000', space + '$') + color_text('#ff0000', i) + color_text('#000000', '=') + color_text('#00ff00', obj[i]) + color_text('#000000', '</br>');
		}

		try{
			type = getType(obj[i]);
		} catch(err) {
			console.log(err);
			console.log(i + ':' + obj[i]);
			type = 'Object';
		}

		if (type == 'Object') {
			s += dir(obj[i], space, i);
		}
	}
	return s;
}

function color_text(color, text) {
	return '<font color="' + color + '">' + text + '</font>';
}

function getType(x) {
	if (x == null) {
		return "null";
	}
	var t = typeof x;
	if (t != "object") {
		return t;
	}
	var c = Object.prototype.toString.apply(x);
	c = c.substring(8, c.length - 1);
	if (c != "Object") {
		return c;
	}
	if (x.constructor == Object) {
		return c
	}
	if ("classname" in x.prototype.constructor && typeof x.prototype.constructor.classname == "string") {
		return x.constructor.prototype.classname;
	}
	return "<unknown type>";
}


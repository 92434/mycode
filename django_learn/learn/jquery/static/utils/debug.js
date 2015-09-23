function dir(obj, space, label) {
	var s = '';


	try{
		s += space + '$' + label + ':' + obj + '=' + obj.toSource() + '</br>';
	} catch(err) {
		s += space + '$' + label + ':' + obj + '</br>';
	}

	space += '>'

	for (i in obj) {
		var type = '';

		try{
			s += space + '$' + i + '=' + obj[i] + '=' + obj[i].toSource() + '</br>';
		} catch(err) {
			s += space + '$' + i + '=' + obj[i] + '</br>';
		}

		try{
			type = getType(obj[i]);
		} catch(err) {
			console.log(err)
			console.log(i + ':' + obj[i])
			type = 'Object'
		}

		if (type == 'Object') {
			//s += space + i + ':' + obj[i] + '</br>';
			s += dir(obj[i], space, i);
		}
	}
	return s;
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


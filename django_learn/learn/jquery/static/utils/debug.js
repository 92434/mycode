function dir(obj, space) {
	var s = '';

	for (i in obj) {
		if (getType(obj[i]) == 'Object') {
			s += space + i + ':' + obj[i] + '</br>';
			space += '>';
			s += dir(obj[i], space);
		} else {
			s += space + '$' + i + "=" + obj[i] + "</br>";
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


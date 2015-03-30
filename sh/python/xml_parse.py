import xml.etree.cElementTree
# -*- coding: utf-8 -*-

filename = "./timers.xml"
def get_tags(root, path):
	
	path_tags = path[:-1]
	tag = path[-1]
	
	parents = [root]
	children = []
	tags = []

	for i in path_tags:
		for j in parents:
			children += j.findall(i)
		parents = children
		children = []

	for i in parents:
		tags.append(i.get(tag))
	return tags
#root = xml.etree.cElementTree.parse(filename).getroot()
strxml = """
<root>
	<content>
		<timers>
			<timer begin="2938" description="Kleiner Test" end="2999" serviceref="1:0:1:6DCA:44D:1:C00000:0:0:0:"/>
			<timer begin="1131390236" description="instant record" end="1131490236" serviceref="1:0:1:6DCA:44D:1:C00000:0:0:0:"/>
		</timers>
	</content>
	<content>
		<timers>
			<timer begin="2939" description="Kleiner Test" end="2999" serviceref="1:0:1:6DCA:44D:1:C00000:0:0:0:"/>
			<timer begin="1131390237" description="instant record" end="1131490236" serviceref="1:0:1:6DCA:44D:1:C00000:0:0:0:"/>
			<timer begin="1131390238" description="instant record" end="1131490236" serviceref="1:0:1:6DCA:44D:1:C00000:0:0:0:"/>
		</timers>
	</content>
</root>
"""
root = xml.etree.cElementTree.XML(strxml)
print get_tags(root, ["content", "timers", "timer", "begin"])

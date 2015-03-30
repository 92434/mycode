import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Result;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.sax.SAXTransformerFactory;
import javax.xml.transform.sax.TransformerHandler;
import javax.xml.transform.stream.StreamResult;

import org.xml.sax.Attributes;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.AttributesImpl;
import org.xml.sax.helpers.DefaultHandler;

/**
 * 使用SAX操作xml的简单例子
 * @author Administrator
 *
 */
public class XmlSaxTest {
	void parseXml01(){
		String xmlPath = "./user01.xml";  
		String xmlName = xmlPath.substring(xmlPath.lastIndexOf("/"));
		try {
			//获取SAX分析器的工厂实例，专门负责创建SAXParser分析器
			SAXParserFactory saxParserFactory = SAXParserFactory.newInstance();
			//获取SAXParser分析器的实例
			SAXParser saxParser = saxParserFactory.newSAXParser();
			//和其他解析方式一样，也要间接通过InputStream输入流对象获取xml信息
			//1、直接指定绝对路径获取文件输入流对象
			//InputStream inputStream = new FileInputStream(xmlPath);
			//2、使用类的相对路径查找xml路径
			//InputStream inputStream = this.getClass().getResourceAsStream(xmlName);
			//3、也可以指定路径完成InputStream输入流的实例化操作  
			InputStream inputStream = new FileInputStream(new File(xmlPath));  
			//4、使用InputSource输入源作为参数也可以转换xml  
			//InputSource inputSource = new InputSource(inputStream);

			//解析xml文档
			saxParser.parse(inputStream, new XmlSAXHandler01());//这里传递了自定义的XmlSAXHandler()管理者参数来解析xml,不像以前都是直接调用返回的Document对象
		} catch (ParserConfigurationException e) {
			e.printStackTrace();
		} catch (SAXException e) {
			e.printStackTrace();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 解析SAX的处理者01
	 * @author Administrator
	 *
	 */
	class XmlSAXHandler01 extends DefaultHandler {  

		@Override
			public void startDocument() throws SAXException {
				System.out.println("---->startDocument() is invoked...");
				super.startDocument();
			}

		@Override
			public void endDocument() throws SAXException {
				System.out.println("---->endDocument() is invoked...");
				super.endDocument();
			}

		@Override
			public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {
				System.out.println("-------->startElement() is invoked...");
				super.startElement(uri, localName, qName, attributes);
			}

		@Override
			public void endElement(String uri, String localName, String qName) throws SAXException {
				System.out.println("-------->endElement() is invoked...");
				super.endElement(uri, localName, qName);
			}

		@Override
			public void characters(char[] ch, int start, int length) throws SAXException {
				System.out.println("------------>characters() is invoked...");
				super.characters(ch, start, length);
			}
	}

	void parseXml02(){
		String xmlPath = "./user02.xml";  
		try {
			//获取SAX分析器的工厂实例，专门负责创建SAXParser分析器
			SAXParserFactory saxParserFactory = SAXParserFactory.newInstance();
			//获取SAXParser分析器的实例
			SAXParser saxParser = saxParserFactory.newSAXParser();
			InputStream inputStream = new FileInputStream(new File(xmlPath));  

			//解析xml文档
			saxParser.parse(inputStream, new XmlSAXHandler02());
		} catch (ParserConfigurationException e) {
			e.printStackTrace();
		} catch (SAXException e) {
			e.printStackTrace();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 解析SAX的处理者02
	 * @author Administrator
	 *
	 */
	class XmlSAXHandler02 extends DefaultHandler {  

		@Override
			public void startDocument() throws SAXException {
				System.out.println("---->startDocument() is invoked...");
			}

		@Override
			public void endDocument() throws SAXException {
				System.out.println("---->endDocument() is invoked...");
			}

		@Override
			public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {
				System.out.println("-------->startElement() is invoked...");
				System.out.println("uri的属性值：" + uri);
				System.out.println("localName的属性值：" + localName);
				System.out.println("qName的属性值：" + qName);
				if(attributes.getLength()>0){
					System.out.println("element属性值-->" + attributes.getQName(0) + "：" + attributes.getValue(0)); //根据下标获取属性name和value，也可以直接传递属性name获取属性值：attributes.getValue("id")
				}
			}

		@Override
			public void endElement(String uri, String localName, String qName) throws SAXException {
				System.out.println("-------->endElement() is invoked...");
				System.out.println("uri的属性值：" + uri);
				System.out.println("localName的属性值：" + localName);
				System.out.println("qName的属性值：" + qName);
			}

		@Override
			public void characters(char[] ch, int start, int length) throws SAXException {
				System.out.println("------------>characters() is invoked...");
				System.out.println("节点元素文本内容：" + new String(ch, start, length));
			}
	}



	/**
	 * Xml节点对象
	 * @author Administrator
	 *
	 */
	class Node {

		private Long id;
		private String name;
		private String text;
		private List<Attribute> attributeList;
		private List<Node> nodeList;

		public Long getId() {
			return id;
		}
		public void setId(Long id) {
			this.id = id;
		}
		public String getName() {
			return name;
		}
		public void setName(String name) {
			this.name = name;
		}
		public String getText() {
			return text;
		}
		public void setText(String text) {
			this.text = text;
		}
		public List<Attribute> getAttributeList() {
			return attributeList;
		}
		public void setAttributeList(List<Attribute> attributeList) {
			this.attributeList = attributeList;
		}
		public List<Node> getNodeList() {
			return nodeList;
		}
		public void setNodeList(List<Node> nodeList) {
			this.nodeList = nodeList;
		}
	}

	/**
	 * Xml属性对象
	 * @author Administrator
	 *
	 */
	class Attribute {

		private String name;
		private String value;

		public String getName() {
			return name;
		}
		public void setName(String name) {
			this.name = name;
		}
		public String getValue() {
			return value;
		}
		public void setValue(String value) {
			this.value = value;
		}
	}

	List<Node> nodeList = null;
	Node node = null;


	void parseXml03(){
		String xmlPath = "./user03.xml";  
		try {
			//获取SAX分析器的工厂实例，专门负责创建SAXParser分析器
			SAXParserFactory saxParserFactory = SAXParserFactory.newInstance();
			//获取SAXParser分析器的实例
			SAXParser saxParser = saxParserFactory.newSAXParser();
			InputStream inputStream = new FileInputStream(new File(xmlPath));  

			//解析xml文档
			saxParser.parse(inputStream, new XmlSAXHandler03());

			//迭代list
			if(nodeList.size() > 0){
				for (Node node : nodeList) {
					System.out.println("-----------------------------------------");
					System.out.println("【节点】" + node.getName() + "：" + node.getText());
					List<Attribute> attributeList = node.getAttributeList();
					if (attributeList != null) {
						for (Attribute attribute : attributeList) {
							System.out.println("【属性】" + attribute.getName() + "：" + attribute.getValue());							
						}
					}
				}
			}

		} catch (ParserConfigurationException e) {
			e.printStackTrace();
		} catch (SAXException e) {
			e.printStackTrace();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 解析SAX的处理者03
	 * @author Administrator
	 *
	 */
	class XmlSAXHandler03 extends DefaultHandler {  

		@Override
			public void startDocument() throws SAXException {
				nodeList = new ArrayList<Node>();
			}

		@Override
			public void endDocument() throws SAXException {

			}

		@Override
			public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {
				node = new Node();
				node.setId(null);
				node.setName(qName);
				//封装当前节点的属性
				List<Attribute> attributeList = new ArrayList<Attribute>();
				if(attributes.getLength()>0){
					for (int i = 0; i < attributes.getLength(); i++) {
						Attribute attribute = new Attribute();
						attribute.setName(attributes.getQName(i));
						attribute.setValue(attributes.getValue(i));
						attributeList.add(attribute);
					}
				}
				node.setAttributeList(attributeList);
			}

		@Override
			public void endElement(String uri, String localName, String qName) throws SAXException {

			}

		@Override
			public void characters(char[] ch, int start, int length) throws SAXException {
				if(node != null){
					node.setText(new String(ch, start, length));
					//因为startElement()在characters()的前面调用，所以必须放在后面才能把文本添加进去
					nodeList.add(node);
					node = null;
					//在这里之所以重新置为null是在解析标签的时候节点与节点之间的回车符、Tab符或者空格以及普通文本等等这些字符串也算一个节点
					//如果不这样，那么解析的时候会把之前添加成功的节点内的文本给替换掉。
				}
			}
	}

	/**
	 * xml节点对象
	 * @author Administrator
	 *
	 */
	class User {

		private Long id;
		private String name;
		private Long age;
		private String hobby;

		public Long getId() {
			return id;
		}
		public void setId(Long id) {
			this.id = id;
		}
		public String getName() {
			return name;
		}
		public void setName(String name) {
			this.name = name;
		}
		public Long getAge() {
			return age;
		}
		public void setAge(Long age) {
			this.age = age;
		}
		public String getHobby() {
			return hobby;
		}
		public void setHobby(String hobby) {
			this.hobby = hobby;
		}		
	}

	void parseXml04(){
		String xmlPath = "./user04.xml";  
		try {
			//获取SAX分析器的工厂实例，专门负责创建SAXParser分析器
			SAXParserFactory saxParserFactory = SAXParserFactory.newInstance();
			//获取SAXParser分析器的实例
			SAXParser saxParser = saxParserFactory.newSAXParser();
			InputStream inputStream = new FileInputStream(new File(xmlPath));  

			//解析xml文档
			saxParser.parse(inputStream, new XmlSAXHandler04());

			//迭代list
			if(userList.size() > 0){
				for (User user : userList) {
					System.out.println("-----------------------------------------");
					System.out.println("【Id】" + user.getId());
					System.out.println("【姓名】" + user.getName());
					System.out.println("【年龄】" + user.getAge());
					System.out.println("【爱好】" + user.getHobby());
				}
			}
		} catch (ParserConfigurationException e) {
			e.printStackTrace();
		} catch (SAXException e) {
			e.printStackTrace();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	List<User> userList = null;
	User user = null;

	/**
	 * 解析SAX的处理者04
	 * @author Administrator
	 *
	 */
	class XmlSAXHandler04 extends DefaultHandler {  

		private String currentQName;  //因为startElement()才能获取到标签名称，但是标签的内容在characters()获取，而且他们的执行顺序一直是顺序的，所以可以使用currentQName来过渡一下获取上一次的标签名

		@Override
			public void startDocument() throws SAXException {
				userList = new ArrayList<User>();
			}

		@Override
			public void endDocument() throws SAXException {

			}

		@Override
			public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {
				if(qName.equals("user")){
					user = new User(); //每次解析到user标签了才会创建user对象的实例
					//添加user标签中的id属性
					if(attributes.getLength() > 0){
						user.setId(Long.valueOf(attributes.getValue("id")));
					}
				}
				this.currentQName = qName;
			}

		@Override
			public void endElement(String uri, String localName, String qName) throws SAXException {
				//需要说明的是，因为每一个非空标签都有characters(),那么无法知道user子标签循环完了
				//但是可以这样，如果不考虑子标签顺序可以判断是否解析到了最后一个子标签来判断
				//或者直接在user标签的endElement()中添加即可。
				if(qName.equals("user")){  
					userList.add(user);
					user = null;
				}
				this.currentQName = null;
			}

		@Override
			public void characters(char[] ch, int start, int length) throws SAXException {
				String content = new String(ch, start, length);
				//System.out.println(currentQName + "：" + content);
				if(user != null && currentQName != null){
					if(currentQName.equals("name")){
						user.setName(content);
					}else if(currentQName.equals("age")){
						user.setAge(Long.valueOf(content));
					}else if(currentQName.equals("hobby")){
						user.setHobby(content);
					}
				}
			}
	}

	void buildXml01(){
		try {
			//创建保存xml的结果流对象
			Result reultXml = new StreamResult(new FileOutputStream(new File("./user.xml")));
			//获取sax生产工厂对象实例
			SAXTransformerFactory saxTransformerFactory = (SAXTransformerFactory) SAXTransformerFactory.newInstance();
			//获取sax生产处理者对象实例
			TransformerHandler transformerHandle = saxTransformerFactory.newTransformerHandler();
			transformerHandle.setResult(reultXml);
			//获取sax生产器
			Transformer transformer = transformerHandle.getTransformer();
			//transformer.setOutputProperty(OutputKeys.ENCODING,"UTF-8");//xml的编码格式
			transformer.setOutputProperty(OutputKeys.INDENT,"yes");//换行
			//开始封装document文档对象，这里和解析一样都是成双成对的构造标签
			transformerHandle.startDocument();
			AttributesImpl attrImple = new AttributesImpl();
			transformerHandle.startElement("", "", "Users",attrImple);

			attrImple.addAttribute("", "", "id", "string", "123");
			transformerHandle.startElement("", "", "user", attrImple);
			transformerHandle.characters("这个是用户的信息".toCharArray(), 0, "这个是用户的信息".length());
			transformerHandle.endElement("", "", "user");

			transformerHandle.endElement("", "", "Users");
			//因为没有appendChild等等添加子元素的方法，sax提供的是构造在startElement()和endElement()区间内的标签为包含的节点的父节点
			transformerHandle.endDocument();

			System.out.println("xml文档生成成功！");

		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (TransformerConfigurationException e) {
			e.printStackTrace();
		} catch (SAXException e) {
			e.printStackTrace();
		}
	}

	public static void main(String... args) {
		XmlSaxTest test = new XmlSaxTest();
		test.parseXml01();
		test.parseXml02();
		test.parseXml03();
		test.parseXml04();
		test.buildXml01();
	}
};

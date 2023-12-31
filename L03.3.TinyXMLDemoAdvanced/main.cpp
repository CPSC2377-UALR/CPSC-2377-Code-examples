/*
* Example code taken from: http://www.grinninglizard.com/tinyxmldocs/tutorial0.html
*/


#include "tinyxml2.h"

#include <list>
#include <string>
#include <map>
using namespace std;

typedef std::map<std::string,std::string> MessageMap;

// a basic window abstraction - demo purposes only
class WindowSettings{
public:
	int x,y,w,h;
	string name;

	WindowSettings(): x(0), y(0), w(100), h(100), name("Untitled"){}

	WindowSettings(int x, int y, int w, int h, const string& name){
		this->x=x;
		this->y=y;
		this->w=w;
		this->h=h;
		this->name=name;
	}
};

class ConnectionSettings{
public:
	string ip;
	double timeout;
};

class AppSettings{
public:
	string m_name;
	MessageMap m_messages;
	list<WindowSettings> m_windows;
	ConnectionSettings m_connection;

	AppSettings() {}

	void save(const char* pFilename);
	void load(const char* pFilename);
	
	// just to show how to do it
	void setDemoValues()
	{
		m_name="MyApp";
		m_messages.clear();
		m_messages["Welcome"]="Welcome to "+m_name;
		m_messages["Farewell"]="Thank you for using "+m_name;
		m_windows.clear();
		m_windows.push_back(WindowSettings(15,15,400,250,"Main"));
		m_connection.ip="Unknown";
		m_connection.timeout=123.456;
	}
};

int main(void){
	

	// block: customise and save settings
	{
		AppSettings settings;
		settings.m_name="HitchHikerApp";
		settings.m_messages["Welcome"]="Don't Panic";
		settings.m_messages["Farewell"]="Thanks for all the fish";
		settings.m_windows.emplace_back(WindowSettings(15,25,300,250,"BookFrame"));
		settings.m_windows.emplace_back(WindowSettings(-15,225,111,7,"OtherFrame"));
		settings.m_connection.ip="192.168.0.77";
		settings.m_connection.timeout=42.0;
		settings.save("appsettings2.xml");
	}
	

	// block: load settings
	{
		AppSettings settings;
		settings.load("appsettings2.xml");
		printf("%s: %s\n", settings.m_name.c_str(),	settings.m_messages["Welcome"].c_str());
		WindowSettings & w=settings.m_windows.front();
		printf("%s: Show window '%s' at %d,%d (%d x %d)\n", 
			settings.m_name.c_str(), w.name.c_str(), w.x, w.y, w.w, w.h);
		printf("%s: %s\n", settings.m_name.c_str(), settings.m_messages["Farewell"].c_str());
	}
	
	system("PAUSE");
}




void AppSettings::save(const char* pFilename)
{
	tinyxml2::XMLDocument doc;  
	tinyxml2::XMLElement* msg;
	tinyxml2::XMLComment * comment;
	string s;
	doc.InsertFirstChild(doc.NewDeclaration());
 
	tinyxml2::XMLElement * root = doc.NewElement(m_name.c_str());  
	doc.InsertEndChild( root );  
	
	s = " Settings for " + m_name + " ";
	comment = doc.NewComment(s.c_str());
	root->LinkEndChild( comment );  

	// block: messages
	{
		MessageMap::iterator iter;

		tinyxml2::XMLElement * msgs = doc.NewElement( "Messages" );  
		root->LinkEndChild( msgs );  
 
		for (iter=m_messages.begin(); iter != m_messages.end(); iter++)
		{
			const string & key=(*iter).first;
			const string & value=(*iter).second;
			msg = doc.NewElement(key.c_str());  
			msg->LinkEndChild( doc.NewText(value.c_str()));  
			msgs->LinkEndChild( msg );  
		}
	}

	// block: windows
	{
		tinyxml2::XMLElement * windowsNode = doc.NewElement( "Windows" );  
		root->LinkEndChild( windowsNode );  

		list<WindowSettings>::iterator iter;

		for (iter=m_windows.begin(); iter != m_windows.end(); iter++)
		{
			const WindowSettings& w=*iter;

			tinyxml2::XMLElement * window;
			window = doc.NewElement("Window" );  
			windowsNode->LinkEndChild( window );  
			window->SetAttribute("name", w.name.c_str());
			window->SetAttribute("x", w.x);
			window->SetAttribute("y", w.y);
			window->SetAttribute("w", w.w);
			window->SetAttribute("h", w.h);
		}
	}

	// block: connection
	{
		tinyxml2::XMLElement * cxn = doc.NewElement( "Connection" );  
		root->LinkEndChild( cxn );  
		cxn->SetAttribute("ip", m_connection.ip.c_str());
		cxn->SetAttribute("timeout", m_connection.timeout); 
	}

	doc.SaveFile(pFilename);  
}

void AppSettings::load(const char* pFilename)
{
	tinyxml2::XMLDocument doc;

	if (doc.LoadFile(pFilename) != tinyxml2::XML_SUCCESS)
	{
		printf("Bad File Path");
		exit(1);
	}

	tinyxml2::XMLHandle hDoc(&doc);
	tinyxml2::XMLElement* pElem;
	tinyxml2::XMLHandle hRoot(0);

	// block: name
	{
		pElem=hDoc.FirstChildElement().ToElement();
		// should always have a valid root but handle gracefully if it does
		if (!pElem) return;
		m_name=pElem->Value();

		// save this for later
		hRoot=tinyxml2::XMLHandle(pElem);
	}

	// block: string table
	{
		m_messages.clear(); // trash existing table

		pElem=hRoot.FirstChildElement().FirstChildElement().ToElement();
		for( pElem; pElem; pElem=pElem->NextSiblingElement())
		{
			const char *pKey=pElem->Value();
			const char *pText=pElem->GetText();
			if (pKey && pText) 
			{
				m_messages[pKey]=pText;
			}
		}
	}

	// block: windows
	{
		m_windows.clear(); // trash existing list

		tinyxml2::XMLElement* pWindowNode = hRoot.FirstChildElement("Windows").FirstChildElement().ToElement();
		for( pWindowNode; pWindowNode; pWindowNode=pWindowNode->NextSiblingElement())
		{
			WindowSettings w;
			const char *pName=pWindowNode->Attribute("name");
			if (pName) w.name=pName;
			
			pWindowNode->QueryIntAttribute("x", &w.x); // If this fails, original value is left as-is
			pWindowNode->QueryIntAttribute("y", &w.y);
			pWindowNode->QueryIntAttribute("w", &w.w);
			pWindowNode->QueryIntAttribute("hh", &w.h);

			m_windows.push_back(w);
		}
	}

	// block: connection
	{
		pElem=hRoot.FirstChild().ToElement();
		if (pElem)
		{
			m_connection.ip=pElem->Attribute("ip");
			pElem->QueryDoubleAttribute("timeout",&m_connection.timeout);
		}
	}
}

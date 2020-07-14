#ifndef XML2JSON_HPP_INCLUDED
#define XML2JSON_HPP_INCLUDED

// Copyright (C) 2015 Alan Zhuang (Cheedoong)	HKUST.  [Updated to the latest version of rapidjson]
// Copyright (C) 2013 Alan Zhuang (Cheedoong)	Tencent, Inc.

#include <iostream>
#include <map>
#include <string>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/encodedstream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/error/en.h"

// Avoided any namespace pollution.

void to_array_form(const char *name, rapidjson::Value &jsvalue, rapidjson::Value &jsvalue_chd, rapidjson::Document::AllocatorType& allocator)
{
    rapidjson::Value jsvalue_target; // target to do some operation
    rapidjson::Value jn;             // this is a must, partially because of the latest version of rapidjson
    jn.SetString(name, allocator);
    jsvalue_target = jsvalue.FindMember(name)->value;
    if(jsvalue_target.IsArray())
    {
        jsvalue_target.PushBack(jsvalue_chd, allocator);
        jsvalue.RemoveMember(name);
        jsvalue.AddMember(jn, jsvalue_target, allocator);
    }
    else
    {
        rapidjson::Value jsvalue_array;
        //jsvalue_array = jsvalue_target;
        jsvalue_array.SetArray();
        jsvalue_array.PushBack(jsvalue_target, allocator);
        jsvalue_array.PushBack(jsvalue_chd, allocator);
        jsvalue.RemoveMember(name);
        jsvalue.AddMember(jn, jsvalue_array, allocator);
    }
}

void add_attributes(rapidxml::xml_node<> *xmlnode, rapidjson::Value &jsvalue, rapidjson::Document::AllocatorType& allocator)
{
    rapidxml::xml_attribute<> *myattr;
    for(myattr = xmlnode->first_attribute(); myattr; myattr = myattr->next_attribute())
    {
        rapidjson::Value jn, jv;
        jn.SetString((std::string("@") + myattr->name()).c_str(), allocator);
        jv.SetString(myattr->value(), allocator);
        jsvalue.AddMember(jn, jv, allocator);
    }
}

void traverse_node(rapidxml::xml_node<> *xmlnode, rapidjson::Value &jsvalue, rapidjson::Document::AllocatorType& allocator)
{
    //cout << "this: " << xmlnode->type() << " name: " << xmlnode->name() << " value: " << xmlnode->value() << endl;
    rapidjson::Value jsvalue_chd;

    jsvalue.SetObject();
    jsvalue_chd.SetObject();
    rapidxml::xml_node<> *xmlnode_chd;

    // classified discussion:
    if((xmlnode->type() == rapidxml::node_data || xmlnode->type() == rapidxml::node_cdata) && xmlnode->value())
    {
        // case: pure_text
        jsvalue.SetString(xmlnode->value(), allocator);  // then addmember("#text" , jsvalue, allocator)
    }
    else if(xmlnode->type() == rapidxml::node_element)
    {
        if(xmlnode->first_attribute())
        {
            if(xmlnode->first_node() && xmlnode->first_node()->type() == rapidxml::node_data && count_children(xmlnode) == 1)
            {
                // case: <e attr="xxx">text</e>
                rapidjson::Value jn, jv;
                jn.SetString("#text", allocator);
                jv.SetString(xmlnode->first_node()->value(), allocator);
                jsvalue.AddMember(jn, jv, allocator);
                add_attributes(xmlnode, jsvalue, allocator);
                return;
            }
            else
            {
                // case: <e attr="xxx">...</e>
                add_attributes(xmlnode, jsvalue, allocator);
            }
        }
        else
        {
            if(!xmlnode->first_node())
            {
                // case: <e />
                jsvalue.SetNull();
                return;
            }
            else if(xmlnode->first_node()->type() == rapidxml::node_data && count_children(xmlnode) == 1)
            {
                // case: <e>text</e>
                jsvalue.SetString(rapidjson::StringRef(xmlnode->first_node()->value()), allocator);
                return;
            }
        }
        if(xmlnode->first_node())
        {
            // case: complex else...
            std::map<std::string, int> name_count;
            for(xmlnode_chd = xmlnode->first_node(); xmlnode_chd; xmlnode_chd = xmlnode_chd->next_sibling())
            {
                std::string current_name;
                const char *name_ptr = NULL;
                rapidjson::Value jn, jv;
                if(xmlnode_chd->type() == rapidxml::node_data || xmlnode_chd->type() == rapidxml::node_cdata)
                {
                    current_name = "#text";
                    name_count[current_name]++;
                    jv.SetString("#text", allocator);
                    name_ptr = jv.GetString();
                }
                else if(xmlnode_chd->type() == rapidxml::node_element)
                {
                    current_name = xmlnode_chd->name();
                    name_count[current_name]++;
                    name_ptr = xmlnode_chd->name();
                }
                traverse_node(xmlnode_chd, jsvalue_chd, allocator);
                if(name_count[current_name] > 1 && name_ptr)
                    to_array_form(name_ptr, jsvalue, jsvalue_chd, allocator);
                else
                {
                    jn.SetString(name_ptr, allocator);
                    jsvalue.AddMember(jn, jsvalue_chd, allocator);
                }
            }
        }
    }
    else
    {
        std::cerr << "err data!!" << std::endl;
    }
}

std::string xml2json(const char *xml_str)
{
    //file<> fdoc("track_orig.xml"); // could serve another use case
    rapidxml::xml_document<> *xml_doc = new rapidxml::xml_document<>();
    xml_doc->parse<0> (const_cast<char *>(xml_str));

    rapidjson::Document js_doc;
    js_doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = js_doc.GetAllocator();

    rapidxml::xml_node<> *xmlnode_chd;

    for(xmlnode_chd = xml_doc->first_node(); xmlnode_chd; xmlnode_chd = xmlnode_chd->next_sibling())
    {
        rapidjson::Value jsvalue_chd;
        jsvalue_chd.SetObject();
        traverse_node(xmlnode_chd, jsvalue_chd, allocator);
        js_doc.AddMember(rapidjson::StringRef(xmlnode_chd->name()), jsvalue_chd, allocator);
    }

    delete xml_doc;
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    js_doc.Accept(writer);
    return buffer.GetString();
}

#endif

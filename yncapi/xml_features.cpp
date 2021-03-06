/* 
 * Copyright (C) 2018, Alexandre Courtois
 * 
 * This file is part of YNCAPI.
 *
 * YNCAPI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * YNCAPI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with YNCAPI.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */
 
#include "xml_features.h"
#include <iostream>

bool yncapi::xmlValidate(const XMLDocument& xmlDoc) {
    if(xmlDoc.Error()) {
        std::cerr << ERROR_XML(xmlDoc.ErrorID()) << endl;
        return FAILURE;
    }

    return SUCCESS;
}

XMLNode* yncapi::xmlFindNode(XMLNode* rootNode, const string& name) {
    if(rootNode && name.size()) {
        if(rootNode->Value() == name)
            return rootNode;

        XMLNode* cursor = rootNode->NextSibling();

        while(cursor != nullptr && cursor->Value() != name)
            cursor = cursor->NextSibling();

        return cursor;
    }

    return nullptr;
}

string yncapi::xmlExtract(const string& xmlSrc, const string& elements) {
    if(xmlSrc.size() && elements.size()) {
        XMLDocument doc;
        vector<string> elementList = slice(elements, '/');
        vector<string>::iterator it;

        doc.Parse(xmlSrc.c_str());

        if(xmlValidate(doc)) {
            XMLNode* cursor = doc.FirstChild();

            it = elementList.begin();

            while(it != elementList.end()) {
                XMLNode* subCursor = xmlFindNode(cursor, it->c_str());

                if(subCursor == nullptr)
                    return ERROR_XML_PARSING_NODE;

                cursor = subCursor->FirstChild();

                it++;
            }

            if(cursor)
                return cursor->Value();

            return XML_EMPTY;
        }

        return ERROR_XML_INVALID_XMLDOC;
    }

    return ERROR_XML_EMPTY;
}

string yncapi::xmlBuildRequest(const string& type, const string& path, const string& value) {
    vector<string> nodes = slice(path, '/');
    string request = XML_DECLARATION + type;

    for(unsigned int i = 0; i < nodes.size(); ++i)
        request += ("<" + nodes[i] + ">");

    request += value;

    for(unsigned int i = 0; i < nodes.size(); ++i)
        request += ("</" + nodes[nodes.size() - i - 1] + ">");

    request += XML_END;

    return request;
}

string yncapi::xmlGetAttribute(const string& xmlSrc, const string& nodeId, const string& attrId) {
    if(xmlSrc.size() && nodeId.size() && attrId.size()) {
        XMLDocument doc;
        doc.Parse(xmlSrc.c_str());

        if(xmlValidate(doc)) {
            XMLNode* node = xmlFindNode(doc.RootElement(), nodeId);

            if(node == nullptr)
                return string(ERROR_XML_PARSING_NODE) + ":" + nodeId;

            return node->ToElement()->Attribute(attrId.c_str());
        }

        return ERROR_XML_INVALID_XMLDOC;
    }

    return ERROR_XML_EMPTY;
}

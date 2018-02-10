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

#ifndef XML_FEATURES_H
#define XML_FEATURES_H

#include "tools.h"
#include <tinyxml2.h>

#define SUCCESS                         true
#define FAILURE                         false

#define ERROR_XML_                      string("ERROR_XML_")
#define ERROR_XML(code)                 string("ERROR_XML_" + std::to_string(code))
#define ERROR_XML_EMPTY                 string("ERROR_XML_EMPTY")
#define ERROR_XML_FILE_READ             string("ERROR_XML_FILE_READ")
#define ERROR_XML_PARSING_ELEMENT       string("ERROR_XML_PARSING_ELEMENT")
#define ERROR_XML_INVALID_XMLDOC        string("ERROR_XML_INVALID_XMLDOC")
#define ERROR_XML_PARSING_NODE          string("ERROR_XML_PARSING_NODE")
#define ERROR_XML_BAD_TYPE              string("ERROR_XML_BAD_TYPE")

#define XML_DECLARATION                 string("<?xml version=\"1.0\" encoding=\"utf-8\"?>")
#define HTTP_GET                        string("<YAMAHA_AV cmd=\"GET\">")
#define HTTP_PUT                        string("<YAMAHA_AV cmd=\"PUT\">")
#define XML_END                         string("</YAMAHA_AV>")
#define XML_EMPTY                       string("<empty>")
#define XML_ROOTNODE                    string("YAMAHA_AV")
#define XML_ERR_ATTR                    string("RC")

using namespace tinyxml2;

namespace yncapi {
    /**
     * @brief Check for a valid XML document.
     * @param xmlDoc the XMLDocument to check
     * @return true if the document is valid, false if not.
     */
    bool xmlValidate(const XMLDocument& xmlDoc);

    /**
     * @brief Try to find a specific node by its value in a tree
     * @param rootNode Root node of the tree
     * @param name The node name
     * @return nullptr if no found, else a pointer to the node
     */
    XMLNode* xmlFindNode(XMLNode* rootNode, const string& name);

    /**
     * @brief Extract an element from an XML string. For example :
     *        If the XML string is : "<A><B>E</B></A>"
     *        The element path string to extract "E" must be : "A/B"
     * @param xmlSrc Source XML string
     * @param elements Element path string
     * @return A string that contains either the element, or an error code
     */
    string xmlExtract(const string& xmlSrc, const string& elements);

    /**
     * @brief Automatically builds a proper XML document to send to the network player
     * @param type Basically HTTP_PUT or HTTP_GET
     * @param path Something like "A/B/C" to generate "<A><B><C>value</C></B></A>"
     * @param value The value to add to the path. By default "GetParam" to keep read accesses as simples as possible
     * @return The proper built XML string
     */
    string xmlBuildRequest(const string& type, const string& path, const string& value = "GetParam");

    /**
     * @brief Extract an attribute from specific node
     * @param xmlSrc Valid XML source string
     * @param nodeId Node where to find the attribute
     * @param attrId Attribute name
     * @return Either the attribute value, or an error
     */
    string xmlGetAttribute(const string& xmlSrc, const string& nodeId, const string& attrId);
}

#endif

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
    bool xmlValidate(const XMLDocument& xmlDoc) {
        if(xmlDoc.Error()) {
            cerr << ERROR_XML(xmlDoc.ErrorID()) << endl;
            return FAILURE;
        }

        return SUCCESS;
    }

    /**
     * @brief Try to find a specific node by its value in a tree
     * @param rootNode Root node of the tree
     * @param name The node name
     * @return nullptr if no found, else a pointer to the node
     */
    XMLNode* xmlFindNode(XMLNode* rootNode, const string& name) {
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

    /**
     * @brief Extract an element from an XML string. For example :
     *        If the XML string is : "<A><B>E</B></A>"
     *        The element path string to extract "E" must be : "A/B"
     * @param xmlSrc Source XML string
     * @param elements Element path string
     * @return A string that contains either the element, or an error code
     */
    string xmlExtract(const string& xmlSrc, const string& elements) {
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

    /**
     * @brief Automatically builds a proper XML document to send to the network player
     * @param type Basically HTTP_PUT or HTTP_GET
     * @param path Something like "A/B/C" to generate "<A><B><C>value</C></B></A>"
     * @param value The value to add to the path. By default "GetParam" to keep read accesses as simples as possible
     * @return The proper built XML string
     */
    string xmlBuildRequest(const string& type, const string& path, const string& value = "GetParam") {
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

    /**
     * @brief Extract an attribute from specific node
     * @param xmlSrc Valid XML source string
     * @param nodeId Node where to find the attribute
     * @param attrId Attribute name
     * @return Either the attribute value, or an error
     */
    string xmlGetAttribute(const string& xmlSrc, const string& nodeId, const string& attrId) {
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
}

#endif

#include "ssdp_features.h"

std::string yncapi::PacketSSDP::toLower(std::string str) const {
    for(std::string::iterator i = str.begin(); i != str.end(); ++i)
        *i = static_cast<char>(std::tolower(*i));

    return str;
}

void yncapi::PacketSSDP::parse(const std::string& data) {
    std::istringstream in(data);

    // Extract the HTTP version from the first line
    std::string version;

    if(in >> version) {
        if((version.size() >= 8) && (version[6] == '.') &&
                (toLower(version.substr(0, 5)) == "http/")   &&
                isdigit(version[5]) && isdigit(version[7])) {
            m_majorVersion = version[5] - '0';
            m_minorVersion = version[7] - '0';
        } else {
            // Invalid HTTP version
            m_status = sf::Http::Response::InvalidResponse;
            return;
        }
    }

    // Extract the status code from the first line
    int status;

    if(in >> status) {
        m_status = static_cast<sf::Http::Response::Status>(status);
    } else {
        // Invalid status code
        m_status = sf::Http::Response::InvalidResponse;
        return;
    }

    // Ignore the end of the first line
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Parse the other lines, which contain fields, one by one
    parseFields(in);

    m_body.clear();

    // Determine whether the transfer is chunked
    if(toLower(getField("transfer-encoding")) != "chunked") {
        // Not chunked - just read everything at once
        std::copy(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>(), std::back_inserter(m_body));
    } else {
        // Chunked - have to read chunk by chunk
        std::size_t length;

        // Read all chunks, identified by a chunk-size not being 0
        while(in >> std::hex >> length) {
            // Drop the rest of the line (chunk-extension)
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            // Copy the actual content data
            std::istreambuf_iterator<char> it(in);
            std::istreambuf_iterator<char> itEnd;

            for(std::size_t i = 0; ((i < length) && (it != itEnd)); i++)
                m_body.push_back(*it++);
        }

        // Drop the rest of the line (chunk-extension)
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Read all trailers (if present)
        parseFields(in);
    }
}

void yncapi::PacketSSDP::parseFields(std::istream &in) {
    std::string line;

    while(std::getline(in, line) && (line.size() > 2)) {
        std::string::size_type pos = line.find(": ");

        if(pos != std::string::npos) {
            // Extract the field name and its value
            std::string field = line.substr(0, pos);
            std::string value = line.substr(pos + 2);

            // Remove any trailing \r
            if(!value.empty() && (*value.rbegin() == '\r'))
                value.erase(value.size() - 1);

            // Add the field
            m_fields[toLower(field)] = value;
        }
    }
}

const std::string& yncapi::PacketSSDP::getField(const std::string& field) {
    // on the first call only, retrieve the packet data
    if(firstGet) {
        firstGet = false;
        std::string data((char*)(this->getData()));
        parse(data);
    }

    // then try to find the field name
    TFieldTable::const_iterator it = m_fields.find(toLower(field));

    // field found ? return value
    // field not found ? return empty string
    if(it != m_fields.end()) {
        return it->second;
    } else {
        static const std::string empty = "";
        return empty;
    }
}

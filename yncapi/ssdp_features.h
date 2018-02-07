#ifndef SSDP_FEATURES_H
#define SSDP_FEATURES_H

#include "yncapi.h"
#include <SFML/Network.hpp>
#include <map>
#include <string>
#include <sstream>

namespace yncapi {
    class PacketSSDP: public sf::Packet {
        private:
            typedef std::map<std::string, std::string> TFieldTable;

            TFieldTable m_fields;
            sf::Http::Response::Status m_status;
            unsigned int m_majorVersion;
            unsigned int m_minorVersion;
            std::string m_body;
            bool firstGet = true;

            /**
             * @brief Lower characters of the parameter string
             * @param str String to lower
             * @return Lowered string
             */
            std::string toLower(std::string str) const;
            
            /**
             * @brief Parse SSDP packet, like HTTP packet
             * @param data SSDP packet data
             */
            void parse(const std::string& data);
            
            /**
             * @brief Parse a field and push it to the field table if valid
             * @param in Field to parse
             */
            void parseFields(std::istream &in);

        public:
            /**
             * @brief Get a field value by its field name
             * @param field Field name
             * @return Field value
             */
            const std::string& getField(const std::string& field);

    };
}

#endif

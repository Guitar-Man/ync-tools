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

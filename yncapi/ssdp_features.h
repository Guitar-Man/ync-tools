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
             * @brief 
             * @param str
             */
            std::string toLower(std::string str) const;
            
            /**
             * @brief 
             * @param data
             */
            void parse(const std::string& data);
            
            /**
             * @brief 
             * @param in
             */
            void parseFields(std::istream &in);

        public:
            /**
             * @brief 
             * @param field
             */
            const std::string& getField(const std::string& field);

    };
}

#endif

#ifndef _XILAB_CREDENTIALS_H_
#define _XILAB_CREDENTIALS_H_

namespace xilab{
    namespace configuration{  
        namespace retries{
            static constexpr const int NETWORK = 10;
            static constexpr const int LORA = 10;
        } // namespace retries

        namespace lora{
            static constexpr const uint8_t PIN_SS = 18;
            static constexpr const uint8_t PIN_RST = 14;
            static constexpr const uint8_t PIN_DIO0 = 26;
            static constexpr const long REGION = 868E6;
        } // namespace lora

        namespace wlan{    
            static constexpr const char* SSID =
            static constexpr const char* PASSWORD =
        } // namespace wlan

        namespace server{
            static constexpr const char* SERVER_ADDRESS = "192.168.2.104";
            static constexpr const int PORT = 8080;
            static constexpr const char* API_USER_NAME = "admin";
            static constexpr const char* API_USER_PASSWORD = "admin";
        } // namespace server
    } // namespace credentials
} // namespace xilab


#endif // _XILAB_CREDENTIALS_H_


//1681886808664455233813288


#ifndef _XILAB_UTILS_H_
#define _XILAB_UTILS_H_
#include <ArduinoJson.h>


/**
 * @brief This is a helper method
 * It provides a printf style serial printing
 * @param fmt Format
 * @param ... arguments
 */
void LOGn(const char *fmt, ...) {
#ifdef DEBUG
    char buff[1024];
    va_list pargs;
    va_start(pargs, fmt);
    vsnprintf(buff, 1024, fmt, pargs);
    va_end(pargs);
    Serial.println(buff);
#endif

}
namespace xilab{
    namespace utils{

        enum JsonResults{
            OK,
            PARSED = 301,
            NOT_PARSED = 302,
            PARSE_ERROR = 303
        };

        class JsonUtil{
            public:
                static JsonUtil& getInstance(){
                    static JsonUtil instance;
                    return instance;
                }
            private:
                JsonUtil(){}

                StaticJsonDocument<1024> doc;
                JsonResults parsed = NOT_PARSED;
                DeserializationError lastError;
            public:
                int parse(String input){
                    LOGn("[JSON PARSE  ] Start parsing.");
                    lastError = deserializeJson(doc, input);

                    if (lastError) {
                        LOGn("[JSON PARSE  ] Cannot parse document. Error: %s.", lastError.c_str());
                        return PARSE_ERROR;
                    }
                    parsed = OK;
                    return OK;
                }

                bool isParsed(){
                    return parsed == OK;
                }

                int extractInteger(String json, String node, String element){
                    if(parsed != OK){
                        LOGn("[JSON PARSE  ] Cannot extract value: %s.", lastError.c_str());
                        return parsed;
                    }
                    int result = -1;

                    if(node.equals("")){
                        result = doc[element];
                    }else{
                        result = doc[node][element];
                    }
                    return result;
                }

                float extractFloat(String json, String node, String element){
                    if(parsed != OK){
                        LOGn("[JSON PARSE  ] Cannot extract value: %s.", lastError.c_str());
                        return parsed;
                    }
                    float result = -1;
                    if(node.equals("")){
                        result = doc[element];
                    }else{
                        result = doc[node][element];
                    }
                    return result;
                }
                
                String extractString(String json, String node, String element){
                    if(parsed != OK){
                        LOGn("[JSON PARSE  ] Cannot extract value: %s.", lastError.c_str());
                        return "";
                    }
                    String result = "";
                    if(node.equals("")){
                        return doc[element];
                    }else{
                        return doc[node][element];
                    }
                }
        }; // class json
    } // utils
} // xilab

#endif // _XILAB_UTILS_H_
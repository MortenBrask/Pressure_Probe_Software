#include "Probe_Webserver.h"

//data_handler is taking care of parsing the json formatted messages received from the webpage
//it saves and sets the events and data 
void data_handler(uint8_t * payload){
    
    Serial.println("Data received");
    StaticJsonDocument<500> JSONDoc;                        //Memory pool
    auto error = deserializeJson(JSONDoc, payload);
    
    if (error) {                                            //Check for errors in parsing
        Serial.println("Parsing failed");
        return; 
    }
    
    const char * jstype = JSONDoc["type"];                  //Get sensor type value        

    Serial.println("type received =");
    Serial.println(jstype);

    if(!strcmp(jstype,"Start")){      
    #ifdef PROBE_DEBUG
        Serial.println("start received");
    #endif
        probe_event = E_start;
        test_flags.once = 0;
    }
    else if(!strcmp(jstype,"Stop")){
    #ifdef PROBE_DEBUG
        Serial.println("stop received");
    #endif
        probe_event = E_stop;
        test_flags.once = 0;
    }
    else if(!strcmp(jstype,"Set")){
    #ifdef PROBE_DEBUG
        Serial.println("set received");
    #endif
        probe_event = E_set;
        test_flags.once = 0;
    }
    else if(!strcmp(jstype,"Reset")){
    #ifdef PROBE_DEBUG
        Serial.println("reset received");
    #endif
        probe_event = E_reset;
        test_flags.once = 0;
    }
    else if(!strcmp(jstype,"Plus")){
    #ifdef PROBE_DEBUG
        Serial.println("plus received");
    #endif
        probe_event = E_plus;
        test_flags.once = 0;
    }
    else if(!strcmp(jstype,"Minus")){
    #ifdef PROBE_DEBUG
        Serial.println("minus received");
    #endif
        probe_event = E_minus;
        test_flags.once = 0;
    }
    else if(!strcmp(jstype,"Raw_max_on")){
    #ifdef PROBE_DEBUG
        Serial.println("raw max received");
    #endif
        test_flags.raw_max_on = 1;        
    }
    else if(!strcmp(jstype,"Raw_roc_on")){
    #ifdef PROBE_DEBUG
        Serial.println("raw roc received");
    #endif
        probe_event = E_raw_roc_on;
        test_flags.raw_roc_on = 1; 
    }
    else if(!strcmp(jstype,"Raw_max_off")){
    #ifdef PROBE_DEBUG
        Serial.println("raw max received");
    #endif
        test_flags.raw_max_on = 0;        
    }
    else if(!strcmp(jstype,"Raw_roc_off")){
    #ifdef PROBE_DEBUG
        Serial.println("raw roc received");
    #endif
        test_flags.raw_roc_on = 0; 
    }
    else if(!strcmp(jstype,"VAS")){
        int jsvas = JSONDoc["vas"];
        configuration_data.trx_data.socket_rx_data.msg_type = DATA_VAS_MEASUREMENT;
        configuration_data.trx_data.socket_trx_type = socket_RX;
        configuration_data.trx_data.socket_rx_data.measurement_data.measurement = jsvas;
    #ifdef PROBE_DEBUG
        Serial.println("VAS data received");
    #endif
        probe_event = E_rx_vas;
        test_flags.once = 0;
    }
    else if(!strcmp(jstype,"Temp")){
        int jstemp = JSONDoc["temp"];
        configuration_data.trx_data.socket_rx_data.msg_type = DATA_VAS_MEASUREMENT;
        configuration_data.trx_data.socket_trx_type = socket_RX;
        configuration_data.trx_data.socket_rx_data.measurement_data.measurement = jstemp;
    #ifdef PROBE_DEBUG
        Serial.println("Temp data received");
    #endif
        probe_event = E_rx_temp;
        test_flags.once = 0;
    }
    else if(!strcmp(jstype,"factor")){
        float jscal_factor = JSONDoc["factor"];
        configuration_data.trx_data.socket_rx_data.msg_type = DATA_CALIBRATION_FACTOR;
        configuration_data.trx_data.socket_trx_type = socket_RX;
        configuration_data.trx_data.socket_rx_data.calibration_data.calibration_factor = jscal_factor;
    #ifdef PROBE_DEBUG
        Serial.println("calibration factor data received");
    #endif
        probe_event = E_rx_calibration_factor;
        test_flags.once = 0;
    }
    else if(!strcmp(jstype,"weight")){
        float jscal_weight = JSONDoc["weight"];
        configuration_data.trx_data.socket_rx_data.msg_type = DATA_CALIBRATION_WEIGHT;
        configuration_data.trx_data.socket_trx_type = socket_RX;
        configuration_data.trx_data.socket_rx_data.calibration_data.known_weight = jscal_weight;
    #ifdef PROBE_DEBUG
        Serial.println("calibration factor data received");
    #endif
        probe_event = E_rx_weight;
        test_flags.once = 0;
    }
    else if(!strcmp(jstype,"ID")){
        const char * jsid = JSONDoc["id"];
        configuration_data.trx_data.socket_rx_data.msg_type = DATA_UNIQUE_ID;
        configuration_data.trx_data.socket_trx_type = socket_RX;
        configuration_data.trx_data.socket_rx_data.user_settings.unique_id = (int)jsid;
    #ifdef PROBE_DEBUG
        Serial.println("data received =");
        Serial.println(jsid);
    #endif
        probe_event = E_rx_id;
        test_flags.once = 0;        
    }
    else if(!strcmp(jstype,"site_label")){
        const char * jssite = JSONDoc["site"];
        configuration_data.trx_data.socket_rx_data.msg_type = DATA_SITE_LABEL;
        configuration_data.trx_data.socket_trx_type = socket_RX;
        configuration_data.trx_data.socket_rx_data.user_settings.site_label = jssite;
    #ifdef PROBE_DEBUG
        Serial.println("data received =");
        Serial.println(jssite);
    #endif
        probe_event = E_rx_site_label;
        test_flags.once = 0;        
    }
    else if(!strcmp(jstype,"Settings")){        
        float jsroc = JSONDoc["roc"];
        int jsmaxpress = JSONDoc["max_press"];
        int jsfreq = JSONDoc["vas_freq"];
        configuration_data.user_settings.target_roc = jsroc;
        configuration_data.user_settings.max_pressure = jsmaxpress;
        configuration_data.user_settings.vas_freq = jsfreq;
    #ifdef PROBE_DEBUG
        Serial.println("data received =");
        Serial.println(jsroc);
        Serial.println(jsmaxpress);
        Serial.println(jsfreq);
    #endif
        probe_event = E_rx_settings;
        test_flags.once = 0;
        
    }
    
}
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                #ifdef PROBE_DEBUG
                IPAddress ip = webSocket.remoteIP(num);                
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
                #endif

                probe_event = E_ws_connected;
                test_flags.once = 0;
            }
                break;
        case WStype_TEXT:
            {
                #ifdef PROBE_DEBUG
                Serial.printf("[%u] get Text: %s\n", num, payload);
                #endif 

                data_handler(payload);
            }                        
            break;
        case WStype_BIN:

            #ifdef PROBE_DEBUG
            Serial.printf("[%u] get binary length: %u\n", num, length);
            #endif            
            break;
		case WStype_ERROR:                              //the webpage handles reconnect on error...			
		case WStype_FRAGMENT_TEXT_START:
		case WStype_FRAGMENT_BIN_START:
		case WStype_FRAGMENT:
		case WStype_FRAGMENT_FIN:
        default:
			break;
    }    
}

//template_processor inserts the correct html in the webpage being served, should it contain keywords that matches one of the listed below
String template_processor(const String& var)
{
    //Here is to be inserted the correct links for the instruction videos to show. insert the correct URL based on test progress (test_flags.test) like the default example
    if(var == "SOURCEVIDEO"){
        switch (test_flags.test){
            case TEST_1:
                return String("https://www.youtube.com/embed/mLy7_ZMYRao");      //URL to an instruction video to TEST_1
            case TEST_2:
                return String("https://www.youtube.com/embed/mLy7_ZMYRao");
            case TEST_3:
                return String("https://www.youtube.com/embed/FXQow21r86Y");
            case TEST_4:
                return String("https://www.youtube.com/embed/FXQow21r86Y");
            case TEST_5:
                return String("https://www.youtube.com/embed/N-ks1kCI6Pw");
            case TEST_6:
                return String("https://www.youtube.com/embed/N-ks1kCI6Pw");
            case TEST_7:
                return String("https://www.youtube.com/embed/4k_YWlNdheA");
            //case TEST_8:
                //return String("https://www.youtube.com/embed/FXQow21r86Y");
            default:
                return String("https://www.youtube.com/embed/NpEaa2P7qZI");
        }         
    }
    else if(var == "NEXT_LINK"){
        return link_strings[test_flags.test];
    }
    else if(var == "VIDEO_TEXT"){
        return video_strings[test_flags.test];
    }
    else if(var == "LD_SELECT"){
        return ld_select_strings[test_flags.test];
    }
    else if(var == "TEMP_NEXT"){
        if(test_flags.test == TEST_7/*TEST_8*/){
            return "download.html";
        }
        else{
            return "video.html";
        }
    }
    else if(var == "CHECK1"){
        return (configuration_data.probe_test_progress.prev_test > TEST_1) ? check_mark[0] : check_mark[1]; 
    }
    else if(var == "CHECK2"){
        return (configuration_data.probe_test_progress.prev_test > TEST_2) ? check_mark[0] : check_mark[1]; 
    }
    else if(var == "CHECK3"){
        return (configuration_data.probe_test_progress.prev_test > TEST_3) ? check_mark[0] : check_mark[1]; 
    }
    else if(var == "CHECK4"){
        return (configuration_data.probe_test_progress.prev_test > TEST_4) ? check_mark[0] : check_mark[1]; 
    }
    else if(var == "CHECK5"){
        return (configuration_data.probe_test_progress.prev_test > TEST_5) ? check_mark[0] : check_mark[1]; 
    }
    else if(var == "CHECK6"){
        return (configuration_data.probe_test_progress.prev_test > TEST_6) ? check_mark[0] : check_mark[1]; 
    }
    else if(var == "CHECK7"){
        return (configuration_data.probe_test_progress.prev_test > TEST_7) ? check_mark[0] : check_mark[1]; 
    }
    else if(var == "CHECK8"){
        return (configuration_data.probe_test_progress.prev_test > TEST_7/*TEST_8*/) ? check_mark[0] : check_mark[1]; 
    }
    else if(var == "PROBE_SELECT"){
        
        if(test_flags.test == TEST_3 || test_flags.test == TEST_5){
            Serial.println("local avg:");
            Serial.println(test_flags.avg_result_local);
            if(test_flags.avg_result_local <= 200){
                return probe_select[0];
            }
            else if(test_flags.avg_result_local <= 400){
                return probe_select[1];
            }
            else if(test_flags.avg_result_local <= 600){
                return probe_select[2];
            }
            else if(test_flags.avg_result_local <= 1000){
                return probe_select[3];
            }
        }
        else if(test_flags.test == TEST_4 || test_flags.test == TEST_6 /*|| test_flags.test == TEST_8*/){
            Serial.println("distal avg:");
            Serial.println(test_flags.avg_result_distal);
            if(test_flags.avg_result_distal <= 200.0){
                return probe_select[0];
            }
            else if(test_flags.avg_result_distal <= 400.0){
                return probe_select[1];
            }
            else if(test_flags.avg_result_distal <= 600.0){
                return probe_select[2];
            }
            else if(test_flags.avg_result_distal <= 1000.0){
                return probe_select[3];
            }
        }
                
    }
    
        
    return String();
}

const String evaluate_request(){

    if(probe_event <= 10){

        S_VALID_NEXT_STATE new_state = valid_next_state[probe_state][probe_event];    

        if(new_state.next_state != probe_state){             

            if(probe_super_state == SS_CONFIG){

                if(probe_state == S_HOME && new_state.next_state == S_VIDEO){
                    probe_super_state = SS_TEST;
                    if(test_flags.resume == 0){
                        configuration_data.user_settings.prev_unique_id = configuration_data.user_settings.unique_id;//starting a new test elimantes ability to download previous testdata. thus the prev_id is equel to the new id
                        test_flags.test_cnt = TEST_1;
                        test_flags.test = TEST_1;
                        test_flags.sub = SUB_ROUTINE_0;
                        test_flags.avg_result_local = 0.0;
                        test_flags.avg_result_distal = 0.0;
                    }
                    else{
                        test_flags.test = static_cast<TEST_ROUTINE>(static_cast<int>(configuration_data.probe_test_progress.prev_test));
                        test_flags.sub = SUB_ROUTINE_0;
                        test_flags.resume = 0;
                    }                    
                }
                probe_state = new_state.next_state;
                probe_event = E_no_event;        
                test_flags.once = 0;
                probe_state_pointer = probe_state_matrix[probe_state].state_pointer;
            }
            else{

                S_TEST_REQUEST_MATRIX check_test = test_request_matrix[test_flags.test][test_flags.sub];

                if(check_test.probe_test_state != new_state.next_state){
                    probe_state = S_HOME;
                    probe_event = E_no_event;
                    test_flags.once = 0;
                    probe_state_pointer = probe_state_matrix[probe_state].state_pointer;
                }

                if(new_state.next_state == S_HOME){
                    probe_super_state = SS_CONFIG;
                    configuration_data.probe_test_progress.prev_test = test_flags.test;
                    configuration_data.probe_test_progress.prev_sub = test_flags.sub;
                }
                else{           
                    if(test_flags.test < TEST_FINISH){            
                        if(test_flags.sub == SUB_ROUTINE_3 || check_test.probe_test_state == S_NA){
                            configuration_data.probe_test_progress.prev_sub = SUB_ROUTINE_0;
                            //configuration_data.probe_test_progress.prev_test = static_cast<TEST_ROUTINE>(static_cast<int>(test_flags.test) + 1);
                            configuration_data.probe_test_progress.prev_test = TEST_ORDER[static_cast<TEST_ROUTINE>(static_cast<int>(test_flags.test_cnt) + 1)];
                            
                            test_flags.test = static_cast<TEST_ROUTINE>(static_cast<int>(TEST_ORDER[test_flags.test_cnt + 1]));
                            //test_flags.test  = static_cast<TEST_ROUTINE>(static_cast<int>(test_flags.test) + 1);

                            test_flags.test_cnt = static_cast<TEST_ROUTINE>(static_cast<int>(test_flags.test_cnt) + 1);

                            test_flags.sub = SUB_ROUTINE_0;                            
                        }
                        else if(test_flags.sub < SUB_ROUTINE_3){
                            configuration_data.probe_test_progress.prev_sub = test_flags.sub;
                            configuration_data.probe_test_progress.prev_test = test_flags.test;
                            
                            test_flags.sub = static_cast<TEST_SUB>(static_cast<int>(test_flags.sub) + 1);                     
                        }                        
                    }
                    if(test_flags.test == TEST_FINISH){
                        if(check_test.probe_test_state == S_SAVE){
                            configuration_data.probe_test_progress.prev_sub = test_flags.sub;
                            configuration_data.probe_test_progress.prev_test = test_flags.test;                            

                            test_flags.test_cnt = TEST_1;
                            test_flags.test  = TEST_1;
                            test_flags.sub = SUB_ROUTINE_0;
                            probe_super_state = SS_CONFIG;                            
                        }                        
                    }
                }                
                probe_state = new_state.next_state;
                probe_event = E_no_event;
                test_flags.once = 0;
                
                probe_state_pointer = probe_state_matrix[probe_state].state_pointer;
            }
        }
        else {
            test_flags.once = 0;
            probe_event = E_no_event;
        }
        switch(new_state.next_state){
        case S_HOME:
            return "/index.html";
        break;
        case S_SETTINGS:
            return "/settings.html";
        break;
        case S_CALIBRATION:
            return "/calibration.html";
        break;
        case S_RAW_CAPTURE:
            return "/raw-data.html";
        break;
        case S_SAVE:
            return "/download.html";
        break;
        case S_VIDEO:
            return "/video.html";
        break;
        case S_MEASURE:
            return "/measure.html";
        break;
        case S_VAS:
            return "/vas.html";
        break;
        case S_TEMP:
            return "/summation.html";
        break;
        case S_COLD:
            return "/coldpress.html";
        break;
        case S_NA:
        break;
    }
    }
    else{
        //event is not a http request
    }        
return "";    
}

#include "lwip/inet.h"
#include "lwip/dns.h"

void redcap_post_message(){
    int timeout = 5000;

    //const char* fingerprint = "B1 BC 96 8B D4 F4 9D 62 2A A8 9A 81 F2 15 01 52 A4 1D 82 9C";

    WiFiClientSecure client;
    client.setTimeout(10);
    //client.setCACert(fingerprint);

    File file = SPIFFS.open("/full_test_protocol.csv", FILE_READ);

//token=4A7B39E342D72F953EC4E03BFD3AA4D4&content=record&format=csv&type=flat&overwriteBehavior=normal&forceAutoNumber=false&data=record_id,unique_id&1234,1234=&returnContent=count&returnFormat=json
                              
    String api_param = "token=4A7B39E342D72F953EC4E03BFD3AA4D4&content=record&format=csv&type=flat&overwriteBehavior=normal&forceAutoNumber=false&data=record_id,site_label,unique_id,pressure_probe_complete\n";
    api_param += configuration_data.user_settings.unique_id;
    api_param += ",";
    api_param += configuration_data.user_settings.site_label;
    api_param += ",";
    api_param += configuration_data.user_settings.unique_id;
    api_param += ",2&returnContent=count&returnFormat=json";

#ifdef PROBE_DEBUG
            
    //api_param + "field=test_result&event=&returnFormat=json";
    Serial.println(api_param);
    //Serial.println(api_body);
    //dns.stop();
    //WiFi.config(WiFi.localIP(), WiFi.gatewayIP(), WiFi.gatewayIP(), WiFi.gatewayIP());
    ip_addr_t dnssrv=dns_getserver(0);
    Serial.print("DNS server: ");
    Serial.println(inet_ntoa(dnssrv));
    Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS: ");
  Serial.println(WiFi.dnsIP());

    #endif
//----------------------------------------------------------------POST record to redcap START----------------------------------------------------------//
    client.setInsecure();
    //WiFiClient client;
    //int code = client.connect("http://open.rsyd.dk/redcap/api/", 443, timeout);
    if (!client.connect("https://open.rsyd.dk", 443, timeout)) {
        Serial.println("Connection FAILED!");
        //Serial.printf("%d",code);
    return;
    }

    client.println("POST /redcap/api/ HTTP/1.1");
    client.println("Host: open.rsyd.dk");
    client.println("Cache-Control: no-cache");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(api_param.length());
    client.println();
    client.println(api_param);
    
    Serial.print("Waiting for response ");
    while (!client.available())
    {
        /*if (millis() - timeout > 50000)
        {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
        }*/
    }
    
    Serial.println("Response From Server");
    String resp_1;
    Serial.println("Response From Server");
    while(client.available())
    {
        char c = client.read();
        //Serial.write(c);
        resp_1 += c;        
    }
    Serial.println(resp_1);
    String response = resp_1.substring(resp_1.length() - 12);
    Serial.println(response);
    if(!response.equals("{\"count\": 1}")){
        Serial.println("not equal");

        //create JSON document with space for 2 field
        const size_t capacity = JSON_OBJECT_SIZE(1);
        DynamicJsonDocument doc(capacity);
        //insert the field value
        doc["type"] = "error";
        
        String json_error;

        serializeJson(doc, json_error); 
        webSocket.broadcastTXT(json_error);

        return;
    } 
//----------------------------------------------------------------POST record to redcap END------------------------------------------------------------//
//----------------------------------------------------------------POST file to redcap START------------------------------------------------------------//
 
    else{
        Serial.println("record uploaded!");
        Serial.println("Begin test result upload");        

        client.flush();

        String boundry = "\n--AaB03x\n";
        String start_request = "";
        start_request = start_request + 
            boundry +
            "Content-Disposition: form-data; name=\"token\"\n\n4A7B39E342D72F953EC4E03BFD3AA4D4" +
            boundry +
            "Content-Disposition: form-data; name=\"content\"\n\nfile" +
            boundry +
            "Content-Disposition: form-data; name=\"action\"\n\nimport" +
            boundry + 
            "Content-Disposition: form-data; name=\"record\"\n\n" +
            configuration_data.user_settings.unique_id +
            boundry +
            "Content-Disposition: form-data; name=\"field\"\n\ntest_result" +
            boundry +
            "Content-Disposition: form-data; name=\"event\"\n\n\n" +
            boundry +
            "Content-Disposition: form-data; name=\"returnFormat\"\n\njson";

        //String start_file_request = ""; 
        String end_request = "";
        start_request = start_request +
                        "\n--AaB03x\n" +
                        "Content-Disposition: form-data; name=\"file\"; filename=\"full_test_protocol.csv\"\n" +
                        "Content-Transfer-Encoding: binary\n\n";
        end_request = end_request + "\n--AaB03x--\n";

        uint16_t full_length;
        full_length = start_request.length() + file.size() + end_request.length();

        //WiFiClient client;
        //if (!client.connect("https://open.rsyd.dk", 443)) {
        //Serial.println("Connected FAILED!");
        //return;
        //}

        //Serial.println("Connected ok!");
        client.println("POST /redcap/api/ HTTP/1.1");
        client.println("Host: open.rsyd.dk");
        client.println("User-Agent: ESP32");
        client.println("Content-Type: multipart/form-data; boundary=AaB03x");
        client.print("Content-Length: "); client.println(full_length);
        client.println();
        client.print(start_request);

        while (file.available()){
        client.write(file.read());
        }

        Serial.println(">>><<<");
        client.println(end_request);

        Serial.print("Waiting for response ");
        while (!client.available())
        {
            /*if (millis() - timeout > 50000)
            {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
            }*/
        }
        
        Serial.println("Response From Server");
        String resp_2;
        while(client.available())
        {
            char c = client.read();            
            resp_2 += c;            
        }
        Serial.print(resp_2);
        
        String response_2 = resp_2.substring(0,15);
        Serial.println(response_2);

        if(!response_2.equals("HTTP/1.1 200 OK")){
            Serial.println("not equal");
            return;
        }
        else{
            Serial.println("upload completed!");
            //create JSON document with space for 2 field
            const size_t capacity = JSON_OBJECT_SIZE(1);
            DynamicJsonDocument doc(capacity);
            //insert the field value
            doc["type"] = "success";
            
            String json_success;

            serializeJson(doc, json_success); 
            webSocket.broadcastTXT(json_success);
        }
        
    }
//---------------------------------------------------POST file to redcap END---------------------------------------------------//
client.stop();    
}

void probe_server_init(void){

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        probe_event = E_req_home;
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, evaluate_request(), String(), false);
        response->addHeader("Cache-Control","no-store");
        request->send(response);               
    });   

    server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){
        probe_event = E_req_home;
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, evaluate_request(), String(), false);
        response->addHeader("Cache-Control","no-store");
        request->send(response);              
    });

    server.on("/home.html", HTTP_GET, [](AsyncWebServerRequest *request){
        probe_event = E_req_home;
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, evaluate_request(), String(), false);
        response->addHeader("Cache-Control","no-store");
        request->send(response);               
    }); 

    server.on("/video.html", HTTP_GET, [](AsyncWebServerRequest *request){ 
        probe_event = E_req_video;
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, evaluate_request(), String(), false, template_processor);
        response->addHeader("Cache-Control","no-store");
        request->send(response);                   
    });    

    server.on("/measure.html", HTTP_GET, [](AsyncWebServerRequest *request){
        probe_event = E_req_measure;
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, evaluate_request(), String(), false, template_processor);
        response->addHeader("Cache-Control","no-store");
        request->send(response);               
    });

    server.on("/vas.html", HTTP_GET, [](AsyncWebServerRequest *request){
        probe_event = E_req_vas;
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, evaluate_request(), String(), false, template_processor);
        response->addHeader("Cache-Control","no-store");
        request->send(response);               
    });

    server.on("/summation.html", HTTP_GET, [](AsyncWebServerRequest *request){
        probe_event = E_req_temp;
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, evaluate_request(), String(), false, template_processor);
        response->addHeader("Cache-Control","no-store");
        request->send(response);               
    });

    server.on("/coldpress.html", HTTP_GET, [](AsyncWebServerRequest *request){
        probe_event = E_req_cold;
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, evaluate_request(), String(), false, template_processor);
        response->addHeader("Cache-Control","no-store");
        request->send(response);               
    });

    server.on("/settings.html", HTTP_GET, [](AsyncWebServerRequest *request){
        probe_event = E_req_settings;
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, evaluate_request(), String(), false);
        response->addHeader("Cache-Control","no-store");
        request->send(response);        
    });

    server.on("/calibration.html", HTTP_GET, [](AsyncWebServerRequest *request){
        probe_event = E_req_calibration;
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, evaluate_request(), String(), false);
        response->addHeader("Cache-Control","no-store");
        request->send(response);        
    });

    server.on("/raw-data.html", HTTP_GET, [](AsyncWebServerRequest *request){
        probe_event = E_req_raw;
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, evaluate_request(), String(), false);
        response->addHeader("Cache-Control","no-store");
        request->send(response);        
    });

    server.on("/download.html", HTTP_GET, [](AsyncWebServerRequest *request){
        probe_event = E_req_save;
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, evaluate_request(), String(), false, template_processor);
        response->addHeader("Cache-Control","no-store");
        request->send(response);        
    });

    server.on("/resume.html", HTTP_GET, [](AsyncWebServerRequest *request){
        
        if(configuration_data.probe_test_progress.prev_test == TEST_FINISH){
            probe_event = E_req_save;
        }
        else {
            probe_event = E_req_video;
        }
        
        test_flags.resume = 1;
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, evaluate_request(), String(), false, template_processor);
        response->addHeader("Cache-Control","no-store");
        request->send(response);        
    });

    server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.js", String(), false);        
    });

    server.on("/settings.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/settings.js", String(), false);        
    });

    server.on("/raw-data.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/raw-data.js", String(), false);        
    });

    server.on("/download.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/download.js", String(), false);        
    });

    server.on("/video.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/video.js", String(), false);        
    });

    server.on("/measurement.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/measurement.js", String(), false);        
    });

    server.on("/summation.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/summation.js", String(), false);        
    });

    server.on("/coldpress.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/coldpress.js", String(), false);        
    });

    server.on("/calibration.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/calibration.js", String(), false);        
    });

    server.on("/websocket.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/websocket.js", String(), false);
    });

    server.on("/prevent-back-button.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/prevent-back-button.js", String(), false);
    });

    server.on("/vas.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/vas.js", String(), false);
    });

    server.on("/css/app.css", HTTP_GET, [](AsyncWebServerRequest *request){        
        request->send(SPIFFS, "/css/app.css", String(), false);               
    });

    server.on("/sounds/click.mp3", HTTP_GET, [](AsyncWebServerRequest *request){        
        request->send(SPIFFS, "/sounds/click.mp3", String(), false);               
    });

    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){        
        request->send(SPIFFS, "/Mimanex_logo_favi.bmp", String(), false);               
    });

    server.on("/downloadtest.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/full_test_protocol.csv", "text/csv", true);
    });

    server.on("/downloadraw.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/raw_capture_measurement.csv", "text/csv", true);
    });

    server.on("/downloadconfig.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/json_conf.txt", "text/csv", true);
    });

    //--------------------------INDIVIUAL DOWNLOAD URL's-------------------------------------------//
    //All the following URL's can be removed or kept. they make it possible to 
    //retreive the individual test data on the go by entering the URL's listed here
    
    server.on("/download1.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/test_1_sub_1.csv", "text/csv", true);
    });

    server.on("/download2.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/test_1_sub_2.csv", "text/csv", true);
    });

    server.on("/download3.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/test_1_sub_3.csv", "text/csv", true);
    });

    server.on("/download4.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/test_2_sub_1.csv", "text/csv", true);
    });

    server.on("/download5.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/test_2_sub_2.csv", "text/csv", true);
    });

    server.on("/download6.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/test_2_sub_3.csv", "text/csv", true);
    });

    server.on("/download7.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/test_3_sub_1.csv", "text/csv", true);
    });
    
    server.on("/download8.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/test_4_sub_1.csv", "text/csv", true);
    });

    server.on("/download9.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/test_5_sub_1.csv", "text/csv", true);
    });

    server.on("/download10.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/test_6_sub_1.csv", "text/csv", true);
    });

    server.on("/download11.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/test_7_sub_1.csv", "text/csv", true);
    });

    server.on("/download12.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/test_8_sub_1.csv", "text/csv", true);
    });    
}
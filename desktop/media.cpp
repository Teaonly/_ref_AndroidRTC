#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

#include "media.h"

using namespace std;

static vector<string> SplitString(const string& s, const string& delim, const bool keep_empty = false) {
    vector<string> result;
    if (delim.empty()) {
        result.push_back(s);
        return result;
    }
    string::const_iterator substart = s.begin(), subend;
    while (true) {
        subend = search(substart, s.end(), delim.begin(), delim.end());
        string temp(substart, subend);
        if (keep_empty || !temp.empty()) {
            result.push_back(temp);
        }
        if (subend == s.end()) {
            break;
        }
        substart = subend + delim.size();
    }
    return result;
}

bool MediaURL::Parse(const std::string &url) {
    proto = "";
    location = "";
    resource = "";

    vector<string> sub_strs;
    string temp;

    sub_strs = SplitString(url, "://");
    if ( sub_strs.size() != 2) {
        return false;
    }
    proto = sub_strs[0];
    temp = sub_strs[1];

    sub_strs = SplitString(temp, ":");        //is TCP channel , get port number
    if(sub_strs.size() == 2) {
        location = sub_strs[0];
        resource = sub_strs[1];
        return true;
    } 

    sub_strs = SplitString(temp, "/");
    if(sub_strs.size() == 2) {
        location = sub_strs[0];
        resource = sub_strs[1];
        return true;
    }

    return false;
}

bool MediaDescription::Parse(const std::string &desc) {
    isVideo = false;
    isAudio = false;

    vector<string> sub_strs;
    string temp;

    sub_strs = SplitString(desc, ":");
    if ( sub_strs.size() < 3) {
        return false;
    }
    
    if ( sub_strs[0] == "video" && sub_strs.size() == 4)
        isVideo = true;
    else if ( sub_strs[0] == "audio" && sub_strs.size() == 3) 
        isAudio = true;
    else
        return false;

    codec = sub_strs[1];
    
    if ( isAudio) {
        sample = atoi( sub_strs[2].c_str() );
        return true;
    } else {
        width = atoi( sub_strs[2].c_str() );
        height = atoi( sub_strs[3].c_str() );
        return true;
    }
    
    return false;
}

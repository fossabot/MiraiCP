// Copyright (c) 2021-2021. Eritque arcus and contributors.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or any later version(in your opinion).
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "Tools.h"
#include "Logger.h"
#include <regex>
#include <utf8.h>

namespace MiraiCP {
    /*工具类实现*/
    std::string Tools::jstring2str(jstring jStr, JNIEnv *env) {
        if (!jStr) {
            Logger::logger.error("警告:kotlin部分返回空字符串, 位置:Tools::jstring2str");
            return "";
        }
        std::u16string s = reinterpret_cast<const char16_t *>(env->GetStringChars(jStr, nullptr));
        if (s.length() == 0) {
            Logger::logger.error("警告:kotlin部分返回空字符串, 位置:Tools::jstring2str");
            return "";
        }
        std::string x;
        utf8::utf16to8(s.begin(), s.end(), std::back_inserter(x));
        return x;
    }

    jstring Tools::str2jstring(const char *stra, JNIEnv *env) {
        if (!stra) {
            Logger::logger.error("警告:C++部分传入空字符串，位置:Tools::str2jstring");
        }
        std::string str(stra);
        std::vector<unsigned short> utf16line;
        utf8::utf8to16(str.begin(), str.end(), std::back_inserter(utf16line));
        auto *c = new jchar[utf16line.size()];
        for (int i = 0; i < utf16line.size(); i++) {
            c[i] = utf16line[i];
        }
        return env->NewString((jchar *) c, (jsize) utf16line.size());
    }
    std::string Tools::replace(std::string str, const std::string &from, const std::string &to) {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substd::string of 'from'
        }
        return str;
    }
    std::vector<unsigned long long> Tools::StringToVector(std::string temp) {
        std::vector<unsigned long long> result;
        temp.erase(temp.begin());
        temp.pop_back();
        std::regex ws_re("[,]+");
        std::vector<std::string> v(std::sregex_token_iterator(temp.begin(), temp.end(), ws_re, -1),
                                   std::sregex_token_iterator());
        result.reserve(v.size());
        for (auto &&s: v)
            result.push_back(std::stoull(s));
        return result;
    }

    std::string Tools::escapeFromMiraiCode(const std::string &s) {
        //[	\[
        //]	\]
        //:	\:
        //,	\,
        //\	\\ /
        return Tools::replace(
                Tools::replace(
                        Tools::replace(
                                Tools::replace(
                                        Tools::replace(s,
                                                       "\\\\", "\\"),
                                        "\\,", ","),
                                "\\:", ":"),
                        "\\]", "]"),
                "\\[", "[");
    }

    std::string Tools::escapeToMiraiCode(const std::string &s) {
        //[	\[
        //]	\]
        //:	\:
        //,	\,
        //\	\\ /
        return replace(replace(replace(replace(replace(s,
                                                       "\\", "\\\\"),
                                               ",", "\\,"),
                                       ":", "\\:"),
                               "]", "\\]"),
                       "[", "\\[");
    }
} // namespace MiraiCP

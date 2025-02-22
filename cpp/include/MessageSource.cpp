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

#include "Config.h"
#include "LowLevelAPI.h"

namespace MiraiCP {
    void MessageSource::recall(JNIEnv *env) const {
        json j;
        j["source"] = this->serializeToString();
        std::string re = Config::koperation(Config::Recall, j, env);
        if (re == "Y") return;
        if (re == "E1") MiraiCPThrow(BotException());
        if (re == "E2") MiraiCPThrow(RecallException());
    }

    MessageSource::MessageSource(std::string ids,
                                 std::string internalids,
                                 std::string source)
        : ids(std::move(ids)),
          internalids(std::move(internalids)),
          source(std::move(source)) {}

    std::string MessageSource::serializeToString() const {
        return source;
    }

    MessageSource MessageSource::deserializeFromString(const std::string &source) {
        json j = json::parse(source);
        try {
            return {j["ids"].dump(), j["internalIds"].dump(), source};
        } catch (json::type_error &e) {
            Logger::logger.error("消息源序列化出错，格式不符合(MessageSource::deserializeFromString)");
            Logger::logger.error(source);
            Logger::logger.error(e.what());
            MiraiCPThrow(IllegalArgumentException(std::string("消息源序列化出错，格式不符合(MessageSource::deserializeFromString), ") + e.what()));
        }
    }

    MessageSource MessageSource::quoteAndSendMiraiCode(const std::string &content, QQID groupid, JNIEnv *env) const {
        json obj;
        json sign;
        obj["messageSource"] = this->serializeToString();
        obj["msg"] = content;
        sign["MiraiCode"] = true;
        sign["groupid"] = groupid;
        obj["sign"] = sign.dump();
        std::string re = Config::koperation(Config::SendWithQuote, obj, env);
        return MessageSource::deserializeFromString(re);
    }

    MessageSource MessageSource::quoteAndSendMsg(const std::string &content, QQID groupid, JNIEnv *env) const {
        json obj;
        json sign;
        obj["messageSource"] = this->serializeToString();
        obj["msg"] = content;
        sign["MiraiCode"] = false;
        sign["groupid"] = groupid;
        obj["sign"] = sign.dump();
        std::string re = Config::koperation(Config::SendWithQuote, obj, env);
        return MessageSource::deserializeFromString(re);
    }

    MessageSource Contact::sendMsg0(const std::string &msg, int retryTime, bool miraicode, JNIEnv *env) {
        if (msg.empty()) {
            MiraiCPThrow(IllegalArgumentException("不能发送空信息, 位置: Contact::SendMsg"));
        }
        std::string re = LowLevelAPI::send0(msg, this, retryTime, miraicode, env,
                                            "reach a error area, Contact::SendMiraiCode");
        if (re == "ET")
            MiraiCPThrow(TimeOutException("发送消息过于频繁导致的tx服务器未能即使响应, 位置: Contact::SendMsg"));
        if (Tools::starts_with(re, "EBM"))
            MiraiCPThrow(BotIsBeingMutedException(std::stoi(re.substr(3))));
        return MessageSource::deserializeFromString(re);
    }
} // namespace MiraiCP

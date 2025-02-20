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

#ifndef MIRAICP_PRO_FRIEND_H
#define MIRAICP_PRO_FRIEND_H

#include "LowLevelAPI.h"

namespace MiraiCP {
    /// 好友类声明
    class Friend : public Contact {
    public:
        /// 删除好友(delete是C++关键字
        void deleteFriend(JNIEnv *env = ThreadManager::getEnv());

        void refreshInfo(JNIEnv *env = ThreadManager::getEnv()) override;

        /*!
         * @brief 发送戳一戳
         * @warning 发送戳一戳的前提是登录该bot的协议是phone
         * @throw MiraiCP::BotException, MiraiCP::IllegalStateException
         */
        void sendNudge();

        /*!
         * @brief 构建好友对象
         * @param friendid q号
         * @param botid 对应机器人id
         */
        explicit Friend(QQID friendid, QQID botid, JNIEnv * = ThreadManager::getEnv());

        explicit Friend(Contact c) : Contact(std::move(c)) { refreshInfo(); };
    };
} // namespace MiraiCP

#endif //MIRAICP_PRO_FRIEND_H

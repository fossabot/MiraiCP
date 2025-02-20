/*
 * Copyright (C) 2020-2021 Eritque arcus and contributors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or any later version(in your opinion).
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package tech.eritquearcus.miraicp.shared

import com.google.gson.annotations.SerializedName

object Config {
    data class Contact(
        val type: Int,
        val id: Long,
        val groupid: Long,
        val nickornamecard: String,
        val botid: Long,
        val anonymous: Boolean = false
    )

    data class SendRequest(
        val contact: Contact,
        val content: String
    )

    data class ForwardMessageJson(
        @SerializedName("type") val type: Int,
        @SerializedName("id") val id: Long,
        @SerializedName("id2") val id2: Long,
        @SerializedName("content") val content: Content
    )

    data class Content(
        @SerializedName("type") val type: Int,
        @SerializedName("id") val id: Long,
        @SerializedName("id2") val id2: Long,
        @SerializedName("value") val value: List<Value>
    )

    data class Value(
        @SerializedName("name") val name: String,
        @SerializedName("id") val id: Long,
        @SerializedName("time") val time: Int,
        @SerializedName("message") val message: String
    )

    // Announcement params
    data class AP(
        val sendToNewMember: Boolean = false,
        /** 置顶. 可以有多个置顶公告 */
        val isPinned: Boolean = false,
        /** 显示能够引导群成员修改昵称的窗口 */
        val showEditCard: Boolean = false,
        /** 使用弹窗 */
        val showPopup: Boolean = false,
        /** 需要群成员确认 */
        val requireConfirmation: Boolean = false,
    )

    data class OnlineA(
        val content: String,
        val fid: String,
        val imageid: String,
        val confirmationNum: Int,
        val senderid: Long,
        val groupid: Long,
        val botid: Long,
        val time: Long,
        val params: AP
    )

    // Announcement identify
    data class IdentifyA(
        val botid: Long,
        val groupid: Long,
        val type: Int,
        val fid: String?
    )

    data class BriefOfflineA(
        val content: String,
        val params: AP
    )

    data class GroupSetting(
        val name: String,
        val isMuteAll: Boolean,
        val isAllowMemberInvite: Boolean,
        val isAutoApproveEnabled: Boolean,
        val isAnonymousChatEnabled: Boolean
    )

    data class ContactInfo(
        val nickornamecard: String,
        val avatarUrl: String,
        val setting: GroupSetting = GroupSetting("", false, false, false, false)
    )

    data class DInfo(
        val url: String,
        val md5: String,
        val sha1: String
    )

    data class FInfo(
        val size: Long,
        val uploaderid: Long,
        val expirytime: Long,
        val uploadtime: Long,
        val lastmodifytime: Long,
    )

    data class FileInfo(
        val id: String,
        val internalid: Int = 0,
        val name: String = "",
        val path: String = "/",
        val dinfo: DInfo,
        val finfo: FInfo
    )

    data class Message(
        val messageSource: String
    )
}

object CPPConfig {
    data class cppPath(
        val path: String,
        val dependencies: List<String>?
    )

    data class AdvanceConfig(
        val maxThread: Int?
    )

    data class pluginConfig(
        val pluginConfig: List<cppPath>,
        val advanceConfig: AdvanceConfig?
    )

    data class loaderConfig(
        val accounts: List<Account>?,
        val cppPaths: List<cppPath>,
        val advanceConfig: AdvanceConfig?
    ) {
        data class Account(
            val heatBeat: String?,
            val id: Long,
            val passwords: String,
            val protocol: String?,
            val md5: Boolean?,
            val autoLogin: Boolean?,
            var logined: Boolean?
        )
    }
}

object CPPEvent {
    //消息事件
    data class GroupMessage(
        val group: Config.Contact,
        val member: Config.Contact,
        val message: String,
        val source: String,
        val type: Int = 1
    )

    data class PrivateMessage(
        val friend: Config.Contact,
        val message: String,
        val source: String,
        val type: Int = 2
    )

    data class GroupInvite(
        val source: GroupInviteSource,
        val request: String,
        val type: Int = 3
    ) {
        //群邀请
        data class GroupInviteSource(
            val botid: Long,
            val eventid: Long,
            val inviterid: Long,
            val groupid: Long,
            val groupname: String,
            val inviternick: String
        )
    }

    data class Request(
        val text: String,
        val accept: Boolean,
        val botid: Long,
        val ban: Boolean?
    )

    //好友邀请
    data class NewFriendRequest(
        val source: NewFriendRequestSource,
        val request: String,
        val type: Int = 4
    ) {
        data class NewFriendRequestSource(
            val botid: Long,
            val eventid: Long,
            val message: String,
            val fromid: Long,
            val fromgroupid: Long,
            val fromnick: String
        )
    }

    //群成员加入
    data class MemberJoin(
        val group: Config.Contact,
        val member: Config.Contact,
        /*
    invite - 1
    active - 2
    retrieve - 3
     */
        val jointype: Int,
        //如果没有则为member.id
        val inviterid: Long = 0,
        val type: Int = 5
    )

    //群成员退出
    data class MemberLeave(
        val group: Config.Contact,
        val memberid: Long,
        /*
    kick - 1
    quit - 2
     */
        val leavetype: Int,
        //如果没有则为memberid
        val operatorid: Long = 0,
        val type: Int = 6
    )

    //撤回
    data class RecallEvent(
        val etype: Int,
        val authorid: Long,
        val operatorid: Long,
        val ids: String,
        val internalids: String,
        val time: Int,
        val groupid: Long = 0,
        val botid: Long,
        val type: Int = 7
    )

    data class BotJoinGroup(
        val etype: Int,
        val group: Config.Contact,
        val inviterid: Long,
        val type: Int = 9
    )

    data class GroupTempMessage(
        val group: Config.Contact,
        val member: Config.Contact,
        val message: String,
        val source: String,
        val type: Int = 10
    )

    data class BotOnline(
        val botid: Long,
        val type: Int = 11
    )

    data class TimeOutEvent(
        val msg: String,
        val type: Int = 12
    )

    data class NugdeEvent(
        val from: Config.Contact,
        val target: Config.Contact,
        val botid: Long,
        val type: Int = 13
    )
    data class BotLeaveEvent(
        val groupid: Long,
        val botid: Long,
        //val leavetype: Int TODO(见Mirai源码, 目前BotLeave还不稳定)
        val type : Int = 14
    )
    data class MemberJoinRequestEvent(
        val group: Config.Contact,
        val inviter: Config.Contact,
        val requestData: String,
        val type: Int = 15
    )
}

data class PluginConfig(
    val id: String,
    val author: String,
    val description: String,
    val name: String,
    val time: String,
    val version: String,
    val MiraiCPversion: String
)
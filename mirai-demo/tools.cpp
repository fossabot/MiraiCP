#include "pch.h"

/*日志类实现*/
void Logger::init(JNIEnv* env) {
    this->env = env;
    this->javaclass = this->env->FindClass("org/example/mirai/plugin/CPP_lib");
    this->sinfo = this->env->GetStaticMethodID(this->javaclass, "SendLog", "(Ljava/lang/String;)V");
    this->swarning = this->env->GetStaticMethodID(this->javaclass, "SendW", "(Ljava/lang/String;)V");
    this->serror = this->env->GetStaticMethodID(this->javaclass, "SendE", "(Ljava/lang/String;)V");
}
Logger::~Logger() {
    this->env->DeleteLocalRef(this->javaclass);
}
void Logger::Warning(string log) {
    this->env->CallStaticVoidMethod(this->javaclass, this->swarning, tools.str2jstring(env, log.c_str()));
}
void Logger::Error(string log) {
    this->env->CallStaticVoidMethod(this->javaclass, this->serror, tools.str2jstring(env, log.c_str()));
}
void Logger::Info(string log) {
    this->env->CallStaticVoidMethod(this->javaclass, this->sinfo, tools.str2jstring(env, log.c_str()));
}

/*图片类实现*/
Image::Image(JNIEnv* env, string imageId) {
    this->env = env;
    this->java_class = env->FindClass("org/example/mirai/plugin/CPP_lib");
    this->Query = env->GetStaticMethodID(java_class, "QueryImgUrl", "(Ljava/lang/String;)Ljava/lang/String;");
    this->id = imageId;
}
string Image::queryURL() {
    return tools.jstring2str(this->env, (jstring)this->env->CallStaticObjectMethod(this->java_class, this->Query, tools.str2jstring(this->env, this->id.c_str())));
}
vector<string> Image::GetImgIdFromMiraiCode(string MiraiCode) {
    vector<string> result = vector<string>();
    string temp = MiraiCode;
    smatch m;
    regex re("\\[mirai:image:(.*?)\\]");
    while (std::regex_search(temp, m, re)) {
        result.push_back(m[1]);
        temp = m.suffix().str();
    }
    return result;
}
Image Image::uploadImg2Friend(JNIEnv* env, long id, string filename) {
    ifstream fin(filename);
    if(!fin){
        logger->Error("文件不存在,位置:C++部分 uploadImg2Friend(),文件名:"+filename);
        fin.close();
        throw invalid_argument("NO_FILE_ERROR");
    }
    fin.close();
    jclass java_class = env->FindClass("org/example/mirai/plugin/CPP_lib");
    jmethodID m = env->GetStaticMethodID(java_class, "uploadImgF", "(JLjava/lang/String;)Ljava/lang/String;");
    string re = tools.jstring2str(env, (jstring)env->CallStaticObjectMethod(java_class, m, (jlong)id, tools.str2jstring(env, filename.c_str())));
    return Image(env, re);
}
Image Image::uploadImg2Group(JNIEnv* env, long groupid, string filename) {
    ifstream fin(filename);
    if (!fin) {
        logger->Error("文件不存在,位置:C++部分 uploadImg2Group(),文件名:" + filename);
        fin.close();
        throw invalid_argument("NO_FILE_ERROR");
    }
    fin.close();
    jclass java_class = env->FindClass("org/example/mirai/plugin/CPP_lib");
    jmethodID m = env->GetStaticMethodID(java_class, "uploadImgG", "(JLjava/lang/String;)Ljava/lang/String;");
    string re = tools.jstring2str(env, (jstring)env->CallStaticObjectMethod(java_class, m, (jlong)groupid, tools.str2jstring(env, filename.c_str())));
    return Image(env, re);
}
Image Image::uploadImg2Member(JNIEnv* env, long groupid, long qqid, string filename) {
    ifstream fin(filename);
    if (!fin) {
        logger->Error("文件不存在,位置:C++部分 uploadImg2Group(),文件名:" + filename);
        fin.close();
        throw invalid_argument("NO_FILE_ERROR");
    }
    fin.close();
    jclass java_class = env->FindClass("org/example/mirai/plugin/CPP_lib");
    jmethodID m = env->GetStaticMethodID(java_class, "uploadImgM", "(JJLjava/lang/String;)Ljava/lang/String;");
    string re = tools.jstring2str(env, (jstring)env->CallStaticObjectMethod(java_class, m, (jlong)groupid,(jlong) qqid, tools.str2jstring(env, filename.c_str())));
    return Image(env, re);
}
string Image::toMiraiCode() {
    return "[mirai:image:" + this->id + "] ";
}

/*好友类实现*/
Friend::Friend (JNIEnv* env, long id){
    this->java_first = env->FindClass("org/example/mirai/plugin/CPP_lib");
    this->Send_Msg_id = env->GetStaticMethodID(java_first, "SendPrivateMSG", "(Ljava/lang/String;J)V");
    this->NickorName_id = env->GetStaticMethodID(java_first, "GetNick", "(J)Ljava/lang/String;");
    this->id = id;
    this->env = env;
    jstring temp = (jstring)this->env->CallStaticObjectMethod(this->java_first, this->NickorName_id, (jlong)id, (jlong)id);
    this->nick = tools.jstring2str(this->env, temp);
}
Friend::~Friend() {
    this->env->DeleteLocalRef(java_first);
}

/*成员类实现*/
Member::Member(JNIEnv* env, long id, long groupid) {
    this->java_first = env->FindClass("org/example/mirai/plugin/CPP_lib");
    this->Send_Msg_id = env->GetStaticMethodID(java_first, "SendPrivateM2M", "(Ljava/lang/String;JJ)V");
    this->NickorName_id = env->GetStaticMethodID(java_first, "GetNameCard", "(JJ)Ljava/lang/String;");
    this->id = id;
    this->groupid = groupid;
    this->env = env;
    jstring temp = (jstring)this->env->CallStaticObjectMethod(this->java_first, this->NickorName_id, (jlong)id, (jlong)groupid);
    this->nameCard = tools.jstring2str(this->env, temp);
}
Member::~Member() {
    this->env->DeleteLocalRef(java_first);
}

/*群聊类实现*/
Group::Group(JNIEnv* env, long id) {
    this->java_first = env->FindClass("org/example/mirai/plugin/CPP_lib");
    this->Send_Msg_id = env->GetStaticMethodID(java_first, "SendGroup", "(Ljava/lang/String;J)V");
    this->env = env;
    this->id = id;
}
Group::~Group() {
    env->DeleteLocalRef(java_first);

}

/*工具类实现*/
string Tools::jstring2str(JNIEnv* env, jstring jstr)
    {
        char* rtn = NULL;
        jclass   clsstring = env->FindClass("java/lang/String");
        jstring   strencode = env->NewStringUTF("GB2312");
        jmethodID   mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
        jbyteArray   barr = (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
        jsize   alen = env->GetArrayLength(barr);
        jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
        if (alen > 0)
        {
            rtn = (char*)malloc(alen + 1);
            memcpy(rtn, ba, alen);
            rtn[alen] = 0;
        }
        env->ReleaseByteArrayElements(barr, ba, 0);
        string stemp(rtn);
        free(rtn);
        return stemp;
    }
jstring Tools::str2jstring(JNIEnv* env, const char* pat)
    {
        //定义java String类 strClass
        jclass strClass = (env)->FindClass("Ljava/lang/String;");
        //获取String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
        jmethodID ctorID = (env)->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
        //建立byte数组
        jbyteArray bytes = (env)->NewByteArray(strlen(pat));
        //将char* 转换为byte数组
        (env)->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
        // 设置String, 保存语言类型,用于byte数组转换至String时的参数
        jstring encoding = (env)->NewStringUTF("GB2312");
        //将byte数组转换为java String,并输出
        return (jstring)(env)->NewObject(strClass, ctorID, bytes, encoding);
    }
string Tools::JLongToString(jlong qqid) {
        auto id = [qqid]() -> string {
            stringstream stream;
            stream << qqid;
            string a;
            stream >> a;
            stream.clear();
            return a;
        };
        return id();
    }